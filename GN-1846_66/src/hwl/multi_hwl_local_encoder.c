
/* Includes-------------------------------------------------------------------- */
#include "global_micros.h"
#include "mpeg2_micro.h"
#include "libc_assist.h"
#include "crypto.h"
#include "platform_assist.h"
#include "multi_hwl_local_encoder.h"
#include "multi_hwl_igmp.h"
#include "multi_hwl.h"
#include "multi_hwl_internal.h"
#include "multi_hwl_monitor.h"
#include "multi_hwl_tags.h"
#include "ts.h"
#if defined(GN1846) || defined(GN1866)  || defined(GN1866B)
#include "multi_main_internal.h"
#endif

/* Global Variables (extern)--------------------------------------------------- */
/* Macro (local)--------------------------------------------------------------- */
#define MODULETASK_STATCK_SIZE					(1024*1024)

/* Private Constants ---------------------------------------------------------- */
/* Private Types -------------------------------------------------------------- */

typedef struct
{
	HWL_LENCODER_InitParam	m_InitParam;
	U8						m_bTaskMark;
	HANDLE32				m_ThreadHandle;

	HWL_LENCODER_Chn		*m_pChn;
}HWL_LENCODER_Handle;


/* Private Variables (static)-------------------------------------------------- */
static HWL_LENCODER_Handle *s_pHandle = NULL;
/* Private Function prototypes ------------------------------------------------ */
/* Functions ------------------------------------------------------------------ */
BOOL HWL_ENCODER_Initiate(HWL_LENCODER_InitParam *pParam)
{
	BOOL lRet = FALSE;

	if (s_pHandle == NULL)
	{
		HWL_LENCODER_Handle *plHandle = (HWL_LENCODER_Handle*) GLOBAL_ZMALLOC(sizeof(HWL_LENCODER_Handle));
		if (plHandle)
		{
			GLOBAL_MEMCPY(&plHandle->m_InitParam, pParam, sizeof(HWL_LENCODER_InitParam));
			plHandle->m_pChn = (HWL_LENCODER_Chn *)GLOBAL_ZMALLOC(plHandle->m_InitParam.m_ChnNum * sizeof(HWL_LENCODER_Chn));
			if (plHandle->m_pChn) 
			{
				S32 i, k;
				HWL_LENCODER_Chn *plChn;



				for (i = 0; i < plHandle->m_InitParam.m_ChnNum; i++) 
				{
					ENC_3531AInitParam lInitParam;

					lInitParam.m_ChnNum = plHandle->m_InitParam.m_SubNumPerCHN;
#if defined(INDEPENDENT_DATA_IP_SUPPORT)
#else
					lInitParam.m_GetEthSetFlagCB = MULTL_GetEthSetFlag;
#endif

					plChn = &plHandle->m_pChn[i];
					plChn->m_EncoderHandle = ENC_3531ACreate(&lInitParam);
					if (!plChn->m_EncoderHandle) 
					{
						GLOBAL_TRACE(("ENC_3531ACreate Failed!\n"));
						lRet = FALSE;
						break;
					}

					plChn->m_pSubCHN = (HWL_LENCODER_SubParam*)GLOBAL_ZMALLOC(sizeof(HWL_LENCODER_SubParam) * plHandle->m_InitParam.m_SubNumPerCHN);
					
					plChn->m_Lock = PAL_CriticalInitiate();


				}
				GLOBAL_TRACE(("Initiate Complete!!!!!!!!!\n"));
				lRet = TRUE;
			}
			else 
			{
				GLOBAL_TRACE(("CHN Malloc Failed!!!!!!!!!!\n"));
				GLOBAL_SAFEFREE(plHandle);
			}
		}
		s_pHandle = plHandle;
	}
	else
	{
		GLOBAL_TRACE(("Error!!!!!!!!!!!!!!!!!\n"));
	}

	return lRet;
}

