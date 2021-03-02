#---------------------------------------------------------
# 不同设备的编译命令
#---------------------------------------------------------
# make PRODUCT=GN1866
# make PRODUCT=GN1866B
# make PRODUCT=GN1846

#---------------------------------------------------------
# 定义编译平台命令
#---------------------------------------------------------
PLF := arm
ifeq ($(PRODUCT), GN1866)
PLF := hi3519
endif
ifeq ($(PRODUCT), GN1866B)
PLF := hi3519
endif
ifeq ($(PRODUCT), GN1846)
PLF := hi3531a
endif

ifeq (,$(filter objs,$(notdir $(CURDIR))))
export PROJECT_DIR := $(CURDIR)/../../
include $(PROJECT_DIR)/workspace/include_global/global_config.mak
include $(PROJECT_DIR)/workspace/include_global/global_exec.mak

#add by leonli
web:
	$(ECHO)
	$(ECHO) Copy WEB Pages To $(EXPORT_WORK_DIR)
	$(CP) -rf $(MK_ROOT)/src/web $(EXPORT_WORK_DIR)

else
#---------------------------------------------------------
# 生成代码（obj）
#---------------------------------------------------------
TARGET			= main

INCLUDES	= -I$(MK_ROOT)/src/app -I$(MK_ROOT)/src/hwl -I$(MK_ROOT)/src/drl -I$(MK_ROOT)/src/frp  -I$(MK_ROOT)/../../workspace/include 
INCLUDES	+= -I$(MK_ROOT)/drivers/fan/
INCLUDES	+= -I$(MK_ROOT)/drivers/dvbci/
INCLUDES	+= -I$(MK_ROOT)/drivers/fpga/ 
INCLUDES	+= -I$(MK_ROOT)/src/card_module
INCLUDES	+= -I$(MK_ROOT)/../../workspace/include_global
INCLUDES	+= -I$(MK_ROOT)/../../workspace/include_driver
INCLUDES	+= -I$(MK_ROOT)/../../workspace/include/linux


ifeq ($(PRODUCT), GN1846)
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_encoder/encoder_3531A
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_encoder/encoder_3531A/hi_sdk_40/include
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_encoder/encoder_3531A/image_warp
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_encoder/encoder_audio_enc/include
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_encoder/encoder_audio_enc
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_encoder/encoder_es2ts
INCLUDES	+= -I$(MK_ROOT)/src/drl/fpga_ps_gpio
INCLUDES	+= -I$(MK_ROOT)/src/drl/hisi_gpio
INCLUDES	+= -I$(MK_ROOT)/src/drl/i2c_gpio
INCLUDES	+= -I$(MK_ROOT)/src/drl/spi_gpio
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_hdmi_recv_adv7612
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_mux
#add by leonli
#INCLUDES	+= -I$(MK_ROOT)/lib/GN1846/lib_adv7612
INCLUDES	+= -I$(MK_ROOT)/src/app/fpga_switch
endif

#add by leonli
ifeq ($(PRODUCT), GN1866)
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_encoder/encoder_3519V101
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_encoder/encoder_3519V101/hi_sdk_40/include
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_encoder/encoder_audio_enc/include
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_encoder/encoder_audio_enc
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_encoder/encoder_es2ts
INCLUDES	+= -I$(MK_ROOT)/src/drl/fpga_ps_gpio
INCLUDES	+= -I$(MK_ROOT)/src/drl/hisi_gpio
INCLUDES	+= -I$(MK_ROOT)/src/drl/i2c_gpio
INCLUDES	+= -I$(MK_ROOT)/src/drl/spi_gpio
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_hdmi_recv_adv7612
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_mux
#add by leonli
INCLUDES	+= -I$(MK_ROOT)/lib/GN1866/lib_adv7612
INCLUDES	+= -I$(MK_ROOT)/src/app/fpga_switch
endif

#add by leonli 
ifeq ($(PRODUCT), GN1866B)
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_encoder/encoder_3519V101
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_encoder/encoder_3519V101/hi_sdk_40/include
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_encoder/encoder_audio_enc/include
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_encoder/encoder_audio_enc
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_encoder/encoder_es2ts
INCLUDES	+= -I$(MK_ROOT)/src/drl/fpga_ps_gpio
INCLUDES	+= -I$(MK_ROOT)/src/drl/hisi_gpio
INCLUDES	+= -I$(MK_ROOT)/src/drl/i2c_gpio
INCLUDES	+= -I$(MK_ROOT)/src/drl/spi_gpio
#INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_hdmi_recv_adv7612
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_hdmi_recv_lt6911c
INCLUDES	+= -I$(MK_ROOT)/src/hwl/hwl_mux
#add by leonli
INCLUDES	+= -I$(MK_ROOT)/lib/GN1866/lib_adv7612
INCLUDES	+= -I$(MK_ROOT)/src/app/fpga_switch
endif

