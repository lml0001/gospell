#ifndef _MUL_MULT_INTERNAL_H_
#define _MUL_MULT_INTERNAL_H_

/* Includes-------------------------------------------------------------------- */
#include "multi_private.h"
#include "multi_main.h"
#include "global_micros.h"
#include "platform_conf.h"
#include "libc_assist.h"
#include "platform_assist.h"
#include "multi_hwl.h"

//encoder main board temperature
#include "hwl_ds8b20.h"

#include "multi_tsp.h"
#include "webmain.h"
#include "mpeg2.h"
#include "mxml.h"
#include "scs_main.h"
#include "upgrade.h"
#include "authorization_mid.h"
#include "multi_tsp.h"
#include "crypto.h"
#include "TunerDemod.h"
#include "multi_hwl_monitor.h"
#include "mxml_warp.h"

#ifdef ENCODER_CARD_PLATFORM
#include "card_app.h"
#endif

#include "fpga_reg.h"

#include "fpga_switch.h"

#include "mpeg2_pes_covert_micro.h"

#define MULT_NEW_KERNEL_FILE_PATHNAME			"/mnt/baku/uImage"
#define MULT_MODULE_FILE_STORAGE_PATHNAME		"/mnt/baku/modules/"
#define MULT_CARD_SYSTEM_MAX_MODULE_NUM			(6)
#define MULT_CARD_SYSTEM_MAX_TYPE_NUM			(16)
/* Macro ---------------------------------------------------------------------- */
#define MULT_DEVICE_COMPLETE_TYPE			((MULT_DEVICE_TYPE << 8) & 0xFF00) | (MULT_DEVICE_SUB_TYPE & 0xFF)

#define MULT_TASK_STATCK_SIZE				(1024*1024)
#define MULT_MAIN_MSG_QUEUE_MAX_TIMEOUT		5000
#define MULT_RELEASE_DATE_STRING_BUF_LEN	20	
#define MULT_RELEASE_DATE_STRING_LEN		19	//"2012-03-30 12:20:59"
#define MULT_SN_STRING_BUF_LEN				20
#define MULT_SN_STRING_LEN					18	//"TC73F84FFFXXXXXXXX"
// #define MULT_AUTH_STRING_BUF_LEN			48
// #define MULT_AUTH_STRING_LEN				32	//"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
#define MULT_INFO_STRING_BUF				64		
#define MULT_APPLICATION_CODE_BUF_SIZE		(4 * 1024)		

#define MULT_CHANNEL_TYPE_ASI_NAME				("ASI")
#define MULT_CHANNEL_TYPE_ETH_NAME				("ETH")
#define MULT_CHANNEL_TYPE_ETH_LOOP_NAME			("ETH_LOOP")
#define MULT_CHANNEL_TYPE_ETH_DEP_NAME			("ETH_DEP")
#define MULT_CHANNEL_TYPE_ETH_LOOP_DEP_NAME		("ETH_LOOP_DEP")
#define MULT_CHANNEL_TYPE_E3DS3_NAME			("E3DS3")  
#define MULT_CHANNEL_TYPE_TUNER_S_NAME			("TUNER_S")
#define MULT_CHANNEL_TYPE_TUNER_C_NAME			("TUNER_C")
#define MULT_CHANNEL_TYPE_TUNER_DTMB_NAME		("TUNER_DTMB")

#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
#define MULT_CHANNEL_TYPE_ENCODER_NAME						("ENCODER")	
#endif


#define MULT_CHANNEL_TYPE_QPSK_NAME				("QPSK")
#define MULT_CHANNEL_TYPE_DTMB_NAME				("DTMB")

#define MULT_CHANNEL_TYPE_MODULATOR_NAME		("MODULATOR")
#define MULT_CHANNEL_TYPE_MODULATOR_NAME2		("QAM")
#define MULT_CHANNEL_TYPE_DEMODULATOR_NAME		("DEMODULATOR")

#define MULT_CHANNEL_SUB_TYPE_TS_OVER_ASI_NAME	("ASI")
#define MULT_CHANNEL_SBU_TYPE_E3DS3_NAME		("E3DS3")  
#define MULT_CHANNEL_SBU_TYPE_TUNER_S_NAME		("TUNER_S")  
#define MULT_CHANNEL_SBU_TYPE_TUNER_C_NAME		("TUNER_C")  
#define MULT_CHANNEL_SBU_TYPE_TUNER_DTMB_NAME	("TUNER_DTMB")  
#define MULT_CHANNEL_SUB_TYPE_ENCODER_CVBS		("CVBS")  

#define MULT_CHANNEL_SUB_TYPE_ENCODER_HI3531A		("ENC_HI3531A")
#define MULT_CHANNEL_SUB_TYPE_ENCODER_HI3519		("ENC_HI3519")

#define MULT_CHANNEL_SUB_TYPE_TS_OVER_UDP_NAME	("TS_OVER_IP")
#define MULT_MODULATOR_SUB_TYPE_AD9789_NAME		("QAM_A")
#define MULT_MODULATOR_SUB_TYPE_BL85KMM_NAME	("QAM_B")

#define MULT_CHANNEL_SBU_TYPE_E3DS3_MAX_NUM      (4)  

#define MULT_AUTH_TOTAL_TRAIL_TIME				(3600 * 24 * 30)
#define MULT_AUTH_COUNT_DOWN_CYCLE				(300)

#define MULT_WEB_BASE_DIR						("/tmp/web/")
#define MULT_DEFAULT_HOME_PAGE					("index.asp")
#define MULTI_DEFAULT_IP_ADDR					0x7878780A
#define MULTI_DEFAULT_IP_MASK					0xFFFFFF00
#define MULTI_DEFAULT_IP_GATE					0x78787801
#define MULTI_DEFAULT_NTP_INTERVAL				(8*3600)
#define MULTI_MIN_NTP_INTERVAL					(10)
#define MULTI_DEFAULT_SUB_IP_ADDR				0xE00A0A01
#define MULTI_DEFAULT_SUB_IP_PORT				5000

#define CGIC_UPLOAD_FILE_MAX_SIZE				(1024*1024)

#define MULT_PARAMETER_DESCRIPTOR			("PARAM%.8X")
#define MULT_MAINTANCE_DESCRIPTOR			("MAIT%.8X")

#define MULT_XML_BASE_DIR					("/tmp/web/tmp/")//xml�Լ�������ʱ�ļ���ź�չ��λ��
// #define MULT_TS_BASE_DIR					("/tmp/web/ts/")//TS�ļ���ź�չ��λ��


#define MULT_DEVICE_INFORMATION_XML			("device_information.xml")
#define MULT_DEVICE_MAINTENANCE_XML			("maintenance.xml")
#define MULT_DEVICE_PARAMETER_XML			("device_parameter.xml")
#define MULT_SYSTEM_PARAMETER_XML			("system_parameter.xml")
#define MULT_SERVICE_PARAMETER_XML			("service_parameter.xml")
#define MULT_MONITOR_SETTING_XML			("monitor_setting.xml")
#define MULT_WEB_USER_MANAGE_FILE_PATHNAME	("umconfig.txt")
#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
#define MULT_EDID_SETTINGS_XML				("edid_settings.xml")
#endif

#define CGIC_UPLOAD_FILE_PATHNAME			("/tmp/web/upload/upload.bin")
#define MULT_IO_TMP_FILE_PATHNAME			"/tmp/temp.tar.gz"


#define MULT_LOGO_FILE_WEB_PATH				("/tmp/web/graphics/logo.jpg")
#define MULT_INTRO_FILE_WEB_PATH			("/tmp/web/graphics/intro.jpg")
#define MULT_MANUAL_FILE_WEB_PATH			("/tmp/web/graphics/manual.pdf")
#define MULT_OVERRIDE_FILE_WEB_PATH			("/tmp/override.tar.gz")//20130709����OEM�ļ����ǹ���



#define MULT_STORAGE_BASE_DIR				("/mnt/mtd/")//xml�Լ�������ʱ�ļ���ź�չ��λ��
// #define MULT_STORAGE_BASE_DIR				("/tmp/")//xml�Լ�������ʱ�ļ���ź�չ��λ��
#define MULT_OEM_FILE_PATHNAME				("oem.bin")
#define MULT_LICENSE_FILE_PATHNAME			("license.bin")
#define MULT_PARAMETER_FILE_PATHNAME		("parameter.bin")
#define MULT_PARAMETER_BAKU_PATHNAME		("parameter.bk")
#define MULT_PARAMETER_DEF_PATHNAME			("parameter.def")
#define MULT_PROGRAM_FILE_PATHNAME			("software.tar.gz")
#define MULT_PROGRAM_BAKU_PATHNAME			("software.tar.bk")
#define MULT_MAINTENACE_FILE_PATHNAME		("maintenance.bin")
#define MULT_PROGRAM_INFO_FILE_PATHNAME		("/tmp/web/upload/program.xml")




// #define MULTI_DEFAULT_IGMP_VERSION			2
// #define MULTI_DEFAULT_IGMP_INTERVAL			20

#define MULT_FRP_LANGUAGE_ENG					0
#define MULT_FRP_LANGUAGE_CHN					1

#define MULT_CONFIG_VERSION_NUMBER				0
#define MULT_PARAMETER_VERSION_NUMBER			0

#define MULT_CRITICAL_TEMP						65
#define MULT_FAN_TEMP							35
#define MULTL_TEMP_ERROR_TORLARANCE				(3)
#define MULT_FAN_OFFSET_TEMP					10

#define MULT_SNTP_FAILED_RETRY_TIMEOUT			10//��

#define SNMP_WARP_MAX_DESCR						260

#define SNMP_DEFAULT_RO_COMMUNITY			"public"
#define SNMP_DEFAULT_RW_COMMUNITY			"private"
#define SNMP_DEFAULT_DEVICE_NAME			"------"
#define SNMP_DEFAULT_DEVICE_LOCATION		"------"
/* Types Define (struct typedef)----------------------------------------------- */

typedef enum
{
	MULT_TASK_MARK_STOP = 0,
	MULT_TASK_MARK_RUN,
	MULT_TASK_MARK_WAIT_STOP
}MULT_TaskMark;

/*ϵͳ��Ϣ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

typedef struct
{
	U8		m_pSoftVersion[MULT_INFO_STRING_BUF];
	U8		m_pHardVersion[MULT_INFO_STRING_BUF];
	U8		m_pSoftRelease[MULT_INFO_STRING_BUF];
	U8		m_pFPGARelease[MULT_INFO_STRING_BUF];
	U8		m_pMuxFPGARelease[MULT_INFO_STRING_BUF];
	U8		m_pSNString[MULT_SN_STRING_BUF_LEN];//���к��ַ���

	BOOL	m_LicenseValid;
#if defined(GN1846) || defined(GN1866)  || defined(GN1866B)
	S32		m_InputBoardType; /* �����������*/
	U8		m_LicenseAAC;/*AAC���빦���Ƿ���Ȩ*/
	U8		m_LicenseAC3;/*AC3���빦���Ƿ���Ȩ,�˰汾δʹ�ã�����*/
#endif
#if defined(GN1866)  || defined(GN1866B)
	U8		m_LicenseHEVC;
#endif
#if defined(GN1866B)
	U8		m_pLt6911cSoftVersion;
