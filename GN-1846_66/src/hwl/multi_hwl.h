#ifndef _MULTI_HWL_H_
#define _MULTI_HWL_H_

#include "global_def.h"
#include "global_micros.h"
#include "multi_private.h"
#include "multi_hwl_tags.h"
#include "liu_iic.h"
#include "TunerDemod.h"
#include "hwl_sdi_spi.h"
#include "spi_gpio.h"
#include "hwl_hdmi_recv_adv7612/hdmi_rx.h"

#define HWL_MAX_CHANNEL_NUM		8
#define NEW_HARDWARE_FOR_LAST_AMP			1//2013.2.21新增DS更改末级放大器对应软件代码。

#define HWL_TUNER_MAX_NUM   4

#define HWL_ASI_MAX_NUM   8

#define HWL_NEW_HARD		0   //4350

//xub added 2018/05/15 for SDI
#if defined(GN1846) || defined(GN1866)  || defined(GN1866B)

typedef struct{
	U32 m_GpioIndex;
	U32 m_Pin;
} HWL_SDI_GpioPin;

#ifndef LEVEL_HIGH
#define LEVEL_HIGH 1
#endif
#ifndef LEVEL_LOW
#define LEVEL_LOW 0
#endif

#define SDI_GRP1_CH_1_2								2
#define SDI_GRP1_CH_3_4								3
#define SDI_GRP2_CH_5_6								4
#define SDI_GRP2_CH_7_8								5
#define SDI_GRP3_CH_9_10								6
#define SDI_GRP3_CH_11_12								7
#define SDI_GRP4_CH_13_14								8
#define SDI_GRP4_CH_15_16								9

/* 信号锁定 */
#define GS2970_LOCKED 1
#define GS2970_UNLOCKED 0

/* SDI音频嵌入组 */
#define GS2970_SDI_AUD_GRP1 0x0
#define GS2970_SDI_AUD_GRP2 0x1
#define GS2970_SDI_AUD_GRP3 0x2
#define GS2970_SDI_AUD_GRP4 0x3

/* 音频输出通道是否静音 */
#define GS2970_AOUT_MUTE 0x1
#define GS2970_AOUT_NO_MUTE 0x0

/* AOUT大小端选择 */
#define GS2970_AOUT_MSB 0x0
#define GS2970_AOUT_LSB 0x1

/* 音频输出格式 */
#define GS2970_AOUT_FMT_AES_EBU 0x0
#define GS2970_AOUT_FMT_LEFT_JUSTIFIED 0x1
#define GS2970_AOUT_FMT_RIGHT_JUSTIFIED 0x2
#define GS2970_AOUT_FMT_I2S 0x3

/* 输出音频通道字长 */
#define GS2970_AUD_WORDLEN_24BIT 0x0
#define GS2970_AUD_WORDLEN_20BIT 0x1
#define GS2970_AUD_WORDLEN_16BIT 0x2
#define GS2970_AUD_WORDLEN_AUTO 0x3 /* SD: 自动 HD: 无效 */

/* 音频输出通道 */
#define GS2970_GRPA_CH1 0x0
#define GS2970_GRPA_CH2 0x1
#define GS2970_GRPA_CH3 0x2
#define GS2970_GRPA_CH4 0x3
#define GS2970_GRPB_CH1 0x4
#define GS2970_GRPB_CH2 0x5
#define GS2970_GRPB_CH3 0x6
#define GS2970_GRPB_CH4 0x7

/* ��Ƶ���ͨ���Ƿ��� */
#define GS2970_AOUT_MUTE 0x1
#define GS2970_AOUT_NO_MUTE 0x0

/* AOUT��С��ѡ�� */
#define GS2970_AOUT_MSB 0x0
#define GS2970_AOUT_LSB 0x1

/* �����Ƶͨ���ֳ� */
#define GS2970_AUD_WORDLEN_24BIT 0x0
#define GS2970_AUD_WORDLEN_20BIT 0x1
#define GS2970_AUD_WORDLEN_16BIT 0x2
#define GS2970_AUD_WORDLEN_AUTO 0x3 /* SD: �Զ� HD: ��Ч */

/* 视频格式 */
enum      
{	
	GS2970_VID_FMT_1920_1080_50P, /* 3G */
	GS2970_VID_FMT_1920_1080_59_94P,
	GS2970_VID_FMT_1920_1080_60P ,

	GS2970_VID_FMT_1920_1080_50I , /* HD */
	GS2970_VID_FMT_1920_1080_59_94I,
	GS2970_VID_FMT_1920_1080_60I,

	GS2970_VID_FMT_1280_720_50P ,
	GS2970_VID_FMT_1280_720_59_94P,	
	GS2970_VID_FMT_1280_720_60P,

	GS2970_VID_FMT_720_576_50I, /* SD */
	GS2970_VID_FMT_704_576_50I,
	GS2970_VID_FMT_640_576_50I,
	GS2970_VID_FMT_544_576_50I,
	GS2970_VID_FMT_480_576_50I,
	GS2970_VID_FMT_352_576_50I,
	GS2970_VID_FMT_352_288_50I,
	GS2970_VID_FMT_320_288_50I,

