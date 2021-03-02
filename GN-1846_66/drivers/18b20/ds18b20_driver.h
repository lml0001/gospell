#ifndef DS18B20_DRIVER_H
#define DS18B20_DRIVER_H

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

#define DS18B20_DRIVER_NAME "ds18b20_driver"
#define DS18B20_NODE_NAME "ds18b20_driver"

//#define DS18B20_DQ_PIN AT91_PIN_PC3 /* DS18B20获取数据的引脚 */
#define DS18B20_DQ_PIN AT91_PIN_PB17 /* DS18B20获取数据的引脚 */

#define DS18B20_AUTHOR "Gospell"
#define DS18B20_DESCRIPTION DS18B20_DRIVER_NAME
#define DS18B20_LICENSE "GPL"

#endif /* DS18B20_DRIVER_H */
/* EOF */
