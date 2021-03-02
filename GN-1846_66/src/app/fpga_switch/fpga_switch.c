/* Includes-------------------------------------------------------------------- */
#include "fpga_switch.h"
#include "libc_assist.h"
#include "global_micros.h"
#include "platform_assist.h"
/* Global Variables (extern)--------------------------------------------------- */
/* Macro (local)--------------------------------------------------------------- */
#define FPGASW_TASK_STATCK_SIZE					(1024*1024)
#define FPGASW_BIND_PORT						(0xEFEF)
#define FPGASW_RECV_ADDR						(0xEFEFEFEF)
#define FPGASW_SEND_ADDR						(0xEFEFEFEF)
#define FPGASW_IO_BUFFER_SIZE					(128)
#define FPGASW_PROTOCOL_SIZE					(66)
#define FPGASW_PROTOCOL_PREFIX					(0x8040)
#define FPGASW_PIN_STATUS_RESET_BIT				(3)
#define FPGASW_RESET_COUNT						(2)
/* Private Constants ---------------------------------------------------------- */
/* Private Types -------------------------------------------------------------- */
typedef enum
{
	FPGASW_STATE_INIT,
	FPGASW_STATE_NORMAL,
	FPGASW_STATE_ERROR,
	FPGASW_STATE_NUM,
}FPGASW_STATE;

typedef struct
{
	S32						m_Slot;
	BOOL					m_bPINStatus;;
	U32						m_Version;
	U32						m_DataIPv4;
	U32						m_pMAC[GLOBAL_MAC_BUF_SIZE];
}FPGASW_Protocol;

typedef struct
{
	FPGASW_InitParam		m_InitParam;
	BOOL					m_ModuleTaskMark;
	HANDLE32				m_ModuleTaskHandle;
	S32						m_State;
	HANDLE32				m_Socket;

	S32						m_SendTimeoutMS;
	S32						m_ResetTimeoutMS;

	U8						m_pIOBuf[FPGASW_IO_BUFFER_SIZE];

	S32						m_CurrentSlotInd;//从FPGA获取的当前的SLOT号！！
	BOOL					m_ResetStatus;
	BOOL					m_ResetPINLowDetected;
	FPGASW_DataPortParam	m_DataPortParam;
	FPGASW_Protocol			m_Protocol;

	S32						m_ResetDurationCount;

}FPGASW_Handle;


/* Private Variables (static)-------------------------------------------------- */
/* Private Function prototypes ------------------------------------------------ */
/* Functions ------------------------------------------------------------------ */

/* 本地函数 --------------------------------------------------------------------------------------------------------------------------------------- */