CFLAGS		= 
CFLAGS		+= $(CROSS_CFLAG)
CFLAGS		+= -D$(PRODUCT)
CFLAGS		+= -D$(PLF)

ifdef debug
CFLAGS		+=  -Wall -g
endif

LIBS		 = -L $(MK_ROOT)/../../workspace/lib 
LIBS		+=  -L $(MK_ROOT)/../../workspace/third/snmp/libs_$(PLF)/lib
LIBS		+=  -L $(MK_ROOT)/../../workspace/third/png/libs_$(PLF)/lib
LIBS		+=  -L $(MK_ROOT)/lib

ifeq ($(PRODUCT), GN1846)
LIBS	+= -L$(MK_ROOT)/src/hwl/hwl_encoder/encoder_3531A/hi_sdk_40/lib
LIBS	+= -L$(MK_ROOT)/src/hwl/hwl_encoder/encoder_audio_enc/lib
endif

#add by leonli
ifeq ($(PRODUCT), GN1866)
LIBS	+= -L$(MK_ROOT)/src/hwl/hwl_encoder/encoder_3519V101/hi_sdk_40/lib
LIBS	+= -L$(MK_ROOT)/src/hwl/hwl_encoder/encoder_audio_enc/lib
endif

#add by leonli
ifeq ($(PRODUCT), GN1866B)
LIBS	+= -L$(MK_ROOT)/src/hwl/hwl_encoder/encoder_3519V101/hi_sdk_40/lib
LIBS	+= -L$(MK_ROOT)/src/hwl/hwl_encoder/encoder_audio_enc/lib
endif

LDFLAGS		= 
LDFLAGS     += -Wl,--start-group
LDFLAGS		+= $(CROSS_LDFLAG)
LDFLAGS		+= -lrt -lpthread
LDFLAGS		+= -lnet_snmp_warp_$(PLF) -lnetsnmpmibs -lnetsnmpagent -lnetsnmp
LDFLAGS		+= -lfrp_menu_advance_$(PLF)
LDFLAGS		+= -ltd_$(PLF)
LDFLAGS		+= -lgps_$(PLF)
LDFLAGS		+= -lperipherals_$(PLF)
#LDFLAGS		+= -ltransenc_$(PLF)
#LDFLAGS		+= -ltransenc_adv_$(PLF)



ifeq ($(PRODUCT), GN1846)
LDFLAGS	+= -lmpi
LDFLAGS	+= -ldnvqe
LDFLAGS	+= -lhdmi
LDFLAGS	+= -live
LDFLAGS	+= -ljpeg6b
LDFLAGS	+= -ljpeg
LDFLAGS	+= -lmd
LDFLAGS	+= -lpciv
LDFLAGS	+= -ltde
LDFLAGS	+= -lupvqe
LDFLAGS	+= -lVoiceEngine
LDFLAGS	+= -ldl
LDFLAGS	+= -ltwolame
LDFLAGS	+= -lrt
LDFLAGS	+= -lfaac
LDFLAGS	+= -lpng
LDFLAGS	+= -lz
LDFLAGS	+= -limage_warp_$(PLF)
LDFLAGS	+= -ladv7612_$(PLF)
endif

#add by leonli
ifeq ($(PRODUCT), GN1866)
LDFLAGS	+= -lmpi
LDFLAGS	+= -ldnvqe
#LDFLAGS	+= -lhdmi
LDFLAGS	+= -live
LDFLAGS	+= -lmd
LDFLAGS	+= -lpciv
LDFLAGS	+= -ltde
LDFLAGS	+= -lupvqe
LDFLAGS	+= -lVoiceEngine
#1866 add
LDFLAGS	+= -l_hiae
LDFLAGS	+= -l_hiaf
LDFLAGS	+= -l_hiawb
LDFLAGS	+= -l_hiawb_dv
LDFLAGS	+= -lhi_cipher
LDFLAGS	+= -lhifisheyecalibrate
LDFLAGS	+= -lhistitch
LDFLAGS	+= -lisp
LDFLAGS	+= -l_hidefog

