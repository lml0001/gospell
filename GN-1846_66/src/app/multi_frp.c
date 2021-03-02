
/* Includes-------------------------------------------------------------------- */
#include "global_micros.h"
#include "platform_assist.h"
#include "multi_main_internal.h"
#include "frp_menu.h"
#include "frp_device.h"
/* Global Variables (extern)--------------------------------------------------- */
extern int g_auto_reboot_times;//20130225 增加改信息表达设备因软件BUG死机导致的自动重设置的次数。
/* Macro (local)--------------------------------------------------------------- */
/* Private Constants ---------------------------------------------------------- */
/* Private Types -------------------------------------------------------------- */


S32 MULT_FRPCB(S32 MenuInfoID, void *pParam, void *pUserParam)
{
	S32 lRet = FRP_MENU_OK;
	MULT_Handle *plHandle = (MULT_Handle *)pUserParam;
	if (plHandle)
	{
		MULT_Config *plConfig;
		MULT_Information *plInfo;
		CHAR_T *plTmpStr;
		U32 lTmpValue;
		U8 lIPFirst;
		plConfig = &plHandle->m_Configuration;
		plInfo = &plHandle->m_Information;

		switch(MenuInfoID)
		{
		case FRP_MENU_ID_NET_IP:
			{
				plTmpStr = (CHAR_T*)pParam;
				GLOBAL_TRACE(("IP Set = %s\n", plTmpStr));
				lTmpValue = PFC_SocketAToN(plTmpStr);
				lIPFirst = ((lTmpValue >> 24) & 0xFF);
				if ( (lIPFirst > 0) && (lIPFirst < 224) )
				{
					plConfig->m_ManageIPv4Addr = lTmpValue;
					MULTL_ManagePortConfig(plHandle);//设置IP地址
					MULT_SaveConfigurationXML(plHandle);
					MULTL_SaveParamterToStorage(plHandle);
				}
				else
				{
					lRet = FRP_MENU_NO;
				}
			}
			break;
		case FRP_MENU_ID_NET_MASK:
			{
				plTmpStr = (CHAR_T*)pParam;
				GLOBAL_TRACE(("Mask Set = %s\n", plTmpStr));
				plConfig->m_ManageIPv4Mask = PFC_SocketAToN(plTmpStr);
				MULTL_ManagePortConfig(plHandle);//设置IP地址
				MULT_SaveConfigurationXML(plHandle);
				MULTL_SaveParamterToStorage(plHandle);
			}
			break;
		case FRP_MENU_ID_NET_GATE:
			{
				plTmpStr = (CHAR_T*)pParam;
				GLOBAL_TRACE(("Gate Set = %s\n", plTmpStr));
				lTmpValue = PFC_SocketAToN(plTmpStr);
				lIPFirst = ((lTmpValue >> 24) & 0xFF);
				if ( (lIPFirst > 0) && (lIPFirst < 224) )
				{
					plConfig->m_ManageIPv4Gate = lTmpValue;
					MULTL_ManagePortConfig(plHandle);//设置IP地址
					MULT_SaveConfigurationXML(plHandle);
					MULTL_SaveParamterToStorage(plHandle);
				}
				else
				{
					lRet = FRP_MENU_NO;
				}

			}
			break;
		case FRP_MENU_ID_RESET:
			{
				GLOBAL_TRACE(("Parameter Reset!!!!!!!!!!!!\n"));
				MULTL_ParameterReset(plHandle);
				/*重启*/
				MULTL_RebootSequence(plHandle);
			}
			break;
		case FRP_MENU_ID_FACTORY:
			{
				GLOBAL_TRACE(("Factory Preset!!!!!!!!!!!!\n"));
				MULTL_FactoryPreset(plHandle);
				/*重启*/
				MULTL_RebootSequence(plHandle);

			}
			break;;
		case FRP_MENU_ID_WARN:
			{
				GLOBAL_TRACE(("Alarm Clear!!!!!!!!!!!!\n"));
				MULTL_ResetAlarmCount(plHandle, GLOBAL_INVALID_INDEX);
			}
			break;;
		case FRP_MENU_ID_LANG:
			{
				S32 lNewLanIndex = (*(S32*)pParam);
				GLOBAL_TRACE(("Language Set = %d!!!!!!!!!!!!\n", lNewLanIndex));
				plConfig->m_FrpLanguage = lNewLanIndex;
				if (plConfig->m_FrpLanguage == 0)
				{
					FRP_MenuBuffSet(FRP_MENU_INFO_ID_ROOT2, plInfo->m_pFRPENG);
				}
				else
				{
					FRP_MenuBuffSet(FRP_MENU_INFO_ID_ROOT2, plInfo->m_pFRPCHN);
				}
				MULT_SaveConfigurationXML(plHandle);
				MULTL_SaveParamterToStorage(plHandle);
			}
			break;;
		default:
			break;
		}
	}
	return lRet;
}




