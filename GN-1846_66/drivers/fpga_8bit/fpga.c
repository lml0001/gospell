/*
*	设备文件路径:/lDev/s_FpgeDevice.
*	主设备号:211 次设备号;1
*	读写操作使用互斥锁.
*	使用打开计数器，使s_FpgeDevice设备只能被打开一次.
*	该驱动内部在读/写过程中使用同一个缓冲区,故用户态程序在进行读写时必须使用用户态锁，
*	避免读写操作交叉进行。
*/

#include <linux/config.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/sched.h>	//调度
#include <linux/kthread.h>	//内核线程
#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>		/* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <linux/spinlock.h>
#include<linux/interrupt.h> 
#include <linux/timer.h>
#include <linux/kfifo.h>	//内核FIFO队列
#include <asm/system.h>		/* cli(), *_flags */
#include <asm/uaccess.h>	/* copy_*_user */
#include <asm/arch-at91rm9200/gpio.h>
#include <asm/io.h>
#include "fpga.h"
#include<linux/delay.h>

//#define TEST_INTERFACE
/**	打开调试开关.	*/
#define DEBUG

/**	打印调试.	*/
#undef PDEBUG

#ifdef DEBUG
#define PDEBUG(fmt,args...)  printk(KERN_INFO "FPGA CPU_COM: " fmt, ## args )
#define KDEBUG(fmt,args...)  printk(KERN_INFO fmt, ## args )
#else
#define PDEBUG(fmt,args...)  
#define KDEBUG(fmt,args...) 	
#endif

#define AT91C_SMC2_RWHOLD_1_CYCLE     ((unsigned int) 0x1 << 28) // (SMC2) Read and Write Signal Hold Time	//hzh
#define AT91C_SMC2_RWHOLD_2_CYCLE     ((unsigned int) 0x2 << 28) // (SMC2) Read and Write Signal Hold Time	//hzh

MODULE_LICENSE("GPL");

static void FPGA_CMDWrite(struct fpga_cmd *array, size_t size);
static ssize_t FPGA_Read(struct file *filp, char __user *buff, size_t size, loff_t *off);
static ssize_t FPGA_Write(struct file *filp, const char __user *buff, size_t size, loff_t *off);
static int FPGA_Open(struct inode *inode, struct file *filp);
static int FPGA_Release(struct inode *, struct file*);
static loff_t FPGA_LLSeek(struct file *filp, loff_t off, int whence);
static int FPGA_IOCTL(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);

struct file_operations s_FpgaFOP =
{
	.owner =	THIS_MODULE, 
	.read =		FPGA_Read, 
	.write =	FPGA_Write, 
	.llseek =	FPGA_LLSeek, 
	.open =		FPGA_Open, 
	.release =	FPGA_Release,
	.ioctl =	FPGA_IOCTL
};


static struct fpga_s
{
	struct cdev				dev;
	/*引用计数及其锁*/
	int						refs;
	spinlock_t				refs_lock;

	/*物理空间读写结构*/
	unsigned char 			*base;//FPGA 访问基础地址
	spinlock_t				io_spinlock;
	
	/*流模式接口*/
	unsigned char			*w_buf;//写缓存
	struct semaphore		w_sem;//多线程写访问锁

	struct tasklet_struct	r_tasklet;
	unsigned char			*r_buf;//读缓存
	unsigned char			*r_user_buf;//读用户缓存
	struct kfifo			*r_fifo;//读队列
	struct task_struct		*r_task;//读任务
	struct timer_list		r_timer;
	int						r_sync;
	int						force_mark;

	/*命令模式接口*/
	struct fpga_cmd		*cmd_array;
	size_t				cmd_size;
	size_t				cmd_idx;
	struct semaphore	cmd_sem;//多线程写访问锁


} s_FpgeDevice;


static int fpga_major = FPGA_MAJOR;
static int fpga_minor = FPGA_MINOR;

// #define FPGA_DRIVER_KERNEL_W_FIFO_COUNT		128
#define FPGA_DRIVER_KERNEL_R_FIFO_COUNT		128
// #define FPGA_DRIVER_KERNEL_W_FIFO_SIZE		(FPGA_IO_BUFF_SIZE * FPGA_DRIVER_KERNEL_R_FIFO_COUNT)
#define FPGA_DRIVER_KERNEL_R_FIFO_SIZE		(FPGA_IO_BUFF_SIZE * FPGA_DRIVER_KERNEL_R_FIFO_COUNT)

