/* Includes-------------------------------------------------------------------- */
#include "global_micros.h"
#include "libc_assist.h"
#include "crypto.h"
#include "platform_assist.h"
#include "multi_main_internal.h"
#include "frp_card.h"
/* Global Variables (extern)--------------------------------------------------- */
extern int g_auto_reboot_times;//20130225 增加改信息表达设备因软件BUG死机导致的自动重设置的次数。
/* Macro (local)--------------------------------------------------------------- */
/* Private Constants ---------------------------------------------------------- */
/* Private Types -------------------------------------------------------------- */
typedef enum
{
	MULT_CARD_INFO_ID_DEVICE_NAME_TYPE = 0,
	MULT_CARD_INFO_ID_IP_ADDR,
	MULT_CARD_INFO_ID_IP_MASK,
	MULT_CARD_INFO_ID_IP_GATE,
	MULT_CARD_INFO_ID_IP_MAC,
	MULT_CARD_INFO_ID_SN,
	MULT_CARD_INFO_ID_SOFTWARE_VERSION,
	MULT_CARD_INFO_ID_HARDWARE_VERSION,
	MULT_CARD_INFO_ID_SOFTWARE_RELEASE,
	MULT_CARD_INFO_ID_NUMBER,
}MULT_CARD_INFO_ID;
/* Private Variables (static)-------------------------------------------------- */
/* Private Function prototypes ------------------------------------------------ */
/* Functions ------------------------------------------------------------------ */
BOOL MULTL_FRPCARDCmdCB(void* pUserParam, S32 CMD, U32 Value)
{
	BOOL lRet = FALSE;
	MULT_Handle *plHandle;
	plHandle = (MULT_Handle *)pUserParam;
	if (plHandle)
	{
		if (CMD == FRPCARD_CMD_RESET_IP)
		{
			CHAR_T plTmpStr[128];
			GLOBAL_TRACE(("Default IP Address Temporary!!! \n"));
			GLOBAL_SNPRINTF((plTmpStr, sizeof(plTmpStr), "ifconfig eth%d ", 0));
			GLOBAL_STRCAT(plTmpStr, PFC_SocketNToA(MULTI_DEFAULT_IP_ADDR));
			GLOBAL_STRCAT(plTmpStr, " netmask ");
			GLOBAL_STRCAT(plTmpStr, PFC_SocketNToA(MULTI_DEFAULT_IP_MASK));
			GLOBAL_TRACE(("%s\n", plTmpStr));
			//PFC_System(plTmpStr);
			GLOBAL_TRACE(("Default IP Address Temporary!!! Done\n"));
		}
		else if (CMD == FRPCARD_CMD_FACTURY_RESET)
		{
			GLOBAL_TRACE(("Factory Reset !!!!!!\n"));
			MULTL_ParameterReset(plHandle);
			/*重启*/
			MULTL_RebootSequence(plHandle);
		}

		lRet = TRUE;
	}
	return lRet;
}

