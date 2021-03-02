#include "fpga_ps_driver.h"

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/config.h>
#include <asm/arch/AT91RM9200_SYS.h>
#include <asm/arch/hardware.h>
#include <asm/arch/board.h>
#include <asm/arch/pio.h>
#include <asm/arch/gpio.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/device.h>
#include <linux/devfs_fs_kernel.h>
#include <linux/delay.h>

#define PULLD_EN 1
#define PULLD_DIS 0
#define LEVEL_HIGH 1
#define LEVEL_LOW 0

#define DRL_TRACE(x) \
do{ \
	udelay(10); \
	printk("[%s][%s][%d]", FPGA_PS_DRIVER_NAME, __FUNCTION__, __LINE__); \
	printk x; \
	printk("\n"); \
}while(0) 
#define DRL_TRUE 1
#define DRL_FALSE 0

struct stFpgaPSConfig 
{
	int m_FpgaIndex;
	int m_DclkPin;
	int m_nConfigPin;
	int m_nStatusPin;
	int m_DataPin;
#ifdef FPGA_PS_CONF_DONE_USE
	int m_ConfigDonePin;
#endif
};

#ifdef FPGA_PS_CONF_DONE_USE
static const struct stFpgaPSConfig  sc_FpgaPSConfig[GN_FPGA_NUM] = 
{
	{GN_FPGA_INDEX_MAIN,				FPGA_MAIN_DCLK_PIN,				FPGA_MAIN_nCONFIG_PIN,				FPGA_MAIN_nSTATUS_PIN,				FPGA_MAIN_DATA_PIN,				FPGA_MAIN_CONF_DONE_PIN},
	{GN_FPGA_INDEX_IP_ASI_OUTPUT,FPGA_IP_ASI_OUTPUT_DCLK_PIN,FPGA_IP_ASI_OUTPUT_nCONFIG_PIN,FPGA_IP_ASI_OUTPUT_nSTATUS_PIN,FPGA_IP_ASI_OUTPUT_DATA_PIN,	FPGA_IP_ASI_OUTPUT_CONF_DONE_PIN},
};
#else
static const struct stFpgaPSConfig  sc_FpgaPSConfig[GN_FPGA_NUM] = 
{
	{GN_FPGA_INDEX_MAIN,				FPGA_MAIN_DCLK_PIN,				FPGA_MAIN_nCONFIG_PIN,				FPGA_MAIN_nSTATUS_PIN,				FPGA_MAIN_DATA_PIN},
	{GN_FPGA_INDEX_IP_ASI_OUTPUT,FPGA_IP_ASI_OUTPUT_DCLK_PIN,FPGA_IP_ASI_OUTPUT_nCONFIG_PIN,FPGA_IP_ASI_OUTPUT_nSTATUS_PIN,FPGA_IP_ASI_OUTPUT_DATA_PIN},
};
#endif

static int s_FpgaIndex = 0;

static int FpgaPS_Open(struct inode *inode, struct file *filep )
{
	return 0;
}

static int FpgaPS_Release(struct inode *inode, struct file *filep )
{
	return 0;
}

static void FpgaPS_GpioInit(void)
{
	at91_set_gpio_input(sc_FpgaPSConfig[s_FpgaIndex].m_nStatusPin, PULLD_EN);
#ifdef FPGA_PS_CONF_DONE_USE
	at91_set_gpio_input(sc_FpgaPSConfig[s_FpgaIndex].m_ConfigDonePin, PULLD_EN);
#endif
	at91_set_gpio_output(sc_FpgaPSConfig[s_FpgaIndex].m_DclkPin, PULLD_EN);
	at91_set_gpio_output(sc_FpgaPSConfig[s_FpgaIndex].m_DataPin, PULLD_EN);
	at91_set_gpio_output(sc_FpgaPSConfig[s_FpgaIndex].m_nConfigPin, PULLD_EN);
}

#define NOOP
static void FpgaPS_WriteData(const char *pData, unsigned long DataSize)
{
	unsigned long i = 0, j;
	char lCh;

	if(DataSize > 0)
	{   
		while(i < DataSize)   
		{			 
			lCh = pData[i];  
			for(j = 0; j < 8; j++) /* 标准PS下载模式 */
			{ 	
				if((lCh >> j) & 0x01)							
				{
					at91_set_gpio_value(sc_FpgaPSConfig[s_FpgaIndex].m_DataPin, LEVEL_HIGH);
				}						
				else
				{
					at91_set_gpio_value(sc_FpgaPSConfig[s_FpgaIndex].m_DataPin, LEVEL_LOW);
				}
				//ndelay(100); /* 时序图这里要求延时，当实际这里Arm的语句执行时间已经超过这个时间，所以用一个空语句代替 */
				NOOP;
				at91_set_gpio_value(sc_FpgaPSConfig[s_FpgaIndex].m_DclkPin, LEVEL_HIGH);
				//ndelay(100);
				NOOP;
				at91_set_gpio_value(sc_FpgaPSConfig[s_FpgaIndex].m_DclkPin, LEVEL_LOW);
			}   
			i++;
		}
	}
}