/* 线程函数 */
void FPGASW_LModuleTaskFn(void *pUserParam)
{
	FPGASW_Handle *plHandle = (FPGASW_Handle*) pUserParam;
	if (plHandle)
	{
		plHandle->m_Socket = PFC_SocketCreate(PFC_SOCKET_TYPE_UDP);
		if (plHandle->m_Socket)
		{
			U32 lValue;
			U32 lTick;

			if (PFC_SocketBind(plHandle->m_Socket, 0, FPGASW_BIND_PORT) == FALSE)
			{
				GLOBAL_TRACE(("Bind Failed!!!!!!!!!!!!!!!!!\n"));
			}
			lValue = 1;
			PFC_SocketOption(plHandle->m_Socket, PFC_SOCKET_OPTION_TYPE_BROADCAST, &lValue, sizeof(lValue));
			lValue = 0;
			PFC_SocketOption(plHandle->m_Socket, PFC_SOCKET_OPTION_TYPE_NONBLOCK, (void*)&lValue, sizeof(lValue));
			lValue = 1000;
			PFC_SocketOption(plHandle->m_Socket, PFC_SOCKET_OPTION_TYPE_RECVTIMEOUT, (void*)&lValue, sizeof(lValue));
			lValue = 500;
			PFC_SocketOption(plHandle->m_Socket, PFC_SOCKET_OPTION_TYPE_SENDTIMEOUT, (void*)&lValue, sizeof(lValue));
			lValue = 1;
			PFC_SocketOption(plHandle->m_Socket, PFC_SOCKET_OPTION_TYPE_ADDRREUSE, (void*)&lValue, sizeof(lValue));
			lValue = 0;
			PFC_SocketOption(plHandle->m_Socket, PFC_SOCKET_OPTION_TYPE_MULTICAST_LOOP, (void*)&lValue, sizeof(lValue));

			{
				PFC_SocketOptionValueMuticast lMulti;
				lMulti.m_InterfaceAddr = 0;
				lMulti.m_MuticasAddr = FPGASW_RECV_ADDR;
				PFC_SocketOption(plHandle->m_Socket, PFC_SOCKET_OPTION_TYPE_JOIN_MEMBERSHIP, (void*)&lMulti, sizeof(lMulti));
			}


			lTick = PFC_GetTickCount();
			plHandle->m_ResetDurationCount = FPGASW_RESET_COUNT;
			plHandle->m_ModuleTaskMark = TRUE;
			while(plHandle->m_ModuleTaskMark)
			{
				FPGASW_Access(plHandle, CAL_TimeDuration(&lTick, PFC_GetTickCount(), 10));
			}

			PFC_SocketClose(plHandle->m_Socket);
			plHandle->m_Socket = NULL;
		}
	}
}

S32 FPGASW_LProtocol(HANDLE32 Handle, U8 *pIOBuf, S32 IOSize, FPGASW_Protocol *pProtocal, BOOL bDecode)
{
	S32 lRet = GLOBAL_INVALID_INDEX;
	FPGASW_Handle *plHandle = (FPGASW_Handle*)Handle;
	if (plHandle)
	{
		if (pProtocal)
		{
			S32 lActSize;
			U16 lPrefix;
			U8 *plTmpBuf;
			plTmpBuf = pIOBuf;

			lActSize = 0;
			if (bDecode)
			{
				if (IOSize == FPGASW_PROTOCOL_SIZE)
				{
					GLOBAL_MSB16_DC(plTmpBuf, lPrefix, lActSize);
					if (lPrefix == FPGASW_PROTOCOL_PREFIX)
					{
						GLOBAL_BYTES_SC(plTmpBuf, 6, lActSize);
						GLOBAL_BYTES_SC(plTmpBuf, 4, lActSize);
						GLOBAL_BYTES_SC(plTmpBuf, 4, lActSize);
						GLOBAL_MSB8_DC(plTmpBuf, pProtocal->m_Slot, lActSize);
						GLOBAL_MSB8_DC(plTmpBuf, pProtocal->m_bPINStatus, lActSize);
						GLOBAL_MSB8_DC(plTmpBuf, pProtocal->m_Version, lActSize);
						lActSize = FPGASW_PROTOCOL_SIZE;

						pProtocal->m_Slot = (pProtocal->m_Slot & 0x0F);

						lRet = lActSize;
					}
					else
					{
						GLOBAL_TRACE(("DATA ERROR\n"));
						CAL_PrintDataBlock(__FUNCTION__, pIOBuf, IOSize);
					}
				}
				else
				{
					GLOBAL_TRACE(("DATA SIZE ERROR [%d]\n", IOSize));
				}
			}
			else
			{
				GLOBAL_MSB16_EC(plTmpBuf, FPGASW_PROTOCOL_PREFIX, lActSize);
				GLOBAL_BYTES_EC(plTmpBuf, pProtocal->m_pMAC, GLOBAL_MAC_BUF_SIZE, lActSize);
				GLOBAL_MSB32_EC(plTmpBuf, pProtocal->m_DataIPv4, lActSize);
				GLOBAL_BYTES_SC(plTmpBuf, 4, lActSize);
				GLOBAL_MSB8_EC(plTmpBuf, (pProtocal->m_Slot | 0x80), lActSize);


				lActSize = FPGASW_PROTOCOL_SIZE;
				lRet = lActSize;
			}
		}
	}
	return lRet;
}

