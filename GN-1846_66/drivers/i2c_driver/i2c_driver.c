#include "i2c_driver.h"

#define PULLD_EN 1
#define PULLD_DIS 0
#define LEVEL_HIGH 1
#define LEVEL_LOW 0
#define DIR_OUTPUT 0 /* DIR的定义相对于ARM */
#define DIR_INPUT 1

#define DRL_TRACE(x) \
	do{ \
	udelay(10); \
	printk("[%s][%s][%d]", I2C_DRIVER_NAME, __FUNCTION__, __LINE__); \
	printk x; \
	printk("\n"); \
	}while(0) 
#define DRL_TRUE 1
#define DRL_FALSE 0

struct stI2cConfig
{
	int m_I2cIndex;
	int m_SdaPin;
	int m_SclPin;

	int m_PageAlignSize; /* 连续写字节数限制 */
};

#define I2C_NO_PAGE_ALIGN 0 /* 无页对齐需求 */
static const struct stI2cConfig  sc_I2cConfig[GN_I2C_NUM] = 
{
	{GN_I2C_INDEX_CVBS_CH1,			I2C_CVBS1_SDA_PIN_1,		I2C_CVBS1_SCL_PIN_1,		I2C_NO_PAGE_ALIGN},
	{GN_I2C_INDEX_CVBS_CH2,			I2C_CVBS1_SDA_PIN_1,		I2C_CVBS1_SCL_PIN_1,		I2C_NO_PAGE_ALIGN},
	{GN_I2C_INDEX_CVBS_CH3,			I2C_CVBS1_SDA_PIN_2,		I2C_CVBS1_SCL_PIN_2,		I2C_NO_PAGE_ALIGN},
	{GN_I2C_INDEX_CVBS_CH4,			I2C_CVBS1_SDA_PIN_2,		I2C_CVBS1_SCL_PIN_2,		I2C_NO_PAGE_ALIGN},
	{GN_I2C_INDEX_CVBS_CH5,			I2C_CVBS2_SDA_PIN_1,		I2C_CVBS2_SCL_PIN_1,		I2C_NO_PAGE_ALIGN},
	{GN_I2C_INDEX_CVBS_CH6,			I2C_CVBS2_SDA_PIN_1,		I2C_CVBS2_SCL_PIN_1,		I2C_NO_PAGE_ALIGN},
	{GN_I2C_INDEX_CVBS_CH7,			I2C_CVBS2_SDA_PIN_2,		I2C_CVBS2_SCL_PIN_2,		I2C_NO_PAGE_ALIGN},
	{GN_I2C_INDEX_CVBS_CH8,			I2C_CVBS2_SDA_PIN_2,		I2C_CVBS2_SCL_PIN_2,		I2C_NO_PAGE_ALIGN},
};

static void I2C_SetPinDirCtrl(int pin, int direction);
static void I2C_GpioInit(int Index)
{
	at91_set_gpio_output(sc_I2cConfig[Index].m_SclPin, PULLD_EN);
	at91_set_gpio_output(sc_I2cConfig[Index].m_SdaPin, PULLD_EN);
}

static int I2C_Open(struct inode *inode, struct file *filep)
{
	return 0;
}

static int I2C_Release(struct inode *inode, struct file *filep)
{
	return 0;
}

static int I2C_GetIndex(int I2cIndex)
{
	int i;

	for (i = 0; i < GN_I2C_NUM; i++) 
	{
		if (I2cIndex == sc_I2cConfig[i].m_I2cIndex) 
		{
			return i;
		}
	}

	return -1;
}

#define I2C_PIN_CHANG_DELAY 10 /* 引脚设置延时时间 */
/* 设置引脚的输出给CPLD，告诉CPLD当前的SDA引脚作输出还是输入 */
static void I2C_SetPinDirCtrl(int pin, int direction)
{
	at91_set_gpio_value(pin, direction);
	udelay(I2C_PIN_CHANG_DELAY);		
}

static void I2C_SetPinValue(int pin, int level)
{
	at91_set_gpio_value(pin, level);
	udelay(I2C_PIN_CHANG_DELAY);		// 时序图要求>4.7us，这里为了安全采用10us
}