#endif
	U32		m_LicenseMode;
	/*�豸�Ӷ�����Ȩ��Ϣ*/
	S32		m_LicenseInASINum;
	S32		m_LicenseInIPNum;
	S32		m_LicenseDescrambleNum;
	S32		m_LicenseOutIPNum;
	S32		m_LicenseOutTsNum;
	S32		m_LicenseSCSNum;

	CHAR_T	m_ApplicationCode[MULT_APPLICATION_CODE_BUF_SIZE];
	CHAR_T	m_LicenseDate[MULT_INFO_STRING_BUF];
	CHAR_T	m_ExpireDate[MULT_INFO_STRING_BUF];
	S32		m_TrailTime;



	CHAR_T	m_pModelName[MULT_INFO_STRING_BUF];
	CHAR_T	m_pWEBENG[MULT_INFO_STRING_BUF];
	CHAR_T	m_pWEBCHN[MULT_INFO_STRING_BUF];
	CHAR_T	m_pFRPENG[MULT_INFO_STRING_BUF];
	CHAR_T	m_pFRPCHN[MULT_INFO_STRING_BUF];
	CHAR_T	m_pManufacter[MULT_INFO_STRING_BUF];
	CHAR_T	m_pManufacterWEBADDR[MULT_INFO_STRING_BUF];
	BOOL	m_bHaveManual;
}MULT_Information;//ȫ��������Ϊ����ʱ��̬���ɣ�

/*ϵͳ����----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct  
{
	S32		m_PLLFreqOffset;//���໷ʱ��Ƶ�ʽ���ֵ
	U32		m_10MPLLDAValue;//���໷ʱ��Ƶ�ʽ���ֵ
	F64		m_10MTrainningValue;//���໷ʱ��Ƶ�ʽ���ֵ
	U32		m_PowerUpCount;//û����һ�μ�+1
	BOOL	m_PrivateChannelSetupMark;//˽��CAS����

#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
	S32				m_AudPtsRelativeDelayTime[MULT_MAX_CHN_NUM]; /* ��λ��us */
	S32				m_PtsDelayTime[MULT_MAX_CHN_NUM]; /* ��λ��us */
	S32				m_MaxPtsPcrInterval[MULT_MAX_CHN_NUM]; /* ��λ��us */
	S32				m_MinPtsPcrInterval[MULT_MAX_CHN_NUM]; /* ��λ��us */
	S32				m_AudDelayFrameNum[MULT_MAX_CHN_NUM];
#endif

}MULT_Maintenace;

/*----SNMP ����----*/
typedef struct  
{
	CHAR_T					m_pRWCommunity[SNMP_WARP_MAX_DESCR];//��д��������
	CHAR_T					m_pROCommunity[SNMP_WARP_MAX_DESCR];//�Զ���������
	CHAR_T					m_pDeviceName[SNMP_WARP_MAX_DESCR];
	CHAR_T					m_pDeviceLocation[SNMP_WARP_MAX_DESCR];
	S32						m_NormalTrapInterval;//ÿ��TRAP�ڵ㣬��������TRAP�ļ��(���룬ϵͳ��С������Ϊ10ms)
	U32						m_TRAPIPAddress;//TRAPĿ���ַ
	U16						m_TRAPPort;//TRAPĿ��˿ں�
	U16						m_AgentPort;//TRAPĿ��˿ں�
	BOOL					m_TRAPGlobalMark;//TRAPȫ�ֿ���
}SNMP_InitParam;

#ifdef SUPPORT_SYSLOG_MODULE

#define MULT_SYSLOG_MAX_SUBMODULE_SUPPORT		32

typedef struct  
{
	BOOL					m_SyslogGlobalMark;//ϵͳȫ�ֿ���
	U32						m_RemoteAddr;//Զ����־��ַ
	U16						m_RemotePort;//Զ����־�˿�
	BOOL					m_RemoteMark;//Զ����־����

	S32						m_LogLevel;//������־�ĵȼ�����Ӧ���� PL_SyslogLevel()
}MULT_Syslog;

#endif

typedef struct
{
	S16		m_WebLanguage;//Webҳ�����ԣ�0��Ӣ��(ENG)��1������(CHN)��
	S16		m_FrpLanguage;//ǰ������������ţ�0��Ӣ��(ENG)��1������(CHN)��

	U32		m_ManageIPv4Addr;//IP��ַ
	U32		m_ManageIPv4Mask;//IP����
	U32		m_ManageIPv4Gate;//IP����
	U8		m_pMAC[GLOBAL_MAC_BUF_SIZE];

	U32		m_NTPServerAddr;
	S32		m_NTPInterval;//���¼�����룩 Ĭ��ֵ8*3600(8��Сʱ)
	BOOL8	m_NTPSyncMark;
// Timing Apply Parameter Start
	S32		m_TimingInterval;//���¼�����룩 Ĭ��ֵ6*3600(6��Сʱ)
	BOOL8	m_TimingMark;
// Timing Apply Parameter End
#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
	U32		m_IpOutputType;
	U32		m_OutputCharset;
	U32     m_VoEnc4KFlag;/*the flag of  input1 vo format*/
#endif

	//S8		m_IGMPVersion;//2,3��Ĭ��Ϊ2���ݲ�֧��3��
	//S32		m_IGMPInterval;//�Զ�����IGMP JOIN���ļ�����룩���Ƽ�ֵ20��
	//BOOL8	m_IGMPAutoMark;//�Զ�����IGMP JOIN���Ŀ���

	SNMP_InitParam m_SNMPInitParam;
	BOOL	m_SNMPGlobalMark;



}MULT_Config;//��Ҫ����Ĳ�����



/*ͨ������---------------------------------------------------------------------------------------------------------------------------------------*/

typedef enum
{
	GS_ETH_PROTOCOL_UDP = 0,
	GS_ETH_PROTOCOL_RTP,
}GS_CONST_PROTOCOL;

typedef enum
{
	GS_MODULATOR_STANDARD_ANNEX_A = 0,
	GS_MODULATOR_STANDARD_ANNEX_B,
	GS_MODULATOR_STANDARD_ANNEX_C
}GS_ModulatorStandard;

typedef enum
{
	GS_MODULATOR_QAM_16 = 0,
	GS_MODULATOR_QAM_32,
	GS_MODULATOR_QAM_64,
	GS_MODULATOR_QAM_128,
	GS_MODULATOR_QAM_256,
	GS_MODULATOR_QAM_512,
	GS_MODULATOR_QAM_1024,
	GS_MODULATOR_QAM_UNUSED,
	GS_MODULATOR_QAM_4,
	GS_MODULATOR_QAM_4NR,
	GS_MODULATOR_QAM_INVALID,
}GS_ModulatorQamMode;


typedef enum
{
	GS_MODULATOR_ANALOG_BAND_8M = 0,
	GS_MODULATOR_ANALOG_BAND_6M,
	GS_MODULATOR_ANALOG_BAND_INVLAID
}GS_ModulatorAnalogBand;

typedef enum
{
	GS_MODULATOR_FEC_ENCODE_1_2 = 0,
	GS_MODULATOR_FEC_ENCODE_2_3,
	GS_MODULATOR_FEC_ENCODE_3_4,
	GS_MODULATOR_FEC_ENCODE_5_6,
	GS_MODULATOR_FEC_ENCODE_7_8,

	GS_MODULATOR_FEC_ENCODE_INVLAID
}GS_ModulatorFec;


typedef enum
{
	GS_MODULATOR_CARRIER_MODE_1 = 0,
	GS_MODULATOR_CARRIER_MODE_3780,
}GS_ModulatorlCarrierMode;

typedef enum
{
	GS_MODULATOR_GUARD_INTERVAL_PN_420C = 0,
	GS_MODULATOR_GUARD_INTERVAL_PN_595,
	GS_MODULATOR_GUARD_INTERVAL_PN_945C,
	GS_MODULATOR_GUARD_INTERVAL_PN_420F,
	GS_MODULATOR_GUARD_INTERVAL_PN_945F,
}GS_ModulatorGuardInterval;

typedef enum
{
	GS_MODULATOR_CODE_RATE_0_4 = 0,//DTMB
	GS_MODULATOR_CODE_RATE_0_6,
	GS_MODULATOR_CODE_RATE_0_8,
}GS_ModulatorCodeRate;


typedef enum
{
	GS_MODULATOR_ISDB_T_TIME_INTERLEAVER_B_52_M_240 = 0,
	GS_MODULATOR_ISDB_T_TIME_INTERLEAVER_B_52_M_720,
}GS_ModulatorTimeInterleaver;

/////////////////////////////////////////////

typedef struct  
{
	/*ASI��ͨ������*/
	U32		m_Researved;
}MULT_SubASIInfo;


typedef struct  
{
	
	U32		m_E3DS3;
	U32		m_Bitorder;
	U32		m_Frameform;
	U32		m_Packetlength;
	U32		m_Scramble;
	U32		m_RSCoding;
	U32		m_InterweaveCoding;
	U32		m_RateRecover;
}MULT_SubESDS3Info;


typedef struct  
{

	U32		m_InputFreq;//��λKHz
	U32		m_LocalFreq;
	U32		m_SymbolRate;//��λKBaud
	U32		m_PolarMethod;
	U32		m_22kSwitch;
	U32		m_Modulation;
	U32		m_Reqtype;
	U32		m_SignalStrength;
	U32		m_SignalQuality;
	U32		m_LockStatus;
	U8		m_Specinv;
}MULT_SubTUNERInfo;

#if defined(GN1846) || defined(GN1866) || defined(GN1866B)

typedef enum
{
	IP_OUTPUT_SPTS = 0,
	IP_OUTPUT_MPTS,
	IP_OUTPUT_TYPE_NUM
} IP_OUTPUT_TYPE;

typedef enum
{
	Networkport_Auto = 0,
	Networkport_100M_F
} Networkport_Mode;

typedef enum
{
	ENC_VI_MODE_PAL,
	ENC_VI_MODE_NTSC,
	ENC_VI_MODE_720P_50,
	ENC_VI_MODE_720P_5994,
	ENC_VI_MODE_720P_60,
	ENC_VI_MODE_1080I_50,
	ENC_VI_MODE_1080I_5994,
	ENC_VI_MODE_1080I_60,
	ENC_VI_MODE_1080P_50,
	ENC_VI_MODE_1080P_5994,
	ENC_VI_MODE_1080P_60,
	ENC_VI_MODE_UHD4KP_30,
	ENC_VI_MODE_AUTO,
	ENC_VI_MODE_UNKNOWN,
	ENC_VI_MODE_NUM
} ENCODER_ViMode;

typedef enum
{
	ENC_VO_MODE_576P25,
	ENC_VO_MODE_480P2997,
	ENC_VO_MODE_720P25,
	ENC_VO_MODE_720P2997,
	ENC_VO_MODE_720P30,
	ENC_VO_MODE_720P50,
	ENC_VO_MODE_720P5994,
	ENC_VO_MODE_720P60,
	ENC_VO_MODE_1080P25,
	ENC_VO_MODE_1080P2997,
	ENC_VO_MODE_1080P30,
	ENC_VO_MODE_1080P50,
	ENC_VO_MODE_1080P5994,
	ENC_VO_MODE_1080P60,
	ENC_VO_MODE_UHD4KP_30,
	ENC_VO_MODE_AUTO,
	ENC_VO_MODE_NUM
} ENCODER_VoMode;

typedef enum
{
	ENC_BR_MODE_CBR = 0,
	ENC_BR_MODE_VBR,
	ENC_BR_MODE_NUM
} ENCODER_BrMode;