static void DEBUG_PrintDataBlock(char* pTitle, unsigned char *pBuf, int Size)
{
	int i;
	printk("%s  ------------------------ %d", pTitle, Size);
	for (i = 0; i < Size; i++)
	{
		if (i % 8 == 0)
		{
			printk("\n  %.2d - %.2d   ", i, i + 8 - 1);
		}
		else if (i % (8 / 2) == 0)
		{
			printk(" ");
		}
		printk("%.2X ", pBuf[i]);
	}
	printk("\n");
	printk("%s  ------------------------\n", pTitle);
}



static void FPGA_ReadTimer(unsigned long data)
{
	tasklet_schedule(&s_FpgeDevice.r_tasklet);//调度FPGA读取小任务
#ifdef TEST_INTERFACE
	mod_timer(&s_FpgeDevice.r_timer, jiffies + msecs_to_jiffies(1000));
#else
	mod_timer(&s_FpgeDevice.r_timer, jiffies + msecs_to_jiffies(10));
#endif
}

#ifdef TEST_INTERFACE
static int s_testvalue;
#endif

static void FPGA_ReadTasklet(unsigned long Data)
{
	unsigned char *plPHYAddr;
	int i, lPayloadSize, lLeft, lTmpSize;

	spin_lock(&s_FpgeDevice.io_spinlock);//读写锁

#ifdef TEST_INTERFACE
	{
		unsigned short plTestBuf[64];

		plPHYAddr = s_FpgeDevice.base /*+ (0x1FD << 1)*/;
		if (s_testvalue%2 == 0)
		{
			plTestBuf[0] = ioread16(plPHYAddr + 0);
			//plTestBuf[1] = ioread16(plPHYAddr + 2);
			//plTestBuf[2] = ioread16(plPHYAddr + 4);
			//plTestBuf[3] = ioread16(plPHYAddr + 6);

			printk("Value1 = 0x%04X\n", plTestBuf[0]);
			//printk("Value2 = 0x%04X\n", plTestBuf[1]);
			//printk("Value3 = 0x%04X\n", plTestBuf[2]);
			//printk("Value4 = 0x%04X\n", plTestBuf[3]);
			printk("\n");
		}
		else
		{
			iowrite16(0x1234, plPHYAddr + 0);
			iowrite16(0x5678, plPHYAddr + 2);
			iowrite16(0xABCD, plPHYAddr + 4);
			iowrite16(0x5A4D, plPHYAddr + 6);
		}
	}

	s_testvalue++;
#else
	while(s_FpgeDevice.force_mark)
	{
		if ((kfifo_len(s_FpgeDevice.r_fifo) + FPGA_IO_BUFF_SIZE) <= FPGA_DRIVER_KERNEL_R_FIFO_SIZE)//判断是否可写入到FIFO中！确保还剩下一个完整的缓冲区块
		{
			plPHYAddr = s_FpgeDevice.base;

			s_FpgeDevice.r_buf[0] = ioread16(plPHYAddr) & 0xFF;
			plPHYAddr +=2;/*因为没有接A0这里地址跳着走*/
			s_FpgeDevice.r_buf[1] = ioread16(plPHYAddr) & 0xFF;
			plPHYAddr +=2;



			if (s_FpgeDevice.r_buf[0] != 0 && s_FpgeDevice.r_buf[0] != 0xFF)
			{
				//printk("Tag = 0x%08X\n", s_FpgeDevice.r_buf[0]);
				//printk("Len = %d\n", s_FpgeDevice.r_buf[1]);

				s_FpgeDevice.r_buf[2] = ioread16(plPHYAddr) & 0xFF;
				plPHYAddr +=2;
				s_FpgeDevice.r_buf[3] = ioread16(plPHYAddr) & 0xFF;
				plPHYAddr +=2;

				lPayloadSize = s_FpgeDevice.r_buf[1] * 4;

				if ((lPayloadSize >= 0) && (lPayloadSize <= FPGA_IO_MAX_PAYLOAD_SIZE))
				{
					i = 4;//头已经被读取
					lLeft = lPayloadSize;

					while(lLeft > 0)
					{
						s_FpgeDevice.r_buf[i] = ioread16(plPHYAddr) & 0xFF;
						plPHYAddr +=2;
						i ++;
						lLeft --;
					}

					lTmpSize = kfifo_put(s_FpgeDevice.r_fifo, s_FpgeDevice.r_buf, FPGA_IO_BUFF_SIZE);
					if (lTmpSize != FPGA_IO_BUFF_SIZE)
					{
						printk("Write to R Fifo Failed! Actual/Protocol/Packet [%d/%d/%d]\n", lTmpSize, (lPayloadSize + 4), FPGA_IO_BUFF_SIZE);
					}

				}
				else
				{
					printk("Read Fpga Error!!! lPayloadSize = %d, Data Skiped\n", lPayloadSize);
					/*错误数据也需要读取然后放弃，以期快速跳过错误！*/
					for (i = 0; i < lPayloadSize ; i++)
					{
						ioread16(plPHYAddr);
						plPHYAddr +=2;
					}

					/*数据错误*/
					break;
				}
				udelay(5);
			}
			else
			{
				/*没有数据可读*/
				break;
			}
		}
		else
		{
			printk("Read FIFO Overflow!!!!!!!\n");
			//kfifo_reset(s_FpgeDevice.r_fifo);
			break;
		}
		//break;
	}

#endif

	spin_unlock(&s_FpgeDevice.io_spinlock);
}



