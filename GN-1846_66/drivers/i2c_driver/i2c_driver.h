#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

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

#define I2C_DRIVER_NAME "i2c_driver"
#define I2C_NODE_NAME "i2c_driver"

#define I2C_IOCTL_MAGIE 'I'
#define I2C_IOCTL_MAXNR 1
#define I2C_GPIO_INIT  _IOW(I2C_IOCTL_MAGIE, 0, int) 

#define GN_I2C_NUM								(8) /* IICͨ����Ŀ */
typedef enum
{
	GN_I2C_INDEX_CVBS_CH1 = 0, 
	GN_I2C_INDEX_CVBS_CH2,	
	GN_I2C_INDEX_CVBS_CH3,
	GN_I2C_INDEX_CVBS_CH4,
	GN_I2C_INDEX_CVBS_CH5,	
	GN_I2C_INDEX_CVBS_CH6,	
	GN_I2C_INDEX_CVBS_CH7,
	GN_I2C_INDEX_CVBS_CH8
}GN_I2cIndex;

#define I2C_CVBS1_SDA_PIN_1		AT91_PIN_PB8
#define I2C_CVBS1_SCL_PIN_1		AT91_PIN_PB7
#define I2C_CVBS1_SDA_PIN_2		AT91_PIN_PB9
#define I2C_CVBS1_SCL_PIN_2		AT91_PIN_PB7

#define I2C_CVBS2_SDA_PIN_1		AT91_PIN_PA27
#define I2C_CVBS2_SCL_PIN_1		AT91_PIN_PA26
#define I2C_CVBS2_SDA_PIN_2		AT91_PIN_PA28
#define I2C_CVBS2_SCL_PIN_2		AT91_PIN_PA26

#define I2C_AUTHOR "Gospell"
#define I2C_DESCRIPTION I2C_DRIVER_NAME
#define I2C_LICENSE "GPL"

#define I2C_DATA_BUFFER_SIZE 512
typedef struct 
{
	char m_I2cIndex;
	int m_DevAddr; /* �豸��ַ,���ڵ�ǰ���ǵ��豸ʹ�õĶ���7λ��IICѰַоƬ���������������7λ�ģ�����10λѰַ��ʹ��ʱ����չ */
	int m_RegAddrWidth; 
	char m_RegAddr[4]; /* ���ֽ���ǰ */
	int m_DataLen; /* ���ݳ��� */
	char m_Data[I2C_DATA_BUFFER_SIZE];
}DRL_I2cProtocol; /* I2C���Э�� */

#endif /* I2C_DRIVER_H */
/* EOF */