/* API函数 ---------------------------------------------------------------------------------------------------------------------------------------- */

/* 创建句柄 */
HANDLE32 FPGASW_Create(FPGASW_InitParam *pParam)
{
	FPGASW_Handle *plHandle = (FPGASW_Handle*)GLOBAL_ZMALLOC(sizeof(FPGASW_Handle));
	if (plHandle)
	{
		GLOBAL_MEMCPY(&plHandle->m_InitParam, pParam, sizeof(FPGASW_InitParam));
		plHandle->m_CurrentSlotInd = GLOBAL_INVALID_INDEX;
	}
	return plHandle;
}

BOOL FPGASW_Probe(HANDLE32 Handle, S32 TimeoutMS)
{
	BOOL lRet = FALSE;
	FPGASW_Handle *plHandle = (FPGASW_Handle*)Handle;
	if (plHandle)
	{
		U32 lTick;
		lTick = PFC_GetTickCount();
		while(TRUE)
		{
			if (plHandle->m_CurrentSlotInd != GLOBAL_INVALID_INDEX)
			{
				lRet = TRUE;
				break;
			}
			else
			{
				if (CAL_Timeout(&lTick, &TimeoutMS, PFC_GetTickCount()))
				{
					break;
				}
			}
			GLOBAL_TRACE(("Timeout = %d\n", TimeoutMS));
			PFC_TaskSleep(500);
		}
	}
	GLOBAL_TRACE(("TimeoutMS = %d\n", TimeoutMS));
	return lRet;
}

void FPGASW_SetDataPortParam(HANDLE32 Handle, FPGASW_DataPortParam *pDataPortParam)
{
	FPGASW_Handle *plHandle = (FPGASW_Handle*)Handle;
	if (plHandle)
	{
		if (pDataPortParam)
		{
			GLOBAL_MEMCPY(&plHandle->m_DataPortParam, pDataPortParam, sizeof(FPGASW_DataPortParam));
			plHandle->m_SendTimeoutMS = 0;
		}
	}
}

S32 FPGASW_GetSlot(HANDLE32 Handle)
{
	S32 lRet = GLOBAL_INVALID_INDEX;
	FPGASW_Handle *plHandle = (FPGASW_Handle*)Handle;
	if (plHandle)
	{
		lRet = plHandle->m_CurrentSlotInd;
	}
	return lRet;
}

BOOL FPGASW_CheckResetStatus(HANDLE32 Handle)
{
	BOOL lRet = FALSE;
	FPGASW_Handle *plHandle = (FPGASW_Handle*)Handle;
	if (plHandle)
	{
		if (plHandle->m_ResetStatus == TRUE)
		{
			lRet = TRUE;
			plHandle->m_ResetStatus = FALSE;
			plHandle->m_ResetTimeoutMS = plHandle->m_InitParam.m_ResetIntervalMS;
			plHandle->m_ResetDurationCount = FPGASW_RESET_COUNT;
		}
	}
	return lRet;
}


/* 启动线程 */
void FPGASW_Start(HANDLE32 Handle)
{
	FPGASW_Handle *plHandle = (FPGASW_Handle*)Handle;
	if (plHandle)
	{
		PFC_TaskCreate("Module Task", FPGASW_TASK_STATCK_SIZE, FPGASW_LModuleTaskFn, 0, plHandle);
	}
}

/* 关闭线程 */
void FPGASW_Stop(HANDLE32 Handle)
{
	FPGASW_Handle *plHandle = (FPGASW_Handle*)Handle;
	if (plHandle)
	{
		if (plHandle->m_ModuleTaskMark)
		{
			plHandle->m_ModuleTaskMark = FALSE;
			if (PFC_TaskWait(plHandle->m_ModuleTaskHandle, 1000))
			{
			}
		}
	}
}