static int FpgaPS_Write(struct file *fileP, const char __user*pData, size_t count, loff_t * f_pos)
{   
	size_t i = count;
	const char *p = pData;
	char *plBuf = (char *)kmalloc(FPGA_PS_DATA_BUFFER_SIZE, GFP_KERNEL);

	if (plBuf == NULL)
	{
		DRL_TRACE(("Error:kmalloc failed!"));
		return -1;
	}
	
	while (i > FPGA_PS_DATA_BUFFER_SIZE) 
	{
		if(copy_from_user(plBuf, p, FPGA_PS_DATA_BUFFER_SIZE))
		{
			kfree(plBuf);
			return -1;
		}
		FpgaPS_WriteData(plBuf, FPGA_PS_DATA_BUFFER_SIZE);
		DRL_TRACE(("Remain Bytes:%u", i));

		p += FPGA_PS_DATA_BUFFER_SIZE;
		i -= FPGA_PS_DATA_BUFFER_SIZE;
	}

	if (i > 0)
	{
		if(copy_from_user(plBuf, p, i))
		{
			kfree(plBuf);
			return -1;
		}
		FpgaPS_WriteData(plBuf, i);
	}
	
	kfree(plBuf);
	return count ;
}

static int FpgaPS_Select(int FpgaIndex)
{
	int i;

	for (i = 0; i < GN_FPGA_NUM; i++) 
	{
		if (FpgaIndex == sc_FpgaPSConfig[i].m_FpgaIndex) 
		{
			s_FpgaIndex = i;
			return DRL_TRUE;
		}
	}

	return DRL_FALSE;
}

/* PS下载rbf前的初始化 */
static int FpgaPS_PinInit(void)
{
	int lTimeOutCounter = 0xffff; /* 超时计数 */
	int lData = 0;

	at91_set_gpio_value(sc_FpgaPSConfig[s_FpgaIndex].m_DclkPin, LEVEL_LOW);
	at91_set_gpio_value(sc_FpgaPSConfig[s_FpgaIndex].m_nConfigPin, LEVEL_HIGH);

	mdelay(10); /* PS下载文档要求此处延时10ms */

	at91_set_gpio_value(sc_FpgaPSConfig[s_FpgaIndex].m_nConfigPin, LEVEL_LOW);

	mdelay(10); /* PS下载文档要求此处延时10ms */

	lTimeOutCounter = 0xffff;
	do {
		lData = at91_get_gpio_value(sc_FpgaPSConfig[s_FpgaIndex].m_nStatusPin);
		ndelay(10000);
	}while (lData == LEVEL_HIGH && lTimeOutCounter-- > 0);
	if (lTimeOutCounter <= 0) 
	{
		DRL_TRACE(("read nstatus err! expected 0, but readed 1"));
		return DRL_FALSE;
	}

#ifdef FPGA_PS_CONF_DONE_USE
	lTimeOutCounter = 0xffff;
	do {
		lData = at91_get_gpio_value(sc_FpgaPSConfig[s_FpgaIndex].m_ConfigDonePin);
		ndelay(10000);
	}while (lData == LEVEL_HIGH && lTimeOutCounter-- > 0);
	if (lTimeOutCounter <= 0) 
	{
		DRL_TRACE(("read configdone err! expected 0, but readed 1"));
		return DRL_FALSE;
	}
#endif
	
	at91_set_gpio_value(sc_FpgaPSConfig[s_FpgaIndex].m_nConfigPin, LEVEL_HIGH);

	lTimeOutCounter = 0xffff;
	do {
		lData = at91_get_gpio_value(sc_FpgaPSConfig[s_FpgaIndex].m_nStatusPin);
		ndelay(10000);
	}while (lData == LEVEL_LOW && lTimeOutCounter-- > 0);
	if (lTimeOutCounter <= 0) 
	{
		DRL_TRACE(("read nstatus err! expected 1, but readed 0"));
		return DRL_FALSE;
	}

	return DRL_TRUE;
}

