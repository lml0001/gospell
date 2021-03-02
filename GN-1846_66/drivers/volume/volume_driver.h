#ifndef VOLUME_DRIVER_H
#define VOLUME_DRIVER_H

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

#define VOLUME_DRIVER_NAME "volume_driver"
#define VOLUME_NODE_NAME "volume_driver"

#define VOLUME_IOCTL_MAGIE 'V'
#define VOLUME_IOCTL_MAXNR 1
#define VOLUME_GPIO_INIT  _IOW(VOLUME_IOCTL_MAGIE, 0, int) 

#define GN_VOLUME_NUM								(8) /* 音量控制通信数目 */
typedef enum
{
	GN_VOLUME_INDEX_CVBS_CH1 = 0,
	GN_VOLUME_INDEX_CVBS_CH2,	
	GN_VOLUME_INDEX_CVBS_CH3,
	GN_VOLUME_INDEX_CVBS_CH4,
	GN_VOLUME_INDEX_CVBS_CH5,
	GN_VOLUME_INDEX_CVBS_CH6,	
	GN_VOLUME_INDEX_CVBS_CH7,
	GN_VOLUME_INDEX_CVBS_CH8
}GN_VolumeIndex;

#define VOLUME_CVBS1_SDA_PIN		AT91_PIN_PB16
#define VOLUME_CVBS1_SCL_PIN		AT91_PIN_PC15

#define VOLUME_CVBS2_SDA_PIN		AT91_PIN_PB15
#define VOLUME_CVBS2_SCL_PIN		AT91_PIN_PC0

#define VOLUME_CVBS3_SDA_PIN		AT91_PIN_PB14
#define VOLUME_CVBS3_SCL_PIN		AT91_PIN_PC1

#define VOLUME_CVBS4_SDA_PIN		AT91_PIN_PB13
#define VOLUME_CVBS4_SCL_PIN		AT91_PIN_PA0

#define VOLUME_CVBS5_SDA_PIN		AT91_PIN_PB5
#define VOLUME_CVBS5_SCL_PIN		AT91_PIN_PB12

#define VOLUME_CVBS6_SDA_PIN		AT91_PIN_PB4
#define VOLUME_CVBS6_SCL_PIN		AT91_PIN_PB1

#define VOLUME_CVBS7_SDA_PIN		AT91_PIN_PB3
#define VOLUME_CVBS7_SCL_PIN		AT91_PIN_PA21

#define VOLUME_CVBS8_SDA_PIN		AT91_PIN_PB2
#define VOLUME_CVBS8_SCL_PIN		AT91_PIN_PA19

#define VOLUME_AUTHOR "Gospell"
#define VOLUME_DESCRIPTION VOLUME_DRIVER_NAME
#define VOLUME_LICENSE "GPL"

#define VOLUME_DATA_BUFFER_SIZE 2
typedef struct 
{
	char m_VolumeIndex;
	int m_DataLen; /* 数据长度 */
	char m_Data[VOLUME_DATA_BUFFER_SIZE];
}DRL_VolumeProtocol; /* Volume组包协议 */

#endif /* VOLUME_DRIVER_H */
/* EOF */