static void I2C_SetPinInput(int pin)
{
	at91_set_gpio_input(pin, PULLD_EN);
	udelay(I2C_PIN_CHANG_DELAY);		// GPIO输入输出切换时间，没有找到相关资料，但必须要延时
}

static void I2C_SetPinOutput(int pin)
{
	at91_set_gpio_output(pin, PULLD_EN);
	udelay(I2C_PIN_CHANG_DELAY);		// GPIO输入输出切换时间，没有找到相关资料，但必须要延时
}

/*  注意：写的时候一定不能产生二义性，一定是sda先为低，然后操作scl。
在操作的时候时钟信号变化后都要修改为低，避免产生起始或结束条件。*/
static void I2C_Start(int I2cIndex)
{
	I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SclPin, LEVEL_HIGH);
	I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SdaPin, LEVEL_HIGH);
	I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SdaPin, LEVEL_LOW);
	I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SclPin, LEVEL_LOW);
}

static void I2C_Stop(int I2cIndex)
{
	I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SdaPin, LEVEL_LOW);
	I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SclPin, LEVEL_HIGH);
	I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SdaPin, LEVEL_HIGH);
}

static void I2C_SendAck(int I2cIndex)
{
	I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SdaPin, LEVEL_LOW);
	I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SclPin, LEVEL_HIGH);
	I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SclPin, LEVEL_LOW);
}

static void I2C_SendNoAck(int I2cIndex)
{
	I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SdaPin, LEVEL_HIGH);
	I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SclPin, LEVEL_HIGH);
	I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SclPin, LEVEL_LOW);
}

static int I2C_WaitAck(int I2cIndex)
{
	int lTimeOut = 500;
	unsigned char lCh;

	I2C_SetPinInput(sc_I2cConfig[I2cIndex].m_SdaPin);
	while(lTimeOut--)
	{
		lCh = at91_get_gpio_value(sc_I2cConfig[I2cIndex].m_SdaPin) & 0x01;
		if(0 == lCh)
			break;	
		udelay(1);
	}	
	I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SclPin, LEVEL_HIGH); // 第九个时钟
	I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SclPin, LEVEL_LOW);

	I2C_SetPinOutput(sc_I2cConfig[I2cIndex].m_SdaPin);
	if(lTimeOut > 0) 
	{
		return DRL_TRUE;
	}
	else 
	{
		return DRL_FALSE;
	}
}

static int I2C_Send8Bit(int I2cIndex, const char ch)
{
	unsigned char lTmp = 0x80;
	int i;

	for(i=0; i<8; i++)
	{
		I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SclPin, LEVEL_LOW);
		if(lTmp & ch)		//MSB first
		{
			I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SdaPin, LEVEL_HIGH);
		}
		else
		{
			I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SdaPin, LEVEL_LOW);
		}
		I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SclPin, LEVEL_HIGH);
		lTmp >>= 1;
	}
	I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SclPin, LEVEL_LOW);
	I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SdaPin, LEVEL_HIGH);
	if (!I2C_WaitAck(I2cIndex)) 
	{
		DRL_TRACE (("wait ack error -1!"));
		return DRL_FALSE;
	}	

	return DRL_TRUE;
}

static void I2C_Read8Bit(int I2cIndex, char *pCh)
{
	unsigned char lTmp, lReadChar = 0;
	int i = 8;

	I2C_SetPinInput(sc_I2cConfig[I2cIndex].m_SdaPin);
	for(i=0; i<8; i++)
	{
		I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SclPin, LEVEL_HIGH);
		lTmp = at91_get_gpio_value(sc_I2cConfig[I2cIndex].m_SdaPin) & 0x01;
		lReadChar <<= 1;
		lReadChar |= lTmp ;
		I2C_SetPinValue(sc_I2cConfig[I2cIndex].m_SclPin, LEVEL_LOW);
	}

	I2C_SetPinOutput(sc_I2cConfig[I2cIndex].m_SdaPin);
	*pCh = lReadChar;
}