typedef enum
{
	ENC_MODE_H264,
	ENC_MODE_H265,
	ENC_MODE_NUM
} ENCODER_EncMode;

typedef enum
{
	ENC_PROFILE_BASE = 0,
	ENC_PROFILE_MP,
	ENC_PROFILE_HP,
	ENC_PROFILE_NUM
} ENCODER_Profile;


typedef enum
{
	ENC_MOSAIC_BLK_SIZE_8 = 0,
	ENC_MOSAIC_BLK_SIZE_16,
	ENC_MOSAIC_BLK_SIZE_32,
	ENC_MOSAIC_BLK_SIZE_64,
	ENC_MOSAIC_BLK_NUM
} ENCODER_MosaicSize;

typedef enum
{
	ENC_AUD_ENC_MODE_MPEG1_L2 = 0,
	ENC_AUD_ENC_MODE_LC_AAC,
	ENC_AUD_ENC_MODE_AC3,
	ENC_AUD_ENC_MODE_EAC3,
	ENC_AUD_ENC_MODE_NUM
} ENCODER_AudEncMode;

typedef enum
{
	ENC_AUD_BR_16K = 0,
	ENC_AUD_BR_64K,
	ENC_AUD_BR_96K, 
	ENC_AUD_BR_112K,
	ENC_AUD_BR_128K,
	ENC_AUD_BR_160K,
	ENC_AUD_BR_192K,
	ENC_AUD_BR_224K,
	ENC_AUD_BR_256K,
	ENC_AUD_BR_320K,
	ENC_AUD_BR_384K,
	ENC_AUD_BR_NUM
} ENCODER_AudBitrate;

typedef enum
{
	ENC_AUD_SAMP_32K = 0, 
	ENC_AUD_SAMP_44_1K,
	ENC_AUD_SAMP_48K,
	ENC_AUD_SAMP_AUTO,
	ENC_AUD_SAMP_NUM
} ENCODER_AudSample;

#if defined(SUPPORT_MOSIAC)
typedef struct
{
	U32					m_x_coordinate;
	U32					m_y_coordinate;
	U32 				m_width;
	U32 				m_height;
	ENCODER_MosaicSize 	m_blk_size;
	BOOL 				m_enable; 
}ENCODER_MOSAIC;;
#endif

typedef struct  
{
	ENCODER_ViMode		m_ViMode;
	ENCODER_VoMode		m_VoMode;
	ENCODER_BrMode		m_BrMode;
	ENCODER_EncMode		m_EncMode;
	ENCODER_Profile		m_Profile;
	S32					m_Bitrate;
	S32					m_ProgBitrate; /* ��Ŀ���� */
	S32					m_Gop;

	S32					m_AudioGroup;

	U32					m_x_coordinate;
	U32					m_y_coordinate;
	U32 				m_transparent;
	BOOL 				m_enable; /*OSD Enable or Disable */

#if defined(SUPPORT_MOSIAC)
	ENCODER_MOSAIC  m_Mosaic_Param;
#endif
	//S32	 				m_AudioGroup;
	ENCODER_AudEncMode	m_AudEncMode;
	ENCODER_AudBitrate	m_AudBitrate;
	ENCODER_AudSample	m_AudSample;
	S32					m_Volume;
	U8					m_pProgName[MPEG2_DB_MAX_SERVICE_NAME_BUF_LEN];
	S32					m_ProgNumber;
	S32					m_VidPid;
	S32					m_AudPid;
	S32					m_PcrPid;
	S32					m_PmtPid;
	U32 				m_Protocol;
	U32					m_IPv4Addr;
	U32					m_IPv4Port;
	BOOL				m_ActiveMark;
}MULT_SubENCODERInfo;

S32 MULTL_XMLIpOutputTypeValueFromStr(CHAR_T *pStr); 
CHAR_T* MULTL_XMLIpOutputTypeValueToStr(S32 Value);
CHAR_T* MULTL_XMLIPv4NetworkportsettingValueToStr(S32 Value);
S32 MULTL_XMLIPv4NetworkportSettingValueFromStr(CHAR_T *pStr);
S32 MULTL_XMLEdidTypeValueFromStr(CHAR_T *pStr); 
CHAR_T* MULTL_XMLEdidTypeValueToStr(S32 Value);
S32 MULTL_XMLEncViModeValueFromStr(CHAR_T *pStr); 
CHAR_T* MULTL_XMLEncViModeValueToStr(S32 Value);
S32 MULTL_XMLEncVoModeValueFromStr(CHAR_T *pStr); 
CHAR_T* MULTL_XMLEncVoModeValueToStr(S32 Value);
S32 MULTL_XMLEncBrModeValueFromStr(CHAR_T *pStr);
CHAR_T* MULTL_XMLEncBrModeValueToStr(S32 Value);
S32 MULTL_XMLEncModeValueFromStr(CHAR_T *pStr); 
CHAR_T* MULTL_XMLEncModeValueToStr(S32 Value);
S32 MULTL_XMLEncProfileValueFromStr(CHAR_T *pStr); 
CHAR_T* MULTL_XMLEncProfileValueToStr(S32 Value);
S32 MULTL_XMLAudEncModeValueFromStr(CHAR_T *pStr);
CHAR_T* MULTL_XMLAudEncModeValueToStr(S32 Value);


//add by leonli
CHAR_T* MULTL_XMLAudGroupValueToStr(S32 Value);
CHAR_T* MULTL_XMLEncMosaicSizeValueToStr(S32 Value);

S32 MULTL_XMLAudBitrateValueFromStr(CHAR_T *pStr);
CHAR_T* MULTL_XMLAudBitrateValueToStr(S32 Value);
S32 MULTL_XMLAudSampValueFromStr(CHAR_T *pStr);
CHAR_T* MULTL_XMLAudSampValueToStr(S32 Value);
#endif

typedef struct  
{
	/*IP��ͨ������*/
	U32 	m_Protocol;
	U32		m_IPv4Addr;
	U32		m_IPv4Port;

	U32		m_IPv4LimitAddr;//����ͨ��Դ��ַ�޶���IGMPv3�޶����ȼۣ�����Ϊ���ʹ��ʱ����IP��ƭ����
	U32		m_IPv4LimitPort;
	BOOL8	m_IPv4LimitMark;
}MULT_SubIPInfo;

typedef struct  
{
	/*��������ͨ������*/
	U8		m_ITUCoding;
	U8		m_AnalogBand;
	U32		m_CenterFreq;	
	U32		m_SymbolRate;	
	U8		m_Mode;	
	BOOL8	m_SpectInv;
	U8		m_GainLevel;
	BOOL8	m_Modulation;

	U32		m_LevelAttenuation;	//��ƽ˥��

	//for qpsk ---add by ding
	U32		m_FecEncode;		//fec ����

	//DTMB
	S32		m_CarrierMode;//���ز������ز�
	S32		m_PNMode;
	S32		m_CodeRate;//0.4 0.6 0.8
	S32		m_TimeInterleave;//��֯ 2��ģʽ
	BOOL	m_DoublePilot;

#ifdef SUPPORT_SFN_MODULATOR
	//SFN
	BOOL	m_RFMute;
#endif

	//RF���ط��������
}MULT_SubModulatorInfo;

typedef struct
{
	union
	{
		MULT_SubASIInfo			m_SubASIInfo;
		MULT_SubIPInfo			m_SubIPInfo;
		MULT_SubModulatorInfo	m_SubModulatorInfo;	
		MULT_SubESDS3Info       m_SubESDS3Info;
		MULT_SubTUNERInfo		m_SubTUNERInfo;
#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
		MULT_SubENCODERInfo		m_SubENCODERInfo;
#endif
	}
	m_SubInfo;
	S32		m_CorrespondTsIndex;//��Ӧ���߼�TS��š�������ʱ�ɾ���ƽָ̨����
	BOOL8	m_ActiveMark;//ͨ������ر����ԣ��رպ����ͨ��������Ҳ��������ݣ���ʱ���ݿ��и��ò���������������ӦTS����Ϣ��ȻҪ�����FPGA��
	U32		m_Bitrate;//��λMBps 2-99
}MULT_SubChannelNode;


typedef struct  
{
	/*ASIͨ������*/
	U32		m_Bitrate;//����������216M * N
}MULT_ASIInfo;

typedef struct  
{
	
	U32		m_Researbed;
}MULT_E3DS3Info;
typedef struct  
{
	
	U32		m_FreqLimitsLow;
	U32		m_FreqLimitsHigh;
	U32		m_SymRateLimitsLow;
	U32		m_SymRateLimitsHigh;
}MULT_TUNERInfo;

#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
typedef struct  
{
	U32	m_VidOutBitrateLimitLow;
	U32	m_VidOutBitrateLimitHigh;
	U32 m_ProgBitrateLimitHigh;
	U32	m_GopLimitLow;
	U32 m_GopLimitHigh;
	S32	m_VolLimitLow;
	S32 m_VolLimitHigh;
	U32 m_ProgNumberLimitLow;
	U32 m_ProgNumberLimitHigh;
	U32 m_ProgNameLimitHigh; /* ��Ŀ���������� */
	U32	m_PidLimitLow;
	U32	m_PidLimitHigh;

#if defined(SUPPORT_WATER_MARK)
	U32 m_x_coordinateLimitLow;
	U32 m_x_coordinateLimitHigh;
	U32 m_y_coordinateLimitLow;
	U32 m_y_coordinateLimitHigh;
	U32 m_transparentLimitLow;
	U32 m_transparentLimitHigh;
#endif

#if defined(SUPPORT_MOSIAC)
	/* Mosaic */
	U32 m_Mosaic_x_coordinateLimitLow;
	U32 m_Mosaic_x_coordinateLimitHigh;
	U32 m_Mosaic_y_coordinateLimitLow;
	U32 m_Mosaic_y_coordinateLimitHigh;
   U32 m_Mosaic_Width_LimitLow;
   U32 m_Mosaic_Width_LimitHigh;
   U32 m_Mosaic_Height_LimitLow;
   U32 m_Mosaic_Height_LimitHigh;
#endif

}MULT_ENCODERInfo;
#endif

typedef struct  
{
	/*IPͨ������*/
	U32		m_IPAddress;
	U32		m_IPMask;
	U32		m_IPGate;

	U8		m_MAC[GLOBAL_MAC_BUF_SIZE];

	U32		m_Bitrate;//����������5-850

	BOOL	m_IPMark;
}MULT_IPInfo;


typedef struct  
{
	/*������ͨ������*/
	S32		m_AdjacentFreqNumber;
	U32		m_CenterFrequenceLimitsLow;
	U32		m_CenterFrequenceLimitsHigh;
	S32		m_SymbolRateLimitsLow;
	S32		m_SymbolRateLimitsHigh;
	S32		m_ExAttValidMark;
	F64		m_ExAttStepping;
	S32		m_ExAttLevel;
	S32		m_ExAttLevelMax;
	F64		m_GainStepping;
	S32		m_GainLevelMax;
	BOOL	m_DPDMark;
	BOOL	m_ALCMark;
	S32		m_FreqAdj;
}MULT_ModulatorInfo;

typedef struct
{
	/*ͨ�ò���*/
	S32		m_OriginalIndex;
	S32		m_ChannelType;
	S32		m_SubType;
	S32		m_DemodType;

// 	U32		m_MaxBitrate;
// 	U32		m_MinBitrate;

	union
	{
		MULT_ModulatorInfo	m_ModulatorInfo;
		MULT_IPInfo			m_IPInfo;
		MULT_ASIInfo		m_ASI;
		MULT_E3DS3Info      m_E3DS3;
		MULT_TUNERInfo		m_TUNER;

#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
		MULT_ENCODERInfo	m_ENCODER;
#endif

	}m_ChannelInfo;

	MULT_SubChannelNode *m_pSubChannelNode;
	S32		m_SubChannelNumber;
}MULT_ChannelNode;

