#ifndef IO_CTRL_DRIVER_H 
#define IO_CTRL_DRIVER_H 

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

#define IO_CTRL_DRIVER_NAME "io_ctrl_driver"
#define IO_CTRL_NODE_NAME "io_ctrl_driver"

#define IO_CTRL_IOCTL_MAGIE 'G'
#define IO_CTRL_IOCTL_MAXNR 2
#define IO_CTRL_CVBS1_RESET _IO(IO_CTRL_IOCTL_MAGIE, 0)
#define IO_CTRL_CVBS2_RESET _IO(IO_CTRL_IOCTL_MAGIE, 1)

#define IO_CTRL_CVBS1_RESET_PIN 					AT91_PIN_PB6	
#define IO_CTRL_CVBS1_AUDIO_RESET_PIN 		AT91_PIN_PB10	
#define IO_CTRL_CVBS1_AUDIO_BYPASS_PIN 	AT91_PIN_PB11	

#define IO_CTRL_CVBS2_RESET_PIN 					AT91_PIN_PA24	
#define IO_CTRL_CVBS2_AUDIO_RESET_PIN 		AT91_PIN_PA29	
#define IO_CTRL_CVBS2_AUDIO_BYPASS_PIN 	AT91_PIN_PB0	

#define IO_CTRL_AUTHOR "Gospell"
#define IO_CTRL_DESCRIPTION IO_CTRL_DRIVER_NAME
#define IO_CTRL_LICENSE "GPL"

#endif /* IO_CTRL_DRIVER_H */
/* EOF */