static __init int FPGA_Initiate(void)
{
	int lRet = 0;
	dev_t lDev = 0;
	lDev = MKDEV(fpga_major, fpga_minor);
	lRet = register_chrdev_region(lDev, 1, "s_FpgeDevice");
	if (lRet < 0)
	{
		printk(KERN_INFO "s_FpgeDevice:can't register major:%d minor:%d", fpga_major,fpga_minor);
		return lRet;
	}


	cdev_init(&s_FpgeDevice.dev, &s_FpgaFOP);
	s_FpgeDevice.dev.owner = THIS_MODULE;
	s_FpgeDevice.dev.ops = &s_FpgaFOP;
	lRet = cdev_add(&s_FpgeDevice.dev, lDev, 1);
	if (lRet)
	{
		lRet=- ENOENT;
		printk(KERN_INFO "Error :%d adding s_FpgeDevice cdev.\n", lRet);
		return lRet;
	}

	/*自定义初始化工作----------------------------------------------------------*/

	/*初始化物理接口*/
	if((s_FpgeDevice.base = ioremap(FPGA_BASE_ADDRESS , 0x800) ) == NULL )
	{
		lRet=-EFAULT;
		return lRet;
	}

	printk("Map ARM Base Address = 0x%08X Map To = 0x%08X\n", FPGA_BASE_ADDRESS, s_FpgeDevice.base);

	/*创建读循环缓存*/
	s_FpgeDevice.r_fifo = kfifo_alloc(FPGA_DRIVER_KERNEL_R_FIFO_SIZE, GFP_KERNEL, NULL);
	if (!s_FpgeDevice.r_fifo)
	{
		printk(KERN_INFO "Create r_fifo Failed, Code = %d.\n", lRet);
		return lRet;
	}

	/*创建读临时缓存*/
	s_FpgeDevice.r_buf = kmalloc(FPGA_IO_BUFF_SIZE, GFP_KERNEL);
	if (s_FpgeDevice.r_buf)
	{
		memset(s_FpgeDevice.r_buf, 0, FPGA_IO_BUFF_SIZE);
	}
	else
	{
		printk(KERN_INFO "Error No Memory For r_buf.\n");
		lRet=- ENOMEM;
		return lRet;
	}

	s_FpgeDevice.r_user_buf = kmalloc(FPGA_IO_BUFF_SIZE, GFP_KERNEL);
	if (s_FpgeDevice.r_user_buf)
	{
		memset(s_FpgeDevice.r_user_buf, 0, FPGA_IO_BUFF_SIZE);
	}
	else
	{
		printk(KERN_INFO "Error No Memory For r_user_buf.\n");
		lRet=- ENOMEM;
		return lRet;
	}

	tasklet_init(&s_FpgeDevice.r_tasklet,FPGA_ReadTasklet, 0);  

	/*创建写临时缓存*/
	s_FpgeDevice.w_buf = kmalloc(FPGA_IO_BUFF_SIZE, GFP_KERNEL);
	if (s_FpgeDevice.w_buf)
	{
		memset(s_FpgeDevice.w_buf, 0, FPGA_IO_BUFF_SIZE);
	}
	else
	{
		printk(KERN_INFO "Error No Memory for w_buf.\n");
		lRet=- ENOMEM;
		return lRet;
	}
	/*初始化写锁*/
	init_MUTEX(&s_FpgeDevice.w_sem);
	init_MUTEX(&s_FpgeDevice.cmd_sem);



	/*创建地址模式临时缓存*/
	s_FpgeDevice.cmd_size = FPGA_CMD_NUM;
	s_FpgeDevice.cmd_array = kmalloc(   sizeof( struct fpga_cmd )* s_FpgeDevice.cmd_size , GFP_KERNEL);
	if(s_FpgeDevice.cmd_array)
	{
		s_FpgeDevice.cmd_idx = 0;
	}
	else
	{
		printk(KERN_INFO "Error s_FpgeDevice  No Memory for fpgacmd.\n");
		lRet=- ENOMEM;
		return lRet;

	}

	AT91_SYS->EBI_SMC2_CSR[5] =  AT91C_SMC2_BAT|AT91C_SMC2_DBW_16|AT91C_SMC2_WSEN|AT91C_SMC2_RWHOLD_2_CYCLE|2;

	return lRet;
}