typedef struct
{
	S16					m_IGMPRepeateTime;//20~255
	BOOL				m_IGMPRepeateMark;
	S16					m_IGMPVersion;//2��3
}MULT_IGMP;


#ifdef MULT_SYSTEM_HAVE_PCR_CORRECT_ADJUST_FUNCTION
#define MULT_PCR_CORRECT_POS_DEFAULT_VALUE	7
#define MULT_PCR_CORRECT_NEG_DEFAULT_VALUE	2
typedef struct
{
	BOOL				m_PCRCMark;
	S32					m_PCRCPos;
	S32					m_PCRCNeg;
}MULT_PCRCorrect;
#endif


typedef struct
{
	MULT_ChannelNode	*m_pInChannel;
	S32					m_InChannelNumber;
	MULT_ChannelNode	*m_pOutChannel;
	S32					m_OutChannelNumber;

	S32					m_MaxInTsNumber;
	S32					m_MaxOutTsNumber;

	S32					m_TDTUpdateInterval;/*TDTʱ����¼��*/

	MULT_IGMP			m_IGMP;

#ifdef MULT_SYSTEM_HAVE_PCR_CORRECT_ADJUST_FUNCTION
	MULT_PCRCorrect		m_PCRCorrect;
#endif

}MULT_Parameter;



/*��ز���/״̬----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define MULT_MONITOR_ENCODE_ID(Slot) ((Slot) + 1)
#define MULT_MONITOR_DECODE_ID(ID) ((ID) - 1)

typedef enum 
{
	MULT_MONITOR_TYPE_TEMP = 0,
	MULT_MONITOR_TYPE_FPGA_HEART_BEAT,
	MULT_MONITOR_TYPE_NTP,
	MULT_MONITOR_TYPE_SCS_EMM,
	MULT_MONITOR_TYPE_SCS_ECM,
	MULT_MONITOR_TYPE_ETH_LINK,
	MULT_MONITOR_TYPE_PLL,
	MULT_MONITOR_TYPE_CHANNEL_IN,//ͨ����ر�����������������ڻ�����û��趨ֵ�ȡ�
	MULT_MONITOR_TYPE_CHANNEL_OUT,//ͨ����ر�����������������ڻ�����û��趨ֵ�ȡ�
	MULT_MONITOR_TYPE_BUFFER_STATUS,
	MULT_MONITOR_TYPE_FAN_FAILURE,
	MULT_MONITOR_TYPE_POWER_FAILURE,
	MULT_MONITOR_TYPE_INPUT_TS_CHANGED,
	MULT_MONITOR_TYPE_TUNER_SHORT_STATUS,
	MULT_MONITOR_TYPE_PROGRAM_BITRATE,

	MULT_MONITOR_TYPE_ENCODER_CVBS_LOCK_CHN,
	MULT_MONITOR_TYPE_ENCODER_INPUT_ERROR_VIDEO,
	MULT_MONITOR_TYPE_ENCODER_OUTPUT_ERROR_NOBITRATE,
	MULT_MONITOR_TYPE_ENCODER_OUTPUT_ERROR_OVERFLOW,


	MULT_MONITOR_TYPE_GPS_LOCK_LOST,//GPSģ���ṩ��δ������Ϣ
	MULT_MONITOR_TYPE_EXT_10M_LOST,//FPGA�ṩ���ⲿ10M��ʧ��Ϣ
	MULT_MONITOR_TYPE_EXT_1PPS_LOST,//FPGA�ṩ���ⲿ1PPS������Ϣ
	MULT_MONITOR_TYPE_GPS_ERROR,//GPSģ���޷����յ���������
	MULT_MONITOR_TYPE_SFN_SIP_ERROR,//SFNģʽ��û���յ�SIP��
	MULT_MONITOR_TYPE_INT_10M_LOST,//FPGA�ṩ���ڲ�10M��ʧ��Ϣ
	MULT_MONITOR_TYPE_INT_1PPS_LOST,//FPGA�ṩ���ڲ�1PPS������Ϣ
	MULT_MONITOR_TYPE_SFN_SIP_CRC32_ERROR,//����SFNģʽ���յ���SIP��CRC32����
    MULT_MONITOR_TYPE_SFN_ERROR,//SFNģʽ���ִ����µ�Ƶ��ʧЧ
	MULT_MONITOR_TYPE_DPD_PARAMETER_SAVE_ERROR,//DPD�����������
	MULT_MONITOR_TYPE_CLK_SYNC_ERROR,//CLKģ�飬ʱ��У�������г��ֵĴ�����ʾ
	//MULT_MONITOR_TYPE_SFN_SIP_CHANGE,//SIP �仯���� //27

#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
	MULT_MONITOR_TYPE_ENCODER_BUFFER_OVERFLOW_ERROR, /* ���뻺������� */
	MULT_MONITOR_TYPE_HDMI_INPUT_LOST_ERROR, /* HDMI �����źŶ�ʧ */
	MULT_MONITOR_TYPE_HDMI_INPUT_FORMAT_ERROR, /* HDMI �����źŸ�ʽ���� */
	MULT_MONITOR_TYPE_AUDIO_IN_GROUP_INPUT_ERROR, /* ��Ƶ������ѡ����� */
#endif

	MULT_MONITOR_TYPE_NUM 
}MULT_MonitorLogSlot;

typedef struct  
{
	S32		m_LowLimit;
	S32		m_HighLimit;
	U8		m_AlarmCount;
	BOOL	m_Mark;
}MULT_MonitorLimitInfo;

typedef struct  
{
	MULT_MonitorLimitInfo	m_LimitInfo;
	S32						m_CurValue;
// 	BOOL					m_Mark;//��Ч���أ��������˵���ͨ������Ч����ʾ

#if defined(GN1846) || defined(GN1866)  || defined(GN1866B)
	U32						m_HdmiSignalLostCount; /* �źŶ�ʧ������ʹ�øò�����ֹ���źŶ�ʧ */			
#endif
}MULT_MonitorSUB;


typedef struct  
{
	MULT_MonitorLimitInfo	m_LimitInfo;
	S32						m_CurValue;
	MULT_MonitorSUB			*m_pSubArray;
	S32						m_SubNumber;
	S32						m_StartTsIndex;
}MULT_MonitorCHN;


typedef struct  
{
	/*״̬��Ϣ*/
	BOOL8			m_GlobalTempMark;//����ϵͳȫ����ʱ���أ��ڲ�������ʱ������Ҫ��ʱ�رձ�����־ϵͳ

	/*��ǰ����״̬*/
	BOOL8			m_bApplyMark;//ΪTRUEʱ�������������ú�û��Ӧ��
	BOOL8			m_bSaveMark;//ΪTRUEʱ�������������ú�û�б���

	/*�����Ϣ������*/
	BOOL8			m_GlobalMark;//����ϵͳȫ�ֿ���
	U8				m_CriticalTemp;//�¶ȱ������ޣ����ޣ�
	U8				m_FanTemp;//�����������¶����ޣ����ޣ�
	S32				m_TempErrorCount;

	S32				m_CurrentTemp;//��ǰ�¶�
	BOOL8			m_FanStatus;//��ǰ���ȿ���״̬����̬��
	BOOL8			m_FPGAStatus;//��ǰFPGA����״̬��
	BOOL8			m_PowerStatus;

	S32				m_EthStatus[4];
	S32				m_LastEthStatus[4];

	BOOL8			m_NTPStatus;//����NTP����ʱ
	BOOL8			m_NTPWorkingStatus;//NTP�첽����״̬
	S32				m_NTPSyncTimeout;//NTPͬ����ʱ

// Timing Apply Parameter Start
    BOOL8			m_TimingWorkingStatus;
    S32				m_TimingTimeout;
	S32 			m_TimingLastDuration;
// Timing Apply Parameter End

// 	BOOL8			m_InputLink;//����IP���ۺ�ͨ��
// 	BOOL8			m_OutputLink;//����IP���ۺ�ͨ��

	/*������־��Ϣ��ƣ��ؼ����ڰ�ͨ����������һ��(��������������ֿ�)����������*/
	HANDLE32		m_LogHandle;

	MULT_MonitorCHN *m_pInChnArray;//ͨ�������Ϣ
	S32				m_InChnNum;
	U32				m_TotalInBitrate;

	MULT_MonitorCHN *m_pOutChnArray;//ͨ�������Ϣ
	S32				m_OutChnNum;
	U32				m_TotalOutBitrate;

	U32				m_InserterBitrate;

	S32				m_CPUUsage;//0~100 CPU������0%~100%
	S32				m_MEMUsage;//0~100 Memory������0%~100%

	S32				m_ETHNumber;

	BOOL8			m_BusyMark;
	BOOL8			m_ApplyParametersBusyMark;


	CHAR_T			m_pTimeDataBuf[64];

	U32				m_CurrentPLLValue;
	S32				m_PLLErrorCount;
	U8				m_ShortCount[4];

	S32				m_FlowCount[MULT_DEVICE_MODUATION_NUM];
	F64				m_CurDuration;


#ifdef SUPPORT_NEW_HWL_MODULE
	HWL_MonitorPIDStatArray	m_PIDArray;
#else
	HWL_PIDStatisticsArray	m_PIDArray;
	S32						m_PIDStatisticsChnIndex;//��Ӧԭʼͨ���ţ����������������͵�
	S16						m_PIDStatisticsSubIndex;
#endif



	HWL_IPStatisticsArray	m_IPArray;
	S32						m_IPStatisticsChnIndex;//��Ӧԭʼͨ���ţ����������������͵�

#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
	HDMI_RxStatusParam		m_HdmiRxStatus[MULT_MAX_CHN_NUM];
	//BOOL					m_Chn2SetParamFlag;//�ڶ���ͨ�����ò���Flag
	S32						m_MEMSize;//�ڴ��С
	S32						m_CpuCurrentTemp;//cpu��ǰ�¶�

#endif

}MULT_Monitor;

/* ͬ�ܼ��� --------------------------------------------------------------------------------------------------------------------------------------- */
#define MULT_SCS_MAX_SERVICE_NUM_PER_TS				64
#define MULT_SCS_MAX_EMM_BITRATE					(1024)//Kbps
#define MULT_SCS_MAX_CRYPTO_CHN						128


#define BSS_SESSION_WORD_LENGTH						16
#define BSS_CONTROL_WORD_LENGTH						8
#define BSS_USERID_LENGTH							16

typedef struct  
{
	U32	m_ISRID;
	U16	m_PID;
	S16 m_ECMInterval;
	BOOL m_OutputMark;
}MULT_SCSECMInfo;



typedef struct  
{
	MULT_SCSECMInfo	m_pInfo[MPEG2_DB_SIMULCRYPT_CA_MAX_NUM];
	S16 m_OutTsIndex;
	U32 m_CryptoStreamID;
}MULT_SCSECMSlot;

typedef struct  
{
	U16	m_PID;
	BOOL m_OutputMark;
}MULT_SCSEMMInfo;

typedef struct  
{
	MULT_SCSEMMInfo	m_pInfo[MPEG2_DB_SIMULCRYPT_CA_MAX_NUM];
	S32	m_OutTsIndex;
}MULT_SCSEMMSlot;


