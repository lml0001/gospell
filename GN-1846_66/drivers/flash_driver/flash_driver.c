#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/delay.h>

#include <linux/config.h>

#include <asm/arch/AT91RM9200_SYS.h>
#include <asm/arch/hardware.h>
#include <asm/arch/board.h>
#include <asm/arch/pio.h>

#include <asm/uaccess.h>
#include <asm/io.h>

#include "flash_driver.h"


static int	s_DummyCycle = 0;

#define  HARD_WARE_CTRL 1							//1使用1886B-A1主板飞线测试  2、使用pro100 ？  3、带FPGA switch （arm 与pro100）功能

#if HARD_WARE_CTRL == 1
	#define DCLK_PIN     AT91C_PIO_PA6			    //13   modified from PC12 to PA6 (1883->1738)
	#define CS_PIN       AT91C_PIO_PA25	            //15   modified from PC14 to PA25 (1883->1738)
	#define DATAOUT_PIN  AT91C_PIO_PA4			    //17   modified from PC1 to PA4 (1883->1738)
	#define DATAIN_PIN   AT91C_PIO_PA5		        //18   modified from PC2 to PA5 (1883->1738)
#elif HARD_WARE_CTRL == 2
	#define DCLK_PIN     AT91C_PIO_PB0			    //70
	#define CS_PIN       AT91C_PIO_PB1              //72	
	#define DATAOUT_PIN  AT91C_PIO_PB2   		    //73
	#define DATAIN_PIN   AT91C_PIO_PB3	            //74
#else
	#define DCLK_PIN     AT91C_PIO_PB22
	#define CS_PIN       AT91C_PIO_PA20
	#define DATAOUT_PIN  AT91C_PIO_PA25
	#define DATAIN_PIN   AT91C_PIO_PB27
	#define   FLASH_CONNECT_TO_ARM_PIN AT91C_PIO_PC14
	#define   FLASH_CONNECT_TO_PRO100_PIN AT91C_PIO_PC15
#endif



#undef PDEBUG
//#define PDEBUG(fmt, args...) printk(fmt, ##args)
#define PDEBUG(fmt, args...)

#undef PWHERE
//#define PWHERE printk("fc :%s line:%d\n", __FUNCTION__, __LINE__)
#define PWHERE

#if HARD_WARE_CTRL == 1
	#define SET(n) (AT91_SYS ->PIOA_SODR |= (n))   // modified from PIOC to PIOA
	#define CLEAN(n) (AT91_SYS ->PIOA_CODR |= (n))  // modified from PIOC to PIOA
#elif HARD_WARE_CTRL == 2
	#define SET(n) (AT91_SYS ->PIOB_SODR |= (n))
	#define CLEAN(n) (AT91_SYS ->PIOB_CODR |= (n))
#else
	#define SET_A(n) (AT91_SYS ->PIOA_SODR |= (n))
	#define CLEAN_A(n) (AT91_SYS ->PIOA_CODR |= (n))

	#define SET_B(n) (AT91_SYS ->PIOB_SODR |= (n))
	#define CLEAN_B(n) (AT91_SYS ->PIOB_CODR |= (n))

	#define SET_C(n) (AT91_SYS ->PIOC_SODR |= (n))
	#define CLEAN_C(n) (AT91_SYS ->PIOC_CODR |= (n))
#endif
/*
 * Init device
 */
static inline void at91_init_SPI_Port(void)
{
#if HARD_WARE_CTRL == 1    
	AT91_SYS ->PIOA_PER = DCLK_PIN | CS_PIN| DATAOUT_PIN | DATAIN_PIN;  // modified from PIOC to PIOA
	AT91_SYS ->PIOA_OER = DCLK_PIN | CS_PIN | DATAOUT_PIN ; // modified from PIOC to PIOA
	AT91_SYS ->PIOA_ODR |= DATAIN_PIN;    // modified from PIOC to PIOA
#elif HARD_WARE_CTRL == 2
	AT91_SYS ->PIOB_PER = DCLK_PIN | CS_PIN| DATAOUT_PIN | DATAIN_PIN;
	AT91_SYS ->PIOB_OER = DCLK_PIN | CS_PIN | DATAOUT_PIN ;
	AT91_SYS ->PIOB_ODR |= DATAIN_PIN;
#else
	AT91_SYS ->PIOB_PER = DCLK_PIN | DATAIN_PIN;
	AT91_SYS ->PIOA_PER = CS_PIN | DATAOUT_PIN;
	AT91_SYS ->PIOB_OER = DCLK_PIN;
	AT91_SYS ->PIOB_ODR |= DATAIN_PIN;
	AT91_SYS ->PIOA_OER = CS_PIN | DATAOUT_PIN;

	AT91_SYS ->PIOC_PER = FLASH_CONNECT_TO_ARM_PIN | FLASH_CONNECT_TO_PRO100_PIN;
	AT91_SYS ->PIOC_OER = FLASH_CONNECT_TO_ARM_PIN | FLASH_CONNECT_TO_PRO100_PIN;
#endif
}
int SPI_FLASHOpen(struct inode *inode,struct file *filep )
{
		at91_init_SPI_Port();
		return 0;
}
int SPI_FLASHRelease(struct inode *inode,struct file *filep )
{
	return 0;
}

