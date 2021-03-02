#include "fpga_localbus_driver.h"

#define PULLD_EN 1
#define PULLD_DIS 0
#define LEVEL_HIGH 1
#define LEVEL_LOW 0

#define DRL_TRACE(x) \
	do{ \
	udelay(10); \
	printk("[%s][%s][%d]", FPGA_LOCALBUS_DRIVER_NAME, __FUNCTION__, __LINE__); \
	printk x; \
	printk("\n"); \
	}while(0) 
#define DRL_TRUE 1
#define DRL_FALSE 0

struct stFpgaLocalbusConfig 
{
	int m_FpgaIndex;
	int m_PhyBaseAddr; /* 总线物理基址 */
	int m_CsrIndex; /* 片选寄存器对应下标，CSR0~CSR7对应0～7 */

	unsigned int m_VirtualBaseAddr; /* 总线内存虚拟基址 */
};

static struct stFpgaLocalbusConfig  s_FpgaLocalbusConfig [GN_FPGA_NUM] = 
{
	{GN_FPGA_INDEX_MAIN,					FPGA_MAIN_BASE_ADDR,				LOCALBUS_NCS2,	0},
	{GN_FPGA_INDEX_IP_ASI_OUTPUT,	FPGA_IP_ASI_OUTPUT_BASE_ADDR, LOCALBUS_NCS5,	0},
};

/*
[localbus驱动说明]
现目前的fpga的localbus通信设置为：
	1) 地址线：A1~A10 10位，设置地址为0x000 ~ 0x3FF
	2) 数据线：D0~D7 8位
	3) 由于A0没有接，所以地址的接线允许直接通过数据线读出16位，所以这里设计的是16位的localbus读写，有效使用的仅为低8位
	4) 在驱动的设计中，按照16位的方式进行读写操作，在用户空间中根据数据线的位数进行取低8位操作即可，这样驱动在数据线变为16位时不会有改动
*/

#define AT91C_SMC2_NWS_128_CYCLE 0x01
#define AT91C_SMC2_RWHOLD_2_CYCLE     ((unsigned int) 0x2 << 28) 
static int FpgaLocabus_Config(int Index)
{
	if (s_FpgaLocalbusConfig[Index].m_PhyBaseAddr <= 0) /* 该索引的设备没有使用localbus */
	{
		return -1;
	}
	
	/* 0x800代表11位，A0~A11，size就是这么大 */
	s_FpgaLocalbusConfig[Index].m_VirtualBaseAddr = (unsigned int)ioremap(s_FpgaLocalbusConfig[Index].m_PhyBaseAddr, 0x800) ;
	if (!s_FpgaLocalbusConfig[Index].m_VirtualBaseAddr)
	{
		DRL_TRACE((KERN_ERR "io remap error!"));
		return -1;
	}

	/*
	片选寄存器设置：
	NWS[6-0]:	定义读写信号脉冲长度，这里使用128个周期
	WSEN[7]:		是否使能NWS生效，这里使用NWS，所以设置为AT91C_SMC2_WSEN
	TDF[11-8]:	数据流动时间，这里使用0个周期
	BAT[12]:		直接访问类型，我们这里连接16位宽的设备，所以设置为AT91C_SMC2_BAT
	DBW[14-13]:	数据总线宽度，我们的设备设计均为16位宽数据，所以这里设置为AT91C_SMC2_DBW_16
	DRP[15]:		数据读协议，采用标准读协议，为0
	ACSS[17-16]:片选启动地址，标准：0
	RSV[23-18]:	保留
	RWSETUP[26-24]:读、写信号启动时间，设置为0，参考datasheet
	RWHOLD[30-28]:读、写信号保持时间，设置为2，参考datasheet
	RSV[31]:	保留
	*/
	AT91_SYS->EBI_SMC2_CSR[s_FpgaLocalbusConfig[Index].m_CsrIndex] =  AT91C_SMC2_RWHOLD_2_CYCLE | AT91C_SMC2_DBW_16 | AT91C_SMC2_BAT | AT91C_SMC2_WSEN | AT91C_SMC2_NWS_128_CYCLE;

	return 0;
}

static int FpgaLocalbus_GetIndex(int FpgaIndex)
{
	int i;

	for (i = 0; i < GN_FPGA_NUM; i++) 
	{
		if (FpgaIndex == s_FpgaLocalbusConfig[i].m_FpgaIndex) 
		{
			return i;
		}
	}

	return -1;
}
static int FpgaLocabus_Open(struct inode *inode, struct file *filp)
{
	return 0 ;
}      

static int FpgaLocabus_Release(struct inode *inode, struct file *filp)
{ 
	return 0;	
}

