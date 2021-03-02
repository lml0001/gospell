#include "ds18b20_driver.h"

#define PULLD_EN 1
#define PULLD_DIS 0
#define LEVEL_HIGH 1
#define LEVEL_LOW 0

#define DRL_TRACE(x) \
	do{ \
	udelay(10); \
	printk("[%s][%s][%d]", DS18B20_DRIVER_NAME, __FUNCTION__, __LINE__); \
	printk x; \
	printk("\n"); \
	}while(0) 
#define DRL_TRUE 1
#define DRL_FALSE 0

#define DS18B20_ROM_FUNC_CMD_READ	0x33
#define DS18B20_ROM_FUNC_CMD_MATCH	0x55
#define DS18B20_ROM_FUNC_CMD_SKIP	0xCC
#define DS18B20_ROM_FUNC_CMD_SEARCH	0xF0
#define DS18B20_ROM_FUNC_CMD_ALARM_SEARCH	0xEC

#define DS18B20_MEM_FUNC_CMD_WRITE_SCRATCHPAD	0x4E
#define DS18B20_MEM_FUNC_CMD_READ_SCRATCHPAD	0xBE
#define DS18B20_MEM_FUNC_CMD_COPY_SCRATCHPAD	0x48
#define DS18B20_MEM_FUNC_CMD_CONVERT_T	0x44
#define DS18B20_MEM_FUNC_CMD_RECALL_E2	0xB8
#define DS18B20_MEM_FUNC_CMD_READ_POWER_SUPPLY	0xB4

static int Ds18b20_Open(struct inode *inode, struct file *filep)
{
	return 0;
}

static int Ds18b20_Release(struct inode *inode, struct file *filep)
{
	return 0;
}

static int Ds18b20_Initialization(void)
{
	int i;

	at91_set_gpio_output(DS18B20_DQ_PIN, PULLD_EN); /* Master Tx for Reset Pulse 480us~960us */

	/* Reset Pulse */
	at91_set_gpio_value(DS18B20_DQ_PIN, LEVEL_HIGH);
	udelay(10);
	at91_set_gpio_value(DS18B20_DQ_PIN, LEVEL_LOW);
	udelay(700); /* 480us - 960us */
	
	at91_set_gpio_input(DS18B20_DQ_PIN, PULLD_EN); /* Master Rx for Presence Pulse Minimum 480us */
	udelay(60); /* 15us - 60us */

	/* DS18B20 Presence Pulse 60~240us */
	for (i=0; i<20; i++)
	{
		udelay(10); 
		if (at91_get_gpio_value(DS18B20_DQ_PIN) == LEVEL_LOW)
		{
			break;
		}
	}

	if (i >= 20)
	{
		DRL_TRACE(("No Resence Pulse! No DS18B20 Slave!!"));
		return DRL_FALSE;
	}
	else
	{
		udelay(480);
		return DRL_TRUE;
	}
}

static void Ds18b20_WriteByte(unsigned char Data)
{
	unsigned char lBit;
	int i = 0;

	at91_set_gpio_output(DS18B20_DQ_PIN, PULLD_EN);
	while(i < 8)
	{
		lBit = (Data >> i) & 0x01;

		/* 启动条件 */
		at91_set_gpio_value(DS18B20_DQ_PIN, LEVEL_LOW); 
		udelay(8);	/* 1us -15us */

		/* 数据写入 */
		if(lBit) /* 60us -120us to set 1 time	*/
		{     
			at91_set_gpio_value(DS18B20_DQ_PIN, LEVEL_HIGH);		
		}
		else /* 60us -120us to set 0 time	*/
		{
			at91_set_gpio_value(DS18B20_DQ_PIN, LEVEL_LOW);
		}
		udelay(60);

		/* 1位传送完成，释放总线 */
		at91_set_gpio_value(DS18B20_DQ_PIN, LEVEL_HIGH);	
		udelay(10);

		i++;
	}
}

static unsigned char Ds18b20_ReadByte(void)
{
	unsigned char lData = 0;	
	int i = 0;

	while(i < 8)
	{
		at91_set_gpio_output(DS18B20_DQ_PIN, PULLD_EN);

		/* 启动条件 */
		at91_set_gpio_value(DS18B20_DQ_PIN, LEVEL_LOW); 
		udelay(1); /* >1us */	

		/* 释放总线 */
		at91_set_gpio_input(DS18B20_DQ_PIN, PULLD_EN);

		udelay(1); /* 从输入启动到采样必须在15us内操作完成 */	

		lData >>= 1;
		if (at91_get_gpio_value(DS18B20_DQ_PIN))
		{
			lData |= 0x80;
		}

		udelay(60); /* 这里最少延时45us */

		i++ ;
	}

	return lData;
}