static __exit void FPGA_Terminate(void)
{
	dev_t devno = MKDEV(fpga_major, fpga_minor);
	cdev_del(&s_FpgeDevice.dev);

	tasklet_kill(&s_FpgeDevice.r_tasklet);
	kfifo_free(s_FpgeDevice.r_fifo);
	kfree(s_FpgeDevice.r_buf);
	kfree(s_FpgeDevice.r_user_buf);

	kfree(s_FpgeDevice.w_buf);

	unregister_chrdev_region(devno, 1);

	printk("Driver Exit.\n");
}


/**
* 同时只能打开一个实例. 
*/
static int FPGA_Open(struct inode *inode, struct file *filp)
{
	int lRet;
	spin_lock(&s_FpgeDevice.refs_lock);

	if (s_FpgeDevice.refs > 1)
	{
		lRet =  - ETXTBSY;
	}
	else
	{
		s_FpgeDevice.force_mark = 1;
		/*启动读软中断定时器*/
		s_FpgeDevice.r_sync = 1;
		init_timer(&s_FpgeDevice.r_timer);

		s_FpgeDevice.r_timer.expires = jiffies + msecs_to_jiffies(10); // 
		s_FpgeDevice.r_timer.data = 0; // 
		s_FpgeDevice.r_timer.function = FPGA_ReadTimer; // 
		add_timer(&s_FpgeDevice.r_timer); //最后，激活定时器

		s_FpgeDevice.refs++;
		lRet = 0;
	}

	spin_unlock(&s_FpgeDevice.refs_lock);

	return lRet;
}




static ssize_t FPGA_Write(struct file *filp, const char __user *buff, size_t count, loff_t *off)
{
	int lActLen;

	if (down_interruptible(&s_FpgeDevice.w_sem)){
		return  - ERESTARTSYS;
	}

	lActLen = 0;
	if (count <= FPGA_IO_BUFF_SIZE)
	{
		if (copy_from_user(s_FpgeDevice.w_buf, buff, count))
		{
			lActLen =  -EFAULT;
		}
		else
		{
			unsigned char *plPHYAddr;
			int i, lPayloadSize, lLeft;

			plPHYAddr = s_FpgeDevice.base;

			lPayloadSize = s_FpgeDevice.w_buf[1] * 4;//协议
			if ((lPayloadSize >= 0) && (lPayloadSize <= FPGA_IO_MAX_PAYLOAD_SIZE))
			{
				if ((count - 4) == lPayloadSize)//保证写入的数据符合协议！
				{
					/*往FPGA写入*/
					i = 0;
					lLeft = lPayloadSize + 4;

 					//DEBUG_PrintDataBlock("From Fifo To FPGA", s_FpgeDevice.w_buf, lLeft);

					spin_lock(&s_FpgeDevice.io_spinlock);//读写锁

					while(lLeft)
					{
						iowrite16((s_FpgeDevice.w_buf[i]), plPHYAddr);
						plPHYAddr +=2;
						lLeft--;
						i++;
					}

					spin_unlock(&s_FpgeDevice.io_spinlock);

				}
				else
				{
					/*错误！！ 需要重新同步！*/
					printk("Read WBUF Payload Failed!!!!!!!\n");
				}
			}
			else
			{
				/*错误！！ 需要重新同步！*/
				printk("Read WBUF Payload Size Error!!!!!!!\n");
			}

		}
	}

	up(&s_FpgeDevice.w_sem);

	return lActLen;
}