typedef struct  
{
	S32	m_SuperCASID;
	S32	m_EMMActualBandwidth;
	S32	m_EMMCurBitrate;
	S32	m_EMMLinkStatus;
	S32	m_ECMLinkStatus;
	BOOL m_bActiveMark;
}MULT_SCSSlot;


typedef struct  
{
	HANDLE32 m_SCSHandle;
	HANDLE32 m_ThreadHandle;
	BOOL	 m_ThreadMark;

	MULT_SCSSlot m_SCSSlot[MPEG2_DB_SIMULCRYPT_CA_MAX_NUM];
	S32		m_SCSCount;

	MULT_SCSECMSlot *m_pECMSlotArray;
	S32		m_ECMMaxNum;
	S32		m_ECMCurNum;

	MULT_SCSEMMSlot *m_pEMMSlotArray;
	S32		m_EMMMaxNum;/*�����������TS������*/

	BOOL	m_ScrambleStatus;//����״̬
	S32		m_CurrentCPDuration;//��ǰʵ�ʼ�������
	S32		m_CurrentComputeTime;//��ǰʵ�ʼ���ʵ��
	U16		m_CurrentCPNumber;//��ǰʵ������

	CHAR_T		m_pECMTimeDataBuf[64];
}MULT_SCS;


/*BSS*/
typedef struct
{	
	U8	m_pSW[BSS_SESSION_WORD_LENGTH];
	U8	m_pKey[BSS_SESSION_WORD_LENGTH];
	U8	m_ActiveMark;
	U32 m_SuperCASID;
}MULT_BISS;


#if defined(GN1846)
#define EDID_CHIP_NUM (4)
#endif

#if defined(GN1866) || defined(GN1866B)
#define EDID_CHIP_NUM (2)
#endif

#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
typedef struct
{	
	S32	m_EdidType[EDID_CHIP_NUM];
}MULTL_Edid;
#endif


/*ϵͳ���----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define MULT_MAX_XML_STRING_SIZE	(1*1024*1024)

typedef struct
{
	MULT_Maintenace		m_MaintSetting;//���ز�������Ϣ,ע���ڻָ��������õ�ʱ����Ҫ������������������
	MULT_Information	m_Information;//�豸OEM����ȨH��Ϣ��ÿ������ʱ��̬���ɣ�
	MULT_Config			m_Configuration;//�豸�趨��Ϣ IPV4 NTP�����ɵ��뵼�������ڳ�������ʱ�ָ���
	MULT_Parameter		m_Parameter;//�豸�������ɵ��뵼�����ڻָ�Ĭ�ϲ����ͳ�������ʱ���ָ���
	MULT_Monitor		m_Monitor;//�豸��ز������ɵ��뵼�����ڻָ�Ĭ�ϲ����ͳ�������ʱ���ָ������а�������ʱ��̬�޸ĵĲ��֣�ͬʱҲ�Ǽ�غ͵ײ�״̬ҳ�����Ϣ��Դ��
#if defined(GN1846) || defined(GN1866)  || defined(GN1866B)
	MULTL_Edid			m_EdidInfo;
	S32					m_AuthMode;/*֮ǰ����Ȩģʽ*/
#endif
	U32					m_MonitoerTimerID;
	CHAR_T				m_pReplayXMLBuf[MULT_MAX_XML_STRING_SIZE];

#ifdef SUPPORT_SYSLOG_MODULE
	MULT_Syslog			m_SyslogParam;
#endif

	HANDLE32	m_MainMsgHandle;//����Ϣ���о��

	HANDLE32	m_DBSHandle;//DVB���ݿ�ģ����
	HANDLE32	m_DBSLock;

	MULT_SCS	m_MultSCS;

	MULT_BISS	m_BSSystemInfo;

	HANDLE32	m_AUTHHandle;//��Ȩ����ģ����

	HANDLE32	m_UPGHandle;//����OEMģ����

	U8			m_TaskMark;

	U32			m_RetCode;

	S16					m_BatchCurTsIndex;//��������ʱ�ĵ�ǰTS��
	S16					m_BatchMaxTsNumber;
	U8					m_BatchCharset;
	S32					m_BatchTimeout;
	BOOL				m_BatchRemoveService;
	BOOL				m_bBatchAnalyzeActive;
	S32					m_TunerCount;
	S32					m_TunerType;

	/*�������*/
	U32					m_bHaveShortTest;
	U8					m_TunerSPolar[4];

	S32					m_MonitorPauseTimeout;
}MULT_Handle;



typedef struct
{
	U8		m_MsgType;
	U8		m_MsgParam;
	U16		m_MsgParamResearved;
	S32		m_MsglValue;
	U32		m_MsgwValue;
}MULT_MainMsg;

typedef enum  
{
	MULTI_MAIN_MSG_TYPE_PSI_ARRIVE = 0,
	MULTI_MAIN_MSG_TYPE_INIPUT_ANALYZE_COMPLETE,
	MULTI_MAIN_MSG_TYPE_WEB_XML_CMD,
	MULTI_MAIN_MSG_TYPE_MONITOR_TIMER,
	MULTI_MAIN_MSG_TYPE_COUNT
}MULT_MainMsgType;


typedef struct  
{
	U32	m_ORIIDs;
	U32	m_NewIDs;
}MULT_IDs;


typedef struct  
{
	MULT_IDs	*m_pIDArray;
	S32			m_CurNum;
	S32			m_MaxNum;
}MULT_IDsArray;


/* Functions prototypes ------------------------------------------------------- */
S16 MULTL_DBActionCB(void* pUserParam, S32 DBActionType, U8* pBuf,S16 Datalen);


void MULTL_WEBXMLPostProcess(MULT_Handle *pHandle, CHAR_T* pXMLStr, S32 StrBufSize);
void MULTL_WEBXMLGetProcess(MULT_Handle *pHandle, CHAR_T* pXMLStr, S32 StrBufSize);

void MULTL_ApplyRemuxParameter(MULT_Handle *pHandle);
void MULTL_ApplyDirectRouteMode(MULT_Handle *pHandle);
void MULTL_ApplyAllParamter(MULT_Handle *pHandle);
void MULTL_SetRemuxApplyMark(MULT_Handle *pHandle, BOOL bClear);
void MULTL_SetSaveMark(MULT_Handle *pHandle, BOOL bClear);
void MULTL_MonitorProcess(MULT_Handle *pHandle, S32 Duration);

void MULTL_Reboot_SDI(MULT_Handle *pHandle) ;
void MULTL_Print_SDI_Reg(MULT_Handle *pHandle);

void MULTL_SaveParamterToStorage(MULT_Handle *pHandle);
BOOL MULTL_LoadParamterFromStorage(MULT_Handle *pHandle, CHAR_T *pFilePathName);
void MULTL_GenerateParamters(MULT_Handle *pHandle, CHAR_T *pFilePathName, CHAR_T* pDescription);
BOOL MULTL_ValidationParameter(MULT_Handle *pHandle, CHAR_T* pFilePathName, CHAR_T *pDescBuf, S32 BufSize);
void MULTL_RebootSequence(MULT_Handle *pHandle);


CHAR_T *MULTL_XMLGetNodeText(mxml_node_t *pParent, CHAR_T* pTagname);
S32 MULTL_XMLGetNodeINT(mxml_node_t *pParent, CHAR_T* pTagname, S32 Radix);
S32 MULTL_XMLGetNodeINTDefault(mxml_node_t *pParent, CHAR_T* pTagname, S32 Radix, S32 Default);
U32 MULTL_XMLGetNodeUINT(mxml_node_t *pParent, CHAR_T* pTagname, S32 Radix);
U32 MULTL_XMLGetNodeUINTDefault(mxml_node_t *pParent, CHAR_T* pTagname, S32 Radix, U32 Default);
F64 MULTL_XMLGetNodeFLOATDefault(mxml_node_t *pParent, CHAR_T* pTagname, F64 Default);
BOOL MULTL_XMLGetNodeBOOL(mxml_node_t *pParent, CHAR_T* pTagname);
BOOL MULTL_XMLGetNodeMark(mxml_node_t *pParent, CHAR_T* pTagname);
BOOL MULTL_XMLGetNodeMarkDefault(mxml_node_t *pParent, CHAR_T* pTagname, BOOL Default);


void MULTL_XMLAddNodeCDData(mxml_node_t *pParent, CHAR_T* pTagname, CHAR_T *pValue);
void MULTL_XMLAddNodeText(mxml_node_t *pParent, CHAR_T* pTagname, CHAR_T *pValue);
void MULTL_XMLAddNodeINT(mxml_node_t *pParent, CHAR_T* pTagname, S32 Value);
void MULTL_XMLAddNodeUINT(mxml_node_t *pParent, CHAR_T* pTagname, U32 Value);
void MULTL_XMLAddNodeFLOAT(mxml_node_t *pParent, CHAR_T* pTagname, F64 Value);
void MULTL_XMLAddNodeFLOATE(mxml_node_t *pParent, CHAR_T* pTagname, F64 Value);
void MULTL_XMLAddNodeHEX(mxml_node_t *pParent, CHAR_T* pTagname, U32 Value);
void MULTL_XMLAddNodeBOOL(mxml_node_t *pParent, CHAR_T* pTagname, BOOL Value);
void MULTL_XMLAddNodeMark(mxml_node_t *pParent, CHAR_T* pTagname, BOOL Value);


CHAR_T* MULTL_XMLMarkValueToStr(BOOL Value);
BOOL MULTL_XMLMarkValueFromStr(CHAR_T* pString);
CHAR_T* MULTL_XMLITUCodingValueToStr(S32 ITUCoding);
S32 MULTL_XMLITUCodingValueFromStr(CHAR_T* pString);
CHAR_T* MULTL_XMLAnalogValueToStr(S32 Analog);
S32 MULTL_XMLAnalogValueFromStr(CHAR_T* pString);
CHAR_T* MULTL_XMLQAMModeValueToStr(S32 Mode);
S32 MULTL_XMLQAMModeValueFromStr(CHAR_T* pString);
S32  MULTL_XMLETHValueFromStr( CHAR_T* pString);
CHAR_T*  MULTL_XMLETHValueToStr( S32 value );
CHAR_T* MULTL_XMLDeliveryTypeToStr(S32 Value);
S32 MULTL_XMLDeliveryTypeFromStr(CHAR_T* pString);
CHAR_T* MULTL_XMLCableDeliveryModeValueToStr(S32 Value);
S32 MULTL_XMLCableDeliveryModeValueFromStr(CHAR_T* pString);
U32 MULTL_XMLGetNodeIP(mxml_node_t *pParent, CHAR_T* pTagname);

S32 MULTL_XMLFecEncodeValueFromStr(CHAR_T* pString);
CHAR_T* MULTL_XMLFecEncodeValueToStr( S32 Value );