void HWL_ENCODER_StopChn(S32 ChnInd)
{
	HWL_LENCODER_Handle *plHandle = s_pHandle;
	if (plHandle)
	{
		GLOBAL_TRACE(("Apply Encoder Chn Parameters ChnInd = %d\n", ChnInd));
		if (GLOBAL_CHECK_INDEX(ChnInd, plHandle->m_InitParam.m_ChnNum))
		{
			HWL_LENCODER_Chn *plChn = &plHandle->m_pChn[ChnInd];

			if (PAL_CriticalEnter(plChn->m_Lock, -1))
			{
				if (plChn->m_SocketHandleNew)
				{
					PFC_SocketClose(plChn->m_SocketHandleNew);
					plChn->m_SocketHandleNew = NULL;
				}

				PAL_CriticalLeave(plChn->m_Lock);
			}
		}
	}

}

void HWL_ENCODER_ReSetChnIPParam(S32 ChnInd)
{
	HWL_LENCODER_Handle *plHandle = s_pHandle;
	if (plHandle)
	{
		GLOBAL_TRACE(("Reset Encoder Chn IP Parameters ChnInd = %d\n", ChnInd));
		if (GLOBAL_CHECK_INDEX(ChnInd, plHandle->m_InitParam.m_ChnNum))
		{
			HWL_LENCODER_ChnParam *plParam;
			HWL_LENCODER_Chn *plChn = &plHandle->m_pChn[ChnInd];

			plParam = &plChn->m_ChnParam;
			/*进入锁*/
			if (PAL_CriticalEnter(plChn->m_Lock, -1))
			{
				/*设置数据IP*/
				{
					CHAR_T plCMD[1024];

					GLOBAL_SPRINTF((plCMD,"ifconfig eth0:0 %s netmask ", PFC_SocketNToA(plParam->m_IPAddr)));
					GLOBAL_STRCAT(plCMD, PFC_SocketNToA(plParam->m_Mask));
					GLOBAL_STRCAT(plCMD, "\r\n");

					GLOBAL_TRACE(("%s", plCMD));
					PFC_System(plCMD);

					//GLOBAL_SPRINTF((plCMD,"route add default gw %s dev eth0:0\r\n",  PFC_SocketNToA(plParam->m_Gate)));
					//GLOBAL_TRACE((plCMD));
					//PFC_System(plCMD);

				}


				if (plChn->m_SocketHandleNew)
				{
					PFC_SocketClose(plChn->m_SocketHandleNew);
					plChn->m_SocketHandleNew = NULL;
				}

				plChn->m_SocketHandleNew = PFC_SocketCreate(PFC_SOCKET_TYPE_UDP);
				if (plChn->m_SocketHandleNew)
				{
					if (PFC_SocketBind(plChn->m_SocketHandleNew, plParam->m_IPAddr, 0))
					{
						S32 lValue;
						lValue = 1;
						PFC_SocketOption(plChn->m_SocketHandleNew, PFC_SOCKET_OPTION_TYPE_NONBLOCK, &lValue,sizeof(lValue));

						lValue = 1;
						PFC_SocketOption(plChn->m_SocketHandleNew, PFC_SOCKET_OPTION_TYPE_NO_CHECK_SUM, &lValue,sizeof(lValue));

						lValue = 1;
						PFC_SocketOption(plChn->m_SocketHandleNew, PFC_SOCKET_OPTION_TYPE_ADDRREUSE, &lValue,sizeof(lValue));

						lValue = 4 * 1024 *1024;
						PFC_SocketOption(plChn->m_SocketHandleNew, PFC_SOCKET_OPTION_TYPE_SENDBUF, &lValue,sizeof(lValue));

						GLOBAL_TRACE(("Setup Socket = 0x%08X Complete!!!!!!!!!\n", plChn->m_SocketHandleNew));
					}
					else
					{
						GLOBAL_TRACE(("Bind Failed!!!!!!!!!!!\n"));
						PFC_SocketClose(plChn->m_SocketHandleNew);
						plChn->m_SocketHandleNew = NULL;
					}
				}
				else
				{
					GLOBAL_TRACE(("Create Socket Failed!!!!!!!!\n"));
				}


				PAL_CriticalLeave(plChn->m_Lock);
			}

		}
		else
		{
			GLOBAL_TRACE(("Overlimit !!!!\n"));
		}
	}
}