	GS2970_VID_FMT_720_480_60I, 
	GS2970_VID_FMT_704_480_60I, 
	GS2970_VID_FMT_640_480_60I, 
	GS2970_VID_FMT_544_480_60I, 
	GS2970_VID_FMT_480_480_60I,
	GS2970_VID_FMT_352_480_60I, 
	GS2970_VID_FMT_352_240_60I,
	GS2970_VID_FMT_320_240_60I, 
	GS2970_VID_FMT_NO_IDENTIFY
};

/* ��Ƶ��Ƕ��״̬ */
#define GS2970_AUD_EMBED 0x01
#define GS2970_AUD_NO_EMBED 0x00

typedef struct
{	/* audio embed select, 注意A和B组嵌入组不能一样 */
	S32 m_GrpA;
	S32 m_GrpB;
}HWL_Gs2970AudEmbedSel;

typedef struct 
{ /* audio output format */
	S32 m_AoutFmt12;
	S32 m_AoutFmt34;
	S32 m_AoutFmt56;
	S32 m_AoutFmt78;
}HWL_Gs2970AudOutFmt;

typedef struct 
{ /* audio output MSB/LSB */
	S32 m_AoutEndian12;
	S32 m_AoutEndian34;
	S32 m_AoutEndian56;
	S32 m_AoutEndian78;
}HWL_Gs2970AudOutEndian;

typedef struct 
{ 
	S32 m_AoutWordLen12;
	S32 m_AoutWordLen34;
	S32 m_AoutWordLen56;
	S32 m_AoutWordLen78;
}HWL_Gs2970AudOutWordLen;

typedef struct 
{ 
	S32 m_AoutMute1;
	S32 m_AoutMute2;
	S32 m_AoutMute3;
	S32 m_AoutMute4;
	S32 m_AoutMute5;
	S32 m_AoutMute6;
	S32 m_AoutMute7;
	S32 m_AoutMute8;
}HWL_Gs2970AudOutMute;

typedef struct 
{ 
	S32 m_AoutSrc1;
	S32 m_AoutSrc2;
	S32 m_AoutSrc3;
	S32 m_AoutSrc4;
	S32 m_AoutSrc5;
	S32 m_AoutSrc6;
	S32 m_AoutSrc7;
	S32 m_AoutSrc8;
}HWL_Gs2970AudOutSrc;

typedef struct
{
	HWL_Gs2970AudEmbedSel m_AudEmbedGroup;
	HWL_Gs2970AudOutFmt m_AudOutFormat; /* 音频直通(AESEBU)/I2S输出 */
	HWL_Gs2970AudOutEndian m_AudOutEndian;
	HWL_Gs2970AudOutWordLen m_AudOutWordLen;
	HWL_Gs2970AudOutMute m_AudOutMute;
	HWL_Gs2970AudOutSrc m_AudOutSrc;

	S32 m_VidOutFormatSel;
}HWL_Gs2970ConfigPara;

typedef struct  /* 这个结构体转换与web对应的参数 */
{
	BOOL m_IsAc3;
	S32 m_AudEmbedGroup;
	S32 m_VidFormat;
}HWL_SdiConfigParam;

typedef struct  /* 这个结构体转换与web对应的参数 */
{
	S32 m_LockStatus;
	S32 m_VidFormat; /* 这里映射的值与web的设置相同 */
	BOOL m_IsAudEmbed; /* 音频是否正常嵌入 */
}HWL_SdiStatusParam;

#if defined(GN1866)

/* 音频组嵌入状态 */
#define GS2970_AUD_EMBED 0x01
#define GS2970_AUD_NO_EMBED 0x00
#endif

typedef struct 
{
	SPI_GpioInitParam  m_InitSpiParam;

	SPI_GpioPin 			m_ResetPin;
	SPI_GpioPin 			m_20_10BitPin;
}HWL_SdiInitParam;

typedef S32 (*HWL_Gs2970ReadGpioCB)(SPI_GpioPin GpioPin, S32 *pPinStatus, void *pUserPara);
typedef BOOL (*HWL_Gs2970WriteGpioCB)(SPI_GpioPin GpioPin, S32 PinStatus, void *pUserPara);
typedef BOOL (*HWL_Gs2970SetupGpioCB)(SPI_GpioPin GpioPin, S32 PinStatus, void *pUserPara);

typedef BOOL (*HWL_Gs2970ReadRegCB)(HANDLE32 Handle, U16 RegAddr, U16 *pRegData, U32 BufSize, void *pUserPara);
typedef BOOL (*HWL_Gs2970WriteRegCB)(HANDLE32 Handle, U16 RegAddr, U16 RegData, U32 BufSize, void *pUserPara);

