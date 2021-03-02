/*
*	�豸�ļ�·��:/lDev/s_FpgeDevice.
*	���豸��:211 ���豸��;1
*	��д����ʹ�û�����.
*	ʹ�ô򿪼�������ʹs_FpgeDevice�豸ֻ�ܱ���һ��.
*	�������ڲ��ڶ�/д������ʹ��ͬһ��������,���û�̬�����ڽ��ж�дʱ����ʹ���û�̬����
*	�����д����������С�
*/

#include <linux/config.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/sched.h>	//����
#include <linux/kthread.h>	//�ں��߳�
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
#include <linux/kfifo.h>	//�ں�FIFO����
#include <asm/system.h>		/* cli(), *_flags */
#include <asm/uaccess.h>	/* copy_*_user */
#include <asm/arch-at91rm9200/gpio.h>
#include <asm/io.h>
#include "fpga.h"
#include<linux/delay.h>

//#define TEST_INTERFACE
/**	�򿪵��Կ���.	*/
#define DEBUG

/**	��ӡ����.	*/
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
	/*���ü���������*/
	int						refs;
	spinlock_t				refs_lock;

	/*����ռ��д�ṹ*/
	unsigned char 			*base;//FPGA ���ʻ�����ַ
	spinlock_t				io_spinlock;
	
	/*��ģʽ�ӿ�*/
	unsigned char			*w_buf;//д����
	struct semaphore		w_sem;//���߳�д������

	struct tasklet_struct	r_tasklet;
	unsigned char			*r_buf;//������
	unsigned char			*r_user_buf;//���û�����
	struct kfifo			*r_fifo;//������
	struct task_struct		*r_task;//������
	struct timer_list		r_timer;
	int						r_sync;
	int						force_mark;

	/*����ģʽ�ӿ�*/
	struct fpga_cmd		*cmd_array;
	size_t				cmd_size;
	size_t				cmd_idx;
	struct semaphore	cmd_sem;//���߳�д������


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
	tasklet_schedule(&s_FpgeDevice.r_tasklet);//����FPGA��ȡС����
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

	spin_lock(&s_FpgeDevice.io_spinlock);//��д��

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
		if ((kfifo_len(s_FpgeDevice.r_fifo) + FPGA_IO_BUFF_SIZE) <= FPGA_DRIVER_KERNEL_R_FIFO_SIZE)//�ж��Ƿ��д�뵽FIFO�У�ȷ����ʣ��һ�������Ļ�������
		{
			plPHYAddr = s_FpgeDevice.base;

			s_FpgeDevice.r_buf[0] = ioread16(plPHYAddr) & 0xFF;
			plPHYAddr +=2;/*��Ϊû�н�A0�����ַ������*/
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
					i = 4;//ͷ�Ѿ�����ȡ
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
					/*��������Ҳ��Ҫ��ȡȻ����������ڿ�����������*/
					for (i = 0; i < lPayloadSize ; i++)
					{
						ioread16(plPHYAddr);
						plPHYAddr +=2;
					}

					/*���ݴ���*/
					break;
				}
				udelay(5);
			}
			else
			{
				/*û�����ݿɶ�*/
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

	/*�Զ����ʼ������----------------------------------------------------------*/

	/*��ʼ������ӿ�*/
	if((s_FpgeDevice.base = ioremap(FPGA_BASE_ADDRESS , 0x800) ) == NULL )
	{
		lRet=-EFAULT;
		return lRet;
	}

	printk("Map ARM Base Address = 0x%08X Map To = 0x%08X\n", FPGA_BASE_ADDRESS, s_FpgeDevice.base);

	/*������ѭ������*/
	s_FpgeDevice.r_fifo = kfifo_alloc(FPGA_DRIVER_KERNEL_R_FIFO_SIZE, GFP_KERNEL, NULL);
	if (!s_FpgeDevice.r_fifo)
	{
		printk(KERN_INFO "Create r_fifo Failed, Code = %d.\n", lRet);
		return lRet;
	}

	/*��������ʱ����*/
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

	/*����д��ʱ����*/
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
	/*��ʼ��д��*/
	init_MUTEX(&s_FpgeDevice.w_sem);
	init_MUTEX(&s_FpgeDevice.cmd_sem);



	/*������ַģʽ��ʱ����*/
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
* ͬʱֻ�ܴ�һ��ʵ��. 
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
		/*���������ж϶�ʱ��*/
		s_FpgeDevice.r_sync = 1;
		init_timer(&s_FpgeDevice.r_timer);

		s_FpgeDevice.r_timer.expires = jiffies + msecs_to_jiffies(10); // 
		s_FpgeDevice.r_timer.data = 0; // 
		s_FpgeDevice.r_timer.function = FPGA_ReadTimer; // 
		add_timer(&s_FpgeDevice.r_timer); //��󣬼��ʱ��

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

			lPayloadSize = s_FpgeDevice.w_buf[1] * 4;//Э��
			if ((lPayloadSize >= 0) && (lPayloadSize <= FPGA_IO_MAX_PAYLOAD_SIZE))
			{
				if ((count - 4) == lPayloadSize)//��֤д������ݷ���Э�飡
				{
					/*��FPGAд��*/
					i = 0;
					lLeft = lPayloadSize + 4;

 					//DEBUG_PrintDataBlock("From Fifo To FPGA", s_FpgeDevice.w_buf, lLeft);

					spin_lock(&s_FpgeDevice.io_spinlock);//��д��

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
					/*���󣡣� ��Ҫ����ͬ����*/
					printk("Read WBUF Payload Failed!!!!!!!\n");
				}
			}
			else
			{
				/*���󣡣� ��Ҫ����ͬ����*/
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
		lTmpSize = kfifo_get(s_FpgeDevice.r_fifo, s_FpgeDevice.r_user_buf, FPGA_IO_BUFF_SIZE);//�ȶ�ȡЭ��ͷ4���ֽ�
		if (lTmpSize == FPGA_IO_BUFF_SIZE)
		{
			lPayloadSize = s_FpgeDevice.r_user_buf[1] * 4;//Э����㳤��
			if ((lPayloadSize >= 0) && (lPayloadSize <= FPGA_IO_MAX_PAYLOAD_SIZE))
			{
 				//DEBUG_PrintDataBlock("From FIFO To Usr", s_FpgeDevice.r_user_buf, lPayloadSize + 4);
				if (copy_to_user(buff, s_FpgeDevice.r_user_buf, lPayloadSize + 4))//���Ƶ��û��ռ�
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
				/*���󣡣� ��Ҫ����ͬ����*/
				DEBUG_PrintDataBlock("Head", s_FpgeDevice.r_user_buf, 4);
				kfifo_reset(s_FpgeDevice.r_fifo);
				printk("Read RFIFO Payload Size Error!!!!!!!\n");
				lActLen =  0;
			}
		}
		else
		{
			/*���󣡣� ��Ҫ����ͬ����*/
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
	del_timer(&s_FpgeDevice.r_timer);//�رն�ʱ��

	lRet = 0;
	spin_unlock(&s_FpgeDevice.refs_lock);

	return lRet;

}


static loff_t FPGA_LLSeek(struct file *filp, loff_t off, int whence)//��ʵ�֣�������seek
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

	spin_lock(&s_FpgeDevice.io_spinlock);//��д��

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