/* 无线程处理接口 */
void FPGASW_Access(HANDLE32 Handle, S32 Duration)
{
	FPGASW_Handle *plHandle = (FPGASW_Handle*)Handle;
	if (plHandle)
	{
		S32 lActLen;
		S32 lSlot;
		BOOL blResetCMD;

		lSlot = GLOBAL_INVALID_INDEX;
		lActLen = 0;

		if (PFC_SocketRecv(plHandle->m_Socket, plHandle->m_pIOBuf, FPGASW_IO_BUFFER_SIZE, &lActLen))
		{
			if (lActLen > 0)
			{
				//CAL_PrintDataBlock(__FUNCTION__, plHandle->m_pIOBuf, lActLen);

				if (FPGASW_LProtocol(plHandle, plHandle->m_pIOBuf, lActLen, &plHandle->m_Protocol, TRUE) == FPGASW_PROTOCOL_SIZE)
				{
					/*获取SLOT号*/
					if (plHandle->m_CurrentSlotInd == GLOBAL_INVALID_INDEX)
					{
						if (plHandle->m_Protocol.m_Slot != GLOBAL_INVALID_INDEX)
						{
							plHandle->m_CurrentSlotInd = plHandle->m_Protocol.m_Slot;
							GLOBAL_TRACE(("Board Slot = %d\n", plHandle->m_CurrentSlotInd));
						}
					}


					/*获取复位状态*/
					if (plHandle->m_CurrentSlotInd != GLOBAL_INVALID_INDEX)//只有获取了SLOT号的时候才会检测复位
					{
						if (CAL_TimeoutCheck(&plHandle->m_ResetTimeoutMS, Duration))
						{
							if ((plHandle->m_Protocol.m_bPINStatus & (1 << FPGASW_PIN_STATUS_RESET_BIT)) != 0)
							{
								//GLOBAL_TRACE(("PIN HIGH\n"))
								if (plHandle->m_ResetDurationCount > 0)
								{
									plHandle->m_ResetDurationCount--;
								}
								else
								{
									if (plHandle->m_ResetPINLowDetected == TRUE)
									{
										if (plHandle->m_ResetStatus == FALSE)
										{
											GLOBAL_TRACE(("Reset Status Set TO TRUE!!!!!!!!!!!!!!!!!!!\n"));
											plHandle->m_ResetStatus = TRUE;
										}
									}
								}
							}
							else
							{
								//GLOBAL_TRACE(("PIN LOW\n"))
								plHandle->m_ResetPINLowDetected = TRUE;
							}
						}
					}
					//else
					//{
					//	GLOBAL_TRACE(("NO IND!\n"));
					//}
				}
				//else
				//{
				//	GLOBAL_TRACE(("Protocol Error!!\n"));
				//}
			}
		}


		if (CAL_TimeoutCheck(&plHandle->m_SendTimeoutMS, Duration))
		{
			/*获取了SLOT和IP地址才会发送*/
			if ((plHandle->m_CurrentSlotInd != GLOBAL_INVALID_INDEX) && (plHandle->m_DataPortParam.m_IPv4 != 0))
			{
				GLOBAL_ZEROSTRUCT(plHandle->m_Protocol);
				plHandle->m_Protocol.m_Slot = plHandle->m_CurrentSlotInd;
				plHandle->m_Protocol.m_DataIPv4 = plHandle->m_DataPortParam.m_IPv4;
				GLOBAL_MEMCPY(&plHandle->m_Protocol.m_pMAC, plHandle->m_DataPortParam.m_pMAC, GLOBAL_MAC_BUF_SIZE);
				//GLOBAL_TRACE(("Data IP = 0x%08X\n", plHandle->m_DataPortParam.m_IPv4));
				//CAL_PrintDataBlock(__FUNCTION__, plHandle->m_DataPortParam.m_pMAC, GLOBAL_MAC_BUF_SIZE);
				lActLen = FPGASW_LProtocol(plHandle, plHandle->m_pIOBuf, lActLen, &plHandle->m_Protocol, FALSE);
				if (lActLen > 0)
				{
					if (PFC_SocketSendTo(plHandle->m_Socket, plHandle->m_pIOBuf, lActLen, &lActLen, FPGASW_SEND_ADDR, FPGASW_BIND_PORT))
					{
						//GLOBAL_TRACE(("Send OK!!!!!!!!!!!!!!!!!\n"));
					}
					else
					{
						GLOBAL_TRACE(("Send Error!\n"));
					}
				}
				else
				{
					GLOBAL_TRACE(("Error!!!!!!!!!!!!!!!!!!!\n"));
				}
				plHandle->m_SendTimeoutMS = plHandle->m_InitParam.m_SendIntervalMS;
			}
		}
		else
		{
			//GLOBAL_TRACE(("Send Timeout = %d\n", plHandle->m_SendTimeoutMS));
		}



	}
}


