#include <linux/config.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>	/* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <linux/spinlock.h>

#include <asm/system.h>		/* cli(), *_flags */
#include <asm/uaccess.h>	/* copy_*_user */
#include <asm/arch-at91rm9200/gpio.h>

#include "fan.h"


static int fan_major=210;
static int fan_minor=1;
static int fan_run=0;
static spinlock_t  fan_lock=SPIN_LOCK_UNLOCKED;

MODULE_LICENSE("GPL");


int fan_ioctl(struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg)
{
	int err=0;
	int retval=0;
	
	if (_IOC_TYPE(cmd) != FAN_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > FAN_IOC_MAXNR) return -ENOTTY;

	/*
	 * the direction is a bitmask, and VERIFY_WRITE catches R/W
	 * transfers. `Type' is user-oriented, while
	 * access_ok is kernel-oriented, so the concept of "read" and
	 * "write" is reversed
	 */
	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if (err) return -EFAULT;
	
	
	switch(cmd){
		int run;
		case FAN_IOCSETVAL:
			if (! capable (CAP_SYS_ADMIN))
				return -EPERM;
				
			retval = __get_user(run, (int __user *)arg);
			spin_lock(&fan_lock);
			fan_run=run;
			//at91_set_gpio_value(AT91_PIN_PC11,fan_run);
			at91_set_gpio_output(AT91_PIN_PC11,fan_run);
			spin_unlock(&fan_lock);
			
			break;
		
		case FAN_IOCGETVAL:
			spin_lock(&fan_lock);
			run=at91_get_gpio_value(AT91_PIN_PC11);
			fan_run=run;
			spin_unlock(&fan_lock);
			retval = __put_user(fan_run, (int __user *)arg);
			
			break;
		case FAN_CHECKHW:
			spin_lock(&fan_lock);
			run=at91_get_gpio_value(AT91_PIN_PC12);
			fan_run=run;
			spin_unlock(&fan_lock);
			retval = __put_user(fan_run, (int __user *)arg);

			break;		
		default:
			retval=ENOTTY;
	}
	
	

	return retval;
}


struct file_operations fan_fops={
	.owner=THIS_MODULE,
	.ioctl=fan_ioctl,
};



static struct cdev fan_dev;


static __init int fan_init(void)
{	
	dev_t dev=0;
	int ret;
	
	dev=MKDEV(fan_major,fan_minor);
	ret=register_chrdev_region(dev,1,"fan");
	if(ret<0)
	{
	  printk(KERN_INFO "fan: can't register major:%d minor:%d\n",fan_major,fan_minor);
	  return ret;
	}
	
	cdev_init(&fan_dev,&fan_fops);
	fan_dev.owner=THIS_MODULE;
	fan_dev.ops=&fan_fops;
	ret=cdev_add(&fan_dev,dev,1);

	if (ret){
	  printk(KERN_INFO "Error %d adding fan cdev", ret);
	  return ret;
	}
	
	//AT91_SYS ->PIOC_PER |= AT91_PIN_PC11;
	//AT91_SYS ->PIOC_ODR |=  AT91_PIN_PC11;
	
	at91_set_gpio_output(AT91_PIN_PC11,fan_run);
	at91_set_gpio_input(AT91_PIN_PC12,1);
	//at91_set_gpio_value(AT91_PIN_PC11,fan_run);

	return 0;
}

static __exit void fan_exit(void)
{
	dev_t devno = MKDEV(fan_major, fan_minor);
	unregister_chrdev_region(devno, 1);
	cdev_del(&fan_dev);
}

module_init(fan_init);
module_exit(fan_exit);


