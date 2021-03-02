#include "io_ctrl_driver.h"

#define PULLD_EN 1
#define PULLD_DIS 0
#define LEVEL_HIGH 1
#define LEVEL_LOW 0

#define DRL_TRACE(x) \
	do{ \
	udelay(10); \
	printk("[%s][%s][%d]", IO_CTRL_DRIVER_NAME, __FUNCTION__, __LINE__); \
	printk x; \
	printk("\n"); \
	}while(0) 
#define DRL_TRUE 1
#define DRL_FALSE 0

static int IOCtrl_Open(struct inode *inode, struct file *filep )
{
	return 0;
}

static int IOCtrl_Release(struct inode *inode, struct file *filep )
{
	return 0;
}

static const struct
{
	int m_ResetPin;
	int m_AudioResetPin;
	int m_AudioByPassPin;
}sc_IOCtrl_Pin[2] = 
{
	{IO_CTRL_CVBS1_RESET_PIN, IO_CTRL_CVBS1_AUDIO_RESET_PIN, IO_CTRL_CVBS1_AUDIO_BYPASS_PIN},
	{IO_CTRL_CVBS2_RESET_PIN, IO_CTRL_CVBS2_AUDIO_RESET_PIN, IO_CTRL_CVBS2_AUDIO_BYPASS_PIN}
};

/* 包含音频引脚复位，cvbs芯片复位 */
static int IOCtrl_CvbsReset(int Index)
{
	at91_set_gpio_value(sc_IOCtrl_Pin[Index].m_ResetPin, LEVEL_HIGH);
	at91_set_gpio_value(sc_IOCtrl_Pin[Index].m_AudioResetPin, LEVEL_HIGH);
	mdelay(10); /* datasheet need >1ms */
	at91_set_gpio_value(sc_IOCtrl_Pin[Index].m_ResetPin, LEVEL_LOW);
	at91_set_gpio_value(sc_IOCtrl_Pin[Index].m_AudioResetPin, LEVEL_LOW);
	mdelay(10); /* datasheet need >200ns */
	at91_set_gpio_value(sc_IOCtrl_Pin[Index].m_AudioByPassPin, LEVEL_LOW);
	at91_set_gpio_value(sc_IOCtrl_Pin[Index].m_ResetPin, LEVEL_HIGH);
	at91_set_gpio_value(sc_IOCtrl_Pin[Index].m_AudioResetPin, LEVEL_HIGH);
	mdelay(10); /* datasheet need >1ms */

	return DRL_TRUE;
}

static void IOCtrl_GpioInit(int Index)
{
	at91_set_gpio_output(sc_IOCtrl_Pin[Index].m_ResetPin, PULLD_EN);
	at91_set_gpio_output(sc_IOCtrl_Pin[Index].m_AudioResetPin, PULLD_EN);
	at91_set_gpio_output(sc_IOCtrl_Pin[Index].m_AudioByPassPin, PULLD_EN);
	udelay(10);
}

static int IOCtrl_Ioctl(struct inode *inode, struct file *filep, unsigned int cmd, unsigned long arg)
{
	unsigned long size;

	if(_IOC_TYPE(cmd) != IO_CTRL_IOCTL_MAGIE) 
		return -EINVAL;
	if(_IOC_NR(cmd) >= IO_CTRL_IOCTL_MAXNR)
		return -EINVAL;

	size = _IOC_SIZE(cmd);
	switch(cmd)
	{
	case IO_CTRL_CVBS1_RESET: /* 每次控制引脚时修改引脚方向，这样做是为了如果有IO口复用，不会出错 */
		IOCtrl_GpioInit(0);
		if (IOCtrl_CvbsReset(0) == DRL_FALSE) 
		{
			DRL_TRACE(("Cvbs I2c reset error!"));
			return -1;
		}
		break;	
	case IO_CTRL_CVBS2_RESET:
		IOCtrl_GpioInit(1);
		if (IOCtrl_CvbsReset(1) == DRL_FALSE) 
		{
			DRL_TRACE(("Cvbs I2c reset error!"));
			return -1;
		}
		break;	
	default: 
		DRL_TRACE(("cmd error!"));
		return -EINVAL;
		break;
	}

	return 0;
}

struct file_operations IO_CTRL_device_fops=
{
	.owner = THIS_MODULE,
	.open = IOCtrl_Open,
	.release = IOCtrl_Release,
	.ioctl = IOCtrl_Ioctl,
};

static struct class *s_DriverClass;
static struct class_device *s_DriverDevice;
static int s_Major;
static int __init IOCtrl_Init(void)
{
	DRL_TRACE(("%s Register", IO_CTRL_DRIVER_NAME));
	s_Major = register_chrdev(0, IO_CTRL_DRIVER_NAME, &IO_CTRL_device_fops);

	if(s_Major<0)
		return s_Major;

	s_DriverClass = class_create(THIS_MODULE, IO_CTRL_DRIVER_NAME);  
	if (IS_ERR(s_DriverClass)) {
		DRL_TRACE(("class create error!"));
		return -1;
	}
	s_DriverDevice = class_device_create(s_DriverClass, MKDEV(s_Major, 0), NULL, IO_CTRL_NODE_NAME);  
	devfs_mk_cdev(MKDEV(s_Major, 0), S_IFCHR | S_IRUGO | S_IWUGO, IO_CTRL_NODE_NAME);

	return 0;
}
static void __exit IOCtrl_Exit(void)
{
	devfs_remove(IO_CTRL_NODE_NAME);
	unregister_chrdev(s_Major,IO_CTRL_DRIVER_NAME);
	class_device_unregister(s_DriverDevice);  
	class_destroy(s_DriverClass);     
}

module_init(IOCtrl_Init);
module_exit(IOCtrl_Exit);

MODULE_AUTHOR(IO_CTRL_AUTHOR);
MODULE_DESCRIPTION(IO_CTRL_DESCRIPTION);
MODULE_LICENSE(IO_CTRL_LICENSE);