/* 模块销毁 */
void FPGASW_Destroy(HANDLE32 Handle)
{
	FPGASW_Handle *plHandle = (FPGASW_Handle*)Handle;
	if (plHandle)
	{
		FPGASW_Stop(Handle);
		GLOBAL_FREE(plHandle);
		plHandle = NULL;
	}
}






static HANDLE32 s_FPGASWHandle = NULL;
static S32 s_CurrentSlot = GLOBAL_INVALID_INDEX;

BOOL FPGASW_WarpInitiate(void)
{
	FPGASW_InitParam lParam;
	GLOBAL_ZEROSTRUCT(lParam);
	lParam.m_ResetIntervalMS = 25 * 1000;
	lParam.m_SendIntervalMS = 5 * 1000;

	s_FPGASWHandle = FPGASW_Create(&lParam);

	FPGASW_Start(s_FPGASWHandle);
}


void FPGASW_WarpProbe(S32 TimeoutMS)
{
	FPGASW_Handle *plHandle = (FPGASW_Handle*)s_FPGASWHandle;
	if (plHandle)
	{
		if (FPGASW_Probe(s_FPGASWHandle, TimeoutMS))
		{
			s_CurrentSlot = FPGASW_GetSlot(s_FPGASWHandle);
			GLOBAL_TRACE(("Detect FPGASW OK CurrentSlot = %d!\n", s_CurrentSlot));
		}
		else
		{
			GLOBAL_TRACE(("Detect FPGASW Failed!\n"));
		}
	}
	else
	{
		GLOBAL_TRACE(("NULL Handle\n"));
	}
}

S32 FPGASW_WarpGetSlot(void)
{
	return s_CurrentSlot;
}

BOOL FPGASW_WarpCheckResetStatus(void)
{
	return FPGASW_CheckResetStatus(s_FPGASWHandle);
}


void FPGASW_WarpSetDataPortParam(FPGASW_DataPortParam *pDataPortParam)
{
	FPGASW_Handle *plHandle = (FPGASW_Handle*)s_FPGASWHandle;
	if (plHandle)
	{
		FPGASW_SetDataPortParam(s_FPGASWHandle, pDataPortParam);
	}
	else
	{
		GLOBAL_TRACE(("NULL Handle\n"));
	}
}

BOOL FPGASW_WarpTerminate(void)
{
	FPGASW_Handle *plHandle = (FPGASW_Handle*)s_FPGASWHandle;
	if (plHandle)
	{
		FPGASW_Destroy(s_FPGASWHandle);
		s_FPGASWHandle = NULL;
	}
	else
	{
		GLOBAL_TRACE(("NULL Handle\n"));
	}
}


/*EOF*/