typedef struct
{
	HWL_SdiInitParam m_InitParam;
	
	U32 m_Gs2970ResetPulseWidth; /* gs2970复位脉冲宽度 ms */

	HWL_Gs2970ReadGpioCB m_ReadGpioCB;
	HWL_Gs2970WriteGpioCB m_WriteGpioCB;
	HWL_Gs2970SetupGpioCB	 m_SetupGpioCB;

	HWL_Gs2970ReadRegCB 	m_ReadRegCB;
	HWL_Gs2970WriteRegCB 	m_WriteRegCB;
}HWL_Gs2970EnvPara;

typedef struct 
{ 
	S32 m_AudGrp1Status;
	S32 m_AudGrp2Status;
	S32 m_AudGrp3Status;
	S32 m_AudGrp4Status;
}HWL_Gs2970AudGroupStatus;


typedef struct  
{
	S32 m_LockStatus;
	S32 m_VidFormat; 
	HWL_Gs2970AudGroupStatus m_AudEmbedStatus;
}HWL_Gs2970StatusPara;

typedef struct 
{
	HWL_Gs2970EnvPara m_EnvPara;
	HANDLE32 m_GS2970SpiHandle;
}GS2970_Handle;

void Init_SDI_Board(void);
BOOL Probe_SDI_Board(void);
void Terminate_SDI_Board(void);
void Destory_SDI_Board(void);
void Manual_Reboot_SDI(void);
void Manual_Print_SDI_Reg(void);

//add by leonli
HANDLE32 SDI_Create(HWL_Gs2970EnvPara *pEnvPara);

#endif
//////////////////////End of SDI //////////////////////////////////////////////////////////////

#define HWL_ASI_MAX_NUM   8

#define HWL_NEW_HARD		0   //4350


typedef enum
{
	HWL_DEVICE_SIMU_3650DS,
	HWL_DEVICE_SIMU_3650DR,
	HWL_DEVICE_SIMU_3710A,
	HWL_DEVICE_SIMU_3760A,
	HWL_DEVICE_SIMU_2620B,
	HWL_DEVICE_SIMU_2700X,
	HWL_DEVICE_SIMU_2730X,
#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
	HWL_DEVICE_SIMU_1846,
#endif
	HWL_DEVICE_SIMU_NUM,
}HWL_DEVICE_SIMU_TYPE;


typedef enum
{
	HWL_CHANNEL_DIRECTION_IN,
	HWL_CHANNEL_DIRECTION_OUT,
	HWL_CHANNEL_DIRECTION_LIMIT
}HWL_ChannelDirection;

typedef enum
{
	HWL_CHANNEL_TYPE_INVALID=-1,
	HWL_CHANNEL_TYPE_ASI=0,
	HWL_CHANNEL_TYPE_IP=1,
	HWL_CHANNEL_TYPE_IP_LOOP=5,
	HWL_CHANNEL_TYPE_IP_DEP=8,
	HWL_CHANNEL_TYPE_TUNER_S=2,
	HWL_CHANNEL_TYPE_E3DS3=3,
	HWL_CHANNEL_TYPE_DVB_C_MODULATOR=4,
	HWL_CHANNEL_TYPE_TUNER_C = 6,
	//add by ding
	HWL_CHANNEL_TYPE_DVB_S_MODULATOR=7,
	//add by xj for 3655
	HWL_CHANNEL_TYPE_IP_LOOP_DEP=10,
	HWL_CHANNEL_TYPE_DTMB_MODULATOR,
	HWL_CHANNEL_TYPE_TUNER_DTMB,
	HWL_CHANNEL_TYPE_IP_OVER_TS_OUT_DVBC,//IPoTS DVBC方案 输出，均针对于射频来说
	HWL_CHANNEL_TYPE_IP_OVER_TS_IN_DVBC,//IPoTS DVBC方案 输入，均针对于射频来说
	HWL_CHANNEL_TYPE_CLK_TYPE,
#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
	HWL_CHANNEL_TYPE_ENCODER,
#endif
	HWL_CHANNEL_TYPE_LIMIT
}HWL_ChannelType;//通道类型，主要用于区分通道适用的参数


typedef enum
{
	HWL_CHANNEL_SUBTYPE_INVALID,
	HWL_CHANNEL_SUBTYPE_IP,
	HWL_CHANNEL_SUBTYPE_ASI,
	HWL_CHANNEL_SUBTYPE_MODULATOR_AD9789,
	HWL_CHANNEL_SUBTYPE_MODULATOR_BL85KMM,
	HWL_CHANNEL_SUBTYPE_E3DS3,
	HWL_CHANNEL_SUBTYPE_TUNER_S_AVL6211,	
	HWL_CHANNEL_SUBTYPE_TUNER_C,
	HWL_CHANNEL_SUBTYPE_TUNER_DTMB_ATBM8869,
	HWL_CHANNEL_TYPE_IP_OVER_TS_OUT_AD9789,
	HWL_CHANNEL_TYPE_IP_OVER_TS_IN_ATBM8869,
	HWL_CHANNEL_SUBTYPE_ENCODER_HI3531A,
    HWL_CHANNEL_SUBTYPE_ENCODER_HI3519
}HWL_ChanneSublType;//通道子类型，树妖用于区分通道的实现方案