/////////////////////////////////////////////////
CHAR_T* MULTL_XMLE3DS3ValueToStr(S32 Value);
S32 MULTL_XMLE3DS3ValueFromStr(CHAR_T* pString);
CHAR_T* MULTL_XMLBitOrderValueToStr(S32 Value);
S32 MULTL_XMLBitOrderValueFromStr(CHAR_T* pString);
CHAR_T* MULTL_XMLFrameformValueToStr(S32 Value);
S32 MULTL_XMLFrameformValueFromStr(CHAR_T* pString);
CHAR_T* MULTL_XMLPacketlengthValueToStr(S32 Value);
S32 MULTL_XMLPacketlengthformValueFromStr(CHAR_T* pString);
/* -DTMB PN----------------------------------------------------------------------------------------------------------------------------------- */
CHAR_T* MULTL_XMLDTMBPNValueToStr(S32 Value);
S32 MULTL_XMLDTMBPNValueFromStr(CHAR_T* pString);
/* -DTMB CodeRate----------------------------------------------------------------------------------------------------------------------------------- */
CHAR_T* MULTL_XMLDTMBCodeRateValueToStr(S32 Value);
S32 MULTL_XMLDTMBCodeRateValueFromStr(CHAR_T* pString);




S32 MULTL_XMLPolarMethodValueFromStr(CHAR_T* pString);
CHAR_T* MULTL_XMLPolarMethodValueToStr(BOOL Value);
S32 MULTL_XMLReqTunerTypeValueFromStr(CHAR_T* pString);
CHAR_T* MULTL_XMLReqTunerTypeValueToStr(BOOL Value);

S32 MULTL_XMLTunerSpecinvValueFromStr(CHAR_T* pString);
CHAR_T* MULTL_XMLTunerSpecinvValueToStr(BOOL Value);



CHAR_T* MULTL_XMLChnTypeValueToStr(S32 Value);
S32 MULTL_XMLChnTypeValueFromStr(CHAR_T* pString);
CHAR_T* MULTL_XMLChnSubTypeValueToStr(S32 Value);
S32 MULTL_XMLChnSubTypeValueFromStr(CHAR_T* pString);


S32 MULTL_GenerateSN(U8 DeviceType, U8 DeviceSubType, U32 ChipID, U8 *pSN, S32 BufSize);
void MULTL_GenerateMAC(U8 DeviceType, U8 DeviceSubType, S32 Index, U32 ChipIDSeries, U8 *pMAC, S32 BufSize);
BOOL MULTL_LoadOEM(MULT_Handle *pHandle, CHAR_T *pPathname, BOOL bCheckValid, CHAR_T* pInformation, S32 BufSize);


void MULTL_GenerateInfoXML(MULT_Handle *pHandle);

void MULTL_DefaultManageIP(MULT_Handle *pHandle);
void MULTL_DefaultConfiguration(MULT_Handle *pHandle, HWL_HWInfo* pHWInfo);
void MULT_SaveConfigurationXML(MULT_Handle *pHandle);
void MULTL_LoadConfigurationXML(MULT_Handle *pHandle, HWL_HWInfo* pHWInfo);

void MULTL_LoadMaintenaceXML(MULT_Handle *pHandle, HWL_HWInfo* pHWInfo);
void MULTL_SaveMaintenaceXML(MULT_Handle *pHandle);


void MULTL_DefaultDataIP(MULT_Handle *pHandle);
void MULTL_DefaultParameter(MULT_Handle *pHandle, HWL_HWInfo* pHWInfo);
void MULTL_SaveParameterXML(MULT_Handle *pHandle);
void MULTL_LoadParameterXML(MULT_Handle *pHandle, HWL_HWInfo* pHWInfo);
void MULTL_ReleaseParameter(MULT_Handle *pHandle);


void MULTL_DefaultMonitorSetting(MULT_Handle *pHandle, HWL_HWInfo* pHWInfo);
void MULTL_SaveMonitorXML(MULT_Handle *pHandle);
void MULTL_LoadMonitorXML(MULT_Handle *pHandle, HWL_HWInfo* pHWInfo);
void MULTL_ReleaseMonitor(MULT_Handle *pHandle);


void MULTL_XMLSaveDescriptors(MULT_Handle *pHandle, mxml_node_t *pXMLParent, U8 DescriptorType, U32 ParentIDs);
void MULTL_XMLLoadDescriptors(MULT_Handle *pHandle, mxml_node_t *pXMLParent, U8 DescriptorType, U32 ParentIDs);
void MULTL_XMLSaveMuxCAs(MULT_Handle *pHandle, mxml_node_t *pXMLParent, U8 CAType, U32 ParentIDs);
void MULTL_XMLLoadMuxCAs(MULT_Handle *pHandle, mxml_node_t *pXMLParent, U8 CAType, U32 ParentIDs);
void MULTL_XMLSaveSCSCAs(MULT_Handle *pHandle, mxml_node_t *pXMLParent, U8 CAType, U32 ParentIDs);
void MULTL_XMLLoadSCSCAs(MULT_Handle *pHandle, mxml_node_t *pXMLParent, U8 CAType, U32 ParentIDs, MULT_IDsArray *pACArray);
void MULTL_XMLSaveMonitorLimitInfo(MULT_Handle *pHandle, mxml_node_t *pXMLParent, MULT_MonitorLimitInfo *pInfo);
void MULTL_XMLLoadMonitorLimitInfo(MULT_Handle *pHandle, mxml_node_t *pXMLParent, MULT_MonitorLimitInfo *pInfo);

void MULTL_LoadChannelInfoXML(MULT_Handle *pHandle, MULT_ChannelNode *pChnNode, mxml_node_t *pXMLParent);
void MULTL_LoadSubChannelInfoXML(MULT_Handle *pHandle, MULT_ChannelNode *pChnNode, MULT_SubChannelNode *pSubNode, mxml_node_t *pXMLParent);

#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
#include "adv7612.h"
#include "multi_hwl_probe.h"
#if defined(GN1846)
#include "encoder_3531A.h"
#elif defined(GN1866)  || defined(GN1866B)
#include "encoder_3519.h"
#endif
void MULTL_LoadEdidXML(MULT_Handle *pHandle, HWL_HWInfo* pHWInfo);
void MULTL_SaveEdidXML(MULT_Handle *pHandle);
void MULTL_ApplyLEncoderParameter(MULT_Handle *pHandle, S32 ChnIndex);
ENC_3531AViMode MULTL_Adv7612ViMode2LEncoderViMode(VideoStd_Type lVidStd);
ENC_3531AAudSample MULTL_Adv7612AudSample2LEncoderAudSample(samplerate_items lAudSample);

#endif

#if defined(GN1866B)
#include "LT6911C.h"
ENC_3531AViMode MULTL_Lt6911cViMode2LEncoderViMode(LT6911C_VideoStd_Type lVidStd);
ENC_3531AAudSample MULTL_Lt6911cAudSample2LEncoderAudSample(lt6911c_samplerate_items lAudSample);
BOOL HWL_LT6911cReset(HANDLE32 Handle, S32 ChnIndex);
#endif

void MULTL_CreateIDs(MULT_IDsArray *pArray, S32 MaxNum);
void MULTL_AddIDs(MULT_IDsArray *pArray, MULT_IDs *pACIDs);
U32 MULTL_GetNewIDs(MULT_IDsArray *pArray, U32 OriIDs);
void MULTL_DestroyIDs(MULT_IDsArray *pArray);

void MULTL_ForceSNTPSync(MULT_Handle *pHandle);


void MULTL_ApplyQAMParameter(MULT_Handle *pHandle, S32 Index);


////////////////////////////////////////
void MULTL_ApplyInE3DS3Parameter(MULT_Handle *pHandle, S32 ChnIndex);
void MULTL_ApplyTunerParameter(MULT_Handle *pHandle, S32 ChnIndex);
void MULTL_ApplyOutASIParameter(MULT_Handle *pHandle, S32 ChnIndex);
void MULTL_ApplyInETHParameter(MULT_Handle *pHandle, S32 ChnIndex);
void MULTL_ApplyOutETHParameter(MULT_Handle *pHandle, S32 ChnIndex);





void MULTL_ResetAlarmCount(MULT_Handle *pHandle, S32 AlarmIndex);
void MULTL_ParameterReset(MULT_Handle *pHandle);
void MULTL_FactoryPreset(MULT_Handle *pHandle);
void MULTL_ManagePortConfig(MULT_Handle *pHandle);
#if defined(INDEPENDENT_DATA_IP_SUPPORT)
#else
#if defined(CPU_DATA_PORT)
BOOL MULTL_GetEthSetFlag(void); 
#endif
#endif

/* ǰ��� ------------------------------------------------------- */

/*��װ����*/
void MULT_FRPWarpPreInitate(MULT_Handle *pHandle);//ϵͳ�ոճ�ʼ����ʱ����ã�����������Ļ��LED��
void MULT_FRPWarpPreInitateSetText(S32 Line, CHAR_T *pText);//����ʽ��ʼ��֮ǰ���������ı�
void MULT_FRPWarpMainInitate(MULT_Handle *pHandle);//ϵͳ��ʼ����ɣ�������������״̬
void MULT_FRPWarpAccess(MULT_Handle *pHandle, S32 Duration);
void MULT_FRPWarpAlarmClear(S32 ModuleInd, S32 Index);
void MULT_FRPWarpRebootShow(void);
void MULT_FRPWarpSetManagePortRelatedData(MULT_Handle *pHandle);
BOOL MULT_FRPWarpIsNewFRP(void);
void MULT_FRPWarpTerminate(void);

/*��ǰ���*/
void MULT_FRPPreConsoleIntiate(void);
void MULT_FRPPreConsoleSetText(S32 Line, CHAR_T *pLineText);
void MULT_FRPPreConsoleTerminate(void);

void MULT_FRPInitiate(MULT_Handle *pHandle);
void MULT_FRPAccess(MULT_Handle *pHandle, S32 Duration);
void MULT_FRPSetManagePortRelatedData(MULT_Handle *pHandle);
void MULT_FRPRebootShow(void);
void MULT_FRPClearAlarm(S32 Index);
void MULT_FRPTerminate(void);


/*��OLEDǰ���*/
void MULT_FRPCARDInitate(MULT_Handle *pHandle, S32 PlatFormType);
void MULT_FRPCARDAccess(MULT_Handle *pHandle, S32 Duration);
void MULT_FRPCARDAfterInitiate(MULT_Handle *pHandle);
void MULT_FRPCARDClearAlarm(S32 Index);
void MULT_FRPCARDRebootShow(void);
void MULT_FRPCARDTerminate(void);

#ifdef USE_FRP_PRE_CONSOLE
void MULT_FRPPreConsoleIntiate(void);
void MULT_FRPPreConsoleSetText(CHAR_T *pLineText, S32 Line);
void MULT_FRPPreConsoleTerminate(void);
#endif


BOOL MULTL_LoadMaintenaceFromStorage(MULT_Handle *pHandle, CHAR_T *pFilePathName);
void MULTL_SaveMaintenaceToStorage(MULT_Handle *pHandle);
void MULTL_GenerateServiceXML(MULT_Handle *pHandle, CHAR_T *pFilePathName);


void MULT_SetApplyBusyMark(MULT_Handle *pHandle, BOOL bActive);
void MULT_AnalyzeBatchStart(MULT_Handle *pHandle, S16 DevIndex, S16 TimeOutForPSIMs, S32 AnalyzeCharset, BOOL bTimeoutRemoveService);
void MULT_AnalyzeStart(MULT_Handle *pHandle, S16 DevIndex, S16 TsIndex, S16 TimeOutForPSIMs, S32 AnalyzeCharset, BOOL bTimeoutRemoveService, S32 AutoMapMode);
BOOL MULTL_GetTsMark(MULT_Handle *pHandle, S32 TsIndex, BOOL bInput);
void MULT_AnalyzeDone(MULT_Handle *pHandle);