LDFLAGS	+= -ldl
LDFLAGS	+= -ltwolame
LDFLAGS	+= -lrt
LDFLAGS	+= -lfaac
LDFLAGS	+= -ladv7612_$(PLF)
LDFLAGS	+= -lz
LDFLAGS	+= -lpng
LDFLAGS	+= -limage_warp_$(PLF)
endif

#add by leonli
ifeq ($(PRODUCT), GN1866B)
LDFLAGS	+= -lmpi
LDFLAGS	+= -ldnvqe
#LDFLAGS	+= -lhdmi
LDFLAGS	+= -live
LDFLAGS	+= -lmd
LDFLAGS	+= -lpciv
LDFLAGS	+= -ltde
LDFLAGS	+= -lupvqe
LDFLAGS	+= -lVoiceEngine
#1866 add
LDFLAGS	+= -l_hiae
LDFLAGS	+= -l_hiaf
LDFLAGS	+= -l_hiawb
LDFLAGS	+= -l_hiawb_dv
LDFLAGS	+= -lhi_cipher
LDFLAGS	+= -lhifisheyecalibrate
LDFLAGS	+= -lhistitch
LDFLAGS	+= -lisp
LDFLAGS	+= -l_hidefog
LDFLAGS	+= -ldl
LDFLAGS	+= -ltwolame
LDFLAGS	+= -lrt
LDFLAGS	+= -lfaac
LDFLAGS	+= -ladv7612_$(PLF)
LDFLAGS	+= -lz
LDFLAGS	+= -lpng
LDFLAGS	+= -limage_warp_$(PLF)
endif

LDFLAGS		+= -lwebs_$(PLF)
LDFLAGS		+= -lmxml_$(PLF)
LDFLAGS		+= -lplatform_$(PLF)
LDFLAGS		+= -lrt
LDFLAGS		+= -lpthread 
LDFLAGS		+= $(MATH_LIB)
LDFLAGS     += -Wl,--end-group


ARFLAGS		=
CXXFLAGS	=


#---------------------------------------------------------
# 输入源代码路径
#---------------------------------------------------------
SRCDIR	= $(MK_ROOT)/
SRCDIR	+= $(MK_ROOT)/src/app
#SRCDIR	+= $(MK_ROOT)/src/card_module
SRCDIR	+= $(MK_ROOT)/src/hwl
SRCDIR	+= $(MK_ROOT)/src/drl
ifeq ($(PRODUCT), GN1846)
SRCDIR	+= $(MK_ROOT)/src/hwl/hwl_encoder/encoder_3531A
SRCDIR	+= $(MK_ROOT)/src/hwl/hwl_encoder/encoder_audio_enc
SRCDIR	+= $(MK_ROOT)/src/hwl/hwl_encoder/encoder_es2ts
SRCDIR	+= $(MK_ROOT)/src/drl/fpga_ps_gpio
SRCDIR	+= $(MK_ROOT)/src/drl/hisi_gpio
SRCDIR	+= $(MK_ROOT)/src/drl/i2c_gpio
SRCDIR	+= $(MK_ROOT)/src/drl/spi_gpio
SRCDIR  += $(MK_ROOT)/src/hwl/hwl_hdmi_recv_adv7612
SRCDIR  += $(MK_ROOT)/src/hwl/hwl_mux
SRCDIR	+= $(MK_ROOT)/src/app/fpga_switch
endif

#add by leonli
ifeq ($(PRODUCT), GN1866)
SRCDIR	+= $(MK_ROOT)/src/hwl/hwl_encoder/encoder_3519V101
SRCDIR	+= $(MK_ROOT)/src/hwl/hwl_encoder/encoder_audio_enc
SRCDIR	+= $(MK_ROOT)/src/hwl/hwl_encoder/encoder_es2ts
SRCDIR	+= $(MK_ROOT)/src/drl/fpga_ps_gpio
SRCDIR	+= $(MK_ROOT)/src/drl/hisi_gpio
SRCDIR	+= $(MK_ROOT)/src/drl/i2c_gpio
SRCDIR	+= $(MK_ROOT)/src/drl/spi_gpio
SRCDIR  += $(MK_ROOT)/src/hwl/hwl_hdmi_recv_adv7612
SRCDIR  += $(MK_ROOT)/src/hwl/hwl_mux
SRCDIR	+= $(MK_ROOT)/src/app/fpga_switch
endif