#ifdef SUPPORT_NEW_HWL_MODULE
typedef struct
{
	/*类型，方向;0输入，1输出*/
	S16	m_Type;
	S16 m_SubType;

	S32	m_SubChnNum;
	S32	m_StartTsIndex;
	S32 m_CurSubSupport;
}HWL_ChannelInfo;

typedef struct
{
	U32					m_ChipID;
	S32					m_InChnNum;
	HWL_ChannelInfo		m_pInChn[HWL_MAX_CHANNEL_NUM];
	S32					m_OutChnNum;
	HWL_ChannelInfo		m_pOutChn[HWL_MAX_CHANNEL_NUM];

	CHAR_T				m_pFPGARelease[64];

	S32					m_DeviceType;

	S32					m_InTsMax;
	S32					m_OutTsMax;
}HWL_HWInfo;

#else
typedef struct
{
	/*类型，方向;0输入，1输出*/
	S16 m_Direction;
	S16	m_Type;
	S16 m_SubType;
	S16 m_DemodType;

	S32	m_StartTsIndex;
	S32 m_CurSubSupport;
}HWL_ChannelInfo;

typedef struct
{
	U32					m_ChipID;
	S32					m_ChannelNum;
	HWL_ChannelInfo		m_pInfoList[HWL_MAX_CHANNEL_NUM];
	CHAR_T				m_pFPGARelease[64];
	//S32					m_HWInserterNum;

	S32					m_InTsMax;
	S32					m_OutTsMax;

	S32					m_DeviceType;

	S32					m_TunerCount;
	S32					m_TunerType;

	/*
	PC13    PC12     SV（软件）      HV（硬件）     PLL硬件        
	1       1         1.04            2.0          ADF4350 
	0       0         2.00            3.0          MAX2871
	*/

#ifdef GQ3760B		//PC13  PC12
	U32					m_HwType;
	S32					m_REFCLKHz;//10M/40M
#endif
}HWL_HWInfo;
#endif

typedef struct
{
	/*IP 参数*/
	U32 m_IPAddress;
	U16	m_IPPort;

}HWL_TsParameter;

typedef struct
{
	/*IP 参数*/
	U32 m_IPAddress;
	U32 m_IPMask;
	U32 m_IPGate;

}HWL_ChannelParam;


/* Functions prototypes ------------------------------------------------------- */

/*FFPG配置------------------------------------------------------------------------------------------------------------------------*/
void HWL_FpgaConfig(void);
BOOL HWL_FPGAGetMuxOK(void);

/*风扇操作*/
void HWL_FanInit(void);//在HWL_Initiate被调用
void HWL_FanEnable(BOOL bEnable);
BOOL HWL_FanStatusGet(void);
void HWL_FanDestroy();//在HWL_Terminate被调用


/*模块基本功能------------------------------------------------------------------------------------------------------------------------*/
BOOL HWL_Initiate(S32 DeviceType);
void HWL_Access(S32 Duration);
void HWL_Pool(void);
void HWL_Terminate(void);

/*硬件信息获取(包括加密ID号)*/
BOOL HWL_GetHWInfo(HWL_HWInfo *pHWInfo);

/*输入流分析功能*/
typedef void (*HWL_TsFilterCB)(S32 SlotIndex, U8 *pTsPacket);
void HWL_AddTsPacketsRequest(S32 SlotIndex, S32 InTsIndex, U16 InPID, HWL_TsFilterCB pDataCB, S32 PESCount);
void HWL_RemoveTsPacketsRequest(S32 SlotIndex);




/*复用参数------------------------------------------------------------------------------------------------------------------------*/
/*PID 映射操作*/
#define HWL_INTERNAL_TS_INDEX 0xFE
void HWL_PerformPIDMap(void);//设置到FPGA
void HWL_AddPIDMap(S16 InTsIndex, U16 InPID, S16 OutTsIndex, U16 OutPID, BOOL bScramble, S32 CwGroupIndex);
void HWL_ClearPIDMap(void);

/*设置输出通道工作方式: 直通或是映射*/
#define HWL_CONST_PASS_MODE_THROUGH		1		//直通
#define HWL_CONST_PASS_MODE_PIDMAP		0
#define MULT_CHANNEL_SBU_TYPE_TUNER_MAX_NUM      (4)  
typedef struct
{
	U8 	passMode;
	U8	inputChannelId;
	U8	outputChanneldId;
}HWL_PassMode_t;
S32 HWL_PassModeSend(HWL_PassMode_t *passMode);

/*加扰参数------------------------------------------------------------------------------------------------------------------------*/
/*控制字操作*/
#define HWL_CONST_EVEN		0		//偶
#define HWL_CONST_ODD		1		//奇
typedef struct
{
	U8	cwGroup;		/* 控制字组 （0 to 127）*/
	U8	channelId;		/* 不使用*/
	U8	evenOrOdd;		/* CW 的奇偶 ：  1奇，0偶*/
	U8	words[8];

}HWL_ControlWord_t;
S32 HWL_ControlWordSet(HWL_ControlWord_t *cw);/**	CW传送.*/
S32 HWL_ControlWordSwitchSet(U8 evenOrOdd);/**	CW切换*/