static int SPI_FLASH_write_byte( unsigned char data)
{
	int i;
	//PDEBUG("send data:%x\n", data);


   ///* while (1)
   // {
   //     CLEAN(DCLK_PIN);
   //     CLEAN(DATAOUT_PIN);
   //     CLEAN(CS_PIN);
   //     CLEAN(DATAIN_PIN);
   //     printk("clean all 4 arm signals\n");
   //     
   //     udelay(1000);

   //     SET(DATAIN_PIN);
   //     SET(DATAOUT_PIN);
   //     SET(DCLK_PIN);
   //     SET(CS_PIN);
   //     printk("set all 4 arm signals\n");


   //     udelay(1000);
   // }*/



	for(i=0;i<8;i++)
	{ 	
		#if HARD_WARE_CTRL == 1
			CLEAN(DCLK_PIN);
		#elif  HARD_WARE_CTRL == 2
			CLEAN(DCLK_PIN);
		#else
			CLEAN_B(DCLK_PIN);
		#endif

		if(( data >>(7-i)) & 1 )	
		{
			// write 1;		
			#if HARD_WARE_CTRL == 1
						SET(DATAOUT_PIN);
			#elif  HARD_WARE_CTRL == 2
						SET(DATAOUT_PIN);
			#else
						SET_A(DATAOUT_PIN);
			#endif			
		}						
		else
		{
			// write 0;	
			#if HARD_WARE_CTRL == 1
						CLEAN(DATAOUT_PIN);
			#elif  HARD_WARE_CTRL == 2
						CLEAN(DATAOUT_PIN);
			#else
						CLEAN_A(DATAOUT_PIN);
			#endif	
		}				
		#if HARD_WARE_CTRL == 1
				SET(DCLK_PIN);
		#elif  HARD_WARE_CTRL == 2
				SET(DCLK_PIN);
		#else
				SET_B(DCLK_PIN);
		#endif
	}	
	return 0;
}

static int SPI_FLASH_read_byte(unsigned char *pdata)
{
	int i;
	unsigned char read_data=0x00;
	int pin_high_low;

	for(i=0;i<8;i++)
	{ 	
#if HARD_WARE_CTRL == 1
		CLEAN(DCLK_PIN);
#elif  HARD_WARE_CTRL == 2
		CLEAN(DCLK_PIN);
#else
		CLEAN_B(DCLK_PIN);
#endif

#if HARD_WARE_CTRL == 1
		SET(DCLK_PIN);
#elif  HARD_WARE_CTRL == 2
		SET(DCLK_PIN);
#else
		SET_B(DCLK_PIN);
#endif

#if HARD_WARE_CTRL == 1
		if (AT91_SYS ->PIOA_PDSR & DATAIN_PIN)   // modified from PIOC to PIOA
#elif  HARD_WARE_CTRL == 2
		if (AT91_SYS ->PIOB_PDSR & DATAIN_PIN)
#else
		if (AT91_SYS ->PIOB_PDSR & DATAIN_PIN)
#endif
		{
			pin_high_low = 1;
		}
		else
		{
			pin_high_low = 0;
		}
		read_data |= pin_high_low<<(7-i);				
	}	
	*pdata = read_data;
	//PDEBUG("read byte:%x\n", read_data);
	return 0;
}

static int SPI_FLASH_write_bytes(const unsigned char  *pdata, int num_bytes)
{   	
	if (pdata)
	{
		for (;num_bytes>0; num_bytes--)
		{
			SPI_FLASH_write_byte(*pdata);
			pdata++;
		}		
	}	
	return 0;
}

static int SPI_FLASH_read_bytes(unsigned char *pdata, int num_bytes)
{   
	int i;
	if (!pdata || num_bytes==0)
	{
		return 0;
	}

	for(i=0;i<s_DummyCycle;i++)
	{ 	
		CLEAN(DCLK_PIN);
		SET(DCLK_PIN);
	}	

	for (;num_bytes>0; num_bytes--)
	{
		SPI_FLASH_read_byte(pdata);
		pdata++;
	}
	return 0;
}

