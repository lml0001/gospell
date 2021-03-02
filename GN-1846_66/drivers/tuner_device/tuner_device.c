/*
*
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>

#include <linux/delay.h>

#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/arch/gpio.h>

#include "tuner_device.h"

#define CARDNAME "Tuner"
//#define DEBUG1 1
#if DEBUG1 > 0 
#define PRINTK3(args...)  printk(CARDNAME ": " args)
#else
#define PRINTK3(args...)  do { } while(0)
#endif
#define PRINTK_ERR(args...)  do { } while(0)//printk(CARDNAME ": " args)

typedef enum
{
	NORMAL=0,
	NO_ACK,
	CHIP_CHANNEL_INVALID,
	DATA_INVALID
}TUNER_Error_Type;

#ifdef GM8358Q
#define IIC_SCL_PIN		AT91_PIN_PA20
#define IIC_SDA_PIN		AT91_PIN_PC14
#define CLK_DELAY_US  5
#define ACK_DELAY_COUNT  (100)
#endif

#ifdef GM8398Q
#define IIC_SCL_PIN		AT91_PIN_PC11
#define IIC_SDA_PIN		AT91_PIN_PC14
#define CLK_DELAY_US  5
#define ACK_DELAY_COUNT  (100)
#endif

#ifdef GC1815B
//复位是PA25
#define IIC_SCL_PIN		AT91_PIN_PC10
#define IIC_SDA_PIN		AT91_PIN_PC11
#define CLK_DELAY_US  5
#define ACK_DELAY_COUNT  (100)
#endif

#define GPIO_PIN_HIGH		1
#define GPIO_PIN_LOW		0


#define MACRO_STR(x) #x



int TunerOpen(struct inode *inode,struct file *filep )
{
	printk("AT9200 I2C Driver USE SCL = %d, SDA = %d Open. \n", IIC_SCL_PIN - PIN_BASE, IIC_SDA_PIN - PIN_BASE);
	at91_set_gpio_output(IIC_SCL_PIN, GPIO_PIN_HIGH);
	at91_set_gpio_output(IIC_SDA_PIN, GPIO_PIN_HIGH);

	at91_set_gpio_value(IIC_SCL_PIN, GPIO_PIN_HIGH);
	at91_set_gpio_value(IIC_SDA_PIN, GPIO_PIN_HIGH);
	return 0;
}


int TunerRelease(struct inode *inode, struct file *filep )
{
	printk("AT9200 I2C Driver Release.\n");
	return 0;
}


void IIC_Start(void)
{
	at91_set_gpio_value(IIC_SCL_PIN, GPIO_PIN_HIGH);
	udelay(CLK_DELAY_US);
	at91_set_gpio_value(IIC_SDA_PIN, GPIO_PIN_LOW);
	udelay(CLK_DELAY_US);
	at91_set_gpio_value(IIC_SCL_PIN, GPIO_PIN_LOW);
}

void IIC_Stop(void)
{
	at91_set_gpio_value(IIC_SDA_PIN, GPIO_PIN_LOW);
	udelay(CLK_DELAY_US);
	at91_set_gpio_value(IIC_SCL_PIN, GPIO_PIN_HIGH);
	udelay(CLK_DELAY_US);
	at91_set_gpio_value(IIC_SDA_PIN, GPIO_PIN_HIGH);
}

void SendNoACK(void)
{
	at91_set_gpio_value(IIC_SDA_PIN, GPIO_PIN_HIGH);
	udelay(CLK_DELAY_US);
	at91_set_gpio_value(IIC_SCL_PIN, GPIO_PIN_HIGH);
	udelay(CLK_DELAY_US);
	at91_set_gpio_value(IIC_SCL_PIN, GPIO_PIN_LOW);
}

void SendACK(void)
{
	at91_set_gpio_value(IIC_SDA_PIN, GPIO_PIN_LOW);
	udelay(CLK_DELAY_US);
	at91_set_gpio_value(IIC_SCL_PIN, GPIO_PIN_HIGH);
	udelay(CLK_DELAY_US);
	at91_set_gpio_value(IIC_SCL_PIN, GPIO_PIN_LOW);
}

int IIC_CheckACK(void)
{
	int lTimeout;
	int lACKError = 0;

	at91_set_gpio_input(IIC_SDA_PIN, 0);//转换方向

	udelay(CLK_DELAY_US * 2);

	lTimeout = ACK_DELAY_COUNT ;
	while(lTimeout > 0)
	{
		if(at91_get_gpio_value(IIC_SDA_PIN) == 0)
		{
			break;;
		}
		else
		{
			lTimeout--;
		}
	}

	at91_set_gpio_value(IIC_SCL_PIN, GPIO_PIN_HIGH);	
	udelay(CLK_DELAY_US);
	at91_set_gpio_value(IIC_SCL_PIN, GPIO_PIN_LOW);
	udelay(CLK_DELAY_US);
	at91_set_gpio_output(IIC_SDA_PIN, 1);
	udelay(CLK_DELAY_US);

	if (lTimeout == 0)
	{
		lACKError = 1;
		IIC_Stop();
	}

	return lACKError;
}

void IIC_Send8bitData(unsigned char value)
{
	unsigned char i;
	unsigned char tmp;

	tmp=0x80;
	for(i=0;i<8;i++)
	{
		udelay(CLK_DELAY_US / 2);
		if(tmp&value)
		{
			at91_set_gpio_value(IIC_SDA_PIN, GPIO_PIN_HIGH);
		}
		else
		{
			at91_set_gpio_value(IIC_SDA_PIN, GPIO_PIN_LOW);
		}
		udelay(CLK_DELAY_US / 2);
		at91_set_gpio_value(IIC_SCL_PIN, GPIO_PIN_HIGH);
		udelay(CLK_DELAY_US);
		tmp>>=1;
		at91_set_gpio_value(IIC_SCL_PIN, GPIO_PIN_LOW);
	}
}

int IIC_Write(unsigned char channel,unsigned char ChipAddr,unsigned char RegAddr, unsigned char *DataBuffer, unsigned char len,unsigned char StopFlag)
{
	int err = NORMAL;
	unsigned char i;
	unsigned char tmpData;

	IIC_Start();

	IIC_Send8bitData(ChipAddr);

	if(IIC_CheckACK())
	{
		PRINTK_ERR("NO_ACK, ChipAddr = 0x%02X\n", ChipAddr);
		err = NO_ACK;
		goto werr_out;
	}

	IIC_Send8bitData(RegAddr);

	if(IIC_CheckACK())
	{
		PRINTK_ERR("NO_ACK, ChipAddr = 0x%02X\n", ChipAddr);
		err = NO_ACK;
		goto werr_out;
	}

	for(i=0;i<len;i++)
	{
		tmpData = *DataBuffer;

		IIC_Send8bitData(tmpData);

		if(IIC_CheckACK())
		{	
			PRINTK_ERR("NO_ACK, ChipAddr = 0x%02X\n", ChipAddr);
			err = NO_ACK;
			goto werr_out;
		}

		DataBuffer++;
	}

	if(StopFlag)
	{
		IIC_Stop();
	}

werr_out:
	return err;
}
int IIC_Read(unsigned char channel,unsigned char ChipAddr, unsigned char *DataBuffer, unsigned char len)
{
	unsigned char j,k,m,tmp,readChar,tmpAddr;
	int err = NORMAL;

	tmpAddr = ChipAddr + 1;

	IIC_Start();

	IIC_Send8bitData(tmpAddr);

	if(IIC_CheckACK())
	{
		PRINTK_ERR("**[TUNER_Read]No ack!addr=%x\n",ChipAddr);
		err = NO_ACK;
		goto rerr_out;
	}

	m = len;
	for(k=0;k<len;k++)
	{
		readChar = 0;
		j=8;
		at91_set_gpio_value(IIC_SDA_PIN, GPIO_PIN_HIGH);

		at91_set_gpio_input(IIC_SDA_PIN, 0);//转换方向

		udelay(CLK_DELAY_US);
		while(j)
		{
			j--;
			at91_set_gpio_value(IIC_SCL_PIN, GPIO_PIN_HIGH);
			udelay(CLK_DELAY_US);
			tmp=at91_get_gpio_value(IIC_SDA_PIN);
			if(tmp>0)
				readChar = (1<<j) | readChar ;
			at91_set_gpio_value(IIC_SCL_PIN, GPIO_PIN_LOW);
			udelay(CLK_DELAY_US);
		}
		at91_set_gpio_output(IIC_SDA_PIN, GPIO_PIN_HIGH);

		if(m==1)
		{
			SendNoACK();
		}
		else
		{
			SendACK();	
		}
		m--;
		*DataBuffer=readChar;
		DataBuffer++;
	}

	IIC_Stop();
rerr_out:

	return err;
}

ssize_t TunerWrite(struct file *filp, const char __user *buf,size_t count,loff_t *f_pos)
{
	unsigned char Channel;
	unsigned char ChipAddr;
	unsigned char RegAddr;
	unsigned char StopFlag;
	unsigned char len;
	unsigned char head[5];
	unsigned char buffer[260];
	int status=0;
	int i;

	/* 检查参数 */
	if(count<2)
	{
		PRINTK_ERR("**[TunerWrite]count<2\n");
		return -EINVAL;
	}

	if(copy_from_user(head,buf,5))
	{
		PRINTK_ERR("**[TunerWrite]copy error\n");
		return -EFAULT;
	}
	i=0;
	Channel = head[i++];
	ChipAddr = head[i++];
	RegAddr = head[i++];
	StopFlag = head[i++];

	len = head[i++];
	if((len>0)&&(len<255))
	{
		if(copy_from_user(buffer , buf + 5, len))
		{
			return -EFAULT;
		}
	}

	status = IIC_Write(Channel,ChipAddr,RegAddr,buffer,len,StopFlag);
	if(status ==NORMAL)
	{
		status =len + 5;
	}
	else
	{
		PRINTK_ERR("IIC Write Status Error!\n",status);
	}
	return status;

}
ssize_t TunerRead(struct file *filp, char *buf,size_t count,loff_t *f_pos)
{
	unsigned char Channel;
	unsigned char ChipAddr;
	unsigned char len;
	unsigned char head[5];
	unsigned char buffer[260];
	int status;

	if(count<2)
	{
		return -EINVAL;
	}
	if(copy_from_user(head,buf,3))
	{
		return -EFAULT;
	}

	Channel = head[0];
	ChipAddr = head[1];
	len = head[2];
	memset(buffer,0,260);
	status = IIC_Read( Channel, ChipAddr, buffer, len);
	if(copy_to_user(buf,buffer,len))
	{
		return -EFAULT;
	}
	return status;
}
int TunerIoctl(struct inode *inode, struct file *filep, unsigned int cmd, unsigned long arg)
{
	unsigned char tmpData;
	unsigned char ch;
	unsigned short tmp16Data;
	unsigned int size;
	if(_IOC_TYPE(cmd) != IOCTOL_MAGIC) 
		return -EINVAL;
	if(_IOC_NR(cmd) >= IOCTL_MAXNR)
		return -EINVAL;
	size = _IOC_SIZE(cmd);
	if (size)
	{

	}
	switch(cmd)
	{
		case TUNER_RESET_SET:
			if  (get_user(tmp16Data, (unsigned short *) arg))
				return -EFAULT;
			tmpData =tmp16Data&0xff;
			ch = (unsigned char)((tmp16Data>>8)&0xff);
			//resetTuner(ch,tmpData);
			break;
		case TUNER_RESET_CLEAN:
			if  (get_user(tmp16Data, (unsigned short *) arg))
				return -EFAULT;
			tmpData = (unsigned char)(tmp16Data&0xff);
			ch = (unsigned char)((tmp16Data>>8)&0xff);
			//resetTuner(ch,tmpData);
			break;
	}
	return 0;
}
/*
*define mac
*/
struct file_operations tuner_device_fops=
{
	.owner = THIS_MODULE,
	.open = TunerOpen,
	.release = TunerRelease,
	.ioctl = TunerIoctl,
	.write = TunerWrite,
	.read = TunerRead,
};

int Tuner_init(void)
{
	int result;

	result = register_chrdev(TUNER_DEV_MINOR,TUNER_DRIVER_NAME,&tuner_device_fops);
	if(result<0)
		return result;

	return 0;
}
void Tuner_exit(void)
{
	unregister_chrdev(TUNER_DEV_MINOR,TUNER_DRIVER_NAME);
}

module_init(Tuner_init);
module_exit(Tuner_exit);

MODULE_AUTHOR("TONCY");
MODULE_DESCRIPTION("at91rm9200 tuner driver");
MODULE_LICENSE("GPL");