void MULT_SNMPInitiate(MULT_Handle *pHandle);
S32 MULTL_SNMPCallbackFunc(HANDLE32 UserHandle, U32 ID, S32 ValueType, void *pValue, S32 ValueSize, BOOL bRead);
void MULT_SNMPTerminate(MULT_Handle *pHandle);
void MULT_SNMPAccess(MULT_Handle *pHandle, S32 Duration);

/*SYSLOG*/
#ifdef SUPPORT_SYSLOG_MODULE
void MULT_SyslogApply(MULT_Handle *pHandle, BOOL bClose);
void MULT_SyslogClean(MULT_Handle *pHandle);
S32 MULT_SyslogGet(MULT_Handle *pHandle, CHAR_T *pXMLBuf, S32 BufSize);
#endif

#ifdef SUPPORT_SYSLOG_MODULE
void MULTL_XMLLoadSyslog(MULT_Handle *pHandle, mxml_node_t *pXMLHolder);
#endif


#ifdef MULT_ENABLE_ECA_AND_SERVICE_LIST
void MULTL_XMLLoadECA(MULT_Handle *pHandle, mxml_node_t *pXMLRoot, BOOL bPost);
void MULTL_XMLSaveECA(MULT_Handle *pHandle, mxml_node_t *pXMLRoot);

void MULTL_XMLLoadSERVL(MULT_Handle *pHandle, mxml_node_t *pXMLRoot, BOOL bPost);
void MULTL_XMLSaveSERVL(MULT_Handle *pHandle, mxml_node_t *pXMLRoot);
#endif


#ifdef GN2000

#define MULT_ENCODER_IN_TS_IND 16
#define MULT_REAL_OUT_TS_IND 0
#define MULT_DECODER_OUT_TS_IND 16
void MULTI_MOSIACAdjustReMuxParameter(MULT_Handle *pHandle);
void MULTI_MOSIACInitiate(void);
void MULTL_MOSIACSaveXML(mxml_node_t *pXMLParent);
void MULTL_MOSIACLoadXML(mxml_node_t *pXMLParent);
void MULTI_MOSIACTerminate(void);


#endif

#ifdef MULT_SUPPORT_FPGA_ETH

#include "tun_module.h"

void MULT_FPGAEthInitiate(U32 ChipID);
void MULT_FPGAEthApply(BOOL bOpen);
void MULT_FPGAEthTerminate(void);
TUN_InitParam *MULT_FPGAEthGetParameterPtr(void);

#endif

#ifdef MULT_TS_BACKUP_SUPPORT

typedef enum  
{
	MULT_BP_TS_BACKUP_MODE_OFF,
	MULT_BP_TS_BACKUP_MODE_MAIN,
	MULT_BP_TS_BACKUP_MODE_BACKUP,
	MULT_BP_TS_BACKUP_MODE_AUTO_REPEAT,
	MULT_BP_TS_BACKUP_MODE_AUTO_ONCE,
	MULT_BP_TS_BACKUP_MODE_REMOTE,
	MULT_BP_TS_BACKUP_MODE_NUM
}MULT_BPTsBackupMode;


typedef struct
{
	MULT_BPTsBackupMode		m_Mode;
	S32						m_BackupTsInd;
	U32						m_LowLimit;
	U32						m_HighLimit;
}MULT_BPTsInBackupInfo;

typedef struct
{
	MULT_BPTsBackupMode		m_Mode;
	S32						m_BackupTsInd;
}MULT_BPTsOutBackupInfo;

void MULT_BPInitiate(MULT_Handle *pHandle, S32 InTsMax, S32 OutTsMax);
S32 MULT_BPGetTsBackupTs(S32 TsInd, BOOL bInput);
void MULT_BPSetInputTsBitrate(S32 TsInd, U32 Bitrate);
void MULT_BPSetOutputRemoteControl(S32 TsInd, BOOL bMain);
BOOL MULT_BPCheckApplyParameters(void);//�Զ��ָ��������ȡ�ñ������ı����Զ��ظ�FALSE
BOOL MULT_BPGetTsIsUsedByBackup(S32 TsInd, BOOL bInput);//�鿴��TS�Ƿ��Ǳ���TS
CHAR_T* MULT_BPModeValueToStr(S32 Value);
S32 MULT_BPModeValueFromStr(CHAR_T* pString);
MULT_BPTsBackupMode MULT_BPGetBackupStatus(S32 TsInd, BOOL bInput);
BOOL MULT_BPProcInTsBackupInfo(S32 TsInd, MULT_BPTsInBackupInfo *pInInfo, BOOL bRead);
BOOL MULT_BPProcOutTsBackupInfo(S32 TsInd, MULT_BPTsOutBackupInfo *pOutInfo, BOOL bRead);


void MULT_BPSaveStatusXMLToBuf(CHAR_T *pBuf, S32 BufSize);
void MULT_BPLoadXML(mxml_node_t *pXMLRoot);
void MULT_BPSaveXML(mxml_node_t *pXMLRoot);
void MULT_BPTerminate(void);




#endif


#ifdef SUPPORT_SFN_MODULATOR

typedef struct  
{
	U32						m_SFNMaxDelay100NS;				//����ӳ�
	U32						m_SFNSIPPN;					//0��PN420��1��PN595��2��PN945
	U32						m_SFNSIPCarrier;			//0�����ز���1���ز�
	U32						m_SFNSIPConstellation;			//000 000:4QAM-NR, 001:4QAM, 010:16QAM, 011:32QAM, 100:64QAM, 101~111:����
	U32						m_SFNSIPCodeRate;			//0��0.4��1�� 0.6��2��0.8��
	U32						m_SFNSIPTI;					//0��240���ţ�1�� 720����
	U32						m_SFNSIPDoublePilot;		//0�����ӣ�1 �ӣ�
	U32						m_SFNSIPPN_Shift;			//0, ����ת��1 ��ת
}MULT_SFN_CMN_SIP;//���з����ͨ�ò���

typedef struct  
{
	U32						m_SFNAddr;					//SIP���㲥Ѱַ
	U32						m_SFNSIPIndviDelay;			//���������ӳ�
	S32						m_SFNSIPIndvFreqOffsetHz;	//���Ƶ��ƫ��(Hz)
	U16						m_SFNSIPIndvPower;			//�����������ʿ��Ƶ�λ0.1dBm
	BOOL					m_SFNSIPIndvPowerMark;		//�����������ʿ�����Ч����
}MULT_SFN_INDV_SIP;//����Ѱַ���������


typedef struct  
{

	/*���ò���*/
	BOOL					m_bUseSFN;//�Ƿ���뵥Ƶ��ģʽ

	S32						m_SFNASIMode;

	//S32						m_CurrentInputTsInd;

	U8						m_bUse10MClkSynSrc;//0�ڲ���1�ⲿ��2��1PPS
	U8						m_Last10MCLKSyncSrc;
	U8						m_bUseEx1PPS;
	U32						m_SFNAddDelay100ns;//���ݼ������еĸ��ӵ�ʱ�䣨100ns����������SFN������֮��Ķ�����ʱ

	U16						m_SFNAddrID;//����ѰַID��
	BOOL					m_bUseIndvSIP;//�Ƿ�ʹ�ö���Ѱַ����
	BOOL					m_bUseCMNSIP;//�Ƿ�ʹ��SIP�еĵ��Ʒ�ʽ��

	BOOL					m_bDeleteSIP;//�Ƿ��Ƴ�SIP��


	/*����SFN����*/
	BOOL					m_bEnableSatSFN;//�򿪴˹��ܵ�ʱ����ҪMPEG2�Ŀհ���������������Ĳ���ִ��
	U16						m_SatSFNNullPacketPID;//��Ҫ�����PID�滻��MPEG2�Ŀհ�
	BOOL					m_bSatSFNSIPCRC32Check;//�򿪴˹��ܵ�ʱ����SIP����CRC32����У��

	/*MUTE*/
	BOOL					m_bTsLostMUTE;
	BOOL					m_bREFLostMUTE;
	BOOL					m_b1PPSLostMUTE;
	BOOL					m_bSIPLostMUTE;


}MULT_SFN_Param;

typedef struct  
{
	/*��Ƶ��������״̬*/
	MULT_SFN_CMN_SIP		m_SFNCMNSIP;//��ǰ�ĵ�Ƶ��ͨ�ò����������ܵ���SIP���������ʱ���������趨�������Ĳ���
	MULT_SFN_INDV_SIP		m_SFNIndvSIP;//��ǰ��Ƶ���Ķ�������

	/*��Ƶ�����������Լ�����ʱ��*/
	S32						m_SFNSIPRecvCount;
	BOOL					m_SFNSIPINDVUpdated;


	BOOL					m_bInt1PPSStatus;//����1PPS״̬
	S32						m_AlarmInt1PPSLostCount;
	BOOL					m_bExt1PPSStatus;//�ⲿ1PPS״̬
	S32						m_AlarmExt1PPSLostCount;
	BOOL					m_bInt10MStatus;//����10M״̬
	S32						m_AlarmInt10MLostCount;
	BOOL					m_bExt10MStatus;//�ⲿ10M״̬
	S32						m_AlarmExt10MLostCount;
	BOOL					m_bTS0Status;
	BOOL					m_bTS1Status;
	S32						m_AlarmTSXLostCount;
	S32						m_CurrentUseTsInd;
	S32						m_AlarmSIPLostCount;
	S32						m_AlarmBitrateErrorCount;
	S32						m_AlarmSIPChangeCount;
	S32						m_AlarmSIPCRC32ErrCount;

	U32						m_NetDelay100ns;

	S32					m_1PPSLosted;
	S32					m_InputLosted;
	/*10M�Ĵ���ָ��̸�ʱ��ģ���Լ�����*/
	BOOL					m_SIPLosted;


	BOOL					m_RFMuted;//��¼��ǰ����Ƿ�MUTE

}MULT_SFN_Status;


/*ģ����Ҫ����*/
void MULT_SFNInitiate(MULT_Handle *pHandle);
void MULT_SFNXMLLoad(mxml_node_t *pXMLRoot, BOOL bPost);
void MULT_SFNXMLSave(mxml_node_t *pXMLRoot, BOOL bStat);
void MULT_SFNApplyParameter(void);
void MULT_SFNApplySIP(MULT_SubModulatorInfo *pSubModInfo);
void MULT_SFNMonitorProcess(S32 Duration);
void MULT_SFNPauseSIPRecv(void);
void MULT_SFNResumeSIPRecv(void);
void MULT_SFNSetInternalCOMData(U8 *pData, S32 DataSize);
void MULT_SFNTerminate(void);

F64 MULTL_SFNGetDTMBBitrate(MULT_SFN_CMN_SIP *pCMNSIP);
void MULTL_SFNSIPParamChange(MULT_SubModulatorInfo *pSubModInfo, MULT_SFN_CMN_SIP *pSFNSISIP, BOOL bToSFNParam);


void MULT_SFNProcSFNParam(MULT_SFN_Param *pParam, BOOL bRead);
void MULT_SFNGetSFNStatus(MULT_SFN_Status *pStatus);
void MULT_SFNApplyByQAMModule(void);