/*设置加扰模块开关*/
void HWL_ScrambleEnable(BOOL bEnable);

/*状态参数------------------------------------------------------------------------------------------------------------------------*/
/*获取通道的码率（通道全部（TsIndex = -1），或者单个TS流）*/
#if defined(GN1846) || defined(GN1866)  || defined(GN1866B)
U32  HWL_GetBitrate(HANDLE32 Handle, S16 CHNIndex, S16 TsIndex);
#else
U32  HWL_GetBitrate(S16 CHNIndex, S16 TsIndex);
#endif

/*调制模块PLL值*/
U32 HWL_PLLValue();

/*调制模块温度.*/
S32 HWL_Temperature();


/**	输出通道缓冲区状态.
*	@(channelId):从0开始，[0-7]
*/
BOOL HWL_GetQAMBuffStatus(U8 channelId);
BOOL HWL_GetTunerShortStatus(U8 channelId);


/*TS包发送参数------------------------------------------------------------------------------------------------------------------------*/
/*从CPU发送TS包到FPGA*/
void HWL_SetDirectOutTsPacket(S16 OutTsIndex, U8 *pTsData, S32 DataSize);


/*硬件TS包发送器功能集合*/

/**	清除Save_H_L 位置的PSI信息
*	@(offset): 如果 = 0xffff  , 为清除全部PSI 信息.
*/
S32 HWL_HWInserterClear(U16 offset);

/**	永久插入TS数据 .*/
S32 HWL_HWInserterSet(S16 SlotIndex, S16 TsIndex, U8 *pData, S32 DataSize, S32 Interval);


/*QAM操作------------------------------------------------------------------------------------------------------------------------*/
enum 
{
	HWL_CONST_MODULATOR_NONE = 0,
	HWL_CONST_MODULATOR_AD9789_QAM,
	HWL_CONST_MODULATOR_AD9789_QPSK_H,//上段
	HWL_CONST_MODULATOR_AD9789_QPSK_L,//下段
	HWL_CONST_MODULATOR_AD9789_DTMB,//国标地面
	HWL_CONST_MODULATOR_NUM
};


enum 
{
	HWL_CONST_MODULATOR_QAM_16 = 0,
	HWL_CONST_MODULATOR_QAM_32,
	HWL_CONST_MODULATOR_QAM_64,
	HWL_CONST_MODULATOR_QAM_128,
	HWL_CONST_MODULATOR_QAM_256,
	HWL_CONST_MODULATOR_QAM_512,
	HWL_CONST_MODULATOR_QAM_1024,
	HWL_CONST_MODULATOR_QAM_QPSK,
	HWL_CONST_MODULATOR_QAM_4,
	HWL_CONST_MODULATOR_QAM_4NR,
	HWL_CONST_MODULATOR_QAM_NUM
};

enum 
{
	HWL_CONST_MODULATOR_STANDARD_ANNEX_A = 0,
	HWL_CONST_MODULATOR_STANDARD_ANNEX_B,
	HWL_CONST_MODULATOR_STANDARD_ANNEX_C,
	HWL_CONST_MODULATOR_STANDARD_ANNEX_NUM
};

enum 
{
	HWL_CONST_MODULATOR_ANALOG_BAND_8M = 0,
	HWL_CONST_MODULATOR_ANALOG_BAND_6M,
	HWL_CONST_MODULATOR_ANALOG_BAND_7M,
	HWL_CONST_MODULATOR_ANALOG_BAND_NUM
};

typedef enum 
{
	HWL_CONST_MODULATOR_FEC_1_OF_2 = 0,
	HWL_CONST_MODULATOR_FEC_2_OF_3,
	HWL_CONST_MODULATOR_FEC_3_OF_4,
	HWL_CONST_MODULATOR_FEC_5_OF_6,
	HWL_CONST_MODULATOR_FEC_7_OF_8,
	HWL_CONST_MODULATOR_FEC_NUM
}HWL_CONST_MODULATOR_FEC;


enum
{
	HWL_MODULATOR_DTMB_CARRIER_SINGLE = 0,
	HWL_MODULATOR_DTMB_CARRIER_MULTI,
	HWL_MODULATOR_DTMB_CARRIER_NUM
};

enum
{
	HWL_MODULATOR_DTMB_PN420 = 0,
	HWL_MODULATOR_DTMB_PN595,
	HWL_MODULATOR_DTMB_PN945,
	HWL_MODULATOR_DTMB_PN420F,
	HWL_MODULATOR_DTMB_PN595F,
	HWL_MODULATOR_DTMB_PN945F,
	HWL_MODULATOR_DTMB_PN_NUM
};