BOOL MULTL_FRPCARInfoCB(void* pUserParam, U32 UserID, FRPCARD_INFO *pInfo)
{
	MULT_Handle *plHandle;
	plHandle = (MULT_Handle *)pUserParam;
	if (plHandle)
	{
		CHAR_T plStrBuf[64];
		MULT_Config *plConfig;
		MULT_Information *plMULTInfo;

		plConfig = &plHandle->m_Configuration;
		plMULTInfo = &plHandle->m_Information;

		pInfo->m_bUTF8 = FALSE;
		//GLOBAL_TRACE(("UserID = %d\n", UserID));
		switch (UserID)
		{
		case MULT_CARD_INFO_ID_DEVICE_NAME_TYPE:
			{
				GLOBAL_SPRINTF((plStrBuf, "  %s", plMULTInfo->m_pModelName));
				GLOBAL_STRCPY(pInfo->m_pInfoText[0], plStrBuf);
				if (plConfig->m_WebLanguage == 0)
				{
					GLOBAL_STRCPY(pInfo->m_pInfoText[1], plMULTInfo->m_pFRPENG);
				}
				else
				{
					GLOBAL_STRCPY(pInfo->m_pInfoText[1], plMULTInfo->m_pFRPCHN);
				}
			}
			break;
		case MULT_CARD_INFO_ID_IP_ADDR:
			{
				if (plConfig->m_WebLanguage == 0)
				{
					GLOBAL_STRCPY(pInfo->m_pInfoText[0], "1.0 IP ADDR");
				}
				else
				{
					GLOBAL_STRCPY(pInfo->m_pInfoText[0], "1.0 IP 地址");
				}
				GLOBAL_STRCPY(pInfo->m_pInfoText[1], PFC_SocketNToA(plConfig->m_ManageIPv4Addr));
			}
			break;
		case MULT_CARD_INFO_ID_IP_MASK:
			{
				if (plConfig->m_WebLanguage == 0)
				{
					GLOBAL_STRCPY(pInfo->m_pInfoText[0], "1.1 NETMASK");
				}
				else
				{
					GLOBAL_STRCPY(pInfo->m_pInfoText[0], "1.1 网络掩码");
				}
				GLOBAL_STRCPY(pInfo->m_pInfoText[1], PFC_SocketNToA(plConfig->m_ManageIPv4Mask));
			}
			break;
		case MULT_CARD_INFO_ID_IP_GATE:
			{
				if (plConfig->m_WebLanguage == 0)
				{
					GLOBAL_STRCPY(pInfo->m_pInfoText[0], "1.2 GATE");
				}
				else
				{
					GLOBAL_STRCPY(pInfo->m_pInfoText[0], "1.2 默认网关");
				}
				GLOBAL_STRCPY(pInfo->m_pInfoText[1], PFC_SocketNToA(plConfig->m_ManageIPv4Gate));
			}
			break;
		case MULT_CARD_INFO_ID_IP_MAC:
			{
				if (plConfig->m_WebLanguage == 0)
				{
					GLOBAL_STRCPY(pInfo->m_pInfoText[0], "1.3 MAC");
				}
				else
				{
					GLOBAL_STRCPY(pInfo->m_pInfoText[0], "1.3 物理地址");
				}
				CAL_StringBinToMAC(plConfig->m_pMAC, sizeof(plConfig->m_pMAC), plStrBuf, sizeof(plStrBuf));
				CAL_StringCharRemove(plStrBuf, ':');
				GLOBAL_STRCPY(pInfo->m_pInfoText[1], plStrBuf);
			}
			break;
		case MULT_CARD_INFO_ID_SN:
			{
				if (plConfig->m_WebLanguage == 0)
				{
					GLOBAL_STRCPY(pInfo->m_pInfoText[0], "1.4 SN");
				}
				else
				{
					GLOBAL_STRCPY(pInfo->m_pInfoText[0], "1.4 SN");
				}

				GLOBAL_STRCAT(pInfo->m_pInfoText[0], "  ");
				GLOBAL_ZEROSTRUCT(plStrBuf);
				GLOBAL_MEMCPY(plStrBuf, plMULTInfo->m_pSNString, 7);
				GLOBAL_STRCAT(pInfo->m_pInfoText[0], plStrBuf);
				GLOBAL_STRCPY(pInfo->m_pInfoText[1], &plMULTInfo->m_pSNString[7]);
			}
			break;
		case MULT_CARD_INFO_ID_SOFTWARE_VERSION:
			{
				if (plConfig->m_WebLanguage == 0)
				{
					GLOBAL_STRCPY(pInfo->m_pInfoText[0], "2.0 Soft Version");
				}
				else
				{
					GLOBAL_STRCPY(pInfo->m_pInfoText[0], "2.0 软件版本");
				}
				GLOBAL_STRCPY(pInfo->m_pInfoText[1], plMULTInfo->m_pSoftVersion);
			}
			break;
		case MULT_CARD_INFO_ID_HARDWARE_VERSION:
			{
				if (plConfig->m_WebLanguage == 0)
				{
					GLOBAL_STRCPY(pInfo->m_pInfoText[0], "2.1 Hard Version");
				}
				else
				{
					GLOBAL_STRCPY(pInfo->m_pInfoText[0], "2.1 硬件版本");
				}
				GLOBAL_STRCPY(pInfo->m_pInfoText[1], plMULTInfo->m_pHardVersion);
			}
			break;
		case MULT_CARD_INFO_ID_SOFTWARE_RELEASE:
			{
				if (plConfig->m_WebLanguage == 0)
				{
					GLOBAL_STRCPY(pInfo->m_pInfoText[0], "2.2 Release");
				}
				else
				{
					GLOBAL_STRCPY(pInfo->m_pInfoText[0], "2.2 发布日期");
				}
				GLOBAL_STRCPY(pInfo->m_pInfoText[1], plMULTInfo->m_pSoftRelease);
			}
			break;
		default:
			break;
		}
	}
	return TRUE;
}