/* 连续读 */
static int I2C_ReadBytes(int I2cIndex, int DevAddr, char *pSubAddr, int SubAddrLen, char *pData, int DataLen) 
{
	int i;

	/* 设备地址 */
	if (!I2C_Send8Bit(I2cIndex, DevAddr & 0xFF))
	{
		return -1;
	}
	
	/* 寄存器地址 */
	for (i=0; i<SubAddrLen; i++)
	{
		if (!I2C_Send8Bit(I2cIndex, pSubAddr[i]))
		{
			return -1;
		}
	}
	
	I2C_Start(I2cIndex);
	if (!I2C_Send8Bit(I2cIndex, (DevAddr & 0xFF) | 0x01))
	{
		return -1;
	}
	for(i=0; i<DataLen-1; i++)
	{
		I2C_Read8Bit(I2cIndex, &pData[i]);
		I2C_SendAck(I2cIndex);
	}
	I2C_Read8Bit(I2cIndex, &pData[i]);
	I2C_SendNoAck(I2cIndex);

	return DataLen;
}

/* 连续写 */
static int I2C_WriteBytes(int I2cIndex, int DevAddr, char *pSubAddr, int SubAddrLen, char *pData, int DataLen) 
{
	int i;

	/* 设备地址 */
	if (!I2C_Send8Bit(I2cIndex, DevAddr & 0xFF))
	{
		return -1;
	}

	/* 寄存器地址 */
	for (i=0; i<SubAddrLen; i++)
	{
		if (!I2C_Send8Bit(I2cIndex, pSubAddr[i]))
		{
			return -1;
		}
	}

	for(i=0; i<DataLen; i++)
	{
		if (!I2C_Send8Bit(I2cIndex, pData[i]))
		{
			return -1;
		}
	}

	return DataLen;
}

static int I2C_Read(struct file *fileP, char *pData, size_t count, loff_t *f_pos)
{
	DRL_I2cProtocol lI2cProto;
	int lIndex = 0;
	int lRet;

	if (count > sizeof(lI2cProto)) 
	{
		DRL_TRACE(("Para Error!!"));
		return -EINVAL;
	}
	copy_from_user((char *)&lI2cProto, pData, count);

	if ((lIndex = I2C_GetIndex(lI2cProto.m_I2cIndex)) < 0)
	{
		DRL_TRACE(("I2cIndex Error!!"));
		return -1;
	}

	I2C_Start(lIndex);
	lRet = I2C_ReadBytes(lIndex, lI2cProto.m_DevAddr, lI2cProto.m_RegAddr, lI2cProto.m_RegAddrWidth, lI2cProto.m_Data, lI2cProto.m_DataLen);		
	I2C_Stop(lIndex);

	copy_to_user(pData, (char *)&lI2cProto, count);

	return lRet;
}

#define MSB32_D(b)	((b[0]<<24) | (b[1] <<16) | (b[2] <<8) | b[3]) /* 高位在前解包 */
/* 高位在前打包 */
#define MSB32_E(b,x) \
do { \
	*((b)+0) = (char)((x)>>24)&0xFF;\
	*((b)+1) = (char)((x)>>16)&0xFF;\
	*((b)+2) = (char)((x)>>8)&0xFF;\
	*((b)+3) = (char)(x)&0xFF; \
}while(0)
static int I2C_Write(struct file *fileP, const char __user*pData,size_t count, loff_t * f_pos)
{   
	DRL_I2cProtocol lI2cProto;
	int lIndex = 0;
	int lRet;

	if (count > sizeof(lI2cProto)) 
	{
		DRL_TRACE(("Para Error!!"));
		return -EINVAL;
	}
	copy_from_user((char *)&lI2cProto, pData, count);

	if ((lIndex = I2C_GetIndex(lI2cProto.m_I2cIndex)) < 0)
	{
		DRL_TRACE(("I2cIndex Error!!"));
		return -1;
	}

	if (sc_I2cConfig[lIndex].m_PageAlignSize == I2C_NO_PAGE_ALIGN)
	{
		I2C_Start(lIndex);
		lRet = I2C_WriteBytes(lIndex, lI2cProto.m_DevAddr, lI2cProto.m_RegAddr, lI2cProto.m_RegAddrWidth, lI2cProto.m_Data, lI2cProto.m_DataLen);		
		I2C_Stop(lIndex);
	}
	else /* 针对使用页对齐方式的eeprom */
	{
		int i;
		int lCurPageRem = 0;
		int lPageNum = 0;
		int lRemPageNum = 0;
		int lPageSize = sc_I2cConfig[lIndex].m_PageAlignSize;
		int lRegAddr;

		lRegAddr = MSB32_D(lI2cProto.m_RegAddr);
		lCurPageRem = lPageSize - (lRegAddr % lPageSize);
		lPageNum = (lI2cProto.m_DataLen - lCurPageRem) / lPageSize;
		lRemPageNum = lI2cProto.m_DataLen - lCurPageRem - lPageSize * lPageNum;

		I2C_Start(lIndex);
		lRet = I2C_WriteBytes(lIndex, lI2cProto.m_DevAddr, lI2cProto.m_RegAddr, lI2cProto.m_RegAddrWidth, lI2cProto.m_Data, lCurPageRem);		
		I2C_Stop(lIndex);
		for (i=0; i<lPageNum; i++)
		{
			I2C_Start(lIndex);
			lRegAddr = MSB32_D(lI2cProto.m_RegAddr) + lCurPageRem + i * lPageSize;
			MSB32_E(lI2cProto.m_RegAddr, lRegAddr);
			lRet = I2C_WriteBytes(lIndex, lI2cProto.m_DevAddr, lI2cProto.m_RegAddr, lI2cProto.m_RegAddrWidth, lI2cProto.m_Data + lCurPageRem + i * lPageSize, lPageSize);		
			I2C_Stop(lIndex);
		}
		I2C_Start(lIndex);
		lRegAddr = MSB32_D(lI2cProto.m_RegAddr) + lCurPageRem + lPageNum * lPageSize;
		MSB32_E(lI2cProto.m_RegAddr, lRegAddr);
		lRet = I2C_WriteBytes(lIndex, lI2cProto.m_DevAddr, lI2cProto.m_RegAddr, lI2cProto.m_RegAddrWidth, lI2cProto.m_Data + lCurPageRem + lPageNum * lPageSize, lRemPageNum);		
		I2C_Stop(lIndex);

		lRet = lI2cProto.m_DataLen;
	}

	return lRet;
}

