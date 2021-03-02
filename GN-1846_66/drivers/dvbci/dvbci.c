/*
*	�豸�ļ�·��:/lDev/s_FPGACIDevice.
*	���豸��:211 ���豸��;1
*	��д����ʹ�û�����.
*	ʹ�ô򿪼�������ʹs_FPGACIDevice�豸ֻ�ܱ���һ��.
*	�������ڲ��ڶ�/д������ʹ��ͬһ��������,���û�̬�����ڽ��ж�дʱ����ʹ���û�̬����
*	�����д����������С�
*/

#include <linux/config.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/sched.h>	//����
#include <linux/kthread.h>	//�ں��߳�
#include <linux/kernel.h>	/* KDEBUG() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>		/* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h> 
#include <linux/timer.h>
#include <linux/kfifo.h>	//�ں�FIFO����
#include <asm/system.h>		/* cli(), *_flags */
#include <asm/uaccess.h>	/* copy_*_user */
#include <asm/arch-at91rm9200/gpio.h>
#include <asm/io.h>
#include<linux/delay.h>
#include "dvbci.h"

/**	fpga-IO�ռ����ַ.*/
#define FGPA_CI_BASE_ADDRESS  0x30000000

/**	�򿪵��Կ���.	*/
#define DEBUG

#ifdef DEBUG
#define KDEBUG(fmt,args...)  printk(KERN_INFO fmt, ## args )
#else
#define KDEBUG(fmt,args...) 	
#endif


#define AT91C_SMC2_RWHOLD_1_CYCLE     ((unsigned int) 0x1 << 28) // (SMC2) Read and Write Signal Hold Time	//hzh
#define AT91C_SMC2_RWHOLD_2_CYCLE     ((unsigned int) 0x2 << 28) // (SMC2) Read and Write Signal Hold Time	//hzh



MODULE_LICENSE("GPL");

static ssize_t FPGACI_Read(struct file *filp, char __user *buff, size_t size, loff_t *off);
static ssize_t FPGACI_Write(struct file *filp, const char __user *buff, size_t size, loff_t *off);
static int FPGACI_Open(struct inode *inode, struct file *filp);
static int FPGACI_Release(struct inode *, struct file*);
static loff_t FPGACI_LLSeek(struct file *filp, loff_t off, int whence);
static int FPGACI_IOCTL(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);

struct file_operations s_FpgaFOP =
{
	.owner =	THIS_MODULE, 
	.read =		FPGACI_Read, 
	.write =	FPGACI_Write, 
	.llseek =	FPGACI_LLSeek, 
	.open =		FPGACI_Open, 
	.release =	FPGACI_Release,
	.ioctl =	FPGACI_IOCTL
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

} s_FPGACIDevice;


static int fpga_ci_major = FPGA_CI_MAJOR;
static int fpga_ci_minor = FPGA_CI_MINOR;


static void DEBUG_PrintDataBlock(char* pTitle, unsigned char *pBuf, int Size)
{
	int i;
	KDEBUG("%s  ------------------------ %d", pTitle, Size);
	for (i = 0; i < Size; i++)
	{
		if (i % 8 == 0)
		{
			KDEBUG("\n  %.2d - %.2d   ", i, i + 8 - 1);
		}
		else if (i % (8 / 2) == 0)
		{
			KDEBUG(" ");
		}
		KDEBUG("%.2X ", pBuf[i]);
	}
	KDEBUG("\n");
	KDEBUG("%s  ------------------------\n", pTitle);
}



