#include "volume_driver.h"

#define PULLD_EN 1
#define PULLD_DIS 0
#define LEVEL_HIGH 1
#define LEVEL_LOW 0

#define DRL_TRACE(x) \
	do{ \
	udelay(10); \
	printk("[%s][%s][%d]", VOLUME_DRIVER_NAME, __FUNCTION__, __LINE__); \
	printk x; \
	printk("\n"); \
	}while(0) 
#define DRL_TRUE 1
#define DRL_FALSE 0

struct stVolumeConfig
{
	int m_VolumeIndex;
	int m_SdaPin;
	int m_SclPin;
};

static const struct stVolumeConfig  sc_VolumeConfig[GN_VOLUME_NUM] = 
{
	{GN_VOLUME_INDEX_CVBS_CH1,			VOLUME_CVBS1_SDA_PIN,		VOLUME_CVBS1_SCL_PIN},
	{GN_VOLUME_INDEX_CVBS_CH2,			VOLUME_CVBS2_SDA_PIN,		VOLUME_CVBS2_SCL_PIN},
	{GN_VOLUME_INDEX_CVBS_CH3,			VOLUME_CVBS3_SDA_PIN,		VOLUME_CVBS3_SCL_PIN},
	{GN_VOLUME_INDEX_CVBS_CH4,			VOLUME_CVBS4_SDA_PIN,		VOLUME_CVBS4_SCL_PIN},
	{GN_VOLUME_INDEX_CVBS_CH5,			VOLUME_CVBS5_SDA_PIN,		VOLUME_CVBS5_SCL_PIN},
	{GN_VOLUME_INDEX_CVBS_CH6,			VOLUME_CVBS6_SDA_PIN,		VOLUME_CVBS6_SCL_PIN},
	{GN_VOLUME_INDEX_CVBS_CH7,			VOLUME_CVBS7_SDA_PIN,		VOLUME_CVBS7_SCL_PIN},
	{GN_VOLUME_INDEX_CVBS_CH8,			VOLUME_CVBS8_SDA_PIN,		VOLUME_CVBS8_SCL_PIN}
};

static void Volume_GpioInit(int Index)
{
	at91_set_gpio_output(sc_VolumeConfig[Index].m_SclPin, PULLD_EN);
	at91_set_gpio_output(sc_VolumeConfig[Index].m_SdaPin, PULLD_EN);
}

static int Volume_Open(struct inode *inode, struct file *filep)
{
	return 0;
}

static int Volume_Release(struct inode *inode, struct file *filep)
{
	return 0;
}

static int Volume_GetIndex(int VolumeIndex)
{
	int i;

	for (i = 0; i < GN_VOLUME_NUM; i++) 
	{
		if (VolumeIndex == sc_VolumeConfig[i].m_VolumeIndex) 
		{
			return i;
		}
	}

	return -1;
}

static int Volume_Write(struct file *fileP, const char __user*pData,size_t count, loff_t * f_pos)
{   
	DRL_VolumeProtocol lVolumeProto;
	int lIndex = 0, i;
	int lData = 0;

	if (count > sizeof(lVolumeProto)) 
	{
		DRL_TRACE(("Para Error!!"));
		return -EINVAL;
	}

	copy_from_user((char *)&lVolumeProto, pData, count);
	if ((lIndex = Volume_GetIndex(lVolumeProto.m_VolumeIndex)) < 0)
	{
		DRL_TRACE(("VolumeIndex Error!!"));
		return -1;
	}
	
	for (i=0; i<lVolumeProto.m_DataLen; i++)
	{
		lData = (lData << 8) | lVolumeProto.m_Data[i]; /* MSB First */
	}

	/* 该时序从1898移植，由于没有datasheet，所以时序没优化 */
	at91_set_gpio_value(sc_VolumeConfig[lIndex].m_SdaPin, LEVEL_LOW);
	at91_set_gpio_value(sc_VolumeConfig[lIndex].m_SclPin, LEVEL_LOW);
	for (i=0; i<11; i++)
	{
		at91_set_gpio_value(sc_VolumeConfig[lIndex].m_SclPin, LEVEL_LOW);
		udelay(3);

		if (((lData >> i) & 0x0001) == 1)   
		{
			at91_set_gpio_value(sc_VolumeConfig[lIndex].m_SdaPin, LEVEL_HIGH);
		}
		else
		{
			at91_set_gpio_value(sc_VolumeConfig[lIndex].m_SdaPin, LEVEL_LOW);
		}
		udelay(3);

		at91_set_gpio_value(sc_VolumeConfig[lIndex].m_SclPin, LEVEL_HIGH);
		udelay(3);

		if(i < 10)
		{
			at91_set_gpio_value(sc_VolumeConfig[lIndex].m_SdaPin, LEVEL_LOW);
		}			
		udelay(3);
	}
	at91_set_gpio_value(sc_VolumeConfig[lIndex].m_SclPin, LEVEL_LOW);

	return lVolumeProto.m_DataLen;
}

static int Volume_Ioctl(struct inode *inode, struct file *filep, unsigned int cmd, unsigned long arg)
{
	int lData;
	unsigned long size;

	if(_IOC_TYPE(cmd) != VOLUME_IOCTL_MAGIE) 
		return -EINVAL;
	if(_IOC_NR(cmd) >= VOLUME_IOCTL_MAXNR)
		return -EINVAL;

	size = _IOC_SIZE(cmd);
	switch(cmd)
	{
	case VOLUME_GPIO_INIT:
		copy_from_user((void *)&lData, (const void *)arg, size);
		Volume_GpioInit(Volume_GetIndex(lData));
		break;
	default: 
		DRL_TRACE(("cmd error!"));
		return -EINVAL;
		break;
	}

	return 0;
}


struct file_operations volume_device_fops=
{
	.owner = THIS_MODULE,
	.open = Volume_Open,
	.ioctl = Volume_Ioctl,
	.release = Volume_Release,
	.write=Volume_Write,
};

static struct class *s_DriverClass;
static struct class_device *s_DriverDevice;
static int s_Major;
static int __init Volume_Init(void)
{
	DRL_TRACE(("%s Register", VOLUME_DRIVER_NAME));
	s_Major = register_chrdev(0, VOLUME_DRIVER_NAME, &volume_device_fops);

	if(s_Major<0)
		return s_Major;

	s_DriverClass = class_create(THIS_MODULE, VOLUME_DRIVER_NAME);  
	if (IS_ERR(s_DriverClass)) {
		DRL_TRACE(("class create error!"));
		return -1;
	}
	s_DriverDevice = class_device_create(s_DriverClass, MKDEV(s_Major, 0), NULL, VOLUME_NODE_NAME);  
	devfs_mk_cdev(MKDEV(s_Major, 0), S_IFCHR | S_IRUGO | S_IWUGO, VOLUME_NODE_NAME);

	return 0;
}
static void __exit Volume_Exit(void)
{
	devfs_remove(VOLUME_NODE_NAME);
	unregister_chrdev(s_Major,VOLUME_DRIVER_NAME);
	class_device_unregister(s_DriverDevice);  
	class_destroy(s_DriverClass);     
}

module_init(Volume_Init);
module_exit(Volume_Exit);

MODULE_AUTHOR(VOLUME_AUTHOR);
MODULE_DESCRIPTION(VOLUME_DESCRIPTION);
MODULE_LICENSE(VOLUME_LICENSE);
/* EOF */