void HWL_ENCODER_SetChnIPParam(S32 ChnInd, HWL_LENCODER_ChnParam *pParam)
{
	HWL_LENCODER_Handle *plHandle = s_pHandle;
	if (plHandle)
	{
		GLOBAL_TRACE(("Apply Encoder Chn IP Parameters ChnInd = %d\n", ChnInd));
		if (GLOBAL_CHECK_INDEX(ChnInd, plHandle->m_InitParam.m_ChnNum))
		{
			HWL_LENCODER_Chn *plChn = &plHandle->m_pChn[ChnInd];

			GLOBAL_MEMCPY(&plChn->m_ChnParam, pParam, sizeof(HWL_LENCODER_ChnParam));
			
			HWL_ENCODER_ReSetChnIPParam(ChnInd);
					
		}
		else
		{
			GLOBAL_TRACE(("Chn Overlimit !!!!\n"));
		}
	}
}

S32 HWL_ENCODER_GetSubIPProtocol(S32 ChnInd, S32 SubInd)
{
	S32 lRet = 0;
	HWL_LENCODER_Handle *plHandle = s_pHandle;
	if (plHandle)
	{
		lRet = plHandle->m_pChn[ChnInd].m_pSubCHN[SubInd].m_EncParam.m_IpProto;
	}
	return lRet;
}


void HWL_ENCODER_SetSubIPParam(S32 ChnInd, S32 SubInd, U32 DestIPv4, U16 DestPort, S32 Protocol)
{
	HWL_LENCODER_Handle *plHandle = s_pHandle;
	if (plHandle)
	{
		GLOBAL_TRACE(("HWL_ENCODER_SetSubEncParam ChnInd = %d, SubInd = %d\n", ChnInd, SubInd));
		if (GLOBAL_CHECK_INDEX(ChnInd, plHandle->m_InitParam.m_ChnNum))
		{
			HWL_LENCODER_Chn *plChn = &plHandle->m_pChn[ChnInd];
			if (GLOBAL_CHECK_INDEX(SubInd, plHandle->m_InitParam.m_SubNumPerCHN))
			{
				plChn->m_pSubCHN[SubInd].m_UDPAddr = DestIPv4;
				plChn->m_pSubCHN[SubInd].m_UDPPort = DestPort;
				plChn->m_pSubCHN[SubInd].m_EncParam.m_IpProto = Protocol;
				GLOBAL_TRACE(("IPAddr = 0x%08X Port = %d, Protocol = %d\n", plChn->m_pSubCHN[SubInd].m_UDPAddr, plChn->m_pSubCHN[SubInd].m_UDPPort, plChn->m_pSubCHN[SubInd].m_EncParam.m_IpProto));
			}
		}
		else
		{
			GLOBAL_TRACE(("Overlimit !!!!\n"));
		}
	}
}


void HWL_ENCODER_SetSubEncParam(S32 ChnInd, S32 SubInd, HWL_LENCODER_SubParam *pParam)
{
	HWL_LENCODER_Handle *plHandle = s_pHandle;
	if (plHandle)
	{
		GLOBAL_TRACE(("HWL_ENCODER_SetSubEncParam ChnInd = %d, SubInd = %d\n", ChnInd, SubInd));
		if (GLOBAL_CHECK_INDEX(ChnInd, plHandle->m_InitParam.m_ChnNum))
		{
			HWL_LENCODER_Chn *plChn = &plHandle->m_pChn[ChnInd];
			if (GLOBAL_CHECK_INDEX(SubInd, plHandle->m_InitParam.m_SubNumPerCHN))
			{
				GLOBAL_MEMCPY(&plChn->m_pSubCHN[SubInd].m_EncParam, &pParam->m_EncParam, sizeof(ENC_3531AParam));
				ENC_3531ASetParam(plChn->m_EncoderHandle, SubInd, &pParam->m_EncParam);
			}
		}
		else
		{
			GLOBAL_TRACE(("Overlimit !!!!\n"));
		}
	}
}

