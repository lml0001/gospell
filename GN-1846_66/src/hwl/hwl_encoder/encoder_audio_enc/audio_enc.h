#ifndef _AUDIO_ENC_H_
#define _AUDIO_ENC_H_
/* Includes-------------------------------------------------------------------- */
#include "global_def.h"
#include "multi_private.h"
#include "hi_comm_aio.h"
#if defined(GN1846)
#include "encoder_3531A.h"
#elif defined(GN1866)  || defined(GN1866B)
#include "encoder_3519.h"
#endif

/* Macro ---------------------------------------------------------------------- */


/* Types Define (struct typedef)----------------------------------------------- */
typedef struct {
	ENC_3531AAudEncMode	m_AudEncMode;
	AUDIO_SAMPLE_RATE_E	m_AudSample; /* ��λ Hz */
	U32					m_AudBitrate; /* ��λ Kbps */
} AUD_EncInitParam;

/* Functions prototypes ------------------------------------------------------- */
HANDLE32 AUD_EncCreate(AUD_EncInitParam *pInitParam);
BOOL AUD_EncProcess(HANDLE32 Handle, U8 *pData, S32 DataLen, U8 **pBuf, S32 *pActLen);
void AUD_EncDestroy(HANDLE32 Handle);
/* ���� AAC ��Ҫ����̶���С������buffer����Ҫ��ȡ��Ҫ�Ĵ�С */
S32 AUD_EncAacGetInputBufSize(HANDLE32 Handle);

#endif
/*EOF*/