/*״̬��ȡ����*/
BOOL MULT_SFNCheckEnabled(void);
BOOL MULT_SFNCheckSIPRecved(void);
void MULT_SFNForceCMDSIPUpdate(void);//���ô˺������ڵ�Ƶ���£��ᴥ��SIP���£����Զ���������RF������
typedef enum
{
	SFN_ERROR_SIP_LOST = 0,
	SFN_ERROR_INT_10M_LOST,
	SFN_ERROR_EXT_10M_LOST,
	SFN_ERROR_INT_1PPS_LOST,
	SFN_ERROR_EXT_1PPS_LOST,
	SFN_ERROR_ASI_LOST,
	SFN_ERROR_SIP_CRC32_ERR,
	SFN_ERROR_SIP_CHANGE,
	SFN_ERROR_BITRATE_ERROR,
	SFN_ERROR_NUM,
}MULT_SFNErrType;

BOOL MULT_SFNCheckSFNError(S32 ErrorType, BOOL bClear);

#endif


#ifdef SUPPORT_SFN_ADAPTER
#define SFN_MAX_INDV_NUM			(32)

typedef enum
{
	SFNA_ERROR_INT_10M_LOST = 0,
	SFNA_ERROR_EXT_10M_LOST,
	SFNA_ERROR_INT_1PPS_LOST,
	SFNA_ERROR_EXT_1PPS_LOST,
	SFNA_ERROR_ASI_LOST,
	SFNA_ERROR_NUM,
}MULT_SFNAErrType;

typedef struct  
{
	U32						m_SFNSIPPN;					//0��PN420��1��PN595��2��PN945
	U32						m_SFNSIPCarrier;			//0�����ز���1���ز�
	U32						m_SFNSIPConstellation;		//000 000:4QAM-NR, 001:4QAM, 010:16QAM, 011:32QAM, 100:64QAM, 101~111:����
	U32						m_SFNSIPCodeRate;			//0��0.4��1�� 0.6��2��0.8��
	U32						m_SFNSIPTI;					//0��240���ţ�1�� 720����
	U32						m_SFNSIPDoublePilot;		//0�����ӣ�1 �ӣ�
	U32						m_SFNSIPPN_Shift;			//0, ����ת��1 ��ת
}MULT_SFNA_CMN_SIP;//���з����ͨ�ò���

typedef struct  
{
	U32						m_SFNAddr;					//SIP���㲥Ѱַ
	U32						m_SFNSIPIndviDelay;			//���������ӳ�
	S32						m_SFNSIPIndvFreqOffsetHz;	//���Ƶ��ƫ��(Hz)
	U16						m_SFNSIPIndvPower;			//�����������ʿ��Ƶ�λ0.1dBm
	BOOL					m_SFNSIPIndvPowerMark;		//�����������ʿ�����Ч����
}MULT_SFNA_INDV_SIP;//����Ѱַ���������

typedef struct  
{
	MULT_SFNA_INDV_SIP		m_Info;
	BOOL					m_ActiveMark;
}MULT_SFNA_INDV_SIP_NODE;//����Ѱַ

typedef struct  
{

	/*���ò���*/
	S32						m_SFNASIMode;/*1=ASI1 2=ASI2 3=AUTO*/

	U8						m_bUse10MClkSynSrc;
	U8						m_bUseEx1PPS;
	U32						m_SFNMaxDelay100NS;			//����ӳ�

	/*��Ƶ������������*/
	TD_DTMBParameter		m_DTMBParam;//��Ƶ����������

	MULT_SFNA_INDV_SIP_NODE	m_pINDVArrap[SFN_MAX_INDV_NUM];

	/*����SFN����*/
	BOOL					m_bEnableSatSFN;//�򿪴˹��ܵ�ʱ����ҪMPEG2�Ŀհ���������������Ĳ���ִ��
	U16						m_SatSFNNullPacketPID;//��Ҫ�����PID�滻��MPEG2�Ŀհ�
}MULT_SFNA_Param;

typedef struct  
{
	BOOL					m_bInt1PPSStatus;//����1PPS״̬
	S32						m_AlarmInt1PPSLostCount;
	BOOL					m_bExt1PPSStatus;//�ⲿ1PPS״̬
	S32						m_AlarmExt1PPSLostCount;
	BOOL					m_bInt10MStatus;//����10M״̬
	S32						m_AlarmInt10MLostCount;
	BOOL					m_bExt10MStatus;//�ⲿ10M״̬
	S32						m_AlarmExt10MLostCount;
	BOOL					m_bTS0Status;
	BOOL					m_bTS1Status;
	S32						m_AlarmTSXLostCount;

	S32						m_CurrentUseTsInd;

}MULT_SFNA_Status;


void MULT_SFNAInitiate(MULT_Handle *pHandle);
void MULT_SFNAXMLLoad(mxml_node_t *pXMLRoot, BOOL bPost);
void MULT_SFNAXMLSave(mxml_node_t *pXMLRoot, BOOL bStat);
void MULT_SFNAApplyParameter(void);
void MULT_SFNAMonitorProcess(S32 Duration);
BOOL MULT_SFNACheckSFNError(S32 ErrorType, BOOL bClear);
void MULT_SFNASetInternalCOMData(U8 *pData, S32 DataSize);
void MULT_SFNATerminate(void);

void MULT_SFNAProcSFNParam(MULT_SFNA_Param *pParam, BOOL bRead);
void MULT_SFNAGetSFNStatus(MULT_SFNA_Status *pStatus);

#endif




#ifdef SUPPORT_GNS_MODULE
/*ģ����Ҫ����*/
void MULT_GNSInitiate(void);
void MULT_GNSXMLLoad(MULT_Handle *pHandle, mxml_node_t *pXMLRoot, BOOL bPost);
void MULT_GNSXMLSave(MULT_Handle *pHandle, mxml_node_t *pXMLRoot, BOOL bStat);
void MULT_GNSApplyParameter(void);
void MULT_GNSMonitorProcess(S32 Duration);
void MULT_GNSTerminate(void);
/*״̬��ȡ����*/
BOOL MULT_GNSCheckEnabled(void);
BOOL MULT_GNSCheckLocked(void);
BOOL MULT_GNSCheckError(void);
BOOL MULT_GNSCheckHaveGNS(void);
#endif


#ifdef SUPPORT_IP_O_TS_MODULE
void MULT_IPoTSInitiate(void);
void MULT_IPoTSXMLLoad(MULT_Handle *pHandle, mxml_node_t *pXMLRoot, BOOL bPost);
void MULT_IPoTSXMLSave(MULT_Handle *pHandle, mxml_node_t *pXMLRoot, BOOL bStatistics);
void MULT_IPoTSMonitorProcess(S32 Duration);
void MULT_IPoTSApply(void);
void MULT_IPoTSTerminate(void);
#endif

#ifdef SUPPORT_CLK_ADJ_MODULE
BOOL MULT_CLKInitiate(MULT_Handle *pMainHandle);
void MULT_CLKResetByModulator(void);
void MULT_CLKSetDefaultDAValue(U32 DAValue);
U32 MULT_CLKGetDefaultDAValue(void);
void MULT_CLKSetTranningValue(F64 TrainningValue);
F64 MULT_CLKGetTranningValue(void);
BOOL MULT_CLKGet10MLockStatus(void);
void MULT_CLKSet10MCLKSYNCSyc(S32 NewSRC);
S32 MULT_CLKGetCurSumOffset(void);
BOOL MULT_CLKProtocolParser(U8 *pData, S32 DataSize);
BOOL MULT_CLKGetSyncError(BOOL bClear);
void MULT_CLKTerminate(void);
void MULTL_CLKProtocolPacker();
#endif

#ifdef SUPPORT_FGPIO_MODULE
#define FPGA_GPIO_DPD_CONTROL_BIT					(0x00000001)
BOOL FGPIO_Initiate(void);
BOOL FGPIO_ProtocolParser(U8 *pData, S32 DataSize);
void FGPIO_ValueSet(S32 PortInd, U32 GPIOMask, U8 bHigh);
void FGPIO_IOMaskSet(S32 PortInd, U32 GPIOMask, U8 bInput);
void FGPIO_Write(S32 PortInd);
BOOL FGPIO_Read(S32 PortInd, U32 *pValue, S32 Timeout);
void FGPIO_Terminate(void);
#endif


#ifdef GM2730S
BOOL MULT_GetOutChannelState(S16 TsIndex);
#endif





#ifdef SUPPORT_NEW_FRP_MENU_MODULE
void MULT_FRPMenuADVInitiate(MULT_Handle *pHandle);
void MULT_FRPMenuADVShowInitateProgressString(CHAR_T *pString);
void MULT_FRPMenuADVShowRebootProgressString(void);
void MULT_FRPMenuADVStart(MULT_Handle *pHandle);
void MULT_FRPMenuADVDestroy(void);
#endif


void MULT_SaveCLKADJModuleParameters(MULT_Handle *pHandle);
void MULT_ResetCLKADJModuleParameters(MULT_Handle *pHandle);


/*SCS�ӿ�*/
void MULTL_SCSManagerInitiate(MULT_Handle *pHandle, HWL_HWInfo* pHWInfo);
void MULTL_SCSManagerApply(MULT_Handle *pHandle, BOOL bClose);
void MULTL_SCSManagerDestroy(MULT_Handle *pHandle);

/*��Ȩ�ӿ�*/
void MULTL_GenerateAuth(MULT_Handle *pHandle);
BOOL MULTL_LoadLicense(MULT_Handle *pHandle, CHAR_T *pPathname, BOOL bCheckValid, CHAR_T* pInformation, S32 BufSize);
#if defined(GN1846) || defined(GN1866)  || defined(GN1866B)
U32 AUTHL_Fpga_ID();
void AUTHL_Test();
#endif
/*SNTPģ��*/
void MULTL_SNTPRequestThread(void *pParam);


/*��ʽ�豸�ӿ�*/
#ifdef ENCODER_CARD_PLATFORM
void MULT_CARDModuleInitiate(MULT_Handle *pHandle, BOOL bSubFlashUpgradeMode);
BOOL MULT_CARDModuleICPProsessRecv(U8 *pData, S32 DataSize);
void MULT_CARDModuleXMLGetProcess(MULT_Handle *pHandle, mxml_node_t* pXML);
void MULT_CARDModuleXMLPostProcess(MULT_Handle *pHandle, mxml_node_t* pXML);
BOOL MULT_CARDModuleIsBusy(void);
void MULT_CARDModuleTerminate(void);
#endif


#ifdef KERNEL_AUTO_UPDATE
void MULT_KernelAutoUpdate(void);
#endif



BOOL PES_Initiate(HANDLE32 DBSHandle);
void PES_Terminate(void);
void PES_XMLAcess(HANDLE32 XMLLoad, HANDLE32 XMLSave);


/*�̼�����ϵͳ*/
typedef enum
{
	MULT_SUB_FLASH_OP_TYPE_VALIDATION_GET = 0,
	MULT_SUB_FLASH_OP_TYPE_CONFIRM,
	MULT_SUB_FLASH_OP_TYPE_UPGRADE_MODE_SWITCH,
	MULT_SUB_FLASH_OP_TYPE_NUM
}MULT_SUB_FLASH_OP_TYPE;

typedef struct  
{
	UPG_MODULEInfo	m_UPGInfo;
	U8		*m_pDataPtr;
	S32		m_DataSize;
	U32		m_DataCRC32;
}MULT_SUB_FLASH_INFO;


BOOL MULT_SubFlashXMLProcess(MULT_Handle *pHandle, mxml_node_t* pBodyNode, CHAR_T *pParameterOrSubType, S32 OPType);
BOOL MULT_SubFlashGetFlashInfo(MULT_SUB_FLASH_INFO *pInfo);
void MULT_SubFlashTestInit(void);

#endif