BOOL HWL_ENCODER_SetTsPacket(S32 ChnInd, S32 SubInd, U8 *pData, S32 DataSize)
{
	HWL_LENCODER_Handle *plHandle = s_pHandle;
	if (plHandle)
	{
		if (GLOBAL_CHECK_INDEX(ChnInd, plHandle->m_InitParam.m_ChnNum))
		{
			HWL_LENCODER_Chn *plChn = &plHandle->m_pChn[ChnInd];
			if (GLOBAL_CHECK_INDEX(SubInd, plHandle->m_InitParam.m_SubNumPerCHN))
			{
				HWL_LENCODER_SubParam *plSub = &plChn->m_pSubCHN[SubInd];
				if (PAL_CriticalEnter(plChn->m_Lock, 0))
				{
					if (plChn->m_SocketHandleNew)
					{
						PFC_SocketSendTo(plChn->m_SocketHandleNew, pData, DataSize, NULL, plSub->m_UDPAddr, plSub->m_UDPPort);
					}

					PAL_CriticalLeave(plChn->m_Lock);
				}
			}
		}
		else
		{
			GLOBAL_TRACE(("Overlimit !!!!\n"));
		}
	}
}

S32 HWL_ENCODER_GetTsBitrate(S32 ChnInd, S32 TsInd)
{
	HWL_LENCODER_Handle *plHandle = s_pHandle;
	if (plHandle)
	{
		if (GLOBAL_CHECK_INDEX(ChnInd, plHandle->m_InitParam.m_ChnNum))
		{
			HWL_LENCODER_Status lStatus;
			ENC_3531AGetStatus(plHandle->m_pChn[ChnInd].m_EncoderHandle, TsInd, &lStatus.m_Status);

			return lStatus.m_Status.m_TotalTsCount * MPEG2_TS_PACKET_SIZE * 8;
		}
		else
		{
			GLOBAL_TRACE(("Overlimit !!!!\n"));
		}
	}
	return 0;
}

BOOL HWL_ENCODER_GetStatus(S32 ChnInd, S32 SubInd, HWL_LENCODER_Status *pStatus)
{
	HWL_LENCODER_Handle *plHandle = s_pHandle;
	if (plHandle)
	{
		if (GLOBAL_CHECK_INDEX(ChnInd, plHandle->m_InitParam.m_ChnNum))
		{
			return ENC_3531AGetStatus(plHandle->m_pChn[ChnInd].m_EncoderHandle, SubInd, &pStatus->m_Status);
		}
		else
		{
			GLOBAL_TRACE(("Overlimit !!!!\n"));
		}
	}
}

BOOL HWL_ENCODER_GetAlarmInfo(S32 ChnInd, S32 SubInd, HWL_LENCODER_AlarmInfo *pAlarmInfo)
{
	HWL_LENCODER_Handle *plHandle = s_pHandle;
	if (plHandle)
	{
		if (GLOBAL_CHECK_INDEX(ChnInd, plHandle->m_InitParam.m_ChnNum))
		{
			return ENC_3531AGetAlarmInfo(plHandle->m_pChn[ChnInd].m_EncoderHandle, SubInd, &pAlarmInfo->m_AlarmInfo);
		}
		else
		{
			GLOBAL_TRACE(("Overlimit !!!!\n"));
		}
	}
}

BOOL HWL_ENCODER_ResetAlarmInfo(S32 ChnInd, S32 SubInd, HWL_LENCODER_AlarmInfo *pAlarmInfo)
{
	HWL_LENCODER_Handle *plHandle = s_pHandle;
	if (plHandle)
	{
		if (GLOBAL_CHECK_INDEX(ChnInd, plHandle->m_InitParam.m_ChnNum))
		{
			return ENC_3531AResetAlarmInfo(plHandle->m_pChn[ChnInd].m_EncoderHandle, SubInd, &pAlarmInfo->m_AlarmInfo);
		}
		else
		{
			GLOBAL_TRACE(("Overlimit !!!!\n"));
		}
	}
}

void HWL_ENCODER_Terminate(void)
{
	HWL_LENCODER_Handle *plHandle = s_pHandle;
	if (plHandle)
	{
		S32 i;
		HWL_LENCODER_Chn *plChn;
		for (i = 0; i < plHandle->m_InitParam.m_ChnNum; i++)
		{
			plChn = &plHandle->m_pChn[i];
			ENC_3531ADestroy(plChn->m_EncoderHandle);
			PAL_CriticalTerminate(plChn->m_Lock);
			GLOBAL_SAFEFREE(plChn->m_pSubCHN);

		}
		GLOBAL_SAFEFREE(plHandle->m_pChn);
		GLOBAL_SAFEFREE(s_pHandle);
		GLOBAL_TRACE(("Terminate Complete!\n"));
	}
}

/*EOF*/
