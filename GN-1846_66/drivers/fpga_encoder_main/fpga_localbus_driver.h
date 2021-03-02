#ifndef FPGA_LOCALBUS_DRIVER_H
#define FPGA_LOCALBUS_DRIVER_H

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

#define FPGA_LOCALBUS_DRIVER_NAME "fpga_localbus_driver"
#define FPGA_LOCALBUS_NODE_NAME "fpga_localbus_driver"

#define FPGA_LOCALBUS_IOCTL_MAGIE 'L'
#define FPGA_LOCALBUS_IOCTL_MAXNR 1
#define FPGA_LOCALBUS_CONFIG  _IOW(FPGA_LOCALBUS_IOCTL_MAGIE, 0, int) 

#define GN_FPGA_NUM								(2) /* fpga数目 */
typedef enum
{
	GN_FPGA_INDEX_MAIN = 0,
	GN_FPGA_INDEX_IP_ASI_OUTPUT
}GN_FpgaIndex;

#define FPGA_MAIN_BASE_ADDR  0x30000000	
#define FPGA_IP_ASI_OUTPUT_BASE_ADDR  0x60000000	

typedef enum 
{
	LOCALBUS_NCS0 = 0,
	LOCALBUS_NCS1,
	LOCALBUS_NCS2,
	LOCALBUS_NCS3,
	LOCALBUS_NCS4,
	LOCALBUS_NCS5,
	LOCALBUS_NCS6,
	LOCALBUS_NCS7
}DRL_LocalbusCsrIndex;

#define FPGA_LOCALBUS_AUTHOR "Gospell"
#define FPGA_LOCALBUS_DESCRIPTION FPGA_LOCALBUS_DRIVER_NAME
#define FPGA_LOCALBUS_LICENSE "GPL"

typedef struct 
{
	int m_FpgaIndex;
	int m_RegAddr;
	unsigned short m_Data;
}DRL_FpgaLocalbusProtocol; /* Localbus组包协议 */

#endif /* FPGA_LOCALBUS_DRIVER_H */
/* EOF */