static __init int FPGA_Initiate(void)
{
	int lRet = 0;
	dev_t lDev = 0;
	lDev = MKDEV(fpga_ci_major, fpga_ci_minor);
	lRet = register_chrdev_region(lDev, 1, "s_FPGACIDevice");
	if (lRet < 0)
	{
		KDEBUG("s_FPGACIDevice:can't register major:%d minor:%d", fpga_ci_major, fpga_ci_minor);
		return lRet;
	}

	cdev_init(&s_FPGACIDevice.dev, &s_FpgaFOP);
	s_FPGACIDevice.dev.owner = THIS_MODULE;
	s_FPGACIDevice.dev.ops = &s_FpgaFOP;
	lRet = cdev_add(&s_FPGACIDevice.dev, lDev, 1);
	if (lRet)
	{
		lRet=- ENOENT;
		KDEBUG("Error :%d adding s_FPGACIDevice cdev.\n", lRet);
		return lRet;
	}

	/*�Զ����ʼ������----------------------------------------------------------*/

	/*��ʼ������ӿ�*/
	if((s_FPGACIDevice.base = ioremap(FGPA_CI_BASE_ADDRESS , SZ_512K) ) == NULL )
	{
		lRet=-EFAULT;
		return lRet;
	}

#if 0
	/*��������ʱ����*/
	s_FPGACIDevice.r_buf = kmalloc(FPGA_IO_BUFF_SIZE, GFP_KERNEL);
	if (s_FPGACIDevice.r_buf)
	{
		memset(s_FPGACIDevice.r_buf, 0, FPGA_IO_BUFF_SIZE);
	}
	else
	{
		KDEBUG("Error No Memory For r_buf.\n");
		lRet=- ENOMEM;
		return lRet;
	}
#endif


	AT91_SYS->EBI_SMC2_CSR[2] =  AT91C_SMC2_BAT|AT91C_SMC2_DBW_16|AT91C_SMC2_WSEN|AT91C_SMC2_RWHOLD_2_CYCLE|1;

	KDEBUG("FPGA CI Driver Initiate!\n");
	return lRet;
}





static __exit void FPGA_Terminate(void)
{
	dev_t devno = MKDEV(fpga_ci_major, fpga_ci_minor);
	cdev_del(&s_FPGACIDevice.dev);


	unregister_chrdev_region(devno, 1);

	KDEBUG("FPGA CI Driver Terminate.\n");
}


/**
* ͬʱֻ�ܴ�һ��ʵ��. 
*/
static int FPGACI_Open(struct inode *inode, struct file *filp)
{
	int lRet;
	spin_lock(&s_FPGACIDevice.refs_lock);

	if (s_FPGACIDevice.refs > 1)
	{
		lRet =  - ETXTBSY;
	}
	else
	{
		s_FPGACIDevice.refs++;
		lRet = 0;
	}

	spin_unlock(&s_FPGACIDevice.refs_lock);

	return lRet;
}




static ssize_t FPGACI_Write(struct file *filp, const char __user *buff, size_t count, loff_t *off)
{
	int lActLen = 0;


	return lActLen;
}


static ssize_t FPGACI_Read(struct file *filp, char __user *buff, size_t count, loff_t *off)
{
	int lActLen = 0;

	return lActLen;
}


static int FPGACI_IOCTL(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	int lRet = -EFAULT;
	unsigned char *plTmpBaseAddr;

	if (_IOC_TYPE(cmd) != FPGA_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > FPGA_IOC_MAXNR) return -ENOTTY;

	plTmpBaseAddr = s_FPGACIDevice.base;
	switch(cmd)
	{
	case FPGA_CI_IOC_CMD_WRITE:
		{
			FPGA_CI_CMD lCMD;

			if(copy_from_user(&lCMD, (const char*)arg, sizeof(lCMD)) == 0)
			{
				KDEBUG("CMD Write Addr = 0x%.4X, Value = 0x%.4X\n", lCMD.offset, lCMD.value);

				plTmpBaseAddr += (lCMD.offset << 1);
				iowrite16(lCMD.value, plTmpBaseAddr);
				lRet = 0;
			}
		}
		break;
	case FPGA_CI_IOC_CMD_READ:
		{
			FPGA_CI_CMD lCMD;

			if(copy_from_user(&lCMD, (const char*)arg, sizeof(lCMD)) == 0)
			{

				plTmpBaseAddr += (lCMD.offset << 1);
				lCMD.value = ioread16(plTmpBaseAddr);

				KDEBUG("CMD Read Addr = 0x%.4X, Value = 0x%.4X\n", lCMD.offset, lCMD.value);

				if(copy_to_user((unsigned char *)arg, &lCMD, sizeof(lCMD)) == 0)
				{
					lRet = 0;
				}
			}

		}
		break;
	default:
		break;

	}
	return lRet;
}


static int FPGACI_Release(struct inode *inode, struct file *filp)
{
	int lRet;

	spin_lock(&s_FPGACIDevice.refs_lock);

	s_FPGACIDevice.refs--;

	lRet = 0;

	spin_unlock(&s_FPGACIDevice.refs_lock);

	return lRet;
}


static loff_t FPGACI_LLSeek(struct file *filp, loff_t off, int whence)//��ʵ�֣�������seek
{
	loff_t lActLen = 0;
	return lActLen;
}


module_init(FPGA_Initiate);
module_exit(FPGA_Terminate);



