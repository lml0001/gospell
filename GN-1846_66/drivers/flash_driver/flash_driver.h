#ifndef __flash_driver_h__
#define __flash_driver_h__

#define SPI_FLASH_DEV_MINOR 255
#define SPI_FLASH_DRIVER_NAME "spi_flash_driver"


#define IOCTOL_MAGIE 'E'
#define CMD_CLR_CS  _IOW(IOCTOL_MAGIE, 0,unsigned int) 
#define CMD_SET_CS  _IOW(IOCTOL_MAGIE, 1,unsigned int) 
#define CMD_FLASH_CONNECT_TO_ARM _IOW(IOCTOL_MAGIE, 2,unsigned int) 
#define CMD_FLASH_CONNECT_TO_PRO100 _IOW(IOCTOL_MAGIE, 3,unsigned int)
#define CMD_SET_DUMMY_CYCLE  _IOW(IOCTOL_MAGIE, 4,unsigned int) 

#define IOCTL_MAXNE 5


#endif