enum
{
	HWL_MODULATOR_DTMB_TI_240 = 0,
	HWL_MODULATOR_DTMB_TI_720,
	HWL_MODULATOR_DTMB_TI_NUM
};

enum
{
	HWL_MODULATOR_DTMB_CodeRate_04 = 0,
	HWL_MODULATOR_DTMB_CodeRate_06,
	HWL_MODULATOR_DTMB_CodeRate_08,
	HWL_MODULATOR_DTMB_CodeRate_NUM
};

///////////////////////////////////////////////////////
typedef struct
{
	U8						E3DS3Select;					/* E3/DS3选择 */
	U8						BitOrder;						/*MSB/LSB*/
	U8						FrameForm;						/*   */
	S16						InOutPacketLength;				/* 188/204 */
	BOOL					InterweaveCoding;
	BOOL					ScrambleChanger;				/*   */
	BOOL					RSCoding;						/*   */				/*    */
	BOOL					CodeRateRecover;				/*     */

}HWL_E3DS3Param_t;

typedef enum
{
	GS_TUNER_ANNEX_A = 0,
	GS_TUNER_ANNEX_B,
	GS_TUNER_ANNEX_C
}GS_TunerReqTDREQTYPE_ype;

typedef enum
{
	GS_TUNER_PLOAR_NONE = 0,
	GS_TUNER_PLOAR_VER,	
	GS_TUNER_PLOAR_HOR
}GS_TunerPolarMethod;

typedef enum
{
	GS_SPECINV_OFF = 0,
	GS_SPECINV_ON,
	GS_SPECINV_AUTO
}GS_TunerSpecinv;




typedef struct
{
	U8						ModulateStandard;			/*调制标准： 见枚举变量 J.83 Annex A/B/C；0:A*/
	S32						ModulateFreq;				/*调制频率： Khz，兼容旧程序，新程序为HZ级别*/
	U8						QamMode;					/*调制模式： 见枚举变量*/
	S32						SymRate;					/*符号率： Kboud/s兼容旧程序，新程序为HZ级别*/
	U8						AnalogBand;					/*调制模拟带宽： 见枚举变量*/
	U8						Gain;						/*放大器增益微调：0~20*/
	S16						Attenuator;					/*衰减器增益微调：0~40*/
	BOOL					SpectrumInvert;				/*频谱翻转开关：0 不翻转，1翻转*/
	BOOL					QamSwitch;					/*调制开关：0 关闭，1 开启*/
	BOOL					RFSwitch;					/*RF输出开关：0 关闭，1 开启*/

	S32						QPSK_FEC;					/*见 HWL_CONST_MODULATOR_FEC */
	U8						m_RollOffFactor;			/*滚降系数选择*/


	S32						DTMB_Carrier;				//0，单载波；1多载波
	S32						DTMB_PN;				//0，PN420；1，PN575；2，PN945
	S32						DTMB_TI;				//0，240符号；1， 720符号
	S32						DTMB_CodeRate;				//0，0.4；1， 0.6；2，0.8；
	S32						DTMB_DoublePilot;


	S32						FreqAdj;//频率微调
}HWL_ModulatorParam_t;

typedef struct
{
	U32 Frequency ;   //unit 1k Hz
	U32 Symbol ;   //unit 0.001MBand
	U32 LocalFreq;
	U8  Modulation;	    //the modulation mode of the signal
	U8  TuneReqType;    //TRT_DVBC TRT_DCABLE TRT_J83C
	U8  PolarMethod;
	U8  Switch_22K;	
	U8  SpectInv;;	

}HWL_TunerParam_t;





/*
typedef struct  
{
U8   ChipID;   
U8   RorW;
U8	 RegisterControl;
U8   FunctionControl;
}HWL_SubTUNER;

typedef struct  
{
HWL_SubTUNER	m_pTUNER[MULT_CHANNEL_SBU_TYPE_TUNER_MAX_NUM];

}HWL_TUNER;


*/

/**
*调制模块初始化，本函数将会把工作参数设置为默认值.
*/
#ifdef GQ3760B
void HWLL_QAMHWTypeInitialize(HWL_HWInfo *pHWInfo);
#endif

void HWL_QAMInitialize(U8 ModulatorType);

void HWL_QAMTerminate();

void HWL_QAMDirectRegSet(U8 ICID, U32 Addr, U32 Value);

/**
*设置PLL值.
*/
void HWL_QamSetPLLParameter(void);


S32 HWL_QAMParameterApply(S16 TsIndex);

void HWL_QAMApplyParameterWithPQ(U32 PValue, U32 QValue);
/////////////////////////////////////

typedef enum
{
	GS_E3DS3_SELECT_E3 = 0,
	GS_E3DS3_SELECT_DS3 ,
	GS_E3DS3_SELECT_AUTO
}GS_ASIE3DS3;

typedef enum
{
	GS_E3DS3_BITORDER_MSB = 0,
	GS_E3DS3_BITORDER_LSB 
}GS_ASIBitorder;

typedef enum
{
	GS_E3DS3_FRAMEFORM_NO = 0,
	GS_E3DS3_FRAMEFORM_YES
}GS_ASIFrameform;