static int I2C_Ioctl(struct inode *inode, struct file *filep, unsigned int cmd, unsigned long arg)
{
	int lData;
	unsigned long size;

	if(_IOC_TYPE(cmd) != I2C_IOCTL_MAGIE) 
		return -EINVAL;
	if(_IOC_NR(cmd) >= I2C_IOCTL_MAXNR)
		return -EINVAL;

	size = _IOC_SIZE(cmd);
	switch(cmd)
	{
	case I2C_GPIO_INIT:
		copy_from_user((void *)&lData, (const void *)arg, size);
		I2C_GpioInit(I2C_GetIndex(lData));
		break;
	default: 
		DRL_TRACE(("cmd error!"));
		return -EINVAL;
		break;
	}

	return 0;
}

struct file_operations i2c_device_fops=
{
	.owner = THIS_MODULE,
	.open = I2C_Open,
	.release = I2C_Release,
	.ioctl = I2C_Ioctl,
	.read = I2C_Read,
	.write=I2C_Write,
};

static struct class *s_DriverClass;
static struct class_device *s_DriverDevice;
static int s_Major;
static int __init I2C_Init(void)
{
	DRL_TRACE(("%s Register", I2C_DRIVER_NAME));
	s_Major = register_chrdev(0, I2C_DRIVER_NAME, &i2c_device_fops);

	if(s_Major<0)
		return s_Major;

	s_DriverClass = class_create(THIS_MODULE, I2C_DRIVER_NAME);  
	if (IS_ERR(s_DriverClass)) {
		DRL_TRACE(("class create error!"));
		return -1;
	}
	s_DriverDevice = class_device_create(s_DriverClass, MKDEV(s_Major, 0), NULL, I2C_NODE_NAME);  
	devfs_mk_cdev(MKDEV(s_Major, 0), S_IFCHR | S_IRUGO | S_IWUGO, I2C_NODE_NAME);

	return 0;
}
static void __exit I2C_Exit(void)
{
	devfs_remove(I2C_NODE_NAME);
	unregister_chrdev(s_Major,I2C_DRIVER_NAME);
	class_device_unregister(s_DriverDevice);  
	class_destroy(s_DriverClass);     
}

module_init(I2C_Init);
module_exit(I2C_Exit);

MODULE_AUTHOR(I2C_AUTHOR);
MODULE_DESCRIPTION(I2C_DESCRIPTION);
MODULE_LICENSE(I2C_LICENSE);
/* EOF */
