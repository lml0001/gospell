
/* Use 'f' as magic number */
#define FAN_IOC_MAGIC  		'f'

#define FAN_IOCSETVAL		_IOW(FAN_IOC_MAGIC,  1, int)
#define FAN_IOCGETVAL		_IOR(FAN_IOC_MAGIC,  2, int)
#define FAN_CHECKHW			_IOR(FAN_IOC_MAGIC,  3, int)

#define FAN_IOC_MAXNR 14