typedef enum
{
	GS_E3DS3_PACKETLENGTH_188 = 0,
	GS_E3DS3_PACKETLENGTH_204

}GS_ASIPacketlength;

void HWL_TunerSetParameter(S16 TsIndex,HWL_TunerParam_t *lParam);
void HWL_TunerApplyParameter(S16 Tunercount, S16 ChannelType);

///////////////////////////////////////////

void HWL_QAMFDACOffsetSet(S32 FrequenceCalibrate);

/**
*获取和设置QAM参数.
*/
BOOL HWL_QamGetParameter(S16 TsIndex, HWL_ModulatorParam_t *param);

/**
*此函数内部会AdjustParam以及ApplyParameter。
*/
BOOL HWL_QAMParameterSet(S16 ChnInd,HWL_ModulatorParam_t *param);

#ifdef USE_NEW_QAM_SETTING_FUNCTIONS

typedef void (*HWL_ModModuleResetCB)(void);

BOOL HWL_QAMParameterSetNew(S16 ChnInd, HWL_ModulatorParam_t *pParam, HWL_ModModuleResetCB pResetCB);
void HWL_QAMGetCurrentParam(S16 ChnInd, HWL_ModulatorParam_t *pParam);
void HWL_QAMForceOutputProgressChange(S32 lNewValue);
void HWL_QAMForceNotLevelOnly(void);
#endif

/*系统操作------------------------------------------------------------------------------------------------------------------------*/
/*IGMP 工作方式设置*/
#define HWL_CONST_IGMP_MODE_AUTO		2
#define HWL_CONST_IGMP_MODE_V3			3

/*硬件存储空间操作   
*	@(Index)	[0,7].
*	@(buff):	用户数据缓冲区.（数据全FF时为擦除动作）
*	@(buffLen):	必须为10.
*	共80个字节的存储空间，仅提供以10字节为单位的读取。
*/
S32 HWL_EncryptChipWrite(U8 *buff , U8 bufflen, U8  Index);
S32 HWL_EncryptChipRead(U8 *buff , U8 bufflen, U8 Index , S32 Timeout);
void HWL_SetI2cAndTunerReset(U8 ChannelNo, U8 Reset,U8 LNBPower);

/************************************************************************************/
/* PID统计的端口设置 */
/************************************************************************************/
#define HWL_CONST_PID_NUM_MAX			250


/**	PID 统计的端口设置.*/
S32 HWL_PidStatisticSend(U8 ChnIndex, U8 SubIndex);

/**	 PID 统计的端口码率查询请求.*/
S32 HWL_PidStatisticSearch(U8 ChnIndex, U8 SubIndex);

typedef struct tagHWL_PidRate
{
	U16	pidVal;
	S32 pidRate;

} HWL_PidRate_t;

/**	回复结构体*/
typedef struct tagHWL_PidRateArray
{
	//HWL_PidStatics_t search;	//标识查询参数.
	U8 physicChannelId;			//
	U32 totalRate;				//通道总码率
	HWL_PidRate_t pidRateArray[HWL_CONST_PID_NUM_MAX];
	U32 channelRateArraySize;

} HWL_PIDStatisticsArray;

S32 HWL_PidStatisticArray(HWL_PIDStatisticsArray *pArray);

/************************************************************************************/
/* 输入IP端口码率统计查询|清除 */
/************************************************************************************/
/*IP统计宏*/
#define HWL_CONST_IP_STATISTICS_FRAME_ITEM_MAX	124
#define HWL_CONST_IP_STATISTICS_FRAME_NUM_MAX	2
#define HWL_CONST_IP_STATISTICS_TOTAL_ITEM_MAX	(HWL_CONST_IP_STATISTICS_FRAME_ITEM_MAX * HWL_CONST_IP_STATISTICS_FRAME_NUM_MAX)

typedef struct
{
	U32 addressv4;
	U16 portNumber;
	U32 portRate;
	U8	protocol;
}HWL_EthDetectionInfo;

typedef struct
{
	U32	totalRate;
	HWL_EthDetectionInfo ipPortInfoArray[HWL_CONST_IP_STATISTICS_TOTAL_ITEM_MAX];
	U32	ArrySize;
}HWL_IPStatisticsArray;

/**	输入IP端口码率统计信息清除..*/
S32 HWL_IPPortStatisticClear(S32 ChnIndex);

/**	输入IP端口码率统计信息查询..*/
S32 HWL_IPPortStatisticSend(S32 ChnIndex);

/*当前输入UDP/RTP包地址信息列表获取*/
S32 HWL_IPPortStatisticArrayGet(S32 ChnIndex, HWL_IPStatisticsArray *pArray);

void HWL_ASIApply(S32 DeviceIndex, S32 ChnIndex, BOOL bInput);
S32 HWL_ASIAdd(S32 DeviceIndex, S32 ChnIndex, S32 SubIndex, U32 BitRate, BOOL8 Active, BOOL bInput);
void HWL_ASIClear(S32 DeviceIndex, BOOL bInput);


