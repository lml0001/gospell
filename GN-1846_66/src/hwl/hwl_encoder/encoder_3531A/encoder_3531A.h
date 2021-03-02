#ifndef _ENCODER_3531A_H_
#define _ENCODER_3531A_H_
/* Includes-------------------------------------------------------------------- */
#include "global_def.h"
#include "ts.h"
#include "multi_private.h"
#include "mpeg2_micro.h"
#include "mpeg2.h"
#include "../hwl_hdmi_recv_adv7612/hdmi_rx.h"

/* Macro ---------------------------------------------------------------------- */
typedef enum {
	VI_MODE_PAL,
	VI_MODE_NTSC,
	VI_MODE_720P_50,
	VI_MODE_720P_5994,
	VI_MODE_720P_60,
	VI_MODE_1080I_50,
	VI_MODE_1080I_5994,
	VI_MODE_1080I_60,
	VI_MODE_1080P_50,
	VI_MODE_1080P_5994,
	VI_MODE_1080P_60,
	VI_MODE_UHD4KP_30,
	VI_MODE_AUTO,
	VI_MODE_UNKNOWN,
	VI_MODE_NUM
} ENC_3531AViMode;

typedef enum {
	VO_MODE_576P_25,
	VO_MODE_480P_2997,
	VO_MODE_720P_25,
	VO_MODE_720P_2997,
	VO_MODE_720P_30,
	VO_MODE_720P_50,
	VO_MODE_720P_5994,
	VO_MODE_720P_60,
	VO_MODE_1080P_25,
	VO_MODE_1080P_2997,
	VO_MODE_1080P_30,
	VO_MODE_1080P_50,
	VO_MODE_1080P_5994,
	VO_MODE_1080P_60,
	VO_MODE_AUTO,
	VO_MODE_NUM
} ENC_3531AVoMode;
typedef enum {
	RC_MODE_CBR = 0,
	RC_MODE_VBR,
	RC_MODE_FIXQP,
	RC_MODE_NUM
} ENC_3531ARcMode;

typedef enum {
	PROFILE_BASE = 0,
	PROFILE_MP,
	PROFILE_HP,
	PROFILE_SVC_T,
	PROFILE_NUM
} ENC_3531AProfile;


/* Types Define (struct typedef)----------------------------------------------- */
typedef struct {
	U32	m_ChnNum;//��Ŀ����!

	/* call back functions */
#if defined(INDEPENDENT_DATA_IP_SUPPORT)
#else
	BOOL (*m_GetEthSetFlagCB)(void);
#endif
} ENC_3531AInitParam;

typedef struct {
	U32	m_Gop; /* ��Χ 1-65536 */
	U32	m_BitRate; /* ��λ��kbps */
} ENC_3531ACbrParam;

typedef struct {
	U32	m_Gop;
	U32	m_MinQp; /* 0-51 Ĭ�� 16 */
	U32	m_MaxQp; /* MinQp-51 Ĭ�� 51 */
	U32	m_MaxBitRate; /* ��λ��kbps */
} ENC_3531AVbrParam;

typedef struct {
	U32	m_Gop;
	U32	m_IQp; /* 0-51 */
	U32	m_PQp; /* 0-51 */
} ENC_3531AFixqpParam;

typedef enum
{
	AUD_ENC_MODE_MPEG1_L2 = 0,
	AUD_ENC_MODE_LC_AAC,
	AUD_ENC_MODE_AC3,
	AUD_ENC_MODE_EAC3,
	AUD_ENC_MODE_NUM
} ENC_3531AAudEncMode;

typedef enum
{
	AUD_BR_16K = 0, 
	AUD_BR_64K,
	AUD_BR_96K, 
	AUD_BR_112K,
	AUD_BR_128K,
	AUD_BR_160K,
	AUD_BR_192K,
	AUD_BR_224K,
	AUD_BR_256K,
	AUD_BR_320K,
	AUD_BR_384K,
	AUD_BR_NUM
} ENC_3531AAudBitrate;

typedef enum
{
	AUD_SAMP_32K = 0, 
	AUD_SAMP_44_1K,
	AUD_SAMP_48K,
	AUD_SAMP_NUM
} ENC_3531AAudSample;

typedef enum
{
	IP_OUT_TYPE_SPTS = 0,
	IP_OUT_TYPE_MPTS,
	IP_OUT_TYPE_NUM
} ENC_3531AIpOutType;

typedef struct {
	U32										m_x_coordinate;
	U32										m_y_coordinate;
	U32 										m_width;
	U32 										m_height;
	U32 										m_blk;
	BOOL 									m_enable;
}ENC_3531AMosaic;