/* Private Variables (static)-------------------------------------------------- */
/* Private Function prototypes ------------------------------------------------ */
/* Functions ------------------------------------------------------------------ */
HANDLE32 s_TmpFRpHandle = NULL;
void MULT_FRPPreConsoleIntiate(void)
{
	s_TmpFRpHandle = FRP_DeviceCreate(FRP_UART_INDEX, 1000);
	FRP_DeviceSetLED(s_TmpFRpHandle, FRP_LED_BACK, FRP_LED_BACK_ON);
	FRP_DeviceSetLED(s_TmpFRpHandle, FRP_LED_STATUS, FRP_LED_GREEN);
	FRP_DeviceSetLED(s_TmpFRpHandle, FRP_LED_ALARM, FRP_LED_GREEN);
	FRP_DeviceSetCursorIndex(s_TmpFRpHandle, GLOBAL_INVALID_INDEX);
	FRP_DeviceSetLCDText(s_TmpFRpHandle, 0, "    INIT DEVICE     ");
	FRP_DeviceSetLCDText(s_TmpFRpHandle, 1, "                    ");
	FRP_DeviceUpdateAll(s_TmpFRpHandle);
}

void MULT_FRPPreConsoleSetText(S32 Line, CHAR_T *pLineText)
{
	FRP_DeviceSetLCDText(s_TmpFRpHandle, Line, pLineText);
	FRP_DeviceUpdateAll(s_TmpFRpHandle);
}

void MULT_FRPPreConsoleTerminate(void)
{
	FRP_DeviceDestroy(s_TmpFRpHandle);
}