U8 HWL_GetHardwareVersion(void);

S32 HWL_TunerCountCheck(void);
S32 HWL_ATBM8869CountCheck(void);




#ifdef GN2000

#define HWL_MAX_AUDIO_OFFSET_PID_NUM		12
typedef struct  
{
	S16		m_TsInd;
	U16		m_PID;
	S32		m_Offset;//MS，对内
	U16		m_PCRPID;
}HWL_AudioOffsetNode;


void HWL_EncoderInit(void);
void HWL_EncoderApply(void);
void HWL_EncoderTerm(void);
void HWL_EncoderSetAudioPIDDelay(HWL_AudioOffsetNode *pArray, S32 PIDNum);
#endif

/*FPGA ETH*/
typedef void (*HWL_FPGAEthTSCB)(S32 Index, U8* Buff);
void HWL_FPGAEthSetCB(HWL_FPGAEthTSCB pCB);
void HWL_FPGAEthRedvFromFPGA(U8 *pData, S32 DataLen);
void HWL_FPGAETHSendToFPGA(S32 Index, U8 *pTsData);



void HWL_TDDemodTest(void);
S32 HWL_TDDemodCheck(S32 *pType);
void HWL_TDInitiateTDModule(S32 HWLSubChnType);
void HWL_TDGetSignalFloat(S32 Slot, BOOL *pbLock, F64 *pStrenth, F64 *pQuanlity, F64 *pBER);
S32 HWL_TDGetDVBCConStellation(S32 Slot);
BOOL HWL_TDGetDVBSChannelParam(S32 Slot, TD_DVBSXChnlInfo *pDVBSXParam);
void HWL_TDApplyParam(S32 Slot, S32 FrequencyHz, S32 SymboRate, S32 Bandwidth, BOOL SpecInv, BOOL bEnable22K);



S32 HWL_QAMDTMBApplyFPGAParameters(BOOL bTone);
BOOL HWL_QAMParameterCheckNeedResetRF(void);


void FIRII_Reload(CHAR_T *pMatlabCoeffFile);


/*编码器*/
U32 HWL_EncoderGetFPGAID(void);
CHAR_T *HWL_EncoderGetFPGAVersion(void);

void HWL_EncoderAuthFixStorageCB(void* pUserParam, U32 StartAddress, U8 *pData, S32 DataSize, BOOL bRead);



/*ETH*/

#ifdef MULT_SYSTEM_HAVE_PCR_CORRECT_ADJUST_FUNCTION
/*PCR修正模式*/
void HWL_SetChnPCRCorrectMode(S32 ChnIndex, U8 PCRUper, U8 PCRLower, BOOL bOpen);
#endif





/*FPGA REG*/
void HWL_FPGARegInitate(void);
BOOL HWL_FPGARegProcess(S32 SlotInd, U16 ChipID, U32 Address, U32 *pData, S32 Size, BOOL bRead, S32 TimeoutMS);
BOOL HWL_FPGARegICPRecv(U8 *pData, S32 DataSize);
void HWL_FPGARegTerminate(void);

void HWL_FPGARegPHYInitate(void);
BOOL HWL_FPGARegPHYDetectOK(S32 Slot);
void HWL_FPGARegPHYDetectAddr(S32 Slot);
S32 HWL_FPGARegPHYGetLinkStatus(S32 Slot);

#if defined(GN1866) || defined(GN1866B) 
typedef enum
{
    MainBoard_V2 = 0,
    MainBoard_V1,
    MainBoard_Num
}HWL_MainBoardVersion;

HWL_MainBoardVersion HWL_GetHwVersion(void);
#endif

#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
#include "hdmi_rx.h"

extern S32 g_LedFlashInterval;
void HWL_RunStatusDebug(void);
void HWL_HdmiRxDebug(void);
void HWL_FpgaSpiDebug(void);

BOOL HWL_HdmiRxDownloadEdid(S32 ChnIndex, S32 EdidType);
BOOL HWL_HdmiRxSetParam(S32 ChnIndex, HDMI_RxCfgParam *pCfgParam);
BOOL HWL_HdmiRxGetStatus(S32 ChnIndex, HDMI_RxStatusParam *pStatus);

BOOL HWL_SDIRxGetStatus(S32 ChnIndex, HWL_SdiStatusParam *pSdiStatusPara, S32 AudEmbedGroup);
/*****************for adv7612*******************************************************/
void Init_Hdmi_Board(void);
void Terminate_Hdmi_Board(void);
BOOL Probe_Hdmi_Board(void);

/*****************for LT6911C*******************************************************/
void Init_HdmiLt6911c_Board(void);
void Terminate_HdmiLt6911c_Board(void);
BOOL Probe_HdmiLt6911c_Board(void);
void HWL_Lt6911cRxDebug(void);
BOOL HWL_LT6911cDownloadProgram(void);
 void HWL_HdmiRxInit(void);
#endif

#endif

/*EOF*/