static int Ds18b20_DataUpdate(void)
{
	if (!Ds18b20_Initialization())			/* Initialization */
	{
		return DRL_FALSE;
	}

	Ds18b20_WriteByte(DS18B20_ROM_FUNC_CMD_SKIP);		/* skip ROM command */
	Ds18b20_WriteByte(DS18B20_MEM_FUNC_CMD_CONVERT_T) ;		/* send convert T */

	return DRL_TRUE;
}

static unsigned char Ds18b20_Crc8(unsigned char *ptr, unsigned char len)  
{ 
	unsigned char i; 
	unsigned char crc=0; 

	while(len-- != 0)  
	{ 
		for(i=1; i!=0; i*=2) 
		{ 
			if((crc&1)!=0) 
			{
				crc >>= 1; 
				crc ^= 0x8C;
			}
			else
			{
				crc >>= 1; 
			}
			if((*ptr & i)!=0)
			{
				crc ^= 0x8C;     
			}
		}  
		ptr++; 
	} 

	return(crc); 
} 

static int Ds18b20_GetTemperature(int *pData)
{
	unsigned char lCrcVal = 0;
	unsigned char pArray[9];
	int i, lData;

	if (!Ds18b20_Initialization())
	{
		return DRL_FALSE;
	}
	Ds18b20_WriteByte(DS18B20_ROM_FUNC_CMD_SKIP);		/* skip ROM command */
	Ds18b20_WriteByte(DS18B20_MEM_FUNC_CMD_READ_SCRATCHPAD) ;		/* Read Scratchpad command */

	for(i = 0; i < 9; i++) /* 共9个暂存器 */
	{
		pArray[i] = Ds18b20_ReadByte();
	}
	lCrcVal = Ds18b20_Crc8(pArray, 8);
	if((lCrcVal != pArray[8]) || (lCrcVal == 0))
	{
		DRL_TRACE(("Ds18b20 CRC Err!"));
		return DRL_FALSE;
	}
	else
	{
		lData = (pArray[1] << 8) | pArray[0];	
		if((lData & 0xf000) == 0x0000) /* 我们只处理正温度值，对于负的温度值不处理 */		
		{	
			lData = lData & 0x7ff;		
			lData = lData * 10000 / 16; /* 为了保持精度，这里乘10000，在应用层除10000，因为驱动不支持浮点处理，1/16=0.0625 */
		}
		else /* 负温度值直接处理为0摄氏度 */
		{
			lData = 0;
		}
	}

	*pData = lData;
	return DRL_TRUE;
}

static int Ds18b20_Read(struct file *fileP, char *pData, size_t data_len, loff_t *f_pos)
{	
	int lData;

	if (Ds18b20_DataUpdate() == DRL_FALSE)
	{
		return -EIO;
	}

	mdelay(750); /* 温度转换时间750ms */

	if (Ds18b20_GetTemperature(&lData) == DRL_FALSE)
	{
		return -EIO;
	}
	copy_to_user(pData, (char *)&lData, sizeof(int)); 

	return sizeof(int);
}

struct file_operations ds18b20_device_fops=
{
	.owner = THIS_MODULE,
	.open = Ds18b20_Open,
	.release = Ds18b20_Release,
	.read = Ds18b20_Read,
};

static struct class *s_DriverClass;
static struct class_device *s_DriverDevice;
static int s_Major;
static int __init Ds18b20_Init(void)
{
	DRL_TRACE(("%s Register", DS18B20_DRIVER_NAME));
	s_Major = register_chrdev(0, DS18B20_DRIVER_NAME, &ds18b20_device_fops);

	if(s_Major<0)
		return s_Major;

	s_DriverClass = class_create(THIS_MODULE, DS18B20_DRIVER_NAME);  
	if (IS_ERR(s_DriverClass)) {
		DRL_TRACE(("class create error!"));
		return -1;
	}
	s_DriverDevice = class_device_create(s_DriverClass, MKDEV(s_Major, 0), NULL, DS18B20_NODE_NAME);  
	devfs_mk_cdev(MKDEV(s_Major, 0), S_IFCHR | S_IRUGO | S_IWUGO, DS18B20_NODE_NAME);

	return 0;
}
static void __exit Ds18b20_Exit(void)
{
	devfs_remove(DS18B20_NODE_NAME);
	unregister_chrdev(s_Major,DS18B20_DRIVER_NAME);
	class_device_unregister(s_DriverDevice);  
	class_destroy(s_DriverClass);     
}

module_init(Ds18b20_Init);
module_exit(Ds18b20_Exit);

MODULE_AUTHOR(DS18B20_AUTHOR);
MODULE_DESCRIPTION(DS18B20_DESCRIPTION);
MODULE_LICENSE(DS18B20_LICENSE);
/* EOF */