typedef struct {
	BOOL				m_ActiveMark; /* �Ƿ�����TRUE������ FALSE�������� */
	BOOL				m_SignalIsLocked; /* �ź��Ƿ����� */

	S32					m_IpProto; /* ����Э�� */
	U32					m_DestIp;
	U16					m_DestPort;
	S32					m_SendTsNum; /* ���� Ts ������ÿ�� sendto �������Ͱ����� TS ����������Χ 1 ~ 7 */
	U32					m_ProgBitrate;
	U8					m_pProgName[MPEG2_DB_MAX_SERVICE_NAME_BUF_LEN];
	MPEG2_PSICharSet	m_PsiCharSet;
	U16					m_ProgNumber;
	ENC_3531AIpOutType	m_IpOutputType; /* MPTS/SPTS */

	U16					m_VidPid;
	U16					m_PcrPid;
	U16					m_AudPid;
	U16					m_PmtPid;

#if defined(SUPPORT_WATER_MARK)
	U32					m_x_coordinate;
	U32					m_y_coordinate;
	U32 				m_transparent;
	BOOL 				m_enable; /*OSD Enable or Disable */
#endif

#if defined(SUPPORT_MOSIAC)
	ENC_3531AMosaic  	m_MosaicParam; /* Mosaic */
#endif


	ENC_3531AViMode		m_ViMode; /* ��Ƶ����ģʽ 1080P50/60 1080i60/50 720P50/60 */
	ENC_3531AVoMode		m_VoMode; /* ��Ƶ���ģʽ */
	ENC_3531AProfile	m_Profile; /* BP/MP/HP */
	
	ENC_3531ARcMode		m_RcMode; /* VBR/CBR/FIXQP */
	union {
		ENC_3531ACbrParam	m_CbrParam;
		ENC_3531AVbrParam	m_VbrParam;
		ENC_3531AFixqpParam	m_FixqpParam;
	} m_RcParam;	

	/* ��Ƶ���� */
	S32	 m_AudioGroup;
	ENC_3531AAudEncMode	m_AudEncMode;
	ENC_3531AAudBitrate	m_AudBitrate;
	ENC_3531AAudSample	m_AudSample;
	S32					m_AudVolume;

	/* �������� */
	S32				m_AudPtsRelativeDelayTime; /* ��Ƶ��� Pts ��ʱ����λ��us */
	S32				m_PtsDelayTime; /* ��λ��us */
	S32				m_MaxPtsPcrInterval; /* ��λ��us */
	S32				m_MinPtsPcrInterval; /* ��λ��us */
	S32				m_AudDelayFrameNum; /* ��Ƶ��ʱ֡������Χ 0 - 250 */
} ENC_3531AParam;

typedef struct {
	U32	m_TotalTsCount; /* ÿ������ TS ���� */
	U32	m_PidTsCount[MPEG2_TS_PACKET_MAX_PID_NUM]; /* ÿ��ÿ�� PID ��Ӧ�� TS ������ */
} ENC_3531AStatus;

typedef struct {
	U32 m_BufferOverFlowCount; /* Buffer ������� */
} ENC_3531AAlarmInfo;

/* Functions prototypes ------------------------------------------------------- */
HANDLE ENC_3531ACreate(ENC_3531AInitParam *pInitParam);
void ENC_3531ADestroy(HANDLE Handle);
BOOL ENC_3531ASetParam(HANDLE Handle, S32 ChnIndex, ENC_3531AParam *pParam);
BOOL ENC_3531AGetStatus(HANDLE Handle, S32 ChnIndex, ENC_3531AStatus *pStatus);
BOOL ENC_3531AGetAlarmInfo(HANDLE Handle, S32 ChnIndex, ENC_3531AAlarmInfo *pAlarmInfo);
/* ���ñ�����Ϣ��pAlarmInfo Ϊ NULL��ȫ�������������ã������ø�ֵȥ��λ�� */
BOOL ENC_3531AResetAlarmInfo(HANDLE Handle, S32 ChnIndex, ENC_3531AAlarmInfo *pAlarmInfo); 


HI_S32 SAMPLE_RGN_AddOsdToVenc(VENC_CHN lVencChn, SIZE_S region_size, HI_S32 s32X, HI_S32 s32Y, HI_S32 transparence, const char *filename, HI_BOOL enable);
int ENC_3531AGetCurrentFrame(VENC_CHN VencChn);
#endif
/*EOF*/