static int SPI_FLASH_driver_write(struct file *pfile,const char __user *pdata_usr,size_t data_len, loff_t * f_pos)
{   
	unsigned char *pdata=NULL;
		
	
	if (!pdata_usr || data_len==0)
	{
		return 0;
	}

	pdata = kmalloc(data_len, GFP_KERNEL);
	if (!pdata)
	{
		printk("fuction:%s,kmalloc failed!", __FUNCTION__);
		return 0;
	}
	if(copy_from_user(pdata, pdata_usr, data_len))
	{
		printk("fuction:%s,copy_from_user failed!", __FUNCTION__);
		kfree(pdata);
		return -1;
	}

	SPI_FLASH_write_bytes(pdata, data_len);
	kfree(pdata);		

	return data_len;
}
static int SPI_FLASH_Driver_read(struct file *pfile, char *pdata_usr, size_t data_len,loff_t *f_pos)
{	
	unsigned char *pdata;
	int i;
	if (!data_len)
	{
		return 0;
	}
	
	pdata = kmalloc(data_len, GFP_KERNEL);
	if (!pdata)
	{
		printk("fuction:%s,kmalloc failed!", __FUNCTION__);
		return 0;
	}
	SPI_FLASH_read_bytes(pdata, data_len);
	if (data_len == 7)
	{
		PDEBUG("\n");
		for (i=0; i<7; i++)
		{
			PDEBUG("%x ", *(pdata+i));
		}
		PDEBUG("\n");
	}
	if(copy_to_user(pdata_usr, pdata, data_len))
	{
		printk("fuction:%s,copy_to_user failed!", __FUNCTION__);
		kfree(pdata);
		return 0;
	}
	kfree(pdata);	

	return data_len;
}

int SPI_FLASH_Ioctl(struct inode *inode, struct file *filep, unsigned int cmd, unsigned long arg)
{
	unsigned long size;
	if(_IOC_TYPE(cmd) != IOCTOL_MAGIE) 
		return -EINVAL;
	if(_IOC_NR(cmd) >= IOCTL_MAXNE)
		return -EINVAL;
	size = _IOC_SIZE(cmd);	
	if (cmd == CMD_CLR_CS)
	{
		#if HARD_WARE_CTRL == 1
				CLEAN(CS_PIN);
		#elif  HARD_WARE_CTRL == 2
				CLEAN(CS_PIN);
		#else
				CLEAN_A(CS_PIN);
		#endif
	}
	else if(cmd == CMD_SET_CS)
	{
		#if HARD_WARE_CTRL == 1
				SET(CS_PIN);
		#elif  HARD_WARE_CTRL == 2
				SET(CS_PIN);
		#else
				SET_A(CS_PIN);
		#endif
	}
	else if(cmd == CMD_SET_DUMMY_CYCLE)
	{
		s_DummyCycle = arg;
	}
#if ((HARD_WARE_CTRL != 1) &&  (HARD_WARE_CTRL !=2))
	else if(cmd == CMD_FLASH_CONNECT_TO_ARM)
	{
		SET_C(FLASH_CONNECT_TO_PRO100_PIN);
		CLEAN_C(FLASH_CONNECT_TO_ARM_PIN);
	}
	else if(cmd == CMD_FLASH_CONNECT_TO_PRO100)
	{
		SET_C(FLASH_CONNECT_TO_ARM_PIN);
		CLEAN_C(FLASH_CONNECT_TO_PRO100_PIN);
	}
#endif
	return 0;	
}
struct file_operations SPI_FLASH_device_fops=
{
	.owner = THIS_MODULE,
	.open = SPI_FLASHOpen,
	.release = SPI_FLASHRelease,
	.ioctl = SPI_FLASH_Ioctl,
	.read = SPI_FLASH_Driver_read,
	.write = SPI_FLASH_driver_write,
};

int SPI_FLASH_driver_init(void)
{
		int result;
		
		result = register_chrdev(SPI_FLASH_DEV_MINOR,SPI_FLASH_DRIVER_NAME,&SPI_FLASH_device_fops);
		if(result<0)
			return result;
		
		return 0;
}
void SPI_FLASH_driver_exit(void)
{
	unregister_chrdev(SPI_FLASH_DEV_MINOR,SPI_FLASH_DRIVER_NAME);
}

module_init(SPI_FLASH_driver_init);
module_exit(SPI_FLASH_driver_exit);

MODULE_AUTHOR("WQL");
MODULE_DESCRIPTION("at91rm9200 SPI_FLASH_io driver");
MODULE_LICENSE("GPL");