static int FpgaPS_IsEnd(void)
{
	int lTimeOutCounter = 0xffff;
	int lData = 0;
	
#ifdef FPGA_PS_CONF_DONE_USE
	lTimeOutCounter = 0xffff;
	do {
		lData = at91_get_gpio_value(sc_FpgaPSConfig[s_FpgaIndex].m_ConfigDonePin);
		ndelay(10000);
	}while (lData == LEVEL_LOW && lTimeOutCounter-- > 0);
	if (lTimeOutCounter <= 0) 
	{
		DRL_TRACE(("read configdone err! expected 1, but readed 0"));
		return DRL_FALSE;
	}
#endif
	
	mdelay(1); /* datasheet need 650us */
	
	return DRL_TRUE;
}

/* 在FPGA的nconfig脚使用一个低脉冲停止rbf的程序运行，这样后必须重新下载rbf程序 */
static void FpgaPS_CleanProg(void)
{
	at91_set_gpio_value(sc_FpgaPSConfig[s_FpgaIndex].m_nConfigPin, LEVEL_LOW);
	mdelay(10); 
	at91_set_gpio_value(sc_FpgaPSConfig[s_FpgaIndex].m_nConfigPin, LEVEL_HIGH);
}

static int FpgaPS_Ioctl(struct inode *inode, struct file *filep, unsigned int cmd, unsigned long arg)
{
	unsigned char lData;
	unsigned long size;

	if(_IOC_TYPE(cmd) != FPGA_PS_IOCTL_MAGIE) 
		return -EINVAL;
	if(_IOC_NR(cmd) >= FPGA_PS_IOCTL_MAXNR)
		return -EINVAL;

	size = _IOC_SIZE(cmd);
	switch(cmd)
	{
		case FPGA_PS_SELECT:
			copy_from_user((void *)&lData, (const void *)arg, size);
			if (FpgaPS_Select(lData) == DRL_FALSE) 
				DRL_TRACE(("Fpga select error!"));
			break;	
		case FPGA_PS_PIN_INIT:
			if (FpgaPS_PinInit() == DRL_FALSE) {
				DRL_TRACE(("Fpga Init Pin error!"));
				lData = 0; /* 0表示失败，1表示成功 */
			}
			else 
			{
				lData = 1; 
			}
			copy_to_user((void *)arg, (void *)&lData, size);
			break;
		case FPGA_PS_PIN_END:
			if (FpgaPS_IsEnd() == DRL_FALSE) {
				DRL_TRACE(("Fpga End Pin error!"));
				lData = 0; /* 0表示失败，1表示成功 */
			}
			else 
			{
				lData = 1; 
			}
			copy_to_user((void *)arg, (void *)&lData, size);
			break;
		case FPGA_PS_CLEAN_PROG:
			FpgaPS_CleanProg();
			break;
		case FPGA_PS_GPIO_INIT:
			FpgaPS_GpioInit();
			break;
		default: 
			DRL_TRACE(("cmd error!"));
			return -EINVAL;
			break;
	}

	return 0;
}

struct file_operations fpga_ps_device_fops=
{
	.owner = THIS_MODULE,
	.open = FpgaPS_Open,
	.release = FpgaPS_Release,
	.ioctl = FpgaPS_Ioctl,
	.write=FpgaPS_Write,
};

static struct class *s_DriverClass;
static struct class_device *s_DriverDevice;
static int s_Major;
static int __init FpgaPS_Init(void)
{
	DRL_TRACE(("%s Register", FPGA_PS_DRIVER_NAME));
	s_Major = register_chrdev(0, FPGA_PS_DRIVER_NAME, &fpga_ps_device_fops);

	if(s_Major<0)
		return s_Major;

	s_DriverClass = class_create(THIS_MODULE, FPGA_PS_DRIVER_NAME);  
	if (IS_ERR(s_DriverClass)) {
		DRL_TRACE(("class create error!"));
		return -1;
	}
	s_DriverDevice = class_device_create(s_DriverClass, MKDEV(s_Major, 0), NULL, FPGA_PS_NODE_NAME);  
	devfs_mk_cdev(MKDEV(s_Major, 0), S_IFCHR | S_IRUGO | S_IWUGO, FPGA_PS_NODE_NAME);

	return 0;
}
static void __exit FpgaPS_Exit(void)
{
	devfs_remove(FPGA_PS_NODE_NAME);
	unregister_chrdev(s_Major,FPGA_PS_DRIVER_NAME);
	class_device_unregister(s_DriverDevice);  
	class_destroy(s_DriverClass);     
}

module_init(FpgaPS_Init);
module_exit(FpgaPS_Exit);

MODULE_AUTHOR(FPGA_PS_AUTHOR);
MODULE_DESCRIPTION(FPGA_PS_DESCRIPTION);
MODULE_LICENSE(FPGA_PS_LICENSE);
/* EOF */
