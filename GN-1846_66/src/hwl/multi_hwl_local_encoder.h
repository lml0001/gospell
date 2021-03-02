#ifndef _MULTI_HWL_LOCAL_ENCODER_H_
#define _MULTI_HWL_LOCAL_ENCODER_H_

/* Includes-------------------------------------------------------------------- */
#include "global_def.h"
#include "mpeg2_micro.h"
#if defined(GN1846)
#include "encoder_3531A.h"
#elif defined(GN1866)  || defined(GN1866B)
#include "encoder_3519.h"
#endif
/* Macro ---------------------------------------------------------------------- */
/* Types Define (struct typedef)----------------------------------------------- */

typedef void (*HWL_LENCODER_CB)(void *pUserParam);

#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
#define LOCAL_IP_ADDRESS 0x7F000001 /* "127.0.0.1" */
#define LOCAL_IP_PORT 0x1000
#endif

typedef struct
{
	/*?????????*/
	S32					m_ChnNum; /* ?????????? */
	S32					m_SubNumPerCHN; /* ??????????????????????? */
}HWL_LENCODER_InitParam;


typedef struct
{
	BOOL					m_bEnable;
	U32						m_UDPAddr; /* ?????????????????? */
	S32						m_UDPPort;

#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
	ENC_3531AParam			m_EncParam;
#endif
}HWL_LENCODER_SubParam;


typedef struct
{
	BOOL					m_bEnable;
	U32						m_IPAddr;
	U32						m_Mask;
	U32						m_Gate;
}HWL_LENCODER_ChnParam;


typedef struct
{
	HWL_LENCODER_ChnParam	m_ChnParam;
	HWL_LENCODER_SubParam	*m_pSubCHN;
	HANDLE32				m_SocketHandleNew;
	HANDLE32				m_EncoderHandle;
	HANDLE32				m_Lock;
}HWL_LENCODER_Chn;

#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
typedef struct {
	ENC_3531AStatus m_Status;
} HWL_LENCODER_Status;

typedef struct {
	ENC_3531AAlarmInfo m_AlarmInfo;
} HWL_LENCODER_AlarmInfo;
#endif

/* Functions prototypes ------------------------------------------------------- */
BOOL HWL_ENCODER_Initiate(HWL_LENCODER_InitParam *pParam);
void HWL_ENCODER_ReSetChnIPParam(S32 ChnInd);
void HWL_ENCODER_SetChnIPParam(S32 ChnInd, HWL_LENCODER_ChnParam *pParam);
S32 HWL_ENCODER_GetSubIPProtocol(S32 ChnInd, S32 SubInd);
void HWL_ENCODER_SetSubIPParam(S32 ChnInd, S32 SubInd, U32 DestIPv4, U16 DestPort, S32 Protocol);
void HWL_ENCODER_StopChn(S32 ChnInd);
void HWL_ENCODER_SetSubEncParam(S32 ChnInd, S32 SubInd, HWL_LENCODER_SubParam *pParam);
BOOL HWL_ENCODER_SetTsPacket(S32 ChnInd, S32 SubInd, U8 *pData, S32 DataSize);
S32 HWL_ENCODER_GetTsBitrate(S32 ChnInd, S32 TsInd);
void HWL_ENCODER_Terminate(void);
#if defined(GN1846) || defined(GN1866)  || defined(GN1866B)
BOOL HWL_ENCODER_GetStatus(S32 ChnInd, S32 SubInd, HWL_LENCODER_Status *pStatus);
BOOL HWL_ENCODER_GetAlarmInfo(S32 ChnInd, S32 SubInd, HWL_LENCODER_AlarmInfo *pAlarmInfo);
BOOL HWL_ENCODER_ResetAlarmInfo(S32 ChnInd, S32 SubInd, HWL_LENCODER_AlarmInfo *pAlarmInfo);
#endif

#endif
/*EOF*/