void MULT_FRPInitiate(MULT_Handle *pHandle)
{
	U32 lCRC32;

#if 0
	//FRP DEVICE模块测试
	{

	}
#endif



#if 0
	//PFC 串口通信模块设置
	{
		BOOL lRet = FALSE;
		HANDLE lCom1;
		U8 lCommand[15];

		GLOBAL_TRACE(("Initiate Test COM1\n"));

		lCom1 = PL_ComDeviceOpen(1, TRUE);

		lRet = PL_ComDeviceSetState(lCom1, 19200, 8, 'N', 2);//需要有校验位但是不使用！

		while(1)
		{
			GLOBAL_MEMSET(lCommand, 0xFF, 15);
			lCommand[0] = 0x4E;
			lCommand[1] = 0xA0;
			lCommand[2] = 0x40;
			lCommand[3] = 0x81;
			lCommand[4] = 0x82;
			lCommand[14] = CRYPTO_CRCXOR(&lCommand[1], 13, 0);
			lRet = PL_ComDeviceWrite(lCom1, lCommand, 15);
			if (lRet != 15)
			{
				GLOBAL_TRACE(("Send Error!\n"));
			}
			else
			{
				CAL_PrintDataBlock("Send Data", lCommand, 15);
			}

			lRet = PL_ComDeviceRead(lCom1, lCommand, 15);
			if (lRet > 0)
			{
				if (lCommand[0] == 0x4E && lCommand[2] == 0x04)
				{
					GLOBAL_TRACE(("Exit!!!  \n"));
					break;
				}
				else
				{
					CAL_PrintDataBlock("Recv Data", lCommand, lRet);
				}
			}
			else
			{
				GLOBAL_TRACE(("No Data!!!  \n"));
			}

		}

		PL_ComDeviceClose(lCom1);

		lCom1 = NULL;

	}

#endif
	MULT_Config *plConfig;
	MULT_Information *plInfo;
	CHAR_T plStrBuf[64], plStrBufCHN[64];
	S32 lOffst;

	plConfig = &pHandle->m_Configuration;
	plInfo = &pHandle->m_Information;


	/**面板各模块初始化.必须先调用.*/
	FRP_MenuInitiate(1, NULL);	

	FRP_MenuSetLED(FRP_LED_BACK, FRP_LED_BACK_ON);
	/*语言设置*/
	FRP_MenuLangSet(plConfig->m_FrpLanguage);

	/**回调函数设置.,,用户需修改为自定义函数.*/
	FRP_MenuCallBackSet(MULT_FRPCB, pHandle);

	/*菜单信息添加*/
	lOffst = CAL_StringCenterOffset(plInfo->m_pModelName, 21);
	if ((lOffst < (21 - GLOBAL_STRLEN(plInfo->m_pModelName))) && (lOffst >= 0))
	{
		GLOBAL_MEMSET(plStrBuf, ' ', sizeof(plStrBuf));
		GLOBAL_STRCPY(&plStrBuf[lOffst], plInfo->m_pModelName);
	}
	
	FRP_MenuBuffSet(FRP_MENU_INFO_ID_ROOT1, plStrBuf);//需要居中操作
	if (plConfig->m_FrpLanguage == 0)
	{
		FRP_MenuBuffSet(FRP_MENU_INFO_ID_ROOT2, plInfo->m_pFRPENG);
	}
	else
	{
		FRP_MenuBuffSet(FRP_MENU_INFO_ID_ROOT2, plInfo->m_pFRPCHN);
	}

	FRP_MenuBuffSet(FRP_MENU_INFO_ID_IP, PFC_SocketNToA(plConfig->m_ManageIPv4Addr));
	FRP_MenuBuffSet(FRP_MENU_INFO_ID_MASK, PFC_SocketNToA(plConfig->m_ManageIPv4Mask));
	FRP_MenuBuffSet(FRP_MENU_INFO_ID_GATE, PFC_SocketNToA(plConfig->m_ManageIPv4Gate));
	CAL_StringBinToMAC(plConfig->m_pMAC, sizeof(plConfig->m_pMAC), plStrBuf, sizeof(plStrBuf));
	FRP_MenuBuffSet(FRP_MENU_INFO_ID_MAC, plStrBuf);
	FRP_MenuBuffSet(FRP_MENU_INFO_ID_SN, plInfo->m_pSNString);
	FRP_MenuBuffSet(FRP_MENU_INFO_ID_SOFTV, plInfo->m_pSoftVersion);
	FRP_MenuBuffSet(FRP_MENU_INFO_ID_HARDV, plInfo->m_pHardVersion);
	FRP_MenuBuffSet(FRP_MENU_INFO_ID_SOFT_DATE, plInfo->m_pSoftRelease);

#ifdef INDIA
	S32 i;
	HWL_HWInfos lTmpHWInfo;
	HWL_GetInformation(&lTmpHWInfo);
	for (i = 0; i < MULT_MAX_DEVICE_NUM; i++)
	{
		if (lTmpHWInfo.m_pSubDevice[i].m_DeviceType != MULT_DEVICE_TYPE_NONE)
		{
			FRP_MenuBuffSet(FRP_MENU_INFO_ID_FPGA_DATE, lTmpHWInfo.m_pSubDevice[i].m_pFPGARelease);
			break;
		}
	}
#else
	FRP_MenuBuffSet(FRP_MENU_INFO_ID_FPGA_DATE, plInfo->m_pFPGARelease);
#endif

	FRP_MenuAlarmConfig(MULT_MONITOR_TYPE_TEMP, "Temp Error", "温度异常");
	FRP_MenuAlarmConfig(MULT_MONITOR_TYPE_FPGA_HEART_BEAT, "Hardware Error", "硬件故障");
	FRP_MenuAlarmConfig(MULT_MONITOR_TYPE_NTP, "NTP Failure", "时间同步失败");
	FRP_MenuAlarmConfig(MULT_MONITOR_TYPE_SCS_EMM, "EMM Error", "EMM 错误");
	FRP_MenuAlarmConfig(MULT_MONITOR_TYPE_SCS_ECM, "ECM Error", "ECM 错误");
	FRP_MenuAlarmConfig(MULT_MONITOR_TYPE_ETH_LINK, "ETH Link Error", "ETH 连接错误");
	FRP_MenuAlarmConfig(MULT_MONITOR_TYPE_PLL, "PLL Lock Lost", "锁相环失锁");
	FRP_MenuAlarmConfig(MULT_MONITOR_TYPE_CHANNEL_IN, "Input Error", "输入码率错误");
	FRP_MenuAlarmConfig(MULT_MONITOR_TYPE_CHANNEL_OUT, "Output Error", "输出码率错误");
	FRP_MenuAlarmConfig(MULT_MONITOR_TYPE_BUFFER_STATUS, "QAM Overflow", "调制通道溢出");
	FRP_MenuAlarmConfig(MULT_MONITOR_TYPE_PROGRAM_BITRATE, "Service Error", "节目码率错误");


	lCRC32 = CRYPTO_CRC32(GLOBAL_U32_MAX, MULT_DEFAULT_DEVICE_MODEL, strlen(MULT_DEFAULT_DEVICE_MODEL));
	GLOBAL_SPRINTF((plStrBuf, "01. %.8X  LC%.2X-%.2X", lCRC32, pHandle->m_Information.m_LicenseValid, pHandle->m_Information.m_LicenseMode + 1));
	FRP_MenuInfoConfig(0, plStrBuf, plStrBuf);
	GLOBAL_SPRINTF((plStrBuf, "02. Auto %d", g_auto_reboot_times));
	FRP_MenuInfoConfig(1, plStrBuf, plStrBuf);

#if (MULT_MAX_DEVICE_NUM > 1)
	{
		S32 i;
		for (i = 0; i < MULT_MAX_DEVICE_NUM; i++)
		{
			GLOBAL_SPRINTF((plStrBuf, "%.2d. Sub%.2d %.8X", 3 + i, i + 1, pHandle->m_pDevice[i].m_Parameter.m_ChipID));
			FRP_MenuInfoConfig(2 + i, plStrBuf, plStrBuf);
		}
	}
#endif
	GLOBAL_SPRINTF((plStrBuf, "%s", PFC_GetRelease()));
	FRP_MenuInfoConfig(0, plStrBuf, plStrBuf);
	GLOBAL_TRACE(("Core Module Release = %s\n", plStrBuf));

	FRP_MenuUseInfoNode();

	FRP_MenuSetFreeze(FALSE);

}