#add by leonli
ifeq ($(PRODUCT), GN1866B)
SRCDIR	+= $(MK_ROOT)/src/hwl/hwl_encoder/encoder_3519V101
SRCDIR	+= $(MK_ROOT)/src/hwl/hwl_encoder/encoder_audio_enc
SRCDIR	+= $(MK_ROOT)/src/hwl/hwl_encoder/encoder_es2ts
SRCDIR	+= $(MK_ROOT)/src/drl/fpga_ps_gpio
SRCDIR	+= $(MK_ROOT)/src/drl/hisi_gpio
SRCDIR	+= $(MK_ROOT)/src/drl/i2c_gpio
SRCDIR	+= $(MK_ROOT)/src/drl/spi_gpio
SRCDIR  += $(MK_ROOT)/src/hwl/hwl_hdmi_recv_lt6911c
SRCDIR  += $(MK_ROOT)/src/hwl/hwl_mux
SRCDIR	+= $(MK_ROOT)/src/app/fpga_switch
endif

#---------------------------------------------------------
# 添加虚拟路径（可以用mk存放在各个模块内部）
#---------------------------------------------------------
VPATH	+= $(SRCDIR)

#---------------------------------------------------------
# 生成OBJ
#---------------------------------------------------------
SRCS	= $(notdir $(foreach temp, $(SRCDIR), $(wildcard $(temp)/*.c)))
OBJS	= $(patsubst %.c, %.o, $(SRCS))

.PHONY:all

all:$(TARGET)

$(TARGET): $(OBJS)
	$(ECHO)
	$(ECHO) BUILD [$(TARGET)] USE [$(OBJS)]
	$(ECHO)
	$(ECHO) LIBS [$(LIBS)] 
	$(ECHO)
	$(ECHO) INCLUDES [$(INCLUDES)] 
	$(ECHO)
	$(ECHO) CFLAGS [$(CFLAGS)]
	$(ECHO)
	$(ECHO) LDFLAGS [$(LDFLAGS)]
	$(ECHO)
	
	$(ECHO) Make/Clean $(EXPORT_WORK_DIR)
	$(MKDIR) -p  $(EXPORT_WORK_DIR)
	$(RM) -rf $(EXPORT_WORK_DIR)/*

	
	$(CC) $^ -o $@ $(LIBS) $(LDFLAGS)
	$(STRIP) $@
			
	$(ECHO) Copy $(TARGET) To $(EXPORT_WORK_DIR)
	$(CP) $(TARGET) $(EXPORT_WORK_DIR)
	
	$(ECHO) Copy WEB Pages To $(EXPORT_WORK_DIR)
	$(CP) -rf $(MK_ROOT)/src/web $(EXPORT_WORK_DIR)
	
	$(ECHO) Copy CGI To WEB
	$(CP) -rf $(MK_ROOT)/cgi/bin_$(PLF)/* $(EXPORT_WORK_DIR)/web/cgi-bin/
	
ifeq ($(PRODUCT), LR1800S)
	$(ECHO) Copy Overwrite Pages To $(EXPORT_WORK_DIR)
	$(MV) -f $(EXPORT_WORK_DIR)/web/top_min.html $(EXPORT_WORK_DIR)/web/top.html
else ifeq ($(PRODUCT), GM7000)
	$(ECHO) Copy Overwrite Pages To $(EXPORT_WORK_DIR)
	$(MV) -f $(EXPORT_WORK_DIR)/web/top_com.html $(EXPORT_WORK_DIR)/web/top.html
	$(MV) -f $(EXPORT_WORK_DIR)/web/bottom_com.html $(EXPORT_WORK_DIR)/web/bottom.html
	$(MV) -f $(EXPORT_WORK_DIR)/web/io_config_com.html $(EXPORT_WORK_DIR)/web/io_config.html
endif

	$(RM) -f $(EXPORT_WORK_DIR)/web/top_min.html
	$(RM) -f $(EXPORT_WORK_DIR)/web/io_left_com.html
	$(RM) -f $(EXPORT_WORK_DIR)/web/io_config_com.html
	$(RM) -f $(EXPORT_WORK_DIR)/web/top_com.html
	$(RM) -f $(EXPORT_WORK_DIR)/web/bottom_com.html
	$(RM) -f $(EXPORT_WORK_DIR)/web/bottom_card.html
	
	$(ECHO) Copy FixedItem To $(EXPORT_WORK_DIR)
	$(CP) -rf $(EXPORT_FIXED_DIR)/* $(EXPORT_WORK_DIR)/
	
	
ifeq ($(PRODUCT), GM2730X)
	$(ECHO) Copy Init Shell To $(EXPORT_WORK_DIR)
	$(CP) -rf $(MK_ROOT)/shell/init_2730X.sh $(EXPORT_WORK_DIR)/init.sh
else ifeq ($(PRODUCT), GM2730H)
	$(ECHO) Copy Init Shell To $(EXPORT_WORK_DIR)
	$(CP) -rf $(MK_ROOT)/shell/init_2730X.sh $(EXPORT_WORK_DIR)/init.sh
else ifeq ($(PRODUCT), GM2730F)
	$(ECHO) Copy Init Shell To $(EXPORT_WORK_DIR)
	$(CP) -rf $(MK_ROOT)/shell/init_2730X.sh $(EXPORT_WORK_DIR)/init.sh
else ifeq ($(PRODUCT), GM2730S)
	$(ECHO) Copy Init Shell To $(EXPORT_WORK_DIR)
	$(CP) -rf $(MK_ROOT)/shell/init_2730X.sh $(EXPORT_WORK_DIR)/init.sh
else ifeq ($(PRODUCT), GM2700S)
	$(ECHO) Copy Init Shell To $(EXPORT_WORK_DIR)
	$(CP) -rf $(MK_ROOT)/shell/init_2700X.sh $(EXPORT_WORK_DIR)/init.sh
else ifeq ($(PRODUCT), GC1818A)
	$(ECHO) Copy Init Shell To $(EXPORT_WORK_DIR)
	$(CP) -rf $(MK_ROOT)/shell/init_1818A.sh $(EXPORT_WORK_DIR)/init.sh
else ifeq ($(PRODUCT), GC1815B)
	$(ECHO) Copy Init Shell To $(EXPORT_WORK_DIR)
	$(CP) -rf $(MK_ROOT)/shell/init_1815.sh $(EXPORT_WORK_DIR)/init.sh
else ifeq ($(PRODUCT), GN1846)
	$(ECHO) Copy Init Shell To $(EXPORT_WORK_DIR)
	$(CP) -rf $(MK_ROOT)/shell/init_1846.sh $(EXPORT_WORK_DIR)/init.sh
#add by leonli
else ifeq ($(PRODUCT), GN1866)
	$(ECHO) Copy Init Shell To $(EXPORT_WORK_DIR)
	$(CP) -rf $(MK_ROOT)/shell/init_1866.sh $(EXPORT_WORK_DIR)/init.sh	
#add by leonli
else ifeq ($(PRODUCT), GN1866B)
	$(ECHO) Copy Init Shell To $(EXPORT_WORK_DIR)
	$(CP) -rf $(MK_ROOT)/shell/init_1866B.sh $(EXPORT_WORK_DIR)/init.sh
else
	$(ECHO) Copy Init Shell To $(EXPORT_WORK_DIR)
	$(CP) -rf $(MK_ROOT)/shell/init.sh $(EXPORT_WORK_DIR)/
endif
	$(CP) -rf $(MK_ROOT)/shell/ram_run.sh $(EXPORT_WORK_DIR)/

	$(ECHO) Copy Default Shell To $(EXPORT_WORK_DIR)../
	$(CP) -rf $(MK_ROOT)/shell/run.sh $(EXPORT_WORK_DIR)/../
	
	#$(ECHO) Chmod 777 exec files
	#$(PUSHD) $(EXPORT_WORK_DIR) && $(CHMOD) 777 ./syslogd_modified && $(CHMOD) 777 ./main && $(CHMOD) 777 ./web/cgi-bin/upload.cgi && $(POPD) 
	
	sleep 1 # 不加该句，可能在 tar 时报错：在我们读入文件时文件发生了变化
	$(ECHO) Packe All Data To Tar File
	$(PUSHD) $(EXPORT_WORK_DIR) && $(TAR) -zcf ../$(TARGET_PACKET) ./* && $(POPD) 

%o : %c
	$(ECHO) $@ Compiling $(notdir $<)... 
	$(CC) -c $(INCLUDES) $(CFLAGS) $< -o $@ 

endif
# EOF
