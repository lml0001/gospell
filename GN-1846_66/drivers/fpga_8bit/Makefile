CP := cp
PWD := $(shell pwd)
SHARE_DIR := $(PWD)/../../fixeditem/GM8358Q/
KERNEL_DIR := /opt/linux-2.6.13-hzh
PATH :=$(PATH):/usr/local/arm/3.3.2/bin
CROSS_COMPILE = arm-linux-
ARCH = arm
obj-m := fpga.o


default:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules
	$(CP) fpga.ko $(SHARE_DIR)
	$(RM) *.ko
	$(RM) *.o
	$(RM) *.mod.*
	$(RM) .*.cmd
	$(RM) -rf ./.tmp_versions

clean:
	$(RM) *.ko
	$(RM) *.o
	$(RM) *.mod.*
	$(RM) .*.cmd
	$(RM) -rf ./.tmp_versions

# EOF