/*API 函数*/
void MULT_FRPCARDInitate(MULT_Handle *pHandle, S32 PlatFormType)
{
	S32 i;
	FRPCARD_Param lParam;
	GLOBAL_ZEROSTRUCT(lParam);

	GLOBAL_STRCPY(lParam.m_pDevName, "ttyS1");
	lParam.m_AlarmBlinkDurationMS = 10000;
	lParam.m_pCMDCB = MULTL_FRPCARDCmdCB;
	lParam.m_pInfoCB = MULTL_FRPCARInfoCB;
	lParam.m_pUserParam = pHandle;
	lParam.m_BackLightTimeoutMS = 10 * 1000;
	lParam.m_ReadTimeout = 1000;
	lParam.m_bInitateStatusUseScreen = TRUE;
	lParam.m_COMIndex = FRP_UART_INDEX;

	lParam.m_bUseOLEDPanel = TRUE;
	lParam.m_bOLEDBLOFF = TRUE;
	lParam.m_OLEDPanelType = OLED_SCREEN_TYPE_ZJYDZ_128X32;
	GLOBAL_STRCPY(lParam.m_pFontPath, "/tmp/arialuni_U16.bin");
	lParam.m_InfoMaxNum = 20;

	lParam.m_PlatFormType = PlatFormType;
	FRPCARD_Initiate(&lParam);

	FRPCARD_SetCardStatus(GLOBAL_INVALID_INDEX, FRPCARD_CARD_STATUS_NORMAL);
	for (i = 0; i < FRPCARD_MAX_MODULE_SUPPORT; i++)
	{
		FRPCARD_SetCardStatus(i, FRPCARD_CARD_STATUS_NORMAL);
	}

	FRPCARD_SetInitStatus(TRUE);

	//FRPCARD_SetUserKeyCB(MULT_KeyCB, NULL);


	for (i = 0; i < MULT_CARD_INFO_ID_NUMBER; i++)
	{
		FRPCARD_SetInfoNode(i, i);
	}
}

void MULT_FRPCARDAccess(MULT_Handle *pHandle, S32 Duration)
{
	S32 i, k, lCount;
	CAL_LogConfig lLogConfig;
	MULT_Monitor *plMonitor;

	plMonitor = &pHandle->m_Monitor;

	for (i = 0; i < MULT_MONITOR_TYPE_NUM; i++)
	{
		CAL_LogProcConfig(plMonitor->m_LogHandle, i, &lLogConfig, TRUE);
		if (lLogConfig.m_bFP)
		{
			lCount = CAL_LogGetLogInfoCount(plMonitor->m_LogHandle, i);
			if (lCount > 0)
			{
				FRPCARD_SetCardAlarm(GLOBAL_INVALID_INDEX, lCount);
			}
		}
	}
}

void MULT_FRPCARDAfterInitiate(MULT_Handle *pHandle)
{
	MULT_Handle *plHandle = pHandle;
	if (plHandle)
	{
		/*查看子卡插入的状态*/
		S32 i;
		for (i = 0; i < FRPCARD_MAX_MODULE_SUPPORT; i++)
		{
			FRPCARD_SetCardStatus(i, FRPCARD_CARD_STATUS_NO_CARD);
		}

		FRPCARD_SetInitStatus(FALSE);
	}
}

void MULT_FRPCARDClearAlarm(S32 Index)
{
	FRPCARD_SetCardAlarm(Index, 0);
}

void MULT_FRPCARDRebootShow(void)
{

}

void MULT_FRPCARDTerminate(void)
{
	FRPCARD_Terminate();
}


/*EOF*/