static ssize_t FPGA_Read(struct file *filp, char __user *buff, size_t count, loff_t *off)
{
	int lActLen, lPayloadSize, lTmpSize;

	lActLen = 0;
	if (count >= FPGA_IO_BUFF_SIZE)
	{
		lTmpSize = kfifo_get(s_FpgeDevice.r_fifo, s_FpgeDevice.r_user_buf, FPGA_IO_BUFF_SIZE);//先读取协议头4个字节
		if (lTmpSize == FPGA_IO_BUFF_SIZE)
		{
			lPayloadSize = s_FpgeDevice.r_user_buf[1] * 4;//协议计算长度
			if ((lPayloadSize >= 0) && (lPayloadSize <= FPGA_IO_MAX_PAYLOAD_SIZE))
			{
 				//DEBUG_PrintDataBlock("From FIFO To Usr", s_FpgeDevice.r_user_buf, lPayloadSize + 4);
				if (copy_to_user(buff, s_FpgeDevice.r_user_buf, lPayloadSize + 4))//复制到用户空间
				{
					lActLen =  -EFAULT;
				}
				else
				{
					lActLen = lPayloadSize + 4;
				}
			}
			else
			{
				/*错误！！ 需要重新同步！*/
				DEBUG_PrintDataBlock("Head", s_FpgeDevice.r_user_buf, 4);
				kfifo_reset(s_FpgeDevice.r_fifo);
				printk("Read RFIFO Payload Size Error!!!!!!!\n");
				lActLen =  0;
			}
		}
		else
		{
			/*错误！！ 需要重新同步！*/
			if (lTmpSize != 0)
			{
				kfifo_reset(s_FpgeDevice.r_fifo);
				printk("Read R FIFO Failed!!!!!!!\n");
				lActLen =  -EFAULT;
			}
		}
	}
	else
	{
		lActLen =  -ENOENT;
	}
	return lActLen;
}


static int FPGA_IOCTL(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	int lRet = 0;
	struct fpga_cmd __fpga_cmd;

	if (_IOC_TYPE(cmd) != FPGA_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > FPGA_IOC_MAXNR) return -ENOTTY;


	switch(cmd)
	{
	case FPGA_IOC_CLEAR:
		if (down_interruptible(&s_FpgeDevice.cmd_sem))
		{
			return  - ERESTARTSYS;
		}
		s_FpgeDevice.cmd_idx = 0;
		lRet=0;
		PDEBUG("s_FpgeDevice cmd clear\n");
		up(&s_FpgeDevice.cmd_sem);
		break;

	case FPGA_IOC_ADD:
		if (down_interruptible(&s_FpgeDevice.cmd_sem))
		{
			return  - ERESTARTSYS;
		}
		if(s_FpgeDevice.cmd_idx >  s_FpgeDevice.cmd_size)
		{
			lRet=-EMSGSIZE;
		}
		else
		{
			lRet=copy_from_user( &__fpga_cmd, (struct fpga_cmd __user  *)arg, sizeof(struct fpga_cmd ) ) ;
			if(lRet==0)
			{

				PDEBUG("s_FpgeDevice cmd add\n");
				if( __fpga_cmd.addr>= (SZ_64K/2) )
				{
					lRet=-EMSGSIZE;
				}
				else
				{
					s_FpgeDevice.cmd_array[ s_FpgeDevice.cmd_idx++] = __fpga_cmd;
				}
			}
		}
		up(&s_FpgeDevice.cmd_sem);
		break;
	case FPGA_IOC_SEND:
		if (down_interruptible(&s_FpgeDevice.cmd_sem))
		{
			return  - ERESTARTSYS;
		}
		FPGA_CMDWrite(s_FpgeDevice.cmd_array,s_FpgeDevice.cmd_idx);
		lRet=0;
		up(&s_FpgeDevice.cmd_sem);
		break;
	}


	return lRet;
}


static int FPGA_Release(struct inode *inode, struct file *filp)
{
	int lRet;

	spin_lock(&s_FpgeDevice.refs_lock);

	s_FpgeDevice.refs--;

	s_FpgeDevice.force_mark = 0;
	del_timer(&s_FpgeDevice.r_timer);//关闭定时器

	lRet = 0;
	spin_unlock(&s_FpgeDevice.refs_lock);

	return lRet;

}


static loff_t FPGA_LLSeek(struct file *filp, loff_t off, int whence)//空实现，不允许seek
{
	unsigned int lActLen;
	lActLen = kfifo_len(s_FpgeDevice.r_fifo);
	return lActLen;
}


static void FPGA_CMDWrite(struct fpga_cmd *array, size_t size)
{
	int i;
	unsigned short *base;
	unsigned short *address;
	unsigned short val;

	PDEBUG("write fpga_cmd array:%d\n",size);

	spin_lock(&s_FpgeDevice.io_spinlock);//读写锁

	base=(unsigned short *)s_FpgeDevice.base;
	for(i=0;i<size;i++)
	{
		address=base+s_FpgeDevice.cmd_array[i].addr ;
		val=s_FpgeDevice.cmd_array[i].value;
		iowrite16( val, address);
	}

	spin_unlock(&s_FpgeDevice.io_spinlock);
}


module_init(FPGA_Initiate);
module_exit(FPGA_Terminate);