/*读取报警信息并呈现到前面板上*/
void MULT_FRPAccess(MULT_Handle *pHandle, S32 Duration)
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
				if ((i == MULT_MONITOR_TYPE_CHANNEL_IN) || (i == MULT_MONITOR_TYPE_PROGRAM_BITRATE))
				{
					FRP_MenuAlarmSetCount(i, TRUE, lCount);
				}
				else
				{
					FRP_MenuAlarmSetCount(i, FALSE, lCount);
				}
			}
		}
	}


}


void MULT_FRPSetManagePortRelatedData(MULT_Handle *pHandle)
{
	MULT_Config *plConfig;
	CHAR_T plStrBuf[64];

	plConfig = &pHandle->m_Configuration;

	FRP_MenuBuffSet(FRP_MENU_INFO_ID_IP, PFC_SocketNToA(plConfig->m_ManageIPv4Addr));
	FRP_MenuBuffSet(FRP_MENU_INFO_ID_MASK, PFC_SocketNToA(plConfig->m_ManageIPv4Mask));
	FRP_MenuBuffSet(FRP_MENU_INFO_ID_GATE, PFC_SocketNToA(plConfig->m_ManageIPv4Gate));
	CAL_StringBinToMAC(plConfig->m_pMAC, sizeof(plConfig->m_pMAC), plStrBuf, sizeof(plStrBuf));
	FRP_MenuBuffSet(FRP_MENU_INFO_ID_MAC, plStrBuf);
}

void MULT_FRPClearAlarm(S32 Index)
{
	FRP_MenuAlarmClear(Index);
}

void MULT_FRPRebootShow(void)
{
	FRP_MenuSetRebootShow();
}

void MULT_FRPTerminate(void)
{
	FRP_MenuTerminate();
}



/*EOF*/