static ssize_t FpgaLocabus_Read(struct file *filp, char *pData, size_t count, loff_t *f_pos)
{
	DRL_FpgaLocalbusProtocol lLocalbusProto;
	int lIndex;

	if (count > sizeof(lLocalbusProto)) 
	{
		DRL_TRACE(("Para Error!!"));
		return -EINVAL;
	}

	if(copy_from_user(&lLocalbusProto, pData, count))
	{
		return -1;
	}

	if ((lIndex = FpgaLocalbus_GetIndex(lLocalbusProto.m_FpgaIndex)) < 0)
	{
		DRL_TRACE(("FpgaIndex Error!!"));
		return -1;
	}

	if (!s_FpgaLocalbusConfig[lIndex].m_VirtualBaseAddr) /* 没有映射地址则返回 */
	{
		return -1;
	}
	//lLocalbusProto.m_Data = readw((void __iomem *)(s_FpgaLocalbusConfig[lIndex].m_VirtualBaseAddr + ((lLocalbusProto.m_RegAddr << 1) & 0xFFFF)));
	lLocalbusProto.m_Data = ioread16((void __iomem *)(s_FpgaLocalbusConfig[lIndex].m_VirtualBaseAddr + ((lLocalbusProto.m_RegAddr << 1) & 0xFFFF)));

	if (copy_to_user(pData, &lLocalbusProto, sizeof(lLocalbusProto)))
	{
		return -1;
	}

	return sizeof(lLocalbusProto.m_Data);
}

static ssize_t FpgaLocabus_Write(struct file *filp, const char *buff, size_t count, loff_t *f_pos)
{
	DRL_FpgaLocalbusProtocol lLocalbusProto;
	int lIndex;

	if (count > sizeof(lLocalbusProto)) 
	{
		DRL_TRACE(("Para Error!!"));
		return -EINVAL;
	}

	if(copy_from_user(&lLocalbusProto, buff, count))
	{
		return -1;
	}

	if ((lIndex = FpgaLocalbus_GetIndex(lLocalbusProto.m_FpgaIndex)) < 0)
	{
		DRL_TRACE(("FpgaIndex Error!!"));
		return -1;
	}

	if (!s_FpgaLocalbusConfig[lIndex].m_VirtualBaseAddr) /* 没有映射地址则返回 */
	{
		return -1;
	}
	writew(lLocalbusProto.m_Data, (void __iomem *)(s_FpgaLocalbusConfig[lIndex].m_VirtualBaseAddr + ((lLocalbusProto.m_RegAddr << 1) & 0xFFFF)));

	return sizeof(lLocalbusProto.m_Data);
}

static int FpgaLocabus_Ioctl(struct inode *inode, struct file *filep, unsigned int cmd, unsigned long arg)
{
	int lData;
	unsigned long size;

	if(_IOC_TYPE(cmd) != FPGA_LOCALBUS_IOCTL_MAGIE) 
		return -EINVAL;
	if(_IOC_NR(cmd) >= FPGA_LOCALBUS_IOCTL_MAXNR)
		return -EINVAL;

	size = _IOC_SIZE(cmd);
	switch(cmd)
	{
	case FPGA_LOCALBUS_CONFIG:
		copy_from_user((void *)&lData, (const void *)arg, size);
		return FpgaLocabus_Config(FpgaLocalbus_GetIndex(lData));
	default: 
		DRL_TRACE(("cmd error!"));
		return -EINVAL;
		break;
	}

	return 0;
}

struct file_operations fpga_localbus_fops=
{
	.owner = THIS_MODULE,
	.read	= FpgaLocabus_Read,
	.write = FpgaLocabus_Write,
	.ioctl = FpgaLocabus_Ioctl,
	.open = FpgaLocabus_Open,
	.release = FpgaLocabus_Release,
};

static struct class *s_DriverClass;
static struct class_device *s_DriverDevice;
static int s_Major;
static int __init FpgaLocalbus_Init(void)
{
	DRL_TRACE(("%s Register", FPGA_LOCALBUS_DRIVER_NAME));
	s_Major = register_chrdev(0, FPGA_LOCALBUS_DRIVER_NAME, &fpga_localbus_fops);

	if(s_Major<0)
		return s_Major;

	s_DriverClass = class_create(THIS_MODULE, FPGA_LOCALBUS_DRIVER_NAME);  
	if (IS_ERR(s_DriverClass)) {
		DRL_TRACE(("class create error!"));
		return -1;
	}
	s_DriverDevice = class_device_create(s_DriverClass, MKDEV(s_Major, 0), NULL, FPGA_LOCALBUS_NODE_NAME);  
	devfs_mk_cdev(MKDEV(s_Major, 0), S_IFCHR | S_IRUGO | S_IWUGO, FPGA_LOCALBUS_NODE_NAME);

	return 0;
}
static void __exit FpgaLocalbus_Exit(void)
{
	devfs_remove(FPGA_LOCALBUS_NODE_NAME);
	unregister_chrdev(s_Major,FPGA_LOCALBUS_DRIVER_NAME);
	class_device_unregister(s_DriverDevice);  
	class_destroy(s_DriverClass);     
}

module_init(FpgaLocalbus_Init);
module_exit(FpgaLocalbus_Exit);

MODULE_AUTHOR(FPGA_LOCALBUS_AUTHOR);
MODULE_DESCRIPTION(FPGA_LOCALBUS_DESCRIPTION);
MODULE_LICENSE(FPGA_LOCALBUS_LICENSE);
/* EOF */
