//这个是在MAIN函数里调用的模块

/* Includes-------------------------------------------------------------------- */
#include "multi_main_internal.h"
#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
#include "multi_hwl_local_encoder.h"
#include "multi_hwl_probe.h"
#endif

/* Global Variables (extern)--------------------------------------------------- */
/* Macro (local)--------------------------------------------------------------- */
/* Private Constants ---------------------------------------------------------- */
/* Private Types -------------------------------------------------------------- */
/* Private Variables (static)-------------------------------------------------- */
/* Private Function prototypes ------------------------------------------------ */
/* Functions ------------------------------------------------------------------ */
/*Post Ajax 操作*/
void MULTL_WEBXMLSystemConfig(MULT_Handle *pHandle, mxml_node_t* pBodyNode, CHAR_T* pSubType, CHAR_T *pParameter)
{
	MULT_Config *plConfig;

// Timing Apply Parameter Start
    MULT_Monitor *plMonitor;
    plMonitor = &pHandle->m_Monitor;
// Timing Apply Parameter End

	CHAR_T *plTmpStr;

	plConfig = &pHandle->m_Configuration;

	if (GLOBAL_STRCMP(pSubType, "manage_ipv4_setting") == 0)
	{
		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "ipv4_addr");
		if (plTmpStr)
		{
			plConfig->m_ManageIPv4Addr = PFC_SocketAToN(plTmpStr);
		}

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "ipv4_mask");
		if (plTmpStr)
		{
			plConfig->m_ManageIPv4Mask = PFC_SocketAToN(plTmpStr);
		}

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "ipv4_gate");
		if (plTmpStr)
		{
			plConfig->m_ManageIPv4Gate = PFC_SocketAToN(plTmpStr);
		}

		MULTL_ManagePortConfig(pHandle);//设置IP地址

		MULT_FRPWarpSetManagePortRelatedData(pHandle);//传递数据给前面板

		MULT_SaveConfigurationXML(pHandle);
	}
	else if (GLOBAL_STRCMP(pSubType, "manage_mac_setting") == 0)
	{
		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "ip_mac");
		if (plTmpStr)
		{
			CAL_StringMACToBin(plTmpStr, plConfig->m_pMAC, sizeof(plConfig->m_pMAC));
			plConfig->m_pMAC[0] = plConfig->m_pMAC[0] & 0xFE;//确保第一个字节的最低位不为1
		}
		MULTL_ManagePortConfig(pHandle);//设置IP地址

		MULT_FRPWarpSetManagePortRelatedData(pHandle);//传递数据给前面板
		MULT_SaveConfigurationXML(pHandle);
	}
#ifdef MULT_SUPPORT_FPGA_ETH
	else if (GLOBAL_STRCMP(pSubType, "manage_data_ipv4_setting") == 0)
	{
		TUN_InitParam *plTunParam = MULT_FPGAEthGetParameterPtr();
		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "data_ipv4_addr");
		if (plTmpStr)
		{
			plTunParam->m_TUNIPAddr = PFC_SocketAToN(plTmpStr);
		}

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "data_ipv4_mask");
		if (plTmpStr)
		{
			plTunParam->m_TUNIPMask = PFC_SocketAToN(plTmpStr);
		}

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "data_ipv4_gate");
		if (plTmpStr)
		{
			plTunParam->m_TUNIPGate = PFC_SocketAToN(plTmpStr);
		}
		MULT_FPGAEthApply(TRUE);
#ifdef GQ3760A
		MULTL_ApplyInETHParameter(pHandle, 1);	
#endif
		MULT_SaveConfigurationXML(pHandle);
	}
	else if (GLOBAL_STRCMP(pSubType, "manage_data_mac_setting") == 0)
	{
		TUN_InitParam *plTunParam = MULT_FPGAEthGetParameterPtr();
		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "data_ip_mac");
		if (plTmpStr)
		{
			CAL_StringMACToBin(plTmpStr, plTunParam->m_TUNMAC, sizeof(plTunParam->m_TUNMAC));
			plTunParam->m_TUNMAC[0] = plTunParam->m_TUNMAC[0] & 0xFE;//确保第一个字节的最低位不为1
		}
		MULT_FPGAEthApply(TRUE);
#ifdef GQ3760A
		MULTL_ApplyInETHParameter(pHandle, 1);	
#endif
		MULT_SaveConfigurationXML(pHandle);
	}
#endif
#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
	else if (GLOBAL_STRCMP(pSubType, "ip_output_type_setting") == 0)
	{
		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "ip_output_type");
		if (plTmpStr)
		{
			plConfig->m_IpOutputType = MULTL_XMLIpOutputTypeValueFromStr(plTmpStr);
			GLOBAL_TRACE(("Set IpOutType:%d\n", plConfig->m_IpOutputType));
		}
		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "output_charset");
		if (plTmpStr)
		{
			plConfig->m_OutputCharset = atoi(plTmpStr);
			GLOBAL_TRACE(("Set OutputCharset:%d\n", plConfig->m_OutputCharset));
		}
		
		MULTL_ApplyLEncoderParameter(pHandle, 0);
		MULT_SaveConfigurationXML(pHandle);
	}
#endif
	else if (GLOBAL_STRCMP(pSubType, "ntp_setting") == 0)
	{
		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "ipv4_addr");
		if (plTmpStr)
		{
			plConfig->m_NTPServerAddr = PFC_SocketAToN(plTmpStr);
		}

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "ntp_interval");
		if (plTmpStr)
		{
			plConfig->m_NTPInterval = GLOBAL_STRTOL(plTmpStr, NULL, 10);
			if (plConfig->m_NTPInterval < MULTI_MIN_NTP_INTERVAL)
			{
				plConfig->m_NTPInterval = MULTI_MIN_NTP_INTERVAL;
			}
		}

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "ntp_mark");
		if (plTmpStr)
		{
			plConfig->m_NTPSyncMark = MULTL_XMLMarkValueFromStr(plTmpStr);
		}
		MULTL_ForceSNTPSync(pHandle);
		MULT_SaveConfigurationXML(pHandle);
	}
// Timing Apply Parameter Start
	else if (GLOBAL_STRCMP(pSubType, "timing_setting") == 0)
	{
		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "timing_interval");
		if (plTmpStr)
		{
			plConfig->m_TimingInterval = GLOBAL_STRTOL(plTmpStr, NULL, 10);
			if (plConfig->m_TimingInterval < 1)
			{
				plConfig->m_TimingInterval = 1;
			}

            plMonitor->m_TimingTimeout = plConfig->m_TimingInterval;
		}

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "timing_mark");
		if (plTmpStr)
		{
			plConfig->m_TimingMark = MULTL_XMLMarkValueFromStr(plTmpStr);
		}
		MULT_SaveConfigurationXML(pHandle);
	}
// Timing Apply Parameter End
	else if (GLOBAL_STRCMP(pSubType, "time_setting") == 0)
	{
		TIME_T lSetTime;
		STRUCT_TM lTimeStruct;
		S32 lYear, lMonth, lDay, lHour, lMin, lSecond;

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "year");
		if (plTmpStr)
		{
			lYear = GLOBAL_STRTOL(plTmpStr, NULL, 10);
		}

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "month");
		if (plTmpStr)
		{
			lMonth = GLOBAL_STRTOL(plTmpStr, NULL, 10);
		}

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "day");
		if (plTmpStr)
		{
			lDay = GLOBAL_STRTOL(plTmpStr, NULL, 10);
		}

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "hour");
		if (plTmpStr)
		{
			lHour = GLOBAL_STRTOL(plTmpStr, NULL, 10);
		}

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "minute");
		if (plTmpStr)
		{
			lMin = GLOBAL_STRTOL(plTmpStr, NULL, 10);
		}

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "second");
		if (plTmpStr)
		{
			lSecond = GLOBAL_STRTOL(plTmpStr, NULL, 10);
		}

		/*调用系统设置时间函数！注意系统工作在格林威治时间(GMT)/世界标准时(UTC）ZONE 0上*/
		GLOBAL_TRACE(("Set Time %.4d-%.2d-%.2d %.2d:%.2d:%.2d\n", lYear, lMonth, lDay, lHour, lMin, lSecond));
		lTimeStruct.tm_year = lYear - 1900;
		lTimeStruct.tm_mon = lMonth - 1;
		lTimeStruct.tm_mday = lDay;
		lTimeStruct.tm_hour = lHour;
		lTimeStruct.tm_min = lMin;
		lTimeStruct.tm_sec = lSecond;
		lSetTime = GLOBAL_MKTIME(&lTimeStruct);
		GLOBAL_STIME(&lSetTime);
	}
	else if (GLOBAL_STRCMP(pSubType, "ping_cmd") == 0)
	{
		U32 lPingTarget = 0;
		S32 lPingTimeout = 0;
		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "ping_target");
		if (plTmpStr)
		{
			lPingTarget = PFC_SocketAToN(plTmpStr);
		}

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "ping_timeout");
		if (plTmpStr)
		{
			lPingTimeout = GLOBAL_STRTOL(plTmpStr, NULL, 10);
		}

	}
	else if (GLOBAL_STRCMP(pSubType, "language") == 0)
	{
		plConfig->m_WebLanguage = MULTL_XMLGetNodeINT(pBodyNode, "language", 10);
#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
		plConfig->m_FrpLanguage = plConfig->m_WebLanguage;
#endif
		GLOBAL_TRACE(("New Language = %d\n", plConfig->m_WebLanguage));
		MULT_SaveConfigurationXML(pHandle);
	}
	else if (GLOBAL_STRCMP(pSubType, "apply_all") == 0)
	{
		MULTL_ApplyAllParamter(pHandle);
	}
	else if (GLOBAL_STRCMP(pSubType, "save_all_parameters") == 0)
	{
		MULTL_SetSaveMark(pHandle, TRUE);
		/*保存参数文件到固定存储区域！*/
		MULTL_SaveParamterToStorage(pHandle);
	}
	else if (GLOBAL_STRCMP(pSubType, "parameter_backup") == 0)
	{
		CHAR_T plCMD[1024];
		GLOBAL_TRACE(("CMD Parameter Backup!\n"));
		GLOBAL_SPRINTF((plCMD, "%s%s", MULT_STORAGE_BASE_DIR, MULT_PARAMETER_BAKU_PATHNAME));
		MULTL_GenerateParamters(pHandle, plCMD, MULTL_XMLGetNodeText(pBodyNode, "user_description"));
		//MULTL_GenerateInfoXML(pHandle);//更新XML文件
		MULT_SaveConfigurationXML(pHandle);
	}
	else if (GLOBAL_STRCMP(pSubType, "parameter_restore") == 0)
	{
		CHAR_T plCMD[1024];
		GLOBAL_TRACE(("CMD Parameter Restored!!!!!!\n"));

		GLOBAL_SPRINTF((plCMD, "%s%s", MULT_STORAGE_BASE_DIR, MULT_PARAMETER_BAKU_PATHNAME));
		/*解开参数文件*/
		if (MULTL_LoadParamterFromStorage(pHandle, plCMD))
		{
			/*重新保存*/
			MULTL_SaveParamterToStorage(pHandle);
		}
		/*重启*/
		MULTL_RebootSequence(pHandle);
	}
	else if (GLOBAL_STRCMP(pSubType, "parameter_reset") == 0)
	{
		MULTL_ParameterReset(pHandle);
		/*重启*/
		MULTL_RebootSequence(pHandle);
	}
	else if (GLOBAL_STRCMP(pSubType, "config_reset") == 0)
	{
		CHAR_T plCMD[1024];
		GLOBAL_TRACE(("CMD Config Reset!!!!!!\n"));
		/*删除device_parameter.xml*/
		GLOBAL_SPRINTF((plCMD, "rm -rf %s%s", MULT_XML_BASE_DIR, MULT_DEVICE_PARAMETER_XML));
		PFC_System(plCMD);
		/*保存到FLASH*/
		MULTL_SaveParamterToStorage(pHandle);
		/*该命令手动重启*/
	}
	else if (GLOBAL_STRCMP(pSubType, "user_reset") == 0)
	{
		CHAR_T plCMD[1024];
		GLOBAL_TRACE(("CMD User Manage Reset!!!!!!\n"));
		/*删除用户信息*/
		GLOBAL_SPRINTF((plCMD, "rm -rf %s%s", MULT_STORAGE_BASE_DIR, MULT_WEB_USER_MANAGE_FILE_PATHNAME));
		PFC_System(plCMD);
		/*该命令手动重启*/
	}
	else if (GLOBAL_STRCMP(pSubType, "factory_preset") == 0)
	{
		PFC_System("rm -rf %s%s", MULT_STORAGE_BASE_DIR, MULT_PARAMETER_DEF_PATHNAME);
		MULTL_FactoryPreset(pHandle);
	}
	else if (GLOBAL_STRCMP(pSubType, "clk_adj_module_preset") == 0)
	{
		MULT_ResetCLKADJModuleParameters(pHandle);
	}
	else if (GLOBAL_STRCMP(pSubType, "clk_adj_module_save") == 0)
	{
		MULT_SaveCLKADJModuleParameters(pHandle);
	}
	else if (GLOBAL_STRCMP(pSubType, "oem_reset") == 0)
	{
		CHAR_T plCMD[1024];
		GLOBAL_TRACE(("CMD OEM Reset!!!!!!\n"));
		/*删除OEM文件*/
		GLOBAL_SPRINTF((plCMD, "rm -rf %s%s", MULT_STORAGE_BASE_DIR, MULT_OEM_FILE_PATHNAME));
		PFC_System(plCMD);
		/*该命令手动重启*/
	}
	else if (GLOBAL_STRCMP(pSubType, "auth_reset") == 0)
	{
		CHAR_T plCMD[1024];
		GLOBAL_TRACE(("CMD Auth Reset!!!!!!\n"));
		/*删除授权文件*/
		GLOBAL_SPRINTF((plCMD, "rm -rf %s%s", MULT_STORAGE_BASE_DIR, MULT_LICENSE_FILE_PATHNAME));
		PFC_System(plCMD);
		/*该命令手动重启*/
	}
	else if (GLOBAL_STRCMP(pSubType, "user_param") == 0)
	{
		/*备份当前参数文件为默认参数文件*/
		GLOBAL_TRACE(("CMD Set User Defined Default Parameter!!!!!!\n"));

		/*保存参数文件*/
		MULTL_SaveParamterToStorage(pHandle);

		/*复制一份参数文件*/
		PFC_System("cp -f %s%s %s%s", MULT_STORAGE_BASE_DIR, MULT_PARAMETER_FILE_PATHNAME, MULT_STORAGE_BASE_DIR, MULT_PARAMETER_DEF_PATHNAME);
	}
	else if (GLOBAL_STRCMP(pSubType, "auth_random") == 0)
	{
		GLOBAL_TRACE(("CMD Auth Random!!!!!!\n"));
		/*重新设置设备授权本地随机值*/
		AUTH_ResetRadomCode(pHandle->m_AUTHHandle);
		/*该命令手动重启*/
	}
	else if (GLOBAL_STRCMP(pSubType, "private_channel_setup_type") == 0)
	{
		MULT_Maintenace *plMaintenance;
		plMaintenance = &pHandle->m_MaintSetting;
		plMaintenance->m_PrivateChannelSetupMark = MULTL_XMLGetNodeMark(pBodyNode, "private_channel_setup_mark");

		GLOBAL_TRACE(("Private Mark = %d\n", plMaintenance->m_PrivateChannelSetupMark));
		MULTL_SaveMaintenaceXML(pHandle);
	}
	else if (GLOBAL_STRCMP(pSubType, "pll_calibration") == 0)
	{
		MULT_Maintenace *plMaintenance;
		plMaintenance = &pHandle->m_MaintSetting;

		plMaintenance->m_PLLFreqOffset = MULTL_XMLGetNodeINT(pBodyNode, "pll_freq_offset", 10);
		GLOBAL_TRACE(("New Offst = %d\n", plMaintenance->m_PLLFreqOffset));

		MULTL_ApplyQAMParameter(pHandle, 0);

		/*该命令手动重启*/
		MULTL_SaveMaintenaceXML(pHandle);
	}
#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
	else if (GLOBAL_STRCMP(pSubType, "enc_adjust_param") == 0)
	{
		S32 i = 0;
		mxml_node_t *plXMLNode;
		MULT_Maintenace *plMaintenance;
		plMaintenance = &pHandle->m_MaintSetting;

		plXMLNode = mxmlFindElement(pBodyNode, pBodyNode, "enc_adjust_param", NULL, NULL, MXML_DESCEND_FIRST);
		while (plXMLNode) {
			plMaintenance->m_AudPtsRelativeDelayTime[i] = MULTL_XMLGetNodeINT(plXMLNode, "aud_pts_relative_delay_time", 10);
			plMaintenance->m_PtsDelayTime[i] = MULTL_XMLGetNodeINT(plXMLNode, "pts_delay_time", 10);
			plMaintenance->m_MaxPtsPcrInterval[i] = MULTL_XMLGetNodeINT(plXMLNode, "max_pts_pcr_interval", 10);
			plMaintenance->m_MinPtsPcrInterval[i] = MULTL_XMLGetNodeINT(plXMLNode, "min_pts_pcr_interval", 10);
			plMaintenance->m_AudDelayFrameNum[i] = MULTL_XMLGetNodeINT(plXMLNode, "aud_delay_frame_num", 10);
			i++;
			plXMLNode = mxmlFindElement(plXMLNode, pBodyNode, "enc_adjust_param", NULL, NULL, MXML_NO_DESCEND);
		}

		MULTL_ApplyLEncoderParameter(pHandle, 0);
		MULTL_SaveMaintenaceXML(pHandle);
	}
#endif
	else if (GLOBAL_STRCMP(pSubType, "direct_reg") == 0)
	{
		U8 lICID;
		U32 lAddr, lValue;

		lICID = MULTL_XMLGetNodeUINT(pBodyNode, "ic_id", 16);
		lAddr = MULTL_XMLGetNodeUINT(pBodyNode, "ic_addr", 16);
		lValue = MULTL_XMLGetNodeUINT(pBodyNode, "ic_value", 16);


		/*增加调试AD5060的程序*/
#ifdef SUPPORT_CLK_ADJ_MODULE
		{
			if (lICID == 0x80)
			{
				U32 lTmpValue;
				lTmpValue = ((lAddr & 0xFF) << 8) | (lValue & 0xFF);
				GLOBAL_TRACE(("Set DAValue = %.4X\n", lTmpValue));
				MULTL_CLKProtocolPacker(lTmpValue);
			}
			else
			{
				HWL_QAMDirectRegSet(lICID, lAddr, lValue);
			}
		}
#endif

	}
	else if (GLOBAL_STRCMP(pSubType, "direct_p_q") == 0)
	{
		U8 lICID;
		U32 lP, lQ;

		lP = MULTL_XMLGetNodeUINT(pBodyNode, "ad9789_p", 16);
		lQ = MULTL_XMLGetNodeUINT(pBodyNode, "ad9789_q", 16);

#ifndef USE_NEW_QAM_SETTING_FUNCTIONS
		HWL_QAMApplyParameterWithPQ(lP, lQ);
#endif

	}
	else if (GLOBAL_STRCMP(pSubType, "manual_reboot") == 0)
	{
		GLOBAL_TRACE(("CMD Manual Reboot!!!!!!\n"));
		/*重启*/
		MULTL_RebootSequence(pHandle);
	}
	else if (GLOBAL_STRCMP(pSubType, "manual_print_SDI_REG") == 0)
	{
		GLOBAL_TRACE(("Print SDI Chip REG!!!!!!\n"));
		/*打印SDI所有的REG */
#if defined(GN1846) || defined(GN1866)
		MULTL_Print_SDI_Reg(pHandle);
#endif
	}
	else if (GLOBAL_STRCMP(pSubType, "manual_SDI_reboot") == 0)
	{
		GLOBAL_TRACE(("SDI REboot!!!!!!\n"));
		/*重新启动SDI寄存器*/
#if defined(GN1846) || defined(GN1866)
		MULTL_Reboot_SDI(pHandle) ;
#endif
	}
	else if (GLOBAL_STRCMP(pSubType, "snmp_setting") == 0)
	{
		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "snmp_active");
		if (plTmpStr)
		{
			plConfig->m_SNMPGlobalMark = MULTL_XMLMarkValueFromStr(plTmpStr);
		}

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "trap_active");
		if (plTmpStr)
		{
			plConfig->m_SNMPInitParam.m_TRAPGlobalMark =  MULTL_XMLMarkValueFromStr(plTmpStr);
		}
		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "trap_ipv4_addr");
		if (plTmpStr)
		{
			plConfig->m_SNMPInitParam.m_TRAPIPAddress = PFC_SocketAToN(plTmpStr);
		}

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "trap_ip_port");
		if (plTmpStr)
		{
			plConfig->m_SNMPInitParam.m_TRAPPort =  GLOBAL_STRTOL(plTmpStr, NULL, 10);
		}
		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "trap_interval");
		if (plTmpStr)
		{
			plConfig->m_SNMPInitParam.m_NormalTrapInterval =  GLOBAL_STRTOL(plTmpStr, NULL, 10);
		}

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "snmp_agent_port");
		if (plTmpStr)
		{
			plConfig->m_SNMPInitParam.m_AgentPort =  GLOBAL_STRTOL(plTmpStr, NULL, 10);
		}

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "snmp_agent_readcommuniy");
		if (plTmpStr)
		{
			if (GLOBAL_STRLEN(plTmpStr) < MPEG2_DB_MAX_SERVICE_NAME_BUF_LEN)
			{
				GLOBAL_STRCPY(plConfig->m_SNMPInitParam.m_pROCommunity, plTmpStr);
			}
		}
		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "snmp_agent_writecommuniy");
		if (plTmpStr)
		{
			if (GLOBAL_STRLEN(plTmpStr) < MPEG2_DB_MAX_SERVICE_NAME_BUF_LEN)
			{
				GLOBAL_STRCPY(plConfig->m_SNMPInitParam.m_pRWCommunity, plTmpStr);
			}
		}

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "snmp_device_name");
		if (plTmpStr)
		{
			if (GLOBAL_STRLEN(plTmpStr) < MPEG2_DB_MAX_SERVICE_NAME_BUF_LEN)
			{
				GLOBAL_STRCPY(plConfig->m_SNMPInitParam.m_pDeviceName, plTmpStr);
			}
		}
		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "snmp_device_location");
		if (plTmpStr)
		{
			if (GLOBAL_STRLEN(plTmpStr) < MPEG2_DB_MAX_SERVICE_NAME_BUF_LEN)
			{
				GLOBAL_STRCPY(plConfig->m_SNMPInitParam.m_pDeviceLocation, plTmpStr);
			}
		}

		if (plConfig->m_SNMPGlobalMark)
		{
			MULT_SNMPTerminate(pHandle);
			MULT_SNMPInitiate(pHandle);
		}
		else
		{
			MULT_SNMPTerminate(pHandle);
		}	
		MULT_SaveConfigurationXML(pHandle);


	}
	else if (GLOBAL_STRCMP(pSubType, "syslog_manage") == 0)
	{	
#ifdef SUPPORT_SYSLOG_MODULE
		MULTL_XMLLoadSyslog(pHandle, pBodyNode);
		MULT_SaveConfigurationXML(pHandle);
		{
			MULT_Syslog *plTmpSyslog;
			plTmpSyslog = &pHandle->m_SyslogParam;
			if (plTmpSyslog->m_SyslogGlobalMark == FALSE)
			{
				MULT_SyslogApply(pHandle, FALSE);
			}
			else
			{
				MULTL_SaveParamterToStorage(pHandle);
				MULTL_RebootSequence(pHandle);
			}
		}
#endif
	}
	else if (GLOBAL_STRCMP(pSubType, "syslog_clean") == 0)
	{
#ifdef SUPPORT_SYSLOG_MODULE
		MULT_SyslogClean(pHandle);
#endif
	}
	else
	{
		GLOBAL_ASSERT(0);
	}

}



void MULTL_WEBXMLMonitorConfig(MULT_Handle *pHandle, mxml_node_t* pBodyNode, CHAR_T* pSubType, CHAR_T *pParameter)
{
	MULT_Monitor *plMonitor;
	CAL_LogConfig lLogCFG;
	mxml_node_t *plXMLHolder;
	mxml_node_t *plXMLCurrent;

	plMonitor = &pHandle->m_Monitor;
	if (GLOBAL_STRCMP(pSubType, "alarm_settings") == 0)
	{
		U32 lLogIDs;

		plXMLHolder = mxmlFindElement(pBodyNode, pBodyNode, "log_system", NULL, NULL, MXML_DESCEND_FIRST);
		plXMLCurrent = mxmlFindElement(plXMLHolder, plXMLHolder, "log", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			lLogIDs = MULTL_XMLGetNodeUINT(plXMLCurrent, "log_id", 16);
			if (CAL_LogProcConfig(plMonitor->m_LogHandle, MULT_MONITOR_DECODE_ID(lLogIDs), &lLogCFG, TRUE))
			{
				lLogCFG.m_bTrap = MULTL_XMLGetNodeMark(plXMLCurrent, "trap");

				lLogCFG.m_bFP = MULTL_XMLGetNodeMark(plXMLCurrent, "panel");

				lLogCFG.m_LogLevel = MULTL_XMLGetNodeINT(plXMLCurrent, "level", 10);

				CAL_LogProcConfig(plMonitor->m_LogHandle, MULT_MONITOR_DECODE_ID(lLogIDs), &lLogCFG, FALSE);
			}

			plXMLCurrent = mxmlFindElement(plXMLCurrent, plXMLHolder, "log", NULL, NULL, MXML_NO_DESCEND);
		}
		MULTL_SaveMonitorXML(pHandle);
	}
	else if (GLOBAL_STRCMP(pSubType, "status_settings") == 0)
	{
		U32 lLogIDs;

		plMonitor->m_GlobalMark = MULTL_XMLGetNodeMark(pBodyNode, "global_mark");
		plMonitor->m_CriticalTemp = MULTL_XMLGetNodeINT(pBodyNode, "critical_temp", 10);

		/*风扇控制被放弃了！20170519*/
		//plMonitor->m_FanTemp = MULTL_XMLGetNodeINT(pBodyNode, "fan_temp", 10);
		//if (plMonitor->m_FanTemp > plMonitor->m_CriticalTemp)
		//{
		//	plMonitor->m_FanTemp = plMonitor->m_CriticalTemp;//不允许风扇温度大于出错温度，以安全。
		//}

		MULTL_SaveMonitorXML(pHandle);
	}
	else if (GLOBAL_STRCMP(pSubType, "reset_counter") == 0)
	{
		S32 lAlarmIndex;
		U32 lAlarmID;
		if (GLOBAL_STRCMP(pParameter, "all") == 0)
		{
			MULTL_ResetAlarmCount(pHandle, GLOBAL_INVALID_INDEX);
			MULT_FRPWarpAlarmClear(GLOBAL_INVALID_INDEX, GLOBAL_INVALID_INDEX);
		}
		else
		{
			lAlarmID = GLOBAL_STRTOUL(pParameter, NULL, 16);
			lAlarmIndex = MULT_MONITOR_DECODE_ID(lAlarmID);
			MULTL_ResetAlarmCount(pHandle, lAlarmIndex);
			MULT_FRPWarpAlarmClear(GLOBAL_INVALID_INDEX, lAlarmIndex);
		}
	}
	else if (GLOBAL_STRCMP(pSubType, "bitrate_setting") == 0)
	{
		S32 lChnIndex, lSubIndex;
		MULT_MonitorCHN *plCHN = NULL;
		MULT_MonitorSUB *plSub = NULL;

		lChnIndex = MULTL_XMLGetNodeINT(pBodyNode, "chn_index", 10); 
		if (GLOBAL_STRCMP(pParameter, "in") == 0)
		{
			if (GLOBAL_CHECK_INDEX(lChnIndex, plMonitor->m_InChnNum))
			{
				plCHN = &plMonitor->m_pInChnArray[lChnIndex];
			}
		}
		else 
		{
			if (GLOBAL_CHECK_INDEX(lChnIndex, plMonitor->m_InChnNum))
			{
				plCHN = &plMonitor->m_pOutChnArray[lChnIndex];
			}
		}

		if (plCHN)
		{

			plXMLCurrent = mxmlFindElement(pBodyNode, pBodyNode, "sub", NULL, NULL, MXML_DESCEND_FIRST);
			while(plXMLCurrent)
			{
				lSubIndex = MULTL_XMLGetNodeUINT(plXMLCurrent, "sub_ind", 10); 
				if (GLOBAL_CHECK_INDEX(lSubIndex, plCHN->m_SubNumber))
				{
					plSub = &plCHN->m_pSubArray[lSubIndex];

					plXMLHolder = mxmlFindElement(plXMLCurrent, plXMLCurrent, "limit", NULL, NULL, MXML_DESCEND_FIRST);

					plSub->m_LimitInfo.m_LowLimit = MULTL_XMLGetNodeINT(plXMLHolder, "low", 10);
					plSub->m_LimitInfo.m_HighLimit = MULTL_XMLGetNodeINT(plXMLHolder, "high", 10);
					plSub->m_LimitInfo.m_Mark = MULTL_XMLGetNodeMark(plXMLHolder, "mark");

					//GLOBAL_TRACE(("Low Limit = %d, Hight Limit = %d\n", plSub->m_LimitInfo.m_LowLimit, plSub->m_LimitInfo.m_HighLimit));
				}

				plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "sub", NULL, NULL, MXML_NO_DESCEND);
			}
		}

		MULTL_SaveMonitorXML(pHandle);
	}
	else
	{
		GLOBAL_ASSERT(0);
	}

}





void MULTL_WEBXMLScrambleConfig(MULT_Handle *pHandle, mxml_node_t* pBodyNode, CHAR_T* pSubType, CHAR_T *pParameter)
{
	CHAR_T *plTmpStr;

	if (GLOBAL_STRCMP(pSubType, "scs_setting") == 0)
	{
		U32 lSCSIDs, lACIDs;
		U32 lSuperCASID;
		mxml_node_t *plXMLHolder;
		mxml_node_t *plXMLCurrent;

		MPEG2_DBSCSInfo lSCSInfo;
		MPEG2_DBSCSACInfo lACInfo;

		lSCSIDs = MULTL_XMLGetNodeUINT(pBodyNode, "scs_ids", 10); 

		if (MPEG2_DBProcSCSInfo(pHandle->m_DBSHandle, lSCSIDs, &lSCSInfo, TRUE))
		{
			plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "scs_name");
			if (plTmpStr)
			{
				if (GLOBAL_STRLEN(plTmpStr) < MPEG2_DB_MAX_USER_DEFINED_NAME_BUF_LEN)
				{
					GLOBAL_STRCPY(lSCSInfo.m_pSCSName, plTmpStr);
				}
			}

			lSuperCASID = MULTL_XMLGetNodeUINT(pBodyNode, "supercas_id", 16);
			lSCSInfo.m_CASystemID = (lSuperCASID >> 16) & GLOBAL_U16_MAX;
			lSCSInfo.m_CASubSystemID = lSuperCASID & GLOBAL_U16_MAX;

			lSCSInfo.m_EMMPort = MULTL_XMLGetNodeUINT(pBodyNode, "emmg_port", 10);
			lSCSInfo.m_ECMPort = MULTL_XMLGetNodeUINT(pBodyNode, "ecmg_port", 10);
			lSCSInfo.m_ECMChnID = MULTL_XMLGetNodeUINT(pBodyNode, "ecm_chn_id", 16);
			plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "ecmg_ip");
			if (plTmpStr)
			{
				lSCSInfo.m_ECMIPv4 = PFC_SocketAToN(plTmpStr);
			}
			lSCSInfo.m_ActiveMark = MULTL_XMLGetNodeMark(pBodyNode, "active_mark");

			MPEG2_DBProcSCSInfo(pHandle->m_DBSHandle, lSCSIDs, &lSCSInfo, FALSE);

			plXMLHolder = mxmlFindElement(pBodyNode, pBodyNode, "remove_acs", NULL, NULL, MXML_DESCEND_FIRST);
			plXMLCurrent = mxmlFindElement(plXMLHolder, plXMLHolder, "ac", NULL, NULL, MXML_DESCEND_FIRST);
			while(plXMLCurrent)
			{
				lACIDs = MULTL_XMLGetNodeUINT(plXMLCurrent, "ac_ids", 16);
				if (!MPEG2_DBRemoveAC(pHandle->m_DBSHandle, lACIDs))
				{
					GLOBAL_TRACE(("Remove ACID = 0x%.8x Failed~~~~~~~~~~~~~~~~\n", lACIDs));
				}
				else
				{
					GLOBAL_TRACE(("Remove ACID = 0x%.8x Successful~~~~~~~~~~~~~~~~\n", lACIDs));
				}
				plXMLCurrent = mxmlFindElement(plXMLCurrent, plXMLHolder, "ac", NULL, NULL, MXML_NO_DESCEND);
			}



			plXMLHolder = mxmlFindElement(pBodyNode, pBodyNode, "add_modify_acs", NULL, NULL, MXML_DESCEND_FIRST);
			plXMLCurrent = mxmlFindElement(plXMLHolder, plXMLHolder, "ac", NULL, NULL, MXML_DESCEND_FIRST);
			while(plXMLCurrent)
			{
				lACIDs = MULTL_XMLGetNodeUINT(plXMLCurrent, "ac_ids", 16);

				if (lACIDs != 0)
				{
					MPEG2_DBProcACInfo(pHandle->m_DBSHandle, lACIDs, &lACInfo, TRUE);
				}
				else
				{
					GLOBAL_ZEROMEM(&lACInfo, sizeof(MPEG2_DBSCSACInfo));
				}

				plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "ac_name");
				if (plTmpStr)
				{
					if (GLOBAL_STRLEN(plTmpStr) < MPEG2_DB_MAX_USER_DEFINED_NAME_BUF_LEN)
					{
						GLOBAL_STRCPY(lACInfo.m_pACName, plTmpStr);
					}
				}

				plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "data");
				if (plTmpStr)
				{
					lACInfo.m_ACDataSize = CAL_StringHexToBin(plTmpStr, lACInfo.m_pAccessData, sizeof(lACInfo.m_pAccessData));
				}

				if (lACIDs == 0)
				{
					MPEG2_DBAddAC(pHandle->m_DBSHandle, lSCSIDs, &lACInfo);
				}
				else
				{
					MPEG2_DBProcACInfo(pHandle->m_DBSHandle, lACIDs, &lACInfo, FALSE);
				}
				plXMLCurrent = mxmlFindElement(plXMLCurrent, plXMLHolder, "ac", NULL, NULL, MXML_NO_DESCEND);
			}

		}
		MULTL_SetRemuxApplyMark(pHandle, FALSE);
	}
	else if (GLOBAL_STRCMP(pSubType, "advance_setting") == 0)
	{
		U8 lIndex;
		BOOL lScramble;
		mxml_node_t *plXMLCurrent;
		plXMLCurrent = mxmlFindElement(pBodyNode, pBodyNode, "es_setting", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			lIndex = MULTL_XMLGetNodeINT(plXMLCurrent, "es_type_value", 10);
			lScramble = MULTL_XMLGetNodeMark(plXMLCurrent,"scramble");
			MPEG2_DBSetEsStreamTypeMark(pHandle->m_DBSHandle, lIndex, lScramble);
			plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "es_setting", NULL, NULL, MXML_NO_DESCEND);
		}
		MULTL_SetRemuxApplyMark(pHandle, FALSE);
	}
	else if (GLOBAL_STRCMP(pSubType, "services") == 0)
	{
		U32 lServIDs, lSCSCAIDs;
		S32 lSCSCAIndex, lSCSCount;
		BOOL lTmpScrambleMark;
		mxml_node_t *plXMLCurrent;
		CHAR_T plNameBuf[32];
		MPEG2_DBSCSCAInfo lSCSCAInfo;
		MPEG2_DBServiceOutInfo lServInfo;

		lSCSCount = MPEG2_DBGetSCSCount(pHandle->m_DBSHandle);

		plXMLCurrent = mxmlFindElement(pBodyNode, pBodyNode, "service", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			lServIDs = MULTL_XMLGetNodeUINT(plXMLCurrent, "serv_ids", 10);
			if (MPEG2_DBGetServiceOutInfo(pHandle->m_DBSHandle, lServIDs, &lServInfo))
			{
				lTmpScrambleMark = MULTL_XMLGetNodeMark(plXMLCurrent, "scramble_mark");
				if (lTmpScrambleMark)
				{
					MPEG2_DB_SERVICE_MARK_SET_SCRAMBLE(lServInfo.m_ServiceMark);
				}
				else
				{
					MPEG2_DB_SERVICE_MARK_CLEAR_SCRAMBLE(lServInfo.m_ServiceMark);
				}
				MPEG2_DBSetServiceOutInfo(pHandle->m_DBSHandle, lServIDs, &lServInfo);
			}
			plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "service", NULL, NULL, MXML_NO_DESCEND);
		}

		plXMLCurrent = mxmlFindElement(pBodyNode, pBodyNode, "scs", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			lSCSCAIDs = MULTL_XMLGetNodeUINT(plXMLCurrent, "scs_ids", 10);
			if (MPEG2_DBGetSCSCAInfo(pHandle->m_DBSHandle, lSCSCAIDs, &lSCSCAInfo))
			{
				for (lSCSCAIndex = 0; lSCSCAIndex < lSCSCount; lSCSCAIndex++)
				{
					GLOBAL_SPRINTF((plNameBuf, "scs_acids_%d", lSCSCAIndex)); 
					lSCSCAInfo.m_pOutputCaInfo[lSCSCAIndex].m_SPInfo.m_ACIDs = MULTL_XMLGetNodeUINT(plXMLCurrent, plNameBuf, 16);
				}
				MPEG2_DBSetSCSCAInfo(pHandle->m_DBSHandle, lSCSCAIDs, &lSCSCAInfo);
			}
			plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "scs", NULL, NULL, MXML_NO_DESCEND);
		}
		MULTL_SetRemuxApplyMark(pHandle, FALSE);
	}
	else if (GLOBAL_STRCMP(pSubType, "settings") == 0)
	{
		MPEG2_DBSCSSystemInfo lSCSSystemInfo;
		S32 i;

		if (MPEG2_DBProcSCSSystemInfo(pHandle->m_DBSHandle, &lSCSSystemInfo, TRUE))
		{
			lSCSSystemInfo.m_DefaultCPDuration = (MULTL_XMLGetNodeUINT(pBodyNode, "default_cp", 10) & GLOBAL_U16_MAX);
			lSCSSystemInfo.m_NetworkDelay = (MULTL_XMLGetNodeUINT(pBodyNode, "network_delay", 10) & GLOBAL_U16_MAX);
			lSCSSystemInfo.m_bUserFixedCW = MULTL_XMLGetNodeMark(pBodyNode, "fixed_cw_mark");

			plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "fixed_cw");
			if (plTmpStr)
			{
				CAL_StringHexToBin(plTmpStr, lSCSSystemInfo.m_pFxiedCW, sizeof(lSCSSystemInfo.m_pFxiedCW));
			}

			MPEG2_DBProcSCSSystemInfo(pHandle->m_DBSHandle, &lSCSSystemInfo, FALSE);
		}
		MPEG2_DBProcSCSSystemInfo(pHandle->m_DBSHandle, &lSCSSystemInfo, FALSE);

		/*  add 2012-10-26-----------BSS-----------------*/
		{
			pHandle->m_BSSystemInfo.m_SuperCASID = MULTL_XMLGetNodeUINT(pBodyNode, "super_id", 16);
			pHandle->m_BSSystemInfo.m_ActiveMark = MULTL_XMLGetNodeMark(pBodyNode, "sw_mark");
			plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "bss_sw");
			if (plTmpStr)
			{
				CAL_StringHexToBin(plTmpStr, pHandle->m_BSSystemInfo.m_pSW, sizeof(pHandle->m_BSSystemInfo.m_pSW));
			}
			plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "key");						
			if (plTmpStr)
			{
				CAL_StringHexToBin(plTmpStr, pHandle->m_BSSystemInfo.m_pKey, sizeof(pHandle->m_BSSystemInfo.m_pKey));
			}			
		}




		MULTL_SetRemuxApplyMark(pHandle, FALSE);
	}
	else
	{
		GLOBAL_ASSERT(0);
	}
}


void MULTL_WEBXMLRemuxConfig(MULT_Handle *pHandle, mxml_node_t* pBodyNode, CHAR_T* pSubType, CHAR_T *pParameter)
{
	MULT_Config *plConfig;
	CHAR_T *plTmpStr;


	plConfig = &pHandle->m_Configuration;

	if (GLOBAL_STRCMP(pSubType, "remux_setting") == 0)
	{
		MPEG2_DBRemuxInfo lRemuxInfo;
		if (MPEG2_DBProcRemuxSystemInfo(pHandle->m_DBSHandle, &lRemuxInfo, TRUE))
		{
			lRemuxInfo.m_DefaultInputCharSet = MULTL_XMLGetNodeUINT(pBodyNode, "input_charset", 10);
			lRemuxInfo.m_DefaultOutputCharSet = MULTL_XMLGetNodeUINT(pBodyNode, "output_charset", 10);
			lRemuxInfo.m_OutCharsetMarker = MULTL_XMLGetNodeUINT(pBodyNode, "out_charset_marker", 10);
			lRemuxInfo.m_TimeZone = MULTL_XMLGetNodeINT(pBodyNode, "time_zone", 10);
			lRemuxInfo.m_TimeUpdateCycle = MULTL_XMLGetNodeINT(pBodyNode, "time_update_cycle", 10);
			lRemuxInfo.m_TOTMark = MULTL_XMLGetNodeMark(pBodyNode, "tot_mark");
			plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "country_code");
			GLOBAL_MEMCPY(lRemuxInfo.m_pCountryCode, "   ", 3);
			if (plTmpStr)
			{
				if (GLOBAL_STRLEN(plTmpStr) >= 3)
				{
					GLOBAL_MEMCPY(lRemuxInfo.m_pCountryCode, plTmpStr, 3);
				}
			}
			lRemuxInfo.m_VersionAutoINCMark = MULTL_XMLGetNodeMark(pBodyNode, "auto_inc");

#if defined(GM2730S)//SPTS模式专用
			{
				MPEG2_DBSPTSParam pSPTSParam;
				MPEG2_DBProcSPTSMode(pHandle->m_DBSHandle, &pSPTSParam, TRUE);
				pSPTSParam.m_SPTSMark = TRUE;
				pSPTSParam.m_UseECM = FALSE;
				pSPTSParam.m_UseSDT = MULTL_XMLGetNodeMark(pBodyNode, "spts_sdt_mark");
				MPEG2_DBProcSPTSMode(pHandle->m_DBSHandle, &pSPTSParam, FALSE);
			}
#endif
			MPEG2_DBProcRemuxSystemInfo(pHandle->m_DBSHandle, &lRemuxInfo, FALSE);
		}


#ifdef MULT_SYSTEM_HAVE_PCR_CORRECT_ADJUST_FUNCTION
		{
			MULT_PCRCorrect *plPCRCorrect = &pHandle->m_Parameter.m_PCRCorrect;
			plPCRCorrect->m_PCRCMark = MULTL_XMLGetNodeMarkDefault(pBodyNode, "pcrc_mark", TRUE);
			plPCRCorrect->m_PCRCPos = MULTL_XMLGetNodeUINTDefault(pBodyNode, "pcrc_pos", 10, MULT_PCR_CORRECT_POS_DEFAULT_VALUE);
			plPCRCorrect->m_PCRCNeg = MULTL_XMLGetNodeUINTDefault(pBodyNode, "pcrc_neg", 10, MULT_PCR_CORRECT_NEG_DEFAULT_VALUE);
		}
#endif



		MULTL_SetRemuxApplyMark(pHandle, FALSE);
	}
	else if(GLOBAL_STRCMP(pSubType, "input_batch_analyse") == 0)
	{

		S32 lAnalyseTimeout, lAnalyseCharset;
		BOOL lbRemoveService;

		lAnalyseTimeout = MULTL_XMLGetNodeINT(pBodyNode, "analyse_timeout", 10);
		lAnalyseCharset = MULTL_XMLGetNodeINT(pBodyNode, "charset", 10);
		lbRemoveService = MULTL_XMLGetNodeMark(pBodyNode, "analyse_remove_service");

		GLOBAL_TRACE(("Got Analyse ALL CMD DevInd  CharSet = %d, Timeout = %d\n", lAnalyseCharset, lAnalyseTimeout));

		/*发起搜索命令，系统进入等待状态！*/
		MULT_AnalyzeBatchStart(pHandle, 0, lAnalyseTimeout, lAnalyseCharset, !lbRemoveService);		

	}

	else if (GLOBAL_STRCMP(pSubType, "batch_output_service_config") == 0)
	{

		S16 lOutTsIndex;
		U32 lServIDs, lTsIDs;
		S32 lServRouteTsIndex;	
		MPEG2_DBServiceOutInfo lServOutInfo;
		MPEG2_DBTsRouteInfo lTsRouteInfo;
		mxml_node_t *plXMLCurrent;	

		plXMLCurrent = mxmlFindElement(pBodyNode, pBodyNode, "remuxed_service", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "serv_ids");
			if (plTmpStr)
			{
				lServIDs = GLOBAL_STRTOUL(plTmpStr, NULL, 10);
			}

			plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "out_ts_index");
			if (plTmpStr)
			{
				if (GLOBAL_STRCMP(plTmpStr, "OFF") == 0)
				{
					lServRouteTsIndex = GLOBAL_INVALID_INDEX;
				}
				else
				{
					lServRouteTsIndex = GLOBAL_STRTOL(plTmpStr, NULL, 10);
				}

			}			
			MPEG2_DBTsServiceRoute(pHandle->m_DBSHandle, lServIDs, lServRouteTsIndex);		


			plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "remuxed_service", NULL, NULL, MXML_NO_DESCEND);
		}


		plXMLCurrent = mxmlFindElement(pBodyNode, pBodyNode, "serv_ids", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			lServIDs = MULTL_XMLGetNodeINT(plXMLCurrent, "serv_ids", 10);

			lOutTsIndex = MULTL_XMLGetNodeINT(plXMLCurrent, "out_ts_index", 10);

			if (MPEG2_DBGetServiceOutInfo(pHandle->m_DBSHandle,  lServIDs, &lServOutInfo))
			{
				lServOutInfo.m_ServiceID = MULTL_XMLGetNodeINT(plXMLCurrent, "out_service_id", 10);
				plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "out_service_name");
				if (plTmpStr)
				{
					if (GLOBAL_STRLEN(plTmpStr) < MPEG2_DB_MAX_SERVICE_NAME_BUF_LEN)
					{
						GLOBAL_STRCPY(lServOutInfo.m_ServiceName, plTmpStr);
					}
				}

				MPEG2_DBSetServiceOutInfo(pHandle->m_DBSHandle, lServIDs, &lServOutInfo);


			}			

			plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "serv_ids", NULL, NULL, MXML_NO_DESCEND);
		}


		MULTL_SetRemuxApplyMark(pHandle, FALSE);	
	}
	else if (GLOBAL_STRCMP(pSubType, "input_service_remux") == 0)
	{
		S32 lInTsIndex, lServRouteTsIndex;
		MPEG2_DBTsRouteInfo lTsRouteInfo;
		U32 lCAIDs, lServIDs, lTsIDs;
		mxml_node_t *plXMLCurrent;
		BOOL lOutMark;
		plXMLCurrent = mxmlFindElement(pBodyNode, pBodyNode, "remuxed_service", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "serv_ids");
			if (plTmpStr)
			{
				lServIDs = GLOBAL_STRTOUL(plTmpStr, NULL, 10);
			}

			plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "out_ts_index");
			if (plTmpStr)
			{
				if (GLOBAL_STRCMP(plTmpStr, "OFF") == 0)
				{
					lServRouteTsIndex = GLOBAL_INVALID_INDEX;
				}
				else
				{
					lServRouteTsIndex = GLOBAL_STRTOL(plTmpStr, NULL, 10);
#ifdef GN2000
					if (lServRouteTsIndex == 0x00)
					{
						lServRouteTsIndex = MULT_DECODER_OUT_TS_IND;
					}
#endif

				}
			}

			//GLOBAL_TRACE(("ServiceMap Out Ts Index= %d\n", lServRouteTsIndex));
			MPEG2_DBTsServiceRoute(pHandle->m_DBSHandle, lServIDs, lServRouteTsIndex);
			plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "remuxed_service", NULL, NULL, MXML_NO_DESCEND);
		}


		plXMLCurrent = mxmlFindElement(pBodyNode, pBodyNode, "remuxed_ca", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			lOutMark = FALSE;
			plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "mux_ca_ids");
			if (plTmpStr)
			{
				lCAIDs = GLOBAL_STRTOUL(plTmpStr, NULL, 10);
			}

			lOutMark = MULTL_XMLGetNodeMark(plXMLCurrent, "out_mark");
			MPEG2_DBTsMuxCARoute(pHandle->m_DBSHandle, lCAIDs, lOutMark);
			plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "remuxed_ca", NULL, NULL, MXML_NO_DESCEND);
		}


		lInTsIndex = MULTL_XMLGetNodeINT(pBodyNode, "in_ts_index", 10);
		lTsIDs = MPEG2_DBGetTsIDs(pHandle->m_DBSHandle, TRUE, lInTsIndex);

		GLOBAL_ZEROMEM(&lTsRouteInfo, sizeof(MPEG2_DBTsRouteInfo));

		//lTsRouteInfo.m_TsIndex = MULTL_XMLGetNodeINT(pBodyNode, "ts_route_index", 10);
		//lTsRouteInfo.m_ActiveMark = MULTL_XMLGetNodeMark(pBodyNode, "ts_route_mark");

		//GLOBAL_TRACE(("Route out Ts = %d, Mark = %d\n", lTsRouteInfo.m_TsIndex, lTsRouteInfo.m_ActiveMark));

		//MPEG2_DBSetTsRouteInfo(pHandle->m_DBSHandle, lTsIDs, TRUE, &lTsRouteInfo);
		MULTL_SetRemuxApplyMark(pHandle, FALSE);
	}
	else if (GLOBAL_STRCMP(pSubType, "input_service_analyse") == 0)
	{
		S32 lTsIndex;
		S32 lAnalyseTimeout, lAnalyseCharset, lAnalyseAutomapMode;
		BOOL lbRemoveService;

		lTsIndex = MULTL_XMLGetNodeINT(pBodyNode, "input_ts_index", 10);
		lAnalyseTimeout = MULTL_XMLGetNodeINT(pBodyNode, "analyse_timeout", 10);
		lAnalyseCharset = MULTL_XMLGetNodeINT(pBodyNode, "charset", 10);
		lbRemoveService = MULTL_XMLGetNodeMark(pBodyNode, "analyse_remove_service");
#if MULT_SYSTEM_ENABLE_AUTO_MAP_FUNCTION
		lAnalyseAutomapMode = MULTL_XMLGetNodeINT(pBodyNode, "auto_map", 10);
#endif

		GLOBAL_TRACE(("Got Analyse CMD TsIndex = %d, Timeout = %d\n", lTsIndex, lAnalyseTimeout));

		/*发起搜索命令，系统进入等待状态！*/

		TSP_SetCurrentWorkTsIndex(lTsIndex);
		MULT_AnalyzeStart(pHandle, 0, lTsIndex, lAnalyseTimeout, lAnalyseCharset, !lbRemoveService, lAnalyseAutomapMode);//20120829 默认删除节目
	}
	else if (GLOBAL_STRCMP(pSubType, "pidmap") == 0)
	{
		U32 lIDs;
		mxml_node_t *plXMLHolder;
		mxml_node_t *plXMLCurrent;
		MPEG2_DBPIDMapInfo lPIDMapInfo;

		/*先删除，避免数据库容量问题*/
		plXMLHolder = mxmlFindElement(pBodyNode, pBodyNode, "remove_map_items", NULL, NULL, MXML_DESCEND_FIRST);

		plXMLCurrent = mxmlFindElement(plXMLHolder, plXMLHolder, "map_item", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "pidmap_ids");
			if (plTmpStr)
			{
				lIDs = GLOBAL_STRTOUL(plTmpStr, NULL, 10);
				MPEG2_DBRemovePIDMap(pHandle->m_DBSHandle, lIDs);
			}

			plXMLCurrent = mxmlFindElement(plXMLCurrent, plXMLHolder, "map_item", NULL, NULL, MXML_NO_DESCEND);
		}


		plXMLHolder = mxmlFindElement(pBodyNode, pBodyNode, "add_modify_map_items", NULL, NULL, MXML_DESCEND_FIRST);

		plXMLCurrent = mxmlFindElement(plXMLHolder, plXMLHolder, "map_item", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "pidmap_ids");
			if (plTmpStr)
			{
				lIDs = GLOBAL_STRTOUL(plTmpStr, NULL, 10);
			}

			lPIDMapInfo.m_InTsIndex = MULTL_XMLGetNodeINT(plXMLCurrent, "in_ts_index", 10);
			lPIDMapInfo.m_InPID = MULTL_XMLGetNodeINT(plXMLCurrent, "in_pid", 10);
			lPIDMapInfo.m_OutTsIndex = MULTL_XMLGetNodeINT(plXMLCurrent, "out_ts_index", 10);
			lPIDMapInfo.m_OutPID = MULTL_XMLGetNodeINT(plXMLCurrent, "out_pid", 10);
			lPIDMapInfo.m_OutputMark = MULTL_XMLGetNodeMark(plXMLCurrent, "active_mark");

			if (lIDs == 0)
			{
				MPEG2_DBAddPIDMap(pHandle->m_DBSHandle, &lPIDMapInfo);
			}
			else
			{
				MPEG2_DBSetPIDMapInfo(pHandle->m_DBSHandle, lIDs, &lPIDMapInfo);
			}

			plXMLCurrent = mxmlFindElement(plXMLCurrent, plXMLHolder, "map_item", NULL, NULL, MXML_NO_DESCEND);
		}

		MULTL_SetRemuxApplyMark(pHandle, FALSE);
	}
	else if (GLOBAL_STRCMP(pSubType, "manual_ts_inserter") == 0)
	{
		U32 lIDs;
		mxml_node_t *plXMLHolder;
		mxml_node_t *plXMLCurrent;
		MPEG2_DBManualTsInfo lManualTsInfo;

		/*先删除，避免数据库容量问题*/
		plXMLHolder = mxmlFindElement(pBodyNode, pBodyNode, "remove_items", NULL, NULL, MXML_DESCEND_FIRST);

		plXMLCurrent = mxmlFindElement(plXMLHolder, plXMLHolder, "item", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			lIDs = MULTL_XMLGetNodeUINT(plXMLCurrent, "ids", 10);
			MPEG2_DBMANRemove(pHandle->m_DBSHandle, lIDs);
			plXMLCurrent = mxmlFindElement(plXMLCurrent, plXMLHolder, "item", NULL, NULL, MXML_NO_DESCEND);
		}


		plXMLHolder = mxmlFindElement(pBodyNode, pBodyNode, "add_modify_items", NULL, NULL, MXML_DESCEND_FIRST);
		plXMLCurrent = mxmlFindElement(plXMLHolder, plXMLHolder, "item", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			lIDs = MULTL_XMLGetNodeUINT(plXMLCurrent, "ids", 10);
			if (lIDs != 0)
			{
				if (MPEG2_DBMANProcTsInfo(pHandle->m_DBSHandle, lIDs, &lManualTsInfo, TRUE))
				{
					plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "name");
					if (plTmpStr)
					{
						if (GLOBAL_STRLEN(plTmpStr) < MPEG2_DB_MANUAL_INSERTER_NAME_SIZE)
						{
							GLOBAL_STRCPY(lManualTsInfo.m_pName, plTmpStr);
						}
					}
					else
					{
						lManualTsInfo.m_pName[0] = 0;//2013.2.23判断插入描述是否为空0
					}


					lManualTsInfo.m_Bitrate = MULTL_XMLGetNodeINT(plXMLCurrent, "bitrate", 10);
					lManualTsInfo.m_OutTsIndex = MULTL_XMLGetNodeINT(plXMLCurrent, "out_ts_index", 10);
					lManualTsInfo.m_OutMark = MULTL_XMLGetNodeMark(plXMLCurrent, "active_mark");
					MPEG2_DBMANProcTsInfo(pHandle->m_DBSHandle, lIDs, &lManualTsInfo, FALSE);
				}
			}
			plXMLCurrent = mxmlFindElement(plXMLCurrent, plXMLHolder, "item", NULL, NULL, MXML_NO_DESCEND);
		}
		MULTL_SetRemuxApplyMark(pHandle, FALSE);
		
	}
	else if (GLOBAL_STRCMP(pSubType, "nit_information") == 0)
	{
		U32 lIDs;
		mxml_node_t *plXMLHolder;
		mxml_node_t *plXMLCurrent;
		mxml_node_t *plXMLInfo;
		MPEG2_DBNITInfo lNitInfo;
		MPEG2_DBNITTsInfo lNitTsInfo;

		MPEG2_DBGetNITNetworkInfo(pHandle->m_DBSHandle, &lNitInfo);
		lNitInfo.m_NetworkID = MULTL_XMLGetNodeINT(pBodyNode, "network_id", 10);
		lNitInfo.m_VersionNum = MULTL_XMLGetNodeINT(pBodyNode, "version", 10);
		lNitInfo.m_ActiveMark = MULTL_XMLGetNodeMark(pBodyNode, "nit_global_mark");
		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "network_name");
		if (plTmpStr)
		{
			if (GLOBAL_STRLEN(plTmpStr) < MPEG2_DB_MAX_SERVICE_NAME_BUF_LEN)
			{
				GLOBAL_STRCPY(lNitInfo.m_pNetworkName, plTmpStr);
			}
		}
		MPEG2_DBSetNITNetworkInfo(pHandle->m_DBSHandle, &lNitInfo);

		plXMLHolder = mxmlFindElement(pBodyNode, pBodyNode, "remove_nit_tsinfo", NULL, NULL, MXML_DESCEND_FIRST);
		plXMLCurrent = mxmlFindElement(plXMLHolder, plXMLHolder, "ts_info", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "nit_tsinfo_ids");
			if (plTmpStr)
			{
				lIDs = GLOBAL_STRTOUL(plTmpStr, NULL, 10);
				MPEG2_DBRemoveNITTsInfo(pHandle->m_DBSHandle, lIDs);
			}

			plXMLCurrent = mxmlFindElement(plXMLCurrent, plXMLHolder, "ts_info", NULL, NULL, MXML_NO_DESCEND);
		}


		plXMLHolder = mxmlFindElement(pBodyNode, pBodyNode, "add_modify_nit_tsinfo", NULL, NULL, MXML_DESCEND_FIRST);
		plXMLCurrent = mxmlFindElement(plXMLHolder, plXMLHolder, "ts_info", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			lIDs = 0;
			plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "nit_ts_ids");
			if (plTmpStr)
			{
				lIDs = GLOBAL_STRTOUL(plTmpStr, NULL, 10);
			}

			if (lIDs != 0)
			{
				MPEG2_DBGetNITTsInfo(pHandle->m_DBSHandle, lIDs, &lNitTsInfo);
			}
			else
			{
				GLOBAL_ZEROMEM(&lNitTsInfo, sizeof(MPEG2_DBNITTsInfo));
			}


			lNitTsInfo.m_TsID = MULTL_XMLGetNodeINT(plXMLCurrent, "ts_id", 10);
			lNitTsInfo.m_ONID = MULTL_XMLGetNodeINT(plXMLCurrent, "on_id", 10);

			plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "delivery_type");
			if (plTmpStr)
			{
				S32 lDeliverType;
				lDeliverType = MULTL_XMLDeliveryTypeFromStr(plTmpStr);
				switch (lDeliverType)
				{
				case MPEG2_PSI_CABLE_DELIVERY_SYSTEM_DESCRIPTOR_TAG:
					{
						lNitTsInfo.m_DeliveryInfo.m_Type = lDeliverType;
						plXMLInfo = mxmlFindElement(plXMLCurrent, plXMLCurrent, "delivery_info", NULL, NULL, MXML_DESCEND_FIRST);
						lNitTsInfo.m_DeliveryInfo.m_Descriptor.m_Cable.m_frequency = MULTL_XMLGetNodeINT(plXMLInfo, "freq", 10);
						lNitTsInfo.m_DeliveryInfo.m_Descriptor.m_Cable.m_modulation = MULTL_XMLCableDeliveryModeValueFromStr(MULTL_XMLGetNodeText(plXMLInfo, "mode"));
						lNitTsInfo.m_DeliveryInfo.m_Descriptor.m_Cable.m_symbol_rate = MULTL_XMLGetNodeINT(plXMLInfo, "symbol_rate", 10);
						lNitTsInfo.m_DeliveryInfo.m_Descriptor.m_Cable.m_FEC_inner = MULTL_XMLGetNodeINT(plXMLInfo, "fec_inner", 10);
						lNitTsInfo.m_DeliveryInfo.m_Descriptor.m_Cable.m_FEC_outer = MULTL_XMLGetNodeINT(plXMLInfo, "fec_outer", 10);
					}
					break;
				default:
					break;
				}
			}

			if (lIDs == 0)
			{
				MPEG2_DBAddNITTsInfo(pHandle->m_DBSHandle, &lNitTsInfo);
			}
			else
			{
				MPEG2_DBSetNITTsInfo(pHandle->m_DBSHandle, lIDs, &lNitTsInfo);
			}

			plXMLCurrent = mxmlFindElement(plXMLCurrent, plXMLHolder, "ts_info", NULL, NULL, MXML_NO_DESCEND);
		}
		MULTL_SetRemuxApplyMark(pHandle, FALSE);
		
	}
	else if (GLOBAL_STRCMP(pSubType, "eca_setting") == 0)
	{
#ifdef MULT_ENABLE_ECA_AND_SERVICE_LIST
		MULTL_XMLLoadECA(pHandle, pBodyNode, TRUE);
		MULTL_SetRemuxApplyMark(pHandle, FALSE);
#endif
	}
	else if (GLOBAL_STRCMP(pSubType, "servl_setting") == 0)
	{
#ifdef MULT_ENABLE_ECA_AND_SERVICE_LIST
		MULTL_XMLLoadSERVL(pHandle, pBodyNode, TRUE);
		MULTL_SetRemuxApplyMark(pHandle, FALSE);
#endif
	}
	else if (GLOBAL_STRCMP(pSubType, "service_auth") == 0)
	{
#ifdef MULT_ENABLE_ECA_AND_SERVICE_LIST			
		U32 lServIDs;
		mxml_node_t *plXMLCurrent;
		MPEG2_DBServiceOutInfo lServOutInfo;

		plXMLCurrent = mxmlFindElement(pBodyNode, pBodyNode, "service", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			lServIDs = MULTL_XMLGetNodeINT(plXMLCurrent, "serv_ids", 10);
			if (MPEG2_DBGetServiceOutInfo(pHandle->m_DBSHandle,  lServIDs, &lServOutInfo))
			{
				lServOutInfo.m_AuthClass = MULTL_XMLGetNodeUINT(plXMLCurrent, "auth_class", 10);
				lServOutInfo.m_LCN = MULTL_XMLGetNodeUINT(plXMLCurrent, "lcn", 10);
				lServOutInfo.m_LCNVisibale = MULTL_XMLGetNodeMark(plXMLCurrent, "lcn_visiable");
				GLOBAL_TRACE(("LCN Visiable Mark = %d\n", lServOutInfo.m_LCNVisibale));
				lServOutInfo.m_BouquetID = MULTL_XMLGetNodeUINT(plXMLCurrent, "bouquet_id", 10);
				MPEG2_DBSetServiceOutInfo(pHandle->m_DBSHandle, lServIDs, &lServOutInfo);
			}
			plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "service", NULL, NULL, MXML_NO_DESCEND);
		}
		MULTL_SetRemuxApplyMark(pHandle, FALSE);
#endif
	}
	else if (GLOBAL_STRCMP(pSubType, "output_service_config") == 0)
	{
		S32 lSCSCAIndex, lSCSCount;
		S16 lOutTsIndex;
		U32 lTsIDs, lServIDs, lEsIDs, lSCSCAIDs, lMuxCAIDs, lDescIDs;
		MPEG2_DBTsOutInfo lTsOutInfo;
		MPEG2_DBServiceOutInfo lServOutInfo;
		MPEG2_DBEsOutInfo lEsOutInfo;
		MPEG2_DBCaOutInfo lCAOutInfo;
		MPEG2_DBDescriptorInfo lDescInfo;
		MPEG2_DBSCSCAInfo lSCSCAInfo;
		MPEG2_DBTsRouteInfo lTsRouteInfo;
		mxml_node_t *plXMLHolder;
		mxml_node_t *plXMLCurrent;
		CHAR_T plNameBuf[32];

		GLOBAL_ZEROMEM(&lTsOutInfo, sizeof(MPEG2_DBTsOutInfo));
		plXMLHolder = mxmlFindElement(pBodyNode, pBodyNode, "ts", NULL, NULL, MXML_DESCEND_FIRST);

		lOutTsIndex = MULTL_XMLGetNodeINT(plXMLHolder, "out_ts_index", 10);

		lTsIDs = MPEG2_DBGetTsIDs(pHandle->m_DBSHandle, FALSE, lOutTsIndex);
		MPEG2_DBGetTsOutInfo(pHandle->m_DBSHandle, lTsIDs, &lTsOutInfo);

		lTsOutInfo.m_TsID = MULTL_XMLGetNodeINT(plXMLHolder, "ts_id", 10);
		lTsOutInfo.m_ONID = MULTL_XMLGetNodeINT(plXMLHolder, "on_id", 10);

		lTsOutInfo.m_PATVersion = MULTL_XMLGetNodeINT(plXMLHolder, "pat_v", 10);
		lTsOutInfo.m_PATInterval = MULTL_XMLGetNodeINT(plXMLHolder, "pat_i", 10);
		lTsOutInfo.m_PATActiveMark = MULTL_XMLGetNodeMark(plXMLHolder, "pat_mark");

		lTsOutInfo.m_CATVersion = MULTL_XMLGetNodeINT(plXMLHolder, "cat_v", 10);
		lTsOutInfo.m_CATInterval = MULTL_XMLGetNodeINT(plXMLHolder, "cat_i", 10);
		lTsOutInfo.m_CATActiveMark = MULTL_XMLGetNodeMark(plXMLHolder, "cat_mark");

		lTsOutInfo.m_SDTVersion = MULTL_XMLGetNodeINT(plXMLHolder, "sdt_v", 10);
		lTsOutInfo.m_SDTInterval = MULTL_XMLGetNodeINT(plXMLHolder, "sdt_i", 10);
		lTsOutInfo.m_SDTActiveMark = MULTL_XMLGetNodeMark(plXMLHolder, "sdt_mark");

		lTsOutInfo.m_NITInterval = MULTL_XMLGetNodeINT(plXMLHolder, "nit_i", 10);
		lTsOutInfo.m_NITActiveMark = MULTL_XMLGetNodeMark(plXMLHolder, "nit_mark");

		lTsOutInfo.m_TDTTOTInterval = MULTL_XMLGetNodeINT(plXMLHolder, "tdt_interval", 10);
		lTsOutInfo.m_TDTTOTActiveMark = MULTL_XMLGetNodeMark(plXMLHolder, "tdt_mark");

		MPEG2_DBSetTsOutInfo(pHandle->m_DBSHandle, lTsIDs, &lTsOutInfo);


		plXMLHolder = mxmlFindElement(pBodyNode, pBodyNode, "services", NULL, NULL, MXML_DESCEND_FIRST);
		plXMLCurrent = mxmlFindElement(plXMLHolder, plXMLHolder, "service", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			lServIDs = MULTL_XMLGetNodeINT(plXMLCurrent, "serv_ids", 10);
			if (MPEG2_DBGetServiceOutInfo(pHandle->m_DBSHandle,  lServIDs, &lServOutInfo))
			{
				if (MULTL_XMLGetNodeMark(plXMLCurrent, "serv_remove_mark") == TRUE)
				{
					MPEG2_DBSetServiceTsIndex(pHandle->m_DBSHandle, lServIDs, GLOBAL_INVALID_INDEX, FALSE);
				}
				else
				{
					plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "serv_name");
					if (plTmpStr)
					{
						if (GLOBAL_STRLEN(plTmpStr) < MPEG2_DB_MAX_SERVICE_NAME_BUF_LEN)
						{
							GLOBAL_STRCPY(lServOutInfo.m_ServiceName, plTmpStr);
						}
					}

					plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "serv_provider_name");
					if (plTmpStr)
					{
						if (GLOBAL_STRLEN(plTmpStr) < MPEG2_DB_MAX_SERVICE_NAME_BUF_LEN)
						{
							GLOBAL_STRCPY(lServOutInfo.m_ServiceProviderName, plTmpStr);
						}
					}

					lServOutInfo.m_ServiceID = MULTL_XMLGetNodeINT(plXMLCurrent, "serv_id", 10);
					lServOutInfo.m_ServiceType = MULTL_XMLGetNodeINT(plXMLCurrent, "serv_type", 10);
					lServOutInfo.m_ServiceType = MULTL_XMLGetNodeINT(plXMLCurrent, "serv_type", 10);
					lServOutInfo.m_PMTPID = MULTL_XMLGetNodeINT(plXMLCurrent, "serv_pmt_pid", 10);
					lServOutInfo.m_PMTVersion = MULTL_XMLGetNodeINT(plXMLCurrent, "serv_pmt_version", 10);
					lServOutInfo.m_PMTInterval = MULTL_XMLGetNodeINT(plXMLCurrent, "serv_pmt_interval", 10);
					lServOutInfo.m_PMTActiveMark = MULTL_XMLGetNodeMark(plXMLCurrent, "serv_pmt_mark");
					lServOutInfo.m_PCRPID = MULTL_XMLGetNodeINT(plXMLCurrent, "serv_pcr_pid", 10);
#ifdef TRANS_PCR_PID_FUNCTION
					lServOutInfo.m_TransPCRPID = MULTL_XMLGetNodeINT(plXMLCurrent, "trans_pcr_pid", 10);
#endif
// 					lServOutInfo.m_PCRMark = MULTL_XMLGetNodeMark(plXMLCurrent, "serv_pcr_mark");

					if (MULTL_XMLGetNodeMark(plXMLCurrent, "serv_eit_sc_mark"))
					{
						MPEG2_DB_SERVICE_MARK_SET_HAVE_EIT_SCHEDULE(lServOutInfo.m_ServiceMark);
					}
					else
					{
						MPEG2_DB_SERVICE_MARK_CLEAR_HAVE_EIT_SCHEDULE(lServOutInfo.m_ServiceMark);
					}

					if (MULTL_XMLGetNodeMark(plXMLCurrent, "serv_eit_cn_mark"))
					{
						MPEG2_DB_SERVICE_MARK_SET_HAVE_EIT_CUR_NEXT(lServOutInfo.m_ServiceMark);
					}
					else
					{
						MPEG2_DB_SERVICE_MARK_CLEAR_HAVE_EIT_CUR_NEXT(lServOutInfo.m_ServiceMark);
					}

					MPEG2_DBSetServiceOutInfo(pHandle->m_DBSHandle, lServIDs, &lServOutInfo);
				}
			}

			plXMLCurrent = mxmlFindElement(plXMLCurrent, plXMLHolder, "service", NULL, NULL, MXML_NO_DESCEND);
		}

		plXMLHolder = mxmlFindElement(pBodyNode, pBodyNode, "element_streams", NULL, NULL, MXML_DESCEND_FIRST);
		plXMLCurrent = mxmlFindElement(plXMLHolder, plXMLHolder, "es", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			lEsIDs = MULTL_XMLGetNodeINT(plXMLCurrent, "es_ids", 10);
			if (MPEG2_DBGetEsOutInfo(pHandle->m_DBSHandle, lEsIDs, &lEsOutInfo))
			{
				lEsOutInfo.m_EsPID = MULTL_XMLGetNodeINT(plXMLCurrent, "es_pid", 10);
				lEsOutInfo.m_EsType = MULTL_XMLGetNodeINT(plXMLCurrent, "es_type", 10);
				lEsOutInfo.m_OutputMark = MULTL_XMLGetNodeMark(plXMLCurrent, "es_mark");
				lEsOutInfo.m_ScrambleMark = MULTL_XMLGetNodeMark(plXMLCurrent, "es_scr");
				MPEG2_DBSetEsOutInfo(pHandle->m_DBSHandle, lEsIDs, &lEsOutInfo);
			}
			plXMLCurrent = mxmlFindElement(plXMLCurrent, plXMLHolder, "es", NULL, NULL, MXML_NO_DESCEND);
		}


		plXMLHolder = mxmlFindElement(pBodyNode, pBodyNode, "mux_cas", NULL, NULL, MXML_DESCEND_FIRST);
		plXMLCurrent = mxmlFindElement(plXMLHolder, plXMLHolder, "ca", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			lMuxCAIDs = MULTL_XMLGetNodeINT(plXMLCurrent, "ca_ids", 10);
			if (MPEG2_DBGetMuxCAOutInfo(pHandle->m_DBSHandle, lMuxCAIDs, &lCAOutInfo))
			{
				lCAOutInfo.m_CaPID = MULTL_XMLGetNodeINT(plXMLCurrent, "ca_pid", 10);
				lCAOutInfo.m_OutputMark = MULTL_XMLGetNodeMark(plXMLCurrent, "ca_mark");
				MPEG2_DBSetMuxCAOutInfo(pHandle->m_DBSHandle, lMuxCAIDs, &lCAOutInfo);
			}
			plXMLCurrent = mxmlFindElement(plXMLCurrent, plXMLHolder, "ca", NULL, NULL, MXML_NO_DESCEND);
		}

		plXMLHolder = mxmlFindElement(pBodyNode, pBodyNode, "descriptors", NULL, NULL, MXML_DESCEND_FIRST);
		plXMLCurrent = mxmlFindElement(plXMLHolder, plXMLHolder, "desc", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			lDescIDs = MULTL_XMLGetNodeINT(plXMLCurrent, "desc_ids", 10);
			if (MPEG2_DBGetDescriptorInfo(pHandle->m_DBSHandle, lDescIDs, &lDescInfo))
			{
				lDescInfo.m_OutputMark = MULTL_XMLGetNodeMark(plXMLCurrent, "desc_mark");//对复用描述符仅仅能修改输出标志
				MPEG2_DBSetDescriptorInfo(pHandle->m_DBSHandle, lDescIDs, &lDescInfo);
			}
			plXMLCurrent = mxmlFindElement(plXMLCurrent, plXMLHolder, "desc", NULL, NULL, MXML_NO_DESCEND);
		}



		lSCSCount = MPEG2_DBGetSCSCount(pHandle->m_DBSHandle);
		plXMLHolder = mxmlFindElement(pBodyNode, pBodyNode, "scs_cas", NULL, NULL, MXML_DESCEND_FIRST);
		plXMLCurrent = mxmlFindElement(plXMLHolder, plXMLHolder, "ca", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			lSCSCAIDs = MULTL_XMLGetNodeINT(plXMLCurrent, "ca_ids", 10);
			if (MPEG2_DBGetSCSCAInfo(pHandle->m_DBSHandle, lSCSCAIDs, &lSCSCAInfo))
			{
				for (lSCSCAIndex = 0; lSCSCAIndex < lSCSCount; lSCSCAIndex++)
				{
					GLOBAL_SPRINTF((plNameBuf, "ca_pid_%d", lSCSCAIndex));
					lSCSCAInfo.m_pOutputCaInfo[lSCSCAIndex].m_CaPID = MULTL_XMLGetNodeINT(plXMLCurrent, plNameBuf, 10);
					GLOBAL_SPRINTF((plNameBuf, "ca_mark_%d", lSCSCAIndex));
					lSCSCAInfo.m_pOutputCaInfo[lSCSCAIndex].m_OutputMark = MULTL_XMLGetNodeMark(plXMLCurrent, plNameBuf);

					//XUEJING配合数据库修改增加CA_PRIVATE_INFO编辑功能！2016-4-7
					{
						GLOBAL_SPRINTF((plNameBuf, "scs_ca_private_data_%d", lSCSCAIndex));
						lSCSCAInfo.m_pCAPrivateData[lSCSCAIndex].m_PrivateDataLen = CAL_StringHexToBin(MULTL_XMLGetNodeText(plXMLCurrent, plNameBuf), lSCSCAInfo.m_pCAPrivateData[lSCSCAIndex].m_pPrivateData, sizeof(lSCSCAInfo.m_pCAPrivateData[lSCSCAIndex].m_pPrivateData));
						GLOBAL_SPRINTF((plNameBuf, "scs_ca_private_mark_%d", lSCSCAIndex));
						lSCSCAInfo.m_pCAPrivateData[lSCSCAIndex].m_OutputMark = MULTL_XMLGetNodeMark(plXMLCurrent, plNameBuf);
					}

				}
				MPEG2_DBSetSCSCAInfo(pHandle->m_DBSHandle, lSCSCAIDs, &lSCSCAInfo);
			}
			plXMLCurrent = mxmlFindElement(plXMLCurrent, plXMLHolder, "ca", NULL, NULL, MXML_NO_DESCEND);
		}



		/*设置直通模式*/
		GLOBAL_ZEROMEM(&lTsRouteInfo, sizeof(MPEG2_DBTsRouteInfo));

		if (MPEG2_DBGetTsRouteInfo(pHandle->m_DBSHandle, lTsIDs, FALSE, &lTsRouteInfo))
		{
			lTsRouteInfo.m_TsIndex = MULTL_XMLGetNodeINT(pBodyNode, "ts_route_index", 10);
			lTsRouteInfo.m_ActiveMark = MULTL_XMLGetNodeMark(pBodyNode, "ts_route_mark");
#ifdef SUPPORT_NEW_HWL_MODULE
			lTsRouteInfo.m_bRouteWithNullPacket = MULTL_XMLGetNodeMark(pBodyNode, "null");
			lTsRouteInfo.m_bRouteWithPCRCorrection = MULTL_XMLGetNodeMark(pBodyNode, "pcr");
#endif
			GLOBAL_TRACE(("Out Ts %d Route From In Ts %d, Mark = %d, Null = %d, PCR = %d\n", lOutTsIndex, lTsRouteInfo.m_TsIndex, lTsRouteInfo.m_ActiveMark, lTsRouteInfo.m_bRouteWithNullPacket, lTsRouteInfo.m_bRouteWithPCRCorrection));
			MPEG2_DBSetTsRouteInfo(pHandle->m_DBSHandle, lTsIDs, &lTsRouteInfo);
		}
		else
		{
			GLOBAL_TRACE(("Get Ts Route Info Failed, TsIDs = %d\n", lOutTsIndex));
		}



		//#if defined(GM2750)//确保通道只能直通
		//		lTsIDs = MPEG2_DBGetTsIDs(pHandle->m_DBSHandle, FALSE, 0);
		//		MPEG2_DBGetTsRouteInfo(pHandle->m_DBSHandle, lTsIDs, FALSE, &lTsRouteInfo);
		//
		//		if (lTsRouteInfo.m_ActiveMark == TRUE)
		//		{
		//			lTsIDs = MPEG2_DBGetTsIDs(pHandle->m_DBSHandle, FALSE, 32);
		//			MPEG2_DBGetTsRouteInfo(pHandle->m_DBSHandle, lTsIDs, FALSE, &lTsRouteInfo);
		//			lTsRouteInfo.m_ActiveMark = TRUE;
		//			if (lTsRouteInfo.m_TsIndex == 0)
		//			{
		//				lTsRouteInfo.m_TsIndex = 32;
		//			}
		//			else
		//			{
		//				lTsRouteInfo.m_TsIndex = 0;
		//			}
		//			MPEG2_DBSetTsRouteInfo(pHandle->m_DBSHandle, lTsIDs, &lTsRouteInfo);
		//		}
		//		else
		//		{
		//			lTsIDs = MPEG2_DBGetTsIDs(pHandle->m_DBSHandle, FALSE, 32);
		//			MPEG2_DBGetTsRouteInfo(pHandle->m_DBSHandle, lTsIDs, FALSE, &lTsRouteInfo);
		//
		//			if (lTsRouteInfo.m_ActiveMark == TRUE)
		//			{
		//				lTsIDs = MPEG2_DBGetTsIDs(pHandle->m_DBSHandle, FALSE, 0);
		//				MPEG2_DBGetTsRouteInfo(pHandle->m_DBSHandle, lTsIDs, FALSE, &lTsRouteInfo);
		//				lTsRouteInfo.m_ActiveMark = TRUE;
		//				if (lTsRouteInfo.m_TsIndex == 0)
		//				{
		//					lTsRouteInfo.m_TsIndex = 32;
		//				}
		//				else
		//				{
		//					lTsRouteInfo.m_TsIndex = 0;
		//				}
		//				MPEG2_DBSetTsRouteInfo(pHandle->m_DBSHandle, lTsIDs, &lTsRouteInfo);
		//			}
		//			else
		//			{
		//
		//				lTsIDs = MPEG2_DBGetTsIDs(pHandle->m_DBSHandle, FALSE, 0);
		//				MPEG2_DBGetTsRouteInfo(pHandle->m_DBSHandle, lTsIDs, FALSE, &lTsRouteInfo);
		//				lTsRouteInfo.m_ActiveMark = TRUE;
		//				lTsRouteInfo.m_TsIndex = 0;
		//				MPEG2_DBSetTsRouteInfo(pHandle->m_DBSHandle, lTsIDs, &lTsRouteInfo);
		//
		//				lTsIDs = MPEG2_DBGetTsIDs(pHandle->m_DBSHandle, FALSE, 32);
		//				MPEG2_DBGetTsRouteInfo(pHandle->m_DBSHandle, lTsIDs, FALSE, &lTsRouteInfo);
		//				lTsRouteInfo.m_ActiveMark = TRUE;
		//				lTsRouteInfo.m_TsIndex = 32;
		//				MPEG2_DBSetTsRouteInfo(pHandle->m_DBSHandle, lTsIDs, &lTsRouteInfo);
		//			}
		//		}
		//#endif



		MULTL_SetRemuxApplyMark(pHandle, FALSE);
		
	}
	else if (GLOBAL_STRCMP(pSubType, "descriptors") == 0)
	{
		U32 lParentIDs, lIDs;
		S32 lDescriptorType;
		mxml_node_t *plXMLHolder;
		mxml_node_t *plXMLCurrent;
		MPEG2_DBDescriptorInfo lDescInfo;

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "parent_ids");
		if (plTmpStr)
		{
			lParentIDs = GLOBAL_STRTOUL(plTmpStr, NULL, 10);
		}

		lDescriptorType = MPEG2_DESCRIPTOR_INVALID;
		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "descriptor_type");
		if (plTmpStr)
		{
			if (GLOBAL_STRCMP(plTmpStr, "NIT_NETWORK_DESC") == 0)
			{
				lDescriptorType = MPEG2_DESCRIPTOR_NIT_NETWORK_USER;
			}
			else if (GLOBAL_STRCMP(plTmpStr, "NIT_TS_DESC") == 0)
			{
				lDescriptorType = MPEG2_DESCRIPTOR_NIT_TRANSPORT_USER;
			}
			else if (GLOBAL_STRCMP(plTmpStr, "TS_CAT_DESC") == 0)
			{
				lDescriptorType = MPEG2_DESCRIPTOR_CAT_USER;
			}
			else if (GLOBAL_STRCMP(plTmpStr, "SERVICE_PMT_DESC") == 0)
			{
				lDescriptorType = MPEG2_DESCRIPTOR_PMT_SERVICE_INFO_USER;
			}
			else if (GLOBAL_STRCMP(plTmpStr, "SERVICE_SDT_DESC") == 0)
			{
				lDescriptorType = MPEG2_DESCRIPTOR_SDT_SERVICE_INFO_USER;
			}
			else if (GLOBAL_STRCMP(plTmpStr, "ES_PMT_DESC") == 0)
			{
				lDescriptorType = MPEG2_DESCRIPTOR_PMT_ES_INFO_USER;
			}
		}

		plXMLHolder = mxmlFindElement(pBodyNode, pBodyNode, "remove_descriptors", NULL, NULL, MXML_DESCEND_FIRST);
		plXMLCurrent = mxmlFindElement(plXMLHolder, plXMLHolder, "descriptor", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "desc_ids");
			if (plTmpStr)
			{
				lIDs = GLOBAL_STRTOUL(plTmpStr, NULL, 10);
				MPEG2_DBRemoveDescriptor(pHandle->m_DBSHandle, lIDs);
			}
			plXMLCurrent = mxmlFindElement(plXMLCurrent, plXMLHolder, "descriptor", NULL, NULL, MXML_NO_DESCEND);
		}

		plXMLHolder = mxmlFindElement(pBodyNode, pBodyNode, "add_modify_descriptors", NULL, NULL, MXML_DESCEND_FIRST);
		plXMLCurrent = mxmlFindElement(plXMLHolder, plXMLHolder, "descriptor", NULL, NULL, MXML_DESCEND_FIRST);
		while(plXMLCurrent)
		{
			lDescInfo.m_DescriptorDataSize = 0;
			lDescInfo.m_OutputMark = FALSE;

			plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "desc_ids");
			if (plTmpStr)
			{
				lIDs = GLOBAL_STRTOUL(plTmpStr, NULL, 10);
			}

			if (lIDs != 0)
			{
				MPEG2_DBGetDescriptorInfo(pHandle->m_DBSHandle, lIDs, &lDescInfo);
			}


			plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "data");
			if (plTmpStr)
			{
				lDescInfo.m_DescriptorDataSize = CAL_StringHexToBin(plTmpStr, lDescInfo.m_pDescriptorData, sizeof(lDescInfo.m_pDescriptorData));
			}


			plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "out_mark");
			if (plTmpStr)
			{
				lDescInfo.m_OutputMark = MULTL_XMLMarkValueFromStr(plTmpStr);
			}

			if (lIDs == 0)
			{
				MPEG2_DBAddDescriptor(pHandle->m_DBSHandle, lParentIDs, lDescriptorType, &lDescInfo);
			}
			else
			{
				MPEG2_DBSetDescriptorInfo(pHandle->m_DBSHandle, lIDs, &lDescInfo);
			}

			plXMLCurrent = mxmlFindElement(plXMLCurrent, plXMLHolder, "descriptor", NULL, NULL, MXML_NO_DESCEND);
		}
		MULTL_SetRemuxApplyMark(pHandle, FALSE);
		
	}
	else if (GLOBAL_STRCMP(pSubType, "clean_in_ts") == 0)
	{
		S32 lTsIndex;
		lTsIndex = MULTL_XMLGetNodeINT(pBodyNode, "input_ts_index", 10);
		MPEG2_DBClearInTs(pHandle->m_DBSHandle, lTsIndex);
		MULTL_SetRemuxApplyMark(pHandle, FALSE);
	}
#ifdef GN2000
	else if (GLOBAL_STRCMP(pSubType, "mosiac_setting") == 0)
	{
		MULTL_MOSIACLoadXML(pBodyNode);
		MULTL_SetRemuxApplyMark(pHandle, FALSE);
		
	}
#endif
#ifdef MULT_TS_BACKUP_SUPPORT
	else if (GLOBAL_STRCMP(pSubType, "ts_backup_setting") == 0)
	{
		MULT_BPLoadXML(pBodyNode);
		MULTL_SetRemuxApplyMark(pHandle, FALSE);
		
	}
#endif
#ifdef SUPPORT_PES_ANALYS_FUNCTION
	else if (GLOBAL_STRCMP(pSubType, "es_anaylse") == 0)
	{
		PES_XMLAcess(pBodyNode, NULL);
	}
#endif
	else
	{
		GLOBAL_ASSERT(0);
	}

}

void MULTL_WEBXMLIOConfig(MULT_Handle *pHandle, mxml_node_t* pBodyNode, CHAR_T* pSubType, CHAR_T *pParameter)
{
	S32 lChnIndex, lSubIndex, lEncoderInfoEntryIndex = 0;
	CHAR_T *plTmpStr;
	MULT_Parameter *plParameter;
	MULT_ChannelNode *plChnNode;
	MULT_SubChannelNode *plSubNode;
	mxml_node_t *plHolder;
	mxml_node_t *plSubHolder;
	mxml_node_t *plXMLCurrent;


	plParameter = &pHandle->m_Parameter;

	if (GLOBAL_STRCMP(pSubType, "parameters") == 0)
	{
		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "chn_ind");
		if (plTmpStr)
		{
			lChnIndex = GLOBAL_STRTOL(plTmpStr, NULL, 10);
		}

		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "direction");
		if (plTmpStr)
		{
			if (GLOBAL_STRCMP(plTmpStr, "IN") == 0)
			{
				if (GLOBAL_CHECK_INDEX(lChnIndex, plParameter->m_InChannelNumber))
				{
					plChnNode = &plParameter->m_pInChannel[lChnIndex];

					plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "type");
					if (plTmpStr)
					{
						if (GLOBAL_STRCMP(plTmpStr, "ASI") == 0)
						{
							if (plChnNode->m_ChannelType == HWL_CHANNEL_TYPE_ASI)
							{
								plHolder = mxmlFindElement(pBodyNode, pBodyNode, "sub_channel", NULL, NULL, MXML_DESCEND_FIRST);
								if (plHolder)
								{
									plXMLCurrent = plHolder;
									while(plXMLCurrent)
									{
										plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "sub_ind");
										if (plTmpStr)
										{
											lSubIndex = GLOBAL_STRTOL(plTmpStr, NULL, 10);
											if (GLOBAL_CHECK_INDEX(lSubIndex, plChnNode->m_SubChannelNumber))
											{
												plSubNode = &plChnNode->m_pSubChannelNode[lSubIndex];
												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "active_mark");
												if (plTmpStr)
												{
													plSubNode->m_ActiveMark = MULTL_XMLMarkValueFromStr(plTmpStr);
												}
											}
										}
										plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "sub_channel", NULL, NULL, MXML_NO_DESCEND);
									}
								}
							}
							else
							{
								GLOBAL_TRACE(("Channel Type Error! Got ASI Channel = %d\n", plChnNode->m_ChannelType));
							}
						}
#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
                        else if(GLOBAL_STRCMP(plTmpStr, MULT_CHANNEL_TYPE_ENCODER_NAME) == 0)//TYPE = ENCODER
						{
							if (plChnNode->m_ChannelType == HWL_CHANNEL_TYPE_ENCODER)//HWL CHANNEL TYPEs
							{
								plTmpStr = MULTL_XMLGetNodeText( pBodyNode, "sub_type");
								if((GLOBAL_STRCMP(plTmpStr,  MULT_CHANNEL_SUB_TYPE_ENCODER_HI3531A) == 0) || (GLOBAL_STRCMP(plTmpStr,  MULT_CHANNEL_SUB_TYPE_ENCODER_HI3519) == 0))
								{
									plHolder = mxmlFindElement(pBodyNode, pBodyNode, "sub_channel", NULL, NULL, MXML_DESCEND_FIRST);
									if (plHolder)
									{
										plXMLCurrent = plHolder;
										while(plXMLCurrent)
										{
											plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "sub_ind");
											if (plTmpStr)
											{
												lSubIndex = GLOBAL_STRTOL(plTmpStr, NULL, 10);
												if (GLOBAL_CHECK_INDEX(lSubIndex, plChnNode->m_SubChannelNumber))
												{
													MULT_SubENCODERInfo *plEncInfo;
													plSubNode = &plChnNode->m_pSubChannelNode[lSubIndex];
													plEncInfo = &plSubNode->m_SubInfo.m_SubENCODERInfo;

													plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "vi_mode");
													if (plTmpStr) {
														plEncInfo->m_ViMode = MULTL_XMLEncViModeValueFromStr(plTmpStr);
													}

													plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "vo_mode");
													if (plTmpStr) {
														plEncInfo->m_VoMode = MULTL_XMLEncVoModeValueFromStr(plTmpStr);
													}

													plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "br_mode");
													if (plTmpStr) {
														plEncInfo->m_BrMode = MULTL_XMLEncBrModeValueFromStr(plTmpStr);
													}

													plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "enc_mode");
													if (plTmpStr) {
														plEncInfo->m_EncMode = MULTL_XMLEncModeValueFromStr(plTmpStr);
													}

													GLOBAL_TRACE(("plEncInfo->m_EncMode = %d\n", plEncInfo->m_EncMode))

													plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "profile");
													if (plTmpStr) {
														plEncInfo->m_Profile = MULTL_XMLEncProfileValueFromStr(plTmpStr);
													}

													plEncInfo->m_Bitrate = MULTL_XMLGetNodeUINT(plXMLCurrent, "bitrate", 10);
													plEncInfo->m_ProgBitrate = MULTL_XMLGetNodeUINT(plXMLCurrent, "prog_bitrate", 10);
													plEncInfo->m_Gop = MULTL_XMLGetNodeUINT(plXMLCurrent, "gop", 10);

													plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "audio_group");
													if (plTmpStr) {
														plEncInfo->m_AudioGroup  = MULTL_XMLAudGroupValueFromStr(plTmpStr);
													}

													plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "aud_enc_mode");
													if (plTmpStr) {
														plEncInfo->m_AudEncMode = MULTL_XMLAudEncModeValueFromStr(plTmpStr);
													}

													plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "aud_bitrate");
													if (plTmpStr) {
														plEncInfo->m_AudBitrate = MULTL_XMLAudBitrateValueFromStr(plTmpStr);
													}

#if defined(SUPPORT_WATER_MARK)
													plEncInfo->m_x_coordinate = MULTL_XMLGetNodeUINT(plXMLCurrent, "x_coordinate", 10);
													plEncInfo->m_y_coordinate = MULTL_XMLGetNodeUINT(plXMLCurrent, "y_coordinate", 10);
													plEncInfo->m_transparent = MULTL_XMLGetNodeUINT(plXMLCurrent, "transparent", 10);
													plEncInfo->m_enable = MULTL_XMLGetNodeMark(plXMLCurrent, "enable");
#endif


#if defined(SUPPORT_MOSIAC)
													/* Mosaic */
													plEncInfo->m_Mosaic_Param.m_x_coordinate = MULTL_XMLGetNodeUINT(plXMLCurrent, "mosaic_x_coordinate", 10);
													plEncInfo->m_Mosaic_Param.m_y_coordinate = MULTL_XMLGetNodeUINT(plXMLCurrent, "mosaic_y_coordinate", 10);
													plEncInfo->m_Mosaic_Param.m_width = MULTL_XMLGetNodeUINT(plXMLCurrent, "mosaic_width", 10);
													plEncInfo->m_Mosaic_Param.m_height = MULTL_XMLGetNodeUINT(plXMLCurrent, "mosaic_height", 10);
													plEncInfo->m_Mosaic_Param.m_blk_size = MULTL_XMLEncMosaicSizeValueFromStr(MULTL_XMLGetNodeText(plXMLCurrent, "mosaic_size"));
													plEncInfo->m_Mosaic_Param.m_enable = MULTL_XMLGetNodeMark(plXMLCurrent, "mosaic_enable");
#endif


													plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "aud_sample");
													if (plTmpStr) {
														plEncInfo->m_AudSample = MULTL_XMLAudSampValueFromStr(plTmpStr);
													}

													plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "prog_name");
													if (plTmpStr) {
														GLOBAL_STRCPY(plEncInfo->m_pProgName, plTmpStr);
													}

													plEncInfo->m_Volume = MULTL_XMLGetNodeINT(plXMLCurrent, "volume", 10);
													plEncInfo->m_ProgNumber = MULTL_XMLGetNodeUINT(plXMLCurrent, "prog_number", 10);
													plEncInfo->m_VidPid = MULTL_XMLGetNodeUINT(plXMLCurrent, "vid_pid", 10);
													plEncInfo->m_AudPid = MULTL_XMLGetNodeUINT(plXMLCurrent, "aud_pid", 10);
													plEncInfo->m_PcrPid = MULTL_XMLGetNodeUINT(plXMLCurrent, "pcr_pid", 10);
													plEncInfo->m_PmtPid = MULTL_XMLGetNodeUINT(plXMLCurrent, "pmt_pid", 10);
													plEncInfo->m_ActiveMark = MULTL_XMLGetNodeMark(plXMLCurrent, "active");
												}
											}
											plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "sub_channel", NULL, NULL, MXML_NO_DESCEND);
										}
									}

									MULTL_ApplyLEncoderParameter(pHandle, lChnIndex);
								}
							}
							else
							{
								GLOBAL_TRACE(("Channel Type Error! Got　Encoder　Channel = %d\n", plChnNode->m_ChannelType));
							}
						}

#endif
						else if(GLOBAL_STRCMP(plTmpStr, MULT_CHANNEL_TYPE_E3DS3_NAME) == 0)
						{
							if (plChnNode->m_ChannelType == HWL_CHANNEL_TYPE_E3DS3)
							{
								plHolder = mxmlFindElement(pBodyNode, pBodyNode, "sub_channel", NULL, NULL, MXML_DESCEND_FIRST);
								if (plHolder)
								{
									plXMLCurrent = plHolder;
									while(plXMLCurrent)
									{
										plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "sub_ind");
										if (plTmpStr)
										{
											lSubIndex = GLOBAL_STRTOL(plTmpStr, NULL, 10);
											if (GLOBAL_CHECK_INDEX(lSubIndex, plChnNode->m_SubChannelNumber))
											{
												plSubNode = &plChnNode->m_pSubChannelNode[lSubIndex];
												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "e3ds3_mark");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubESDS3Info.m_E3DS3 = MULTL_XMLE3DS3ValueFromStr(plTmpStr);
												}

												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "bit_order");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubESDS3Info.m_Bitorder = MULTL_XMLBitOrderValueFromStr(plTmpStr);
												}

												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "fream_from");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubESDS3Info.m_Frameform = MULTL_XMLFrameformValueFromStr(plTmpStr);
												}

												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "packet_length");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubESDS3Info.m_Packetlength = MULTL_XMLPacketlengthformValueFromStr(plTmpStr);
												}

												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "scramble_changer");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubESDS3Info.m_Scramble = MULTL_XMLMarkValueFromStr(plTmpStr);
												}

												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "rs_code");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubESDS3Info.m_RSCoding = MULTL_XMLMarkValueFromStr(plTmpStr);
												}

												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "interweave_code");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubESDS3Info.m_InterweaveCoding = MULTL_XMLMarkValueFromStr(plTmpStr);
												}

												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "code_rate_recover");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubESDS3Info.m_RateRecover = MULTL_XMLMarkValueFromStr(plTmpStr);
												}


											}
										}
										plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "sub_channel", NULL, NULL, MXML_NO_DESCEND);
									}
								}
								MULTL_ApplyInE3DS3Parameter(pHandle , lChnIndex);
							}
							else
							{
								GLOBAL_TRACE(("Channel Type Error! Got　E3DS3　Channel = %d\n", plChnNode->m_ChannelType));
							}
						}
						else if(GLOBAL_STRCMP(plTmpStr, MULT_CHANNEL_TYPE_TUNER_S_NAME) == 0)
						{
							if (plChnNode->m_ChannelType == HWL_CHANNEL_TYPE_TUNER_S)
							{
								plHolder = mxmlFindElement(pBodyNode, pBodyNode, "sub_channel", NULL, NULL, MXML_DESCEND_FIRST);
								if (plHolder)
								{
									plXMLCurrent = plHolder;
									while(plXMLCurrent)
									{
										plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "sub_ind");
										if (plTmpStr)
										{
											lSubIndex = GLOBAL_STRTOL(plTmpStr, NULL, 10);
											if (GLOBAL_CHECK_INDEX(lSubIndex, plChnNode->m_SubChannelNumber))
											{
												plSubNode = &plChnNode->m_pSubChannelNode[lSubIndex];
												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "in_freq");
												if (plTmpStr)
												{																									
													plSubNode->m_SubInfo.m_SubTUNERInfo.m_InputFreq = GLOBAL_STRTOL(plTmpStr, NULL, 10); 							

												}

												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "local_freq");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubTUNERInfo.m_LocalFreq =  GLOBAL_STRTOL(plTmpStr, NULL, 10);

												}

												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "sym_rate");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubTUNERInfo.m_SymbolRate =  GLOBAL_STRTOL(plTmpStr, NULL, 10);


												}

												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "polar_method");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubTUNERInfo.m_PolarMethod = MULTL_XMLPolarMethodValueFromStr(plTmpStr);

												}

												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "switch_22k");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubTUNERInfo.m_22kSwitch = MULTL_XMLMarkValueFromStr(plTmpStr);

												}
												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "spec_inv");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubTUNERInfo.m_Specinv = MULTL_XMLTunerSpecinvValueFromStr(plTmpStr);

												}


											}
										}
										plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "sub_channel", NULL, NULL, MXML_NO_DESCEND);
									}
								}
								MULTL_ApplyTunerParameter(pHandle, lChnIndex);

							}
							else
							{
								GLOBAL_TRACE(("Channel Type Error! Got　TUNER_S　Channel = %d\n", plChnNode->m_ChannelType));
							}
						}
						else if(GLOBAL_STRCMP(plTmpStr, MULT_CHANNEL_TYPE_TUNER_C_NAME) == 0)
						{
							if (plChnNode->m_ChannelType == HWL_CHANNEL_TYPE_TUNER_C)
							{
								plHolder = mxmlFindElement(pBodyNode, pBodyNode, "sub_channel", NULL, NULL, MXML_DESCEND_FIRST);
								if (plHolder)
								{
									plXMLCurrent = plHolder;
									while(plXMLCurrent)
									{
										plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "sub_ind");
										if (plTmpStr)
										{
											lSubIndex = GLOBAL_STRTOL(plTmpStr, NULL, 10);
											if (GLOBAL_CHECK_INDEX(lSubIndex, plChnNode->m_SubChannelNumber))
											{
												plSubNode = &plChnNode->m_pSubChannelNode[lSubIndex];
												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "in_freq");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubTUNERInfo.m_InputFreq = GLOBAL_STRTOL(plTmpStr, NULL, 10); 
												}

												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "sym_rate");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubTUNERInfo.m_SymbolRate =  GLOBAL_STRTOL(plTmpStr, NULL, 10);
												}

												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "modulation_mode");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubTUNERInfo.m_Modulation = MULTL_XMLQAMModeValueFromStr(plTmpStr);
												}

												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "req_type");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubTUNERInfo.m_Reqtype = MULTL_XMLReqTunerTypeValueFromStr(plTmpStr);
												}
												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "spec_inv");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubTUNERInfo.m_Specinv = MULTL_XMLTunerSpecinvValueFromStr(plTmpStr);

												}
											}
										}
										plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "sub_channel", NULL, NULL, MXML_NO_DESCEND);
									}
								}
								MULTL_ApplyTunerParameter(pHandle, lChnIndex);
							}
							else
							{
								GLOBAL_TRACE(("Channel Type Error! Got　TUNER_C　Channel = %d\n", plChnNode->m_ChannelType));
							}
						}
						else if(GLOBAL_STRCMP(plTmpStr, MULT_CHANNEL_TYPE_TUNER_DTMB_NAME) == 0)
						{
							if (plChnNode->m_ChannelType == HWL_CHANNEL_TYPE_TUNER_DTMB)
							{
								plHolder = mxmlFindElement(pBodyNode, pBodyNode, "sub_channel", NULL, NULL, MXML_DESCEND_FIRST);
								if (plHolder)
								{
									plXMLCurrent = plHolder;
									while(plXMLCurrent)
									{
										plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "sub_ind");
										if (plTmpStr)
										{
											lSubIndex = GLOBAL_STRTOL(plTmpStr, NULL, 10);
											if (GLOBAL_CHECK_INDEX(lSubIndex, plChnNode->m_SubChannelNumber))
											{
												plSubNode = &plChnNode->m_pSubChannelNode[lSubIndex];
												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "in_freq");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubTUNERInfo.m_InputFreq = GLOBAL_STRTOL(plTmpStr, NULL, 10); 
												}

												plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "spec_inv");
												if (plTmpStr)
												{
													plSubNode->m_SubInfo.m_SubTUNERInfo.m_Specinv = MULTL_XMLTunerSpecinvValueFromStr(plTmpStr);

												}
											}
										}
										plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "sub_channel", NULL, NULL, MXML_NO_DESCEND);
									}
								}
								MULTL_ApplyTunerParameter(pHandle, lChnIndex);
							}
							else
							{
								GLOBAL_TRACE(("Channel Type Error! Got　TUNER_C　Channel = %d\n", plChnNode->m_ChannelType));
							}
						}
						else if(GLOBAL_STRCMP(plTmpStr, "ETH") == 0)
						{
							if(plChnNode->m_ChannelType == HWL_CHANNEL_TYPE_IP)
							{
								plChnNode->m_ChannelInfo.m_IPInfo.m_IPAddress = MULTL_XMLGetNodeIP(pBodyNode, "ipv4_addr");
								plChnNode->m_ChannelInfo.m_IPInfo.m_IPMask = MULTL_XMLGetNodeIP(pBodyNode, "ipv4_mask");
								plChnNode->m_ChannelInfo.m_IPInfo.m_IPGate = MULTL_XMLGetNodeIP(pBodyNode, "ipv4_gate");
								plChnNode->m_ChannelInfo.m_IPInfo.m_IPMark = MULTL_XMLGetNodeMark(pBodyNode, "eth_mark");

								plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "ip_mac");
								if (plTmpStr)
								{
									CAL_StringMACToBin(plTmpStr, plChnNode->m_ChannelInfo.m_IPInfo.m_MAC, sizeof(plChnNode->m_ChannelInfo.m_IPInfo.m_MAC));
									plChnNode->m_ChannelInfo.m_IPInfo.m_MAC[0] = plChnNode->m_ChannelInfo.m_IPInfo.m_MAC[0] & 0xFE;//确保第一个字节的最低位不为
								}	

								plHolder = mxmlFindElement(pBodyNode, pBodyNode, "sub_channel", NULL, NULL, MXML_DESCEND_FIRST);
								if (plHolder)
								{
									plXMLCurrent = plHolder;
									while(plXMLCurrent)
									{
										plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "sub_ind");
										if (plTmpStr)
										{
											lSubIndex = GLOBAL_STRTOL(plTmpStr, NULL, 10);
											if (GLOBAL_CHECK_INDEX(lSubIndex, plChnNode->m_SubChannelNumber))
											{
												plSubNode = &plChnNode->m_pSubChannelNode[lSubIndex];
												plSubNode->m_ActiveMark = MULTL_XMLGetNodeMark(plXMLCurrent, "active_mark");
												plSubNode->m_SubInfo.m_SubIPInfo.m_IPv4Addr = MULTL_XMLGetNodeIP(plXMLCurrent, "ipv4_des_addr");
												plSubNode->m_SubInfo.m_SubIPInfo.m_IPv4Port = MULTL_XMLGetNodeUINT(plXMLCurrent, "ipv4_des_port", 10);
												plSubNode->m_SubInfo.m_SubIPInfo.m_Protocol = MULTL_XMLGetNodeUINT(plXMLCurrent, "protocol", 10);
											}
										}
										plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "sub_channel", NULL, NULL, MXML_NO_DESCEND);
									}			
									MULTL_ApplyInETHParameter(pHandle, lChnIndex);	

								}
							}
							else
							{
								GLOBAL_TRACE(("Channel Type Error! Got ETH Channel = %d\n", plChnNode->m_ChannelType));
							}
						}
						else
						{
							GLOBAL_TRACE(("Channel Type Error! Got ChannelType = %s\n", plTmpStr));
						}
					}

				}
			}
			else
			{
				if (GLOBAL_CHECK_INDEX(lChnIndex, plParameter->m_OutChannelNumber))
				{
					plChnNode = &plParameter->m_pOutChannel[lChnIndex];

					plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "type");
					if (plTmpStr)
					{
						if (GLOBAL_STRCMP(plTmpStr, MULT_CHANNEL_TYPE_ASI_NAME) == 0)
						{
							if (plChnNode->m_ChannelType == HWL_CHANNEL_TYPE_ASI)
							{
								plHolder = mxmlFindElement(pBodyNode, pBodyNode, "sub_channel", NULL, NULL, MXML_DESCEND_FIRST);
								if (plHolder)
								{
									plXMLCurrent = plHolder;
									while(plXMLCurrent)
									{
										plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "sub_ind");
										if (plTmpStr)
										{
											lSubIndex = GLOBAL_STRTOL(plTmpStr, NULL, 10);
											if (GLOBAL_CHECK_INDEX(lSubIndex, plChnNode->m_SubChannelNumber))
											{
												plSubNode = &plChnNode->m_pSubChannelNode[lSubIndex];
												plSubNode->m_ActiveMark = MULTL_XMLGetNodeMark(plXMLCurrent, "active_mark");
												plSubNode->m_Bitrate = MULTL_XMLGetNodeUINT(plXMLCurrent, "bitrate", 10 );
											}
										}
										plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "sub_channel", NULL, NULL, MXML_NO_DESCEND);
									}
								}
								MULTL_ApplyOutASIParameter(pHandle , lChnIndex);
							}
							else
							{
								GLOBAL_TRACE(("Channel Type Error! Got ASI But Current Channel = %d\n", plChnNode->m_ChannelType));
							}
						}
						else if (GLOBAL_STRCMP(plTmpStr, MULT_CHANNEL_TYPE_MODULATOR_NAME) == 0)
						{
							if (plChnNode->m_ChannelType == HWL_CHANNEL_TYPE_DVB_C_MODULATOR)
							{
								if (plChnNode->m_ChannelInfo.m_ModulatorInfo.m_ExAttValidMark)
								{
									plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "ex_att_level");
									if (plTmpStr)
									{
										plChnNode->m_ChannelInfo.m_ModulatorInfo.m_ExAttLevel = GLOBAL_STRTOL(plTmpStr, NULL, 10);
									}
								}

								plHolder = mxmlFindElement(pBodyNode, pBodyNode, "sub_channel", NULL, NULL, MXML_DESCEND_FIRST);
								if (plHolder)
								{
									plXMLCurrent = plHolder;
									while(plXMLCurrent)
									{
										plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "sub_ind");
										if (plTmpStr)
										{
											lSubIndex = GLOBAL_STRTOL(plTmpStr, NULL, 10);
											if (GLOBAL_CHECK_INDEX(lSubIndex, plChnNode->m_SubChannelNumber))
											{
												plSubNode = &plChnNode->m_pSubChannelNode[lSubIndex];
												plSubNode->m_ActiveMark = MULTL_XMLGetNodeMark(plXMLCurrent, "active_mark");
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_ITUCoding = MULTL_XMLITUCodingValueFromStr(MULTL_XMLGetNodeText(plXMLCurrent, "itu_coding"));
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_AnalogBand = MULTL_XMLAnalogValueFromStr(MULTL_XMLGetNodeText(plXMLCurrent, "analog_band"));
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_CenterFreq = MULTL_XMLGetNodeUINT(plXMLCurrent, "center_frequence", 10);
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_SymbolRate = MULTL_XMLGetNodeINT(plXMLCurrent, "symbol_rate", 10);
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_Mode = MULTL_XMLQAMModeValueFromStr(MULTL_XMLGetNodeText(plXMLCurrent, "modulation_mode"));
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_SpectInv = MULTL_XMLGetNodeMark(plXMLCurrent, "spectrum_invert");
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_GainLevel = MULTL_XMLGetNodeINT(plXMLCurrent, "gain_level", 10);
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_Modulation = MULTL_XMLGetNodeMark(plXMLCurrent, "modulation_mark");
											}
										}
										plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "sub_channel", NULL, NULL, MXML_NO_DESCEND);
									}
								}

								MULTL_ApplyQAMParameter(pHandle, lChnIndex);
							}
							else
							{
								GLOBAL_TRACE(("Channel Type Error! Got QAM But Current Channel = %d\n", plChnNode->m_ChannelType));
							}

						}
						else if (GLOBAL_STRCMP(plTmpStr, MULT_CHANNEL_TYPE_QPSK_NAME) == 0)
						{
							if (plChnNode->m_ChannelType == HWL_CHANNEL_TYPE_DVB_S_MODULATOR)
							{
								if (plChnNode->m_ChannelInfo.m_ModulatorInfo.m_ExAttValidMark)
								{
									plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "ex_att_level");
									if (plTmpStr)
									{
										plChnNode->m_ChannelInfo.m_ModulatorInfo.m_ExAttLevel = GLOBAL_STRTOL(plTmpStr, NULL, 10);
									}
								}

								plHolder = mxmlFindElement(pBodyNode, pBodyNode, "sub_channel", NULL, NULL, MXML_DESCEND_FIRST);
								if (plHolder)
								{
									plXMLCurrent = plHolder;
									while(plXMLCurrent)
									{
										plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "sub_ind");
										if (plTmpStr)
										{
											lSubIndex = GLOBAL_STRTOL(plTmpStr, NULL, 10);
											if (GLOBAL_CHECK_INDEX(lSubIndex, plChnNode->m_SubChannelNumber))
											{
												plSubNode = &plChnNode->m_pSubChannelNode[lSubIndex];
												plSubNode->m_ActiveMark = MULTL_XMLGetNodeMark(plXMLCurrent, "active_mark");
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_FecEncode = MULTL_XMLFecEncodeValueFromStr(MULTL_XMLGetNodeText(plXMLCurrent, "fec_encode"));
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_CenterFreq = MULTL_XMLGetNodeUINT(plXMLCurrent, "center_frequence", 10);
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_SymbolRate = MULTL_XMLGetNodeUINT(plXMLCurrent, "symbol_rate", 10);
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_SpectInv = MULTL_XMLGetNodeMark(plXMLCurrent, "spectrum_invert");
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_GainLevel = MULTL_XMLGetNodeINT(plXMLCurrent, "gain_level", 10);
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_Modulation = MULTL_XMLGetNodeMark(plXMLCurrent, "modulation_mark");
											}
										}
										plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "sub_channel", NULL, NULL, MXML_NO_DESCEND);
									}
								}

								MULTL_ApplyQAMParameter(pHandle, lChnIndex);
							}
							else
							{
								GLOBAL_TRACE(("Channel Type Error! Got QPSK But Current Channel = %d\n", plChnNode->m_ChannelType));
							}

						}
						else if (GLOBAL_STRCMP(plTmpStr, MULT_CHANNEL_TYPE_DTMB_NAME) == 0)
						{
							if (plChnNode->m_ChannelType == HWL_CHANNEL_TYPE_DTMB_MODULATOR)
							{
								if (plChnNode->m_ChannelInfo.m_ModulatorInfo.m_ExAttValidMark)
								{
									plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "ex_att_level");
									if (plTmpStr)
									{
										plChnNode->m_ChannelInfo.m_ModulatorInfo.m_ExAttLevel = GLOBAL_STRTOL(plTmpStr, NULL, 10);
									}
								}

								plChnNode->m_ChannelInfo.m_ModulatorInfo.m_ALCMark = MULTL_XMLGetNodeMarkDefault(pBodyNode, "alc_mark", FALSE);
#ifdef SUPPORT_MODULATOR_FREQ_1_HZ
								plChnNode->m_ChannelInfo.m_ModulatorInfo.m_FreqAdj = MULTL_XMLGetNodeINT(pBodyNode, "freq_adj", 10);
#else
								plChnNode->m_ChannelInfo.m_ModulatorInfo.m_FreqAdj = 0;
#endif


								plHolder = mxmlFindElement(pBodyNode, pBodyNode, "sub_channel", NULL, NULL, MXML_DESCEND_FIRST);
								if (plHolder)
								{
									plXMLCurrent = plHolder;
									while(plXMLCurrent)
									{
										plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "sub_ind");
										if (plTmpStr)
										{
											lSubIndex = GLOBAL_STRTOL(plTmpStr, NULL, 10);
											if (GLOBAL_CHECK_INDEX(lSubIndex, plChnNode->m_SubChannelNumber))
											{
												plSubNode = &plChnNode->m_pSubChannelNode[lSubIndex];

												plSubNode->m_ActiveMark = MULTL_XMLGetNodeMark(plXMLCurrent, "active_mark");												
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_CenterFreq = MULTL_XMLGetNodeUINT(plXMLCurrent, "center_frequence", 10);
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_SpectInv = MULTL_XMLGetNodeMark(plXMLCurrent, "spectrum_invert");
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_Mode = MULTL_XMLQAMModeValueFromStr(MULTL_XMLGetNodeText(plXMLCurrent, "qam_mode"));
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_GainLevel = MULTL_XMLGetNodeINT(plXMLCurrent, "gain_level", 10);
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_Modulation = MULTL_XMLGetNodeMark(plXMLCurrent, "modulation_mark");
#ifdef SUPPORT_SFN_MODULATOR
												/*单频网打开时，SIP参数必须使用收到的参数*/
												if (MULT_SFNCheckEnabled() == FALSE)
												{
													plSubNode->m_SubInfo.m_SubModulatorInfo.m_CarrierMode = MULTL_XMLGetNodeINT(plXMLCurrent, "carrier", 10);
													plSubNode->m_SubInfo.m_SubModulatorInfo.m_PNMode = MULTL_XMLDTMBPNValueFromStr(MULTL_XMLGetNodeText(plXMLCurrent, "pn_mode"));
													plSubNode->m_SubInfo.m_SubModulatorInfo.m_CodeRate = MULTL_XMLDTMBCodeRateValueFromStr(MULTL_XMLGetNodeText(plXMLCurrent, "code_rate"));
													plSubNode->m_SubInfo.m_SubModulatorInfo.m_TimeInterleave = MULTL_XMLGetNodeINT(plXMLCurrent, "time_interleaver", 10);
													plSubNode->m_SubInfo.m_SubModulatorInfo.m_DoublePilot = MULTL_XMLGetNodeMark(plXMLCurrent, "double_pilot");
												}
#else
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_CarrierMode = MULTL_XMLGetNodeINT(plXMLCurrent, "carrier", 10);
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_PNMode = MULTL_XMLDTMBPNValueFromStr(MULTL_XMLGetNodeText(plXMLCurrent, "pn_mode"));
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_CodeRate = MULTL_XMLDTMBCodeRateValueFromStr(MULTL_XMLGetNodeText(plXMLCurrent, "code_rate"));
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_TimeInterleave = MULTL_XMLGetNodeINT(plXMLCurrent, "time_interleaver", 10);
												plSubNode->m_SubInfo.m_SubModulatorInfo.m_DoublePilot = MULTL_XMLGetNodeMark(plXMLCurrent, "double_pilot");
#endif
											}
										}
										plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "sub_channel", NULL, NULL, MXML_NO_DESCEND);
									}
								}
#ifdef SUPPORT_SFN_MODULATOR
								MULT_SFNApplyByQAMModule();
#else
								MULTL_ApplyQAMParameter(pHandle, lChnIndex);
#endif
							}
							else
							{
								GLOBAL_TRACE(("Channel Type Error! Got DTMB But Current Channel = %d\n", plChnNode->m_ChannelType));
							}

						}
						else if((GLOBAL_STRCMP(plTmpStr, "ETH") == 0) || (GLOBAL_STRCMP(plTmpStr, "ETH_LOOP") == 0) || (GLOBAL_STRCMP(plTmpStr, "ETH_DEP") == 0) || (GLOBAL_STRCMP(plTmpStr, "ETH_LOOP_DEP") == 0))
						{
							GLOBAL_TRACE(("Out ETH Param Set\n"));
							if(plChnNode->m_ChannelType == HWL_CHANNEL_TYPE_IP || plChnNode->m_ChannelType == HWL_CHANNEL_TYPE_IP_LOOP)
							{
								plChnNode->m_ChannelInfo.m_IPInfo.m_IPAddress = MULTL_XMLGetNodeIP(pBodyNode, "ipv4_addr");
								plChnNode->m_ChannelInfo.m_IPInfo.m_IPMask = MULTL_XMLGetNodeIP(pBodyNode, "ipv4_mask");
								plChnNode->m_ChannelInfo.m_IPInfo.m_IPGate = MULTL_XMLGetNodeIP(pBodyNode, "ipv4_gate");

								GLOBAL_TRACE(("IP Addr = 0x%08X\n", plChnNode->m_ChannelInfo.m_IPInfo.m_IPAddress));
								plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "ip_mac");
								if (plTmpStr)
								{
									CAL_StringMACToBin(plTmpStr, plChnNode->m_ChannelInfo.m_IPInfo.m_MAC, sizeof(plChnNode->m_ChannelInfo.m_IPInfo.m_MAC));
									plChnNode->m_ChannelInfo.m_IPInfo.m_MAC[0] = plChnNode->m_ChannelInfo.m_IPInfo.m_MAC[0] & 0xFE;
								}	
							}
							else if((plChnNode->m_ChannelType == HWL_CHANNEL_TYPE_IP_DEP) || (plChnNode->m_ChannelType == HWL_CHANNEL_TYPE_IP_LOOP_DEP))
							{
							}
							else
							{
								GLOBAL_TRACE(("Channel Type Error! Got ETH Channel = %d\n", plChnNode->m_ChannelType));
							}


							plHolder = mxmlFindElement(pBodyNode, pBodyNode, "sub_channel", NULL, NULL, MXML_DESCEND_FIRST);
							if (plHolder)
							{
								plXMLCurrent = plHolder;

								while(plXMLCurrent)
								{
									plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "sub_ind");
									if (plTmpStr)
									{
										lSubIndex = GLOBAL_STRTOL(plTmpStr, NULL, 10);
										if (GLOBAL_CHECK_INDEX(lSubIndex, plChnNode->m_SubChannelNumber))
										{
											plSubNode = &plChnNode->m_pSubChannelNode[lSubIndex];
											plSubNode->m_ActiveMark = MULTL_XMLGetNodeMark(plXMLCurrent, "active_mark");
											plSubNode->m_SubInfo.m_SubIPInfo.m_IPv4Addr = MULTL_XMLGetNodeIP(plXMLCurrent, "ipv4_des_addr");
											plSubNode->m_SubInfo.m_SubIPInfo.m_IPv4Port = MULTL_XMLGetNodeUINT(plXMLCurrent, "ipv4_des_port", 10);
											plSubNode->m_SubInfo.m_SubIPInfo.m_Protocol = MULTL_XMLGetNodeUINT(plXMLCurrent, "protocol", 10);
										}
										else
										{
											GLOBAL_TRACE(("Over Limits!! lSubIndex/MAX %d/%d \n", lSubIndex, plChnNode->m_SubChannelNumber));
										}
									}
									else
									{
										GLOBAL_TRACE(("Not Exist!\n"));
									}
									plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "sub_channel", NULL, NULL, MXML_NO_DESCEND);
								}	
								MULTL_ApplyOutETHParameter(pHandle, lChnIndex);	
								
							}
						}
						else
						{
							GLOBAL_TRACE(("Channel Type Error! Got ChannelType = %s\n", plTmpStr));
						}
					}
				}
			}
		}


	}
	else if (GLOBAL_STRCMP(pSubType, "igmp") == 0)
	{
		MULT_IGMP *plIGMP;
		plIGMP = &plParameter->m_IGMP;
		plIGMP->m_IGMPRepeateTime = MULTL_XMLGetNodeINT(pBodyNode, "igmp_repeate_time", 10);
		plIGMP->m_IGMPRepeateMark = MULTL_XMLGetNodeMark(pBodyNode, "igmp_repeate_mark");
		plIGMP->m_IGMPVersion = MULTL_XMLGetNodeINT(pBodyNode, "igmp_version", 10);
		MULTL_ApplyInETHParameter( pHandle, 0 );
	}
	else if (GLOBAL_STRCMP(pSubType, "sfn_modulator_setting") == 0)
	{
#ifdef SUPPORT_SFN_MODULATOR
		MULT_SFNXMLLoad(pBodyNode, TRUE);
		HWL_QAMForceNotLevelOnly();
		MULT_SFNApplyByQAMModule();
#endif
	}
	else if (GLOBAL_STRCMP(pSubType, "sfn_adaptor_setting") == 0)
	{
#ifdef SUPPORT_SFN_ADAPTER
		MULT_SFNAXMLLoad(pBodyNode, TRUE);
		MULT_SFNAApplyParameter();
#endif
	}
	else if (GLOBAL_STRCMP(pSubType, "gns_setting") == 0)
	{
#ifdef SUPPORT_GNS_MODULE
		MULT_GNSXMLLoad(pHandle, pBodyNode, TRUE);
		MULT_GNSApplyParameter();
#endif
	}
	else if (GLOBAL_STRCMP(pSubType, "ipots_setting") == 0)
	{
#ifdef SUPPORT_IP_O_TS_MODULE

		MULT_IPoTSXMLLoad(pHandle, pBodyNode, TRUE);
		MULT_IPoTSApply();
#endif
	}
	else if (GLOBAL_STRCMP(pSubType, "ext_dpd_setting") == 0)
	{
	}
	else
	{
		GLOBAL_ASSERT(0);
	}
	MULTL_SaveParameterXML(pHandle);
}

void MULTL_WEBXMLUploadConfirm(MULT_Handle *pHandle, mxml_node_t* pBodyNode, CHAR_T* pSubType, CHAR_T *pParameter)
{
	GLOBAL_TRACE(("SubType %s, Parameter %s\n", pSubType, pParameter));
	if (GLOBAL_STRCMP(pSubType, "confirm") == 0)
	{
		if (GLOBAL_STRCMP(pParameter, "ts") == 0)
		{
			U8 *plData;
			U16 lPID;
			S32 lFileSize;
			U32 lNewID;
			GLOBAL_FD lFD;
			MPEG2_DBManualTsInfo lManualInfo;

			lFileSize = 0;
			if ((lFD = GLOBAL_FOPEN(CGIC_UPLOAD_FILE_PATHNAME, "rb")) != NULL)
			{
				lFileSize = CAL_FileSize(lFD);

				if ((lFileSize % MPEG2_TS_PACKET_SIZE) == 0 && (lFileSize <= CGIC_UPLOAD_FILE_MAX_SIZE))
				{
					plData = (U8 *)GLOBAL_MALLOC(lFileSize);
					if (plData)
					{
						GLOBAL_FSEEK(lFD, 0, SEEK_SET);
						if (GLOBAL_FREAD(plData, lFileSize, 1, lFD) > 0)
						{
							if (MPEG2_TsSynByteCheck(plData, lFileSize))
							{
								lPID = MPEG2_TsGetPID(plData);
								if (MPEG2_DBMANGetFreeSpace(pHandle->m_DBSHandle) - lFileSize > 0)
								{
									lNewID = MPEG2_DBMANAdd(pHandle->m_DBSHandle, plData, lFileSize);
									if (MPEG2_DBMANProcTsInfo(pHandle->m_DBSHandle, lNewID, &lManualInfo, TRUE))
									{
										GLOBAL_SPRINTF((lManualInfo.m_pName, "TS %.8X", lNewID));
										MPEG2_DBMANProcTsInfo(pHandle->m_DBSHandle, lNewID, &lManualInfo, FALSE);
									}
								}
							}
						}
						GLOBAL_FREE(plData);
						plData = NULL;
					}
				}
				GLOBAL_FCLOSE(lFD);
				lFD = NULL;
			}
			MULTL_SaveParameterXML(pHandle);
		}
		else if (GLOBAL_STRCMP(pParameter, "software") == 0)
		{
			/*将upload.bin文件复制为program.bin*/
			GLOBAL_FD lFD;
			S32 lFileSize;
			CAL_FCAPHandle *plUPGHandle;

			plUPGHandle = NULL;
			lFD = GLOBAL_FOPEN(CGIC_UPLOAD_FILE_PATHNAME, "rb");
			if (lFD)
			{
				lFileSize = CAL_FileSize(lFD);
				plUPGHandle = UPG_PRGDecodeCreate(lFileSize);
				if (plUPGHandle)
				{
					GLOBAL_FREAD(plUPGHandle->m_pDataBuf, 1, lFileSize, lFD);
					GLOBAL_FCLOSE(lFD);
					if (UPG_PRGValidation(plUPGHandle, MULT_DEVICE_COMPLETE_TYPE) == TRUE)
					{
						S32 lPayloadSize;
						U8 *plPayload;
						CHAR_T plCMD[1024];
						GLOBAL_FD lNewFD;

						/*处理升级文件*/
						GLOBAL_SPRINTF((plCMD, "%s%s", MULT_STORAGE_BASE_DIR, MULT_PROGRAM_FILE_PATHNAME));
						lNewFD = GLOBAL_FOPEN(plCMD, "wb");
						if (lNewFD)
						{
							plPayload = CAL_FCAPGetPayloadPtr(plUPGHandle);
							lPayloadSize = CAL_FCAPGetPayloadSize(plUPGHandle);
							GLOBAL_FWRITE(plPayload, 1, lPayloadSize, lNewFD);
							GLOBAL_FCLOSE(lNewFD);
						}

						GLOBAL_TRACE(("Software Update Complete!\n"));
						/*让设备进入重启状态*/
						MULTL_RebootSequence(pHandle);
					}
					CAL_FCAPDestroy(plUPGHandle);
					plUPGHandle = NULL;
				}
			}

			/*重启*/
			MULTL_RebootSequence(pHandle);
		}
		else if (GLOBAL_STRCMP(pParameter, "kernel") == 0)
		{
			GLOBAL_FD lFD;
			S32 lFileSize;
			CAL_FCAPHandle *plUPGHandle;

			plUPGHandle = NULL;
			lFD = GLOBAL_FOPEN(CGIC_UPLOAD_FILE_PATHNAME, "rb");
			if (lFD)
			{
				lFileSize = CAL_FileSize(lFD);
				plUPGHandle = UPG_PRGDecodeCreate(lFileSize);
				if (plUPGHandle)
				{
					GLOBAL_FREAD(plUPGHandle->m_pDataBuf, 1, lFileSize, lFD);
					GLOBAL_FCLOSE(lFD);
					if (UPG_PRGValidation(plUPGHandle, MULT_DEVICE_COMPLETE_TYPE) == TRUE)
					{
						S32 lPayloadSize;
						U8 *plPayload;
						CHAR_T plCMD[1024];
						GLOBAL_FD lNewFD;

						/*处理升级文件*/
						GLOBAL_SPRINTF((plCMD, "%s", MULT_NEW_KERNEL_FILE_PATHNAME));
						lNewFD = GLOBAL_FOPEN(plCMD, "wb");
						if (lNewFD)
						{
							plPayload = CAL_FCAPGetPayloadPtr(plUPGHandle);
							lPayloadSize = CAL_FCAPGetPayloadSize(plUPGHandle);
							GLOBAL_FWRITE(plPayload, 1, lPayloadSize, lNewFD);
							GLOBAL_FCLOSE(lNewFD);
						}

						GLOBAL_TRACE(("Kernel Update Complete!\n"));
						/*让设备进入重启状态*/
						MULTL_RebootSequence(pHandle);
					}
					CAL_FCAPDestroy(plUPGHandle);
					plUPGHandle = NULL;
				}
			}

			/*重启*/
			MULTL_RebootSequence(pHandle);
		}
#ifdef ENCODER_CARD_PLATFORM
		else if (GLOBAL_STRCMP(pParameter, "modules") == 0)
		{
			GLOBAL_FD lFD;
			S32 lFileSize;
			CAL_FCAPHandle *plUPGHandle;

			plUPGHandle = NULL;
			lFD = GLOBAL_FOPEN(CGIC_UPLOAD_FILE_PATHNAME, "rb");
			if (lFD)
			{
				lFileSize = CAL_FileSize(lFD);
				plUPGHandle = UPG_MODULEDecodeCreate(lFileSize);
				if (plUPGHandle)
				{
					GLOBAL_FREAD(plUPGHandle->m_pDataBuf, 1, lFileSize, lFD);
					GLOBAL_FCLOSE(lFD);
					if (UPG_MODULEValidation(plUPGHandle, MULT_DEVICE_COMPLETE_TYPE) == TRUE)
					{
						UPG_MODULEInfo lInfo;
						S32 lPayloadSize;
						U8 *plPayload;
						CHAR_T plCMD[1024];

						UPG_MODULEGetModuleDevType(plUPGHandle, &lInfo);

						if (lInfo.m_ModuleType > 0)
						{
							/*保存模块固件(FPGA)文件*/
							PFC_System("mkdir -p %s", CARD_MODULE_FILE_STORAGE_PATHNAME);

							GLOBAL_SPRINTF((plCMD, "mv -f %s %s/module_%04x.bin", CGIC_UPLOAD_FILE_PATHNAME, CARD_MODULE_FILE_STORAGE_PATHNAME, lInfo.m_ModuleType));

							PFC_System(plCMD);

							CARD_XMLUpdateModuleFile();

							GLOBAL_TRACE(("Sub Module Type [%04x] Update Complete!\n", lInfo.m_ModuleType));
						}
						else
						{
							GLOBAL_TRACE(("ModuleDevType Error = %04X\n", lInfo.m_ModuleType));
						}

					}
					CAL_FCAPDestroy(plUPGHandle);
					plUPGHandle = NULL;
				}
			}
		}
#endif
		else if (GLOBAL_STRCMP(pParameter, "parameter") == 0)
		{
			CHAR_T plCMD[1024];
			GLOBAL_SPRINTF((plCMD, "%s", CGIC_UPLOAD_FILE_PATHNAME));
			/*解开参数文件*/
			if (MULTL_LoadParamterFromStorage(pHandle, plCMD))
			{
				/*重新保存*/
				MULTL_SaveParamterToStorage(pHandle);
			}
			/*重启*/
			MULTL_RebootSequence(pHandle);
		}
		else if (GLOBAL_STRCMP(pParameter, "oem") == 0)
		{
			/*再次校验后复制OEM文件*/
			if (MULTL_LoadOEM(pHandle, CGIC_UPLOAD_FILE_PATHNAME, TRUE, NULL, 0))
			{
				CHAR_T plTmpCMD[1024];
				GLOBAL_SPRINTF((plTmpCMD, "mv -f %s %s%s", CGIC_UPLOAD_FILE_PATHNAME, MULT_STORAGE_BASE_DIR, MULT_OEM_FILE_PATHNAME));
				PFC_System(plTmpCMD);
				GLOBAL_TRACE(("OEM Update Successful!\n"));
			}
			else
			{
				GLOBAL_TRACE(("OEM Check Failed!\n"));
			}

			/*手动重启*/
			//MULTL_RebootSequence(pHandle);
		}
		else if (GLOBAL_STRCMP(pParameter, "license") == 0)
		{
			BOOL bValid;
			/*再次校验后复制授权文件*/
			bValid = MULTL_LoadLicense(pHandle, CGIC_UPLOAD_FILE_PATHNAME, TRUE, NULL, 0);
			if (bValid)
			{
				CHAR_T plTmpCMD[1024];
				GLOBAL_SPRINTF((plTmpCMD, "mv -f %s %s%s", CGIC_UPLOAD_FILE_PATHNAME, MULT_STORAGE_BASE_DIR, MULT_LICENSE_FILE_PATHNAME));
				PFC_System(plTmpCMD);

			}

			/*重启*/
			MULTL_RebootSequence(pHandle);
		}
		else if (GLOBAL_STRCMP(pParameter, "water_mark") == 0)
		{
			/*water_mark*/
			GLOBAL_TRACE(("Confirm water_mark!!!!!!!!!!!!!!\n"));
			CHAR_T plTmpCMD[1024];
			GLOBAL_SPRINTF((plTmpCMD, "mv -f %s %s", CGIC_UPLOAD_FILE_PATHNAME, WATER_MARK_FILE));
			PFC_System(plTmpCMD);

			MULTL_SetSaveMark(pHandle, FALSE);
		}
		else
		{
			if (MULT_SubFlashXMLProcess(pHandle, pBodyNode, pParameter, MULT_SUB_FLASH_OP_TYPE_CONFIRM) == FALSE)
			{
				GLOBAL_ASSERT(0); 
			}
		}
	}
	else
	{
		GLOBAL_ASSERT(0);
	}
}

void MULTL_WEBXMLPostProcess(MULT_Handle *pHandle, CHAR_T* pXMLStr, S32 StrBufSize)
{
	CHAR_T *plType, *plSubType, *plParameter;
	mxml_node_t *plXML;
	mxml_node_t *plRoot;
	mxml_node_t *plHolder;
	mxml_node_t *plBody;
	plXML = mxmlLoadString(NULL, pXMLStr, MXML_OPAQUE_CALLBACK);

	if (plXML)
	{
		plRoot = mxmlFindElement(plXML, plXML, "root", NULL, NULL, MXML_DESCEND_FIRST);
		if (plRoot)
		{
			plHolder = mxmlFindElement(plRoot, plRoot, "post_head", NULL, NULL, MXML_DESCEND_FIRST);
			if (plHolder)
			{
				plType = MULTL_XMLGetNodeText(plHolder, "type");
				plSubType = MULTL_XMLGetNodeText(plHolder, "subtype");
				plParameter = MULTL_XMLGetNodeText(plHolder, "parameter");

				plBody = mxmlFindElement(plRoot, plRoot, "post_body", NULL, NULL, MXML_DESCEND_FIRST);
				MULT_SetApplyBusyMark(pHandle, TRUE);

				if (GLOBAL_STRCMP(plType, "system_config") == 0)
				{
					MULTL_WEBXMLSystemConfig(pHandle, plBody, plSubType, plParameter);
				}
				else if (GLOBAL_STRCMP(plType, "remux_config") == 0)
				{
					MULTL_WEBXMLRemuxConfig(pHandle, plBody, plSubType, plParameter);
				}
				else if (GLOBAL_STRCMP(plType, "io_config") == 0)
				{
					MULTL_WEBXMLIOConfig(pHandle, plBody, plSubType, plParameter);
				}
				else if (GLOBAL_STRCMP(plType, "scramble") == 0)
				{
					MULTL_WEBXMLScrambleConfig(pHandle, plBody, plSubType, plParameter);
				}
				else if (GLOBAL_STRCMP(plType, "monitor") == 0)
				{
					MULTL_WEBXMLMonitorConfig(pHandle, plBody, plSubType, plParameter);
				}
				else if (GLOBAL_STRCMP(plType, "validation") == 0)
				{
					MULTL_WEBXMLUploadConfirm(pHandle, plBody, plSubType, plParameter);
				}

#ifdef ENCODER_CARD_PLATFORM
				else if (GLOBAL_STRCMP(plType, "modules") == 0)
				{
					MULT_CARDModuleXMLPostProcess(pHandle, plRoot);
				}
#endif
				else
				{
					GLOBAL_ASSERT(0);
				}
				MULT_SetApplyBusyMark(pHandle, FALSE);
			}
		}

		mxmlDelete(plXML);
		plXML = NULL;
	}

}



/*GET AJAX 操作*/
void MULTL_WEBXMLTunerGet(MULT_Handle *pHandle, mxml_node_t* pBodyNode, CHAR_T* pSubType, CHAR_T *pParameter)
{
	if (GLOBAL_STRCMP(pSubType, "tunerstatus") == 0)
	{
		if (GLOBAL_STRCMP(pParameter, "monitor") == 0)
		{
			S32 i, lLogNum;
			BOOL bLock;
			F64 lStrength, lQuality, lBER;
			mxml_node_t *plXML;
			mxml_node_t *plXMLRoot;
			mxml_node_t *plXMLHolder;

			plXML = mxmlNewXML("1.0");
			plXMLRoot = mxmlNewElement(plXML, "root");
			/*----------特殊处理， tuner通道数为4----------*/

			for (i = 0; i <  pHandle->m_TunerCount; i++)
			{	
				plXMLHolder = mxmlNewElement(plXMLRoot, "sig");

				HWL_TDGetSignalFloat(i, &bLock, &lStrength, &lQuality, &lBER);
				MULTL_XMLAddNodeFLOAT(plXMLHolder, "signal_strength", lStrength);
				MULTL_XMLAddNodeFLOAT(plXMLHolder, "signal_quality", lQuality);
				MULTL_XMLAddNodeINT(plXMLHolder, "lock_status", bLock);
				if (bLock)
				{
					if (pHandle->m_TunerType == TD_DEMOD_TYPE_ATBM8869 || pHandle->m_TunerType == TD_DEMOD_TYPE_ATBM8881)
					{
						MULTL_XMLAddNodeText(plXMLHolder, "constellation", TD_DVBCParamConstellationStr(HWL_TDGetDVBCConStellation(i)));
					}
					else if (pHandle->m_TunerType == TD_DEMOD_TYPE_AVL6211)
					{
						TD_DVBSXChnlInfo lDVBSXParam;
						if (HWL_TDGetDVBSChannelParam(i, &lDVBSXParam))
						{
							MULTL_XMLAddNodeText(plXMLHolder, "constellation", TD_DVBSXParamConstellationStr(lDVBSXParam.m_QAMMode));
						}
					}
				}

			}			

			mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
			mxmlDelete(plXML);
			plXML = NULL;	
		}	
	}
	else if (GLOBAL_STRCMP(pSubType, "ip_over_ts") == 0)
	{
#ifdef SUPPORT_IP_O_TS_MODULE

		if (GLOBAL_STRCMP(pParameter, "recv_status") == 0)
		{
			mxml_node_t *plXML;
			mxml_node_t *plXMLRoot;
			mxml_node_t *plXMLHolder;

			plXML = mxmlNewXML("1.0");
			plXMLRoot = mxmlNewElement(plXML, "root");
			MULT_IPoTSXMLSave(pHandle, plXMLRoot, TRUE);
			mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
			mxmlDelete(plXML);
			plXML = NULL;	
		}
#endif
	}
	else
	{
		GLOBAL_ASSERT(0);
	}
}

void MULTL_WEBXMLMonitorGet(MULT_Handle *pHandle, mxml_node_t* pBodyNode, CHAR_T* pSubType, CHAR_T *pParameter)
{
	MULT_Monitor *plMonitor;
	plMonitor = &pHandle->m_Monitor;

	if (GLOBAL_STRCMP(pSubType, "alarms") == 0)
	{
		S32 i, lLogNum;
		mxml_node_t *plXML;
		mxml_node_t *plXMLRoot;
		mxml_node_t *plXMLHolder;

		plXML = mxmlNewXML("1.0");
		plXMLRoot = mxmlNewElement(plXML, "root");

		lLogNum = CAL_LogGetLogCFGCount(plMonitor->m_LogHandle);
		for (i = 0; i < lLogNum; i++)
		{
			if (CAL_LogGetUsedMark(plMonitor->m_LogHandle, i))
			{
				plXMLHolder = mxmlNewElement(plXMLRoot, "log");
				MULTL_XMLAddNodeINT(plXMLHolder, "count", CAL_LogGetLogInfoCount(plMonitor->m_LogHandle, i));
			}
		}

		mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
		mxmlDelete(plXML);
		plXML = NULL;
	}
	else if (GLOBAL_STRCMP(pSubType, "alarms_detail") == 0)
	{
		S32 i, lLogNum, lLogSlot;
		CHAR_T *plTmpStr;
		TIME_T lTimeT;
		STRUCT_TM lTM;
		CAL_LogInfo lLogInfo;
		CHAR_T plTimeBuf[64];
		mxml_node_t *plXML;
		mxml_node_t *plXMLRoot;
		mxml_node_t *plXMLHolder;
		mxml_node_t *plXMLSubHolder;


		plTmpStr = pParameter;
		if (plTmpStr)
		{
			lLogSlot = GLOBAL_STRTOUL(plTmpStr, NULL, 16) - 1;
		}
		else
		{
			lLogSlot = GLOBAL_INVALID_INDEX;
		}

		//GLOBAL_TRACE(("alarms_detail slot = %d\n", lLogSlot));

		plXML = mxmlNewXML("1.0");
		plXMLRoot = mxmlNewElement(plXML, "root");

		if (GLOBAL_CHECK_INDEX(lLogSlot, CAL_LogGetLogCFGCount(plMonitor->m_LogHandle)))
		{
			lLogNum = CAL_LogGetLogInfoCount(plMonitor->m_LogHandle, lLogSlot);
			for (i = 0; i < lLogNum; i++)
			{
				plXMLHolder = mxmlNewElement(plXMLRoot, "detail");
				CAL_LogGetLogInfo(plMonitor->m_LogHandle, lLogSlot, i, &lLogInfo);
				MULTL_XMLAddNodeUINT(plXMLHolder, "usr_data", lLogInfo.m_UserValue);

				lTimeT = lLogInfo.m_ErrorTime;
				GLOBAL_GMTTIME_R(&lTM, &lTimeT);
				CAL_StringDateToStr(plTimeBuf, &lTM);
				MULTL_XMLAddNodeText(plXMLHolder, "time_date", plTimeBuf);
			}
		}

		mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
		mxmlDelete(plXML);
		plXML = NULL;

	}
	else if (GLOBAL_STRCMP(pSubType, "status_new") == 0)
	{
		S32 i;
		mxml_node_t *plXML;
		mxml_node_t *plXMLRoot;
		mxml_node_t *plXMLHolder;
		MULT_SCS *plMultSCS;
		MULT_SCSSlot *plSCSSlot;

		plXML = mxmlNewXML("1.0");
		plXMLRoot = mxmlNewElement(plXML, "root");

		//这里需要按子板来划分不同的功能，目前暂时按照统一的标准来发送
		MULTL_XMLAddNodeINT(plXMLRoot, "temperature", plMonitor->m_CurrentTemp);
		MULTL_XMLAddNodeINT(plXMLRoot, "fan_status", 1/*plMonitor->m_FanStatus*/);
		MULTL_XMLAddNodeINT(plXMLRoot, "ins_bitrate", plMonitor->m_InserterBitrate);

		for (i = 0; i < plMonitor->m_ETHNumber; i++)
		{
			MULTL_XMLAddNodeINT(plXMLRoot, "eth_status", plMonitor->m_EthStatus[i]);
		}

		MULTL_XMLAddNodeINT(plXMLRoot, "cpu_usage", plMonitor->m_CPUUsage);
		MULTL_XMLAddNodeINT(plXMLRoot, "mem_usage", plMonitor->m_MEMUsage);
#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
		MULTL_XMLAddNodeINT(plXMLRoot, "mem_size", plMonitor->m_MEMSize);
		MULTL_XMLAddNodeINT(plXMLRoot, "cpu_temp", plMonitor->m_CpuCurrentTemp);
		
#endif
#ifdef SUPPORT_NEW_HWL_MODULE
		MULTL_XMLAddNodeINT(plXMLRoot, "icp_in", HWL_MonitorInternalCOMBitrateGet(TRUE));
		MULTL_XMLAddNodeINT(plXMLRoot, "icp_out", HWL_MonitorInternalCOMBitrateGet(FALSE));
#endif
		MULTL_XMLAddNodeFLOAT(plXMLRoot, "main_in", plMonitor->m_TotalInBitrate);
		MULTL_XMLAddNodeFLOAT(plXMLRoot, "main_out", plMonitor->m_TotalOutBitrate);

		for (i = 0; i < 4; i++)
		{
			MULTL_XMLAddNodeFLOAT(plXMLRoot, "volt", 0);
		}

		/*增加SCS状态信息*/
		plMultSCS = &pHandle->m_MultSCS;
		MULTL_XMLAddNodeINT(plXMLRoot, "scramble_status", plMultSCS->m_ScrambleStatus);
		MULTL_XMLAddNodeINT(plXMLRoot, "crypto_period_number", plMultSCS->m_CurrentCPNumber);
		MULTL_XMLAddNodeINT(plXMLRoot, "crypto_period_duration", plMultSCS->m_CurrentCPDuration);
		MULTL_XMLAddNodeText(plXMLRoot, "ecm_update", plMultSCS->m_pECMTimeDataBuf);

		for (i = 0; i < MPEG2_DB_SIMULCRYPT_CA_MAX_NUM; i++)
		{
			plSCSSlot = &plMultSCS->m_SCSSlot[i];
			plXMLHolder = mxmlNewElement(plXMLRoot, "scs");
			MULTL_XMLAddNodeINT(plXMLHolder, "active_mark", plSCSSlot->m_bActiveMark);
			MULTL_XMLAddNodeINT(plXMLHolder, "emm_status", plSCSSlot->m_EMMLinkStatus);
			MULTL_XMLAddNodeINT(plXMLHolder, "emm_bitrate", plSCSSlot->m_EMMCurBitrate);
			MULTL_XMLAddNodeINT(plXMLHolder, "ecm_status", plSCSSlot->m_ECMLinkStatus);
		}

		mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
		mxmlDelete(plXML);
		plXML = NULL;
	}
	else if (GLOBAL_STRCMP(pSubType, "bitrate") == 0)
	{
		S32 i, k;
		mxml_node_t *plXML;
		mxml_node_t *plXMLRoot;
		mxml_node_t *plXMLIO;
		mxml_node_t *plXMLChn;
		mxml_node_t *plXMLSub;
		MULT_Parameter *plParameter;
		MULT_MonitorCHN *plMonitorChn;
		MULT_MonitorSUB *plMonitorSub;

		plParameter = &pHandle->m_Parameter;

		plXML = mxmlNewXML("1.0");
		plXMLRoot = mxmlNewElement(plXML, "root");

		plXMLIO = mxmlNewElement(plXMLRoot, "in");

		//GLOBAL_TRACE(("plMonitor->m_InChnNum = %d\n", plMonitor->m_InChnNum));

		for (i = 0; i < plMonitor->m_InChnNum; i++)
		{
			plMonitorChn = &plMonitor->m_pInChnArray[i];
			plXMLChn = mxmlNewElement(plXMLIO, "chn");
			MULTL_XMLAddNodeINT(plXMLChn, "total", plMonitorChn->m_CurValue);
			for (k = 0; k < plMonitorChn->m_SubNumber; k++)
			{
				plMonitorSub = &plMonitorChn->m_pSubArray[k];
				if (plParameter->m_pInChannel[i].m_pSubChannelNode[k].m_ActiveMark != FALSE)
				{
					plXMLSub = mxmlNewElement(plXMLChn, "sub");
					MULTL_XMLAddNodeINT(plXMLSub, "ind", k);
					MULTL_XMLAddNodeINT(plXMLSub, "bit", plMonitorSub->m_CurValue);
				}
			}
		}

		plXMLIO = mxmlNewElement(plXMLRoot, "out");
		for (i = 0; i < plMonitor->m_OutChnNum; i++)
		{
			plMonitorChn = &plMonitor->m_pOutChnArray[i];
			plXMLChn = mxmlNewElement(plXMLIO, "chn");
			MULTL_XMLAddNodeINT(plXMLChn, "total", plMonitorChn->m_CurValue);
			for (k = 0; k < plMonitorChn->m_SubNumber; k++)
			{
				plMonitorSub = &plMonitorChn->m_pSubArray[k];
				if (plParameter->m_pOutChannel[i].m_pSubChannelNode[k].m_ActiveMark != FALSE)
				{
					plXMLSub = mxmlNewElement(plXMLChn, "sub");
					MULTL_XMLAddNodeINT(plXMLSub, "ind", k);
					MULTL_XMLAddNodeINT(plXMLSub, "bit", plMonitorSub->m_CurValue);
				}
			}
		}


		mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
		mxmlDelete(plXML);
		plXML = NULL;

	}
#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
	else if (GLOBAL_STRCMP(pSubType, "input_stat") == 0)
	{
		S32 i, k;
		mxml_node_t *plXML;
		mxml_node_t *plXMLRoot;
		mxml_node_t *plXMLIO;
		INTERFACE_BOARD_TYPE  board_type = INTERFACE_BOARD_UNKNOWN;

		board_type = HWL_Get_BoardType();
		
		plXML = mxmlNewXML("1.0");
		plXMLRoot = mxmlNewElement(plXML, "root");

		for (i = 0; i < MULT_MAX_CHN_NUM; i++) 
		{
			plXMLIO = mxmlNewElement(plXMLRoot, "in_stat");
			MULTL_XMLAddNodeINT(plXMLIO, "ind", k);

			if (MULTL_Adv7612ViMode2LEncoderViMode(plMonitor->m_HdmiRxStatus[i].m_VideoStandard) == VI_MODE_UNKNOWN)//20190530 XJ 修正480P/576P 无法使用但是显示为1080P的问题
			{
				MULTL_XMLAddNodeINT(plXMLIO, "lock_stat", FALSE);
			}
			else
			{
				MULTL_XMLAddNodeINT(plXMLIO, "lock_stat", plMonitor->m_HdmiRxStatus[i].m_SignalIsLocked);
			}

#if defined(GN1846)
    		if (board_type == INTERFACE_BOARD_HDMI)
			{
				MULTL_XMLAddNodeText(plXMLIO, "vid_format", MULTL_XMLEncViModeValueToStr(MULTL_Adv7612ViMode2LEncoderViMode(plMonitor->m_HdmiRxStatus[i].m_VideoStandard)));
				MULTL_XMLAddNodeText(plXMLIO, "aud_sample", MULTL_XMLAudSampValueToStr(MULTL_Adv7612AudSample2LEncoderAudSample(plMonitor->m_HdmiRxStatus[i].m_AudioSample)));
			}
			else
			{
				MULTL_XMLAddNodeText(plXMLIO, "vid_format", MULTL_XMLEncViModeValueToStr(HWL_Gs2970ViMode2LEncoderViMode(plMonitor->m_HdmiRxStatus[i].m_VideoStandard)));
				MULTL_XMLAddNodeText(plXMLIO, "aud_sample", MULTL_XMLAudSampValueToStr(AUD_SAMP_48K));
			}
#endif

#if defined(GN1866)
			if (board_type == INTERFACE_BOARD_HDMI)
			{
				MULTL_XMLAddNodeText(plXMLIO, "vid_format", MULTL_XMLEncViModeValueToStr(MULTL_Adv7612ViMode2LEncoderViMode(plMonitor->m_HdmiRxStatus[i].m_VideoStandard)));
				MULTL_XMLAddNodeText(plXMLIO, "aud_sample", MULTL_XMLAudSampValueToStr(MULTL_Adv7612AudSample2LEncoderAudSample(plMonitor->m_HdmiRxStatus[i].m_AudioSample)));
			}
			else
			{
				MULTL_XMLAddNodeText(plXMLIO, "vid_format", MULTL_XMLEncViModeValueToStr(HWL_Gs2970ViMode2LEncoderViMode(plMonitor->m_HdmiRxStatus[i].m_VideoStandard)));
				MULTL_XMLAddNodeText(plXMLIO, "aud_sample", MULTL_XMLAudSampValueToStr(AUD_SAMP_48K));
			}
#endif

#if defined(GN1866B)
			if (pHandle->m_Information.m_InputBoardType == INTERFACE_BOARD_4K)
			{
				MULTL_XMLAddNodeText(plXMLIO, "vid_format", MULTL_XMLEncViModeValueToStr(MULTL_Lt6911cViMode2LEncoderViMode(plMonitor->m_HdmiRxStatus[i].m_VideoStandard)));
				MULTL_XMLAddNodeText(plXMLIO, "aud_sample", MULTL_XMLAudSampValueToStr(MULTL_Lt6911cAudSample2LEncoderAudSample(plMonitor->m_HdmiRxStatus[i].m_AudioSample)));
			}
			else if (pHandle->m_Information.m_InputBoardType == INTERFACE_BOARD_HDMI)
			{
				MULTL_XMLAddNodeText(plXMLIO, "vid_format", MULTL_XMLEncViModeValueToStr(MULTL_Adv7612ViMode2LEncoderViMode(plMonitor->m_HdmiRxStatus[i].m_VideoStandard)));
				MULTL_XMLAddNodeText(plXMLIO, "aud_sample", MULTL_XMLAudSampValueToStr(MULTL_Adv7612AudSample2LEncoderAudSample(plMonitor->m_HdmiRxStatus[i].m_AudioSample)));
			}
#endif
		}
		    mxml_node_t *plTmpNode;
	        plTmpNode = mxmlFindElement(pBodyNode, pBodyNode, "current_frame_channel", NULL, NULL, MXML_DESCEND_FIRST);
	        if(plTmpNode)
	        {
	            S32 lChnnum;
	            lChnnum = MULTL_XMLGetNodeINT(pBodyNode, "current_frame_channel",10) - 1;
	            int res = 0;
#if defined(GN1846)
                res = ENC_3531AGetCurrentFrame(lChnnum);
#endif

                if(res != 0)
                {
                    MULTL_XMLAddNodeINT(plXMLRoot, "current_frame_state", 0);
                }
                else{
                    MULTL_XMLAddNodeINT(plXMLRoot, "current_frame_state", 1);
                }
	        }

		mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
		mxmlDelete(plXML);
		plXML = NULL;
	}
#endif
	else if (GLOBAL_STRCMP(pSubType, "pid_statistcs") == 0) 
	{
		S32 i;
		S32 lChnIndex, lSubIndex;
		BOOL blInput;
		CHAR_T *plTmpStr;
		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "direction");
		if (plTmpStr)
		{
			if (GLOBAL_STRCMP(plTmpStr, "in") == 0)
			{
				blInput = TRUE;
			}
			else
			{
				blInput = FALSE;
			}
		}
		else
		{
			blInput = TRUE;
		}
		lChnIndex = MULTL_XMLGetNodeINT(pBodyNode, "chn_index", 10);
		lSubIndex = MULTL_XMLGetNodeINT(pBodyNode, "sub_ind", 10);

		//GLOBAL_TRACE(("--------------------------------test by leonli ChnID = %d , SubChnId = %d\n", lChnIndex, lSubIndex));
		{
			/*发起端口设置命令*/
#ifdef SUPPORT_NEW_HWL_MODULE
			BOOL blInput;
			CHAR_T *plTmpStr;
			plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "direction");
			if (plTmpStr)
			{
				if (GLOBAL_STRCMP(plTmpStr, "in") == 0)
				{
					blInput = TRUE;
				}
				else
				{
					blInput = FALSE;
				}
			}
			else
			{
				blInput = TRUE;
			}

			HWL_MonitorPIDStatisticConfig(lChnIndex, lSubIndex, blInput);
#else
			if (blInput)
			{
				if (GLOBAL_CHECK_INDEX(lChnIndex,pHandle->m_Parameter.m_InChannelNumber))
				{
					plMonitor->m_PIDStatisticsChnIndex = pHandle->m_Parameter.m_pInChannel[lChnIndex].m_OriginalIndex;
					for(i = 0; i < lChnIndex; i++)
					{
						lSubIndex +=  pHandle->m_Parameter.m_pInChannel[ i ].m_SubChannelNumber;
					}

					plMonitor->m_PIDStatisticsSubIndex = lSubIndex;
					//根据端口特殊处理
					plMonitor->m_PIDStatisticsChnIndex = 0;
					//因FPGA协议与实际通道的不配	
				}
			}
			else
			{
				if (GLOBAL_CHECK_INDEX(lChnIndex,pHandle->m_Parameter.m_OutChannelNumber))
				{
					plMonitor->m_PIDStatisticsChnIndex = pHandle->m_Parameter.m_pOutChannel[lChnIndex].m_OriginalIndex;
					for(i = 0; i < lChnIndex; i++)
					{
						lSubIndex +=  pHandle->m_Parameter.m_pOutChannel[ i ].m_SubChannelNumber;
					}

					plMonitor->m_PIDStatisticsSubIndex = lSubIndex;
					//根据端口特殊处理
					plMonitor->m_PIDStatisticsChnIndex = 1;
				}
			}
#if (!defined(GN1846) && !defined(GN1866) && !defined(GN1866B))
			HWL_PidStatisticSend(plMonitor->m_PIDStatisticsChnIndex, plMonitor->m_PIDStatisticsSubIndex);
#endif
#endif

		}

		/*简单回复*/
		{
			mxml_node_t *plXML;
			plXML = mxmlNewXML("1.0");
			MULTL_XMLAddNodeText(plXML, "replay", "ok");
			mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
			mxmlDelete(plXML);
			plXML = NULL;
		}
	}
	else if (GLOBAL_STRCMP(pSubType, "pid_update") == 0)
	{
		/*发起更新命令*/

#ifdef SUPPORT_NEW_HWL_MODULE
		HWL_MonitorPIDStatisticResultReq();
#else
#if (!defined(GN1846) && !defined(GN1866) && !defined(GN1866B))
		HWL_PidStatisticSearch(plMonitor->m_PIDStatisticsChnIndex, plMonitor->m_PIDStatisticsSubIndex);
#endif
#endif
		/*简单回复*/
		{
			mxml_node_t *plXML;
			plXML = mxmlNewXML("1.0");
			MULTL_XMLAddNodeText(plXML, "replay", "ok");
			mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
			mxmlDelete(plXML);
			plXML = NULL;
		}
	}
	else if (GLOBAL_STRCMP(pSubType, "pid_get") == 0)
	{
		S32 i;
		mxml_node_t *plXML;
		mxml_node_t *plXMLRoot;
		mxml_node_t *plXMLHolder;

#ifdef SUPPORT_NEW_HWL_MODULE
		HWL_MonitorPIDStatisticResultGet(&plMonitor->m_PIDArray);
		/*生成恢复XML数据！*/
		plXML = mxmlNewXML("1.0");
		plXMLRoot = mxmlNewElement(plXML, "root");
		for (i = 0; i < plMonitor->m_PIDArray.m_Number; i++)
		{
			plXMLHolder = mxmlNewElement(plXMLRoot, "pid");
			MULTL_XMLAddNodeUINT(plXMLHolder, "value", plMonitor->m_PIDArray.m_pNode[i].m_PID);
			MULTL_XMLAddNodeUINT(plXMLHolder, "bps", plMonitor->m_PIDArray.m_pNode[i].m_Bitrate);
		}
		plXMLHolder = mxmlNewElement(plXMLRoot, "pid");
		MULTL_XMLAddNodeUINT(plXMLHolder, "value", 0xFFFF);
		MULTL_XMLAddNodeUINT(plXMLHolder, "bps", plMonitor->m_PIDArray.m_Bitrate);
#else
		/*获取当前数据*/
#if defined(GN1846) || defined(GN1866) ||defined(GN1866B)
		{
			HWL_LENCODER_Status lStatus;

			/*
			m_PIDStatisticsChnIndex: 0 输入，1 输出
			*/
			if (plMonitor->m_PIDStatisticsChnIndex == 0 || 
				(plMonitor->m_PIDStatisticsChnIndex == 1 && pHandle->m_Configuration.m_IpOutputType == IP_OUTPUT_SPTS)) { /* 输入或者 SPTS 输出 PID 统计 */
				HWL_ENCODER_GetStatus(0, plMonitor->m_PIDStatisticsSubIndex, &lStatus);
				plMonitor->m_PIDArray.channelRateArraySize = 0;
				for (i = 0; i < MPEG2_TS_PACKET_MAX_PID_NUM; i++) {
					if (lStatus.m_Status.m_PidTsCount[i] != 0) {
						plMonitor->m_PIDArray.pidRateArray[plMonitor->m_PIDArray.channelRateArraySize].pidRate = lStatus.m_Status.m_PidTsCount[i] * MPEG2_TS_PACKET_SIZE * 8;
						plMonitor->m_PIDArray.pidRateArray[plMonitor->m_PIDArray.channelRateArraySize].pidVal = i;
						plMonitor->m_PIDArray.channelRateArraySize++;
					}
				}
				plMonitor->m_PIDArray.totalRate = HWL_ENCODER_GetTsBitrate(0, plMonitor->m_PIDStatisticsSubIndex);
			}
			else { /* MPTS 输出 PID 统计，只有一个 TS 流输出，所以统计的结果是 TS 输出中的所有 PID 的值 */
				S32 j;
				U32 lTotalBitrate = 0;

				plMonitor->m_PIDArray.channelRateArraySize = 0;
				for (j = 0; j < MULT_MAX_CHN_NUM; j++) {
					HWL_ENCODER_GetStatus(0, j, &lStatus);
					for (i = 0; i < MPEG2_TS_PACKET_MAX_PID_NUM; i++) {
						if (lStatus.m_Status.m_PidTsCount[i] != 0) {
							plMonitor->m_PIDArray.pidRateArray[plMonitor->m_PIDArray.channelRateArraySize].pidRate = lStatus.m_Status.m_PidTsCount[i] * MPEG2_TS_PACKET_SIZE * 8;
							plMonitor->m_PIDArray.pidRateArray[plMonitor->m_PIDArray.channelRateArraySize].pidVal = i;
							plMonitor->m_PIDArray.channelRateArraySize++;
						}
					}
					
					lTotalBitrate += HWL_ENCODER_GetTsBitrate(0, j);
				}
				plMonitor->m_PIDArray.totalRate = lTotalBitrate;
			}
		}
#else
		HWL_PidStatisticArray(&plMonitor->m_PIDArray);
#endif
		/*生成恢复XML数据！*/
		plXML = mxmlNewXML("1.0");
		plXMLRoot = mxmlNewElement(plXML, "root");
		for (i = 0; i < plMonitor->m_PIDArray.channelRateArraySize; i++)
		{
			plXMLHolder = mxmlNewElement(plXMLRoot, "pid");
			MULTL_XMLAddNodeUINT(plXMLHolder, "value", plMonitor->m_PIDArray.pidRateArray[i].pidVal);
			MULTL_XMLAddNodeUINT(plXMLHolder, "bps", plMonitor->m_PIDArray.pidRateArray[i].pidRate);
		}
		plXMLHolder = mxmlNewElement(plXMLRoot, "pid");
		MULTL_XMLAddNodeUINT(plXMLHolder, "value", 0xFFFF);
		MULTL_XMLAddNodeUINT(plXMLHolder, "bps", plMonitor->m_PIDArray.totalRate);
#endif

		mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
		mxmlDelete(plXML);
		plXML = NULL;

	}
	else if (GLOBAL_STRCMP(pSubType, "ip_statistcs") == 0) 
	{
		S32 lChnIndex, lSubIndex;
		BOOL blInput;
		CHAR_T *plTmpStr;
		plTmpStr = MULTL_XMLGetNodeText(pBodyNode, "direction");
		if (plTmpStr)
		{
			if (GLOBAL_STRCMP(plTmpStr, "in") == 0)
			{
				blInput = TRUE;
			}
			else
			{
				blInput = FALSE;
			}
		}
		else
		{
			blInput = TRUE;
		}
		lChnIndex = MULTL_XMLGetNodeINT(pBodyNode, "chn_index", 10);

		GLOBAL_TRACE(("Monitor IP Chn %d Start!!!!!!!!!!!!!!!\n", plMonitor->m_IPStatisticsChnIndex));

		if (blInput)
		{
			if (GLOBAL_CHECK_INDEX(lChnIndex, pHandle->m_Parameter.m_InChannelNumber))
			{
				plMonitor->m_IPStatisticsChnIndex = pHandle->m_Parameter.m_pInChannel[lChnIndex].m_OriginalIndex;
			}
		}
		else
		{
			if (GLOBAL_CHECK_INDEX(lChnIndex,pHandle->m_Parameter.m_OutChannelNumber))
			{
				plMonitor->m_IPStatisticsChnIndex = pHandle->m_Parameter.m_pOutChannel[lChnIndex].m_OriginalIndex;
			}
		}


		{
			/*首先清除所有的信息*/
			HWL_IPPortStatisticClear(plMonitor->m_IPStatisticsChnIndex);
		}

		/*简单回复*/
		{
			mxml_node_t *plXML;
			plXML = mxmlNewXML("1.0");
			MULTL_XMLAddNodeText(plXML, "replay", "ok");
			mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
			mxmlDelete(plXML);
			plXML = NULL;
		}
	}
	else if (GLOBAL_STRCMP(pSubType, "ip_update") == 0)
	{
		/*发起更新命令*/
		GLOBAL_TRACE(("Monitor IP Chn %d Update!!!!!!!!!!!!!!!\n", plMonitor->m_IPStatisticsChnIndex));

		HWL_IPPortStatisticSend(plMonitor->m_IPStatisticsChnIndex);
		/*简单回复*/
		{
			mxml_node_t *plXML;
			plXML = mxmlNewXML("1.0");
			MULTL_XMLAddNodeText(plXML, "replay", "ok");
			mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
			mxmlDelete(plXML);
			plXML = NULL;
		}
	}
	else if (GLOBAL_STRCMP(pSubType, "ip_get") == 0)
	{
		S32 i;
		mxml_node_t *plXML;
		mxml_node_t *plXMLRoot;
		mxml_node_t *plXMLHolder;

		GLOBAL_TRACE(("Monitor IP Chn %d GetData Count = %d\n", plMonitor->m_IPStatisticsChnIndex, plMonitor->m_IPArray.ArrySize));
		/*获取当前数据*/
		HWL_IPPortStatisticArrayGet(plMonitor->m_IPStatisticsChnIndex, &plMonitor->m_IPArray);
		/*生成回复XML数据！*/
		plXML = mxmlNewXML("1.0");
		plXMLRoot = mxmlNewElement(plXML, "root");
		for (i = 0; i < plMonitor->m_IPArray.ArrySize; i++)
		{
			plXMLHolder = mxmlNewElement(plXMLRoot, "ip");
			MULTL_XMLAddNodeText(plXMLHolder, "ipv4", PFC_SocketNToA(plMonitor->m_IPArray.ipPortInfoArray[i].addressv4));
			MULTL_XMLAddNodeUINT(plXMLHolder, "protocol", plMonitor->m_IPArray.ipPortInfoArray[i].protocol);
			MULTL_XMLAddNodeUINT(plXMLHolder, "port", plMonitor->m_IPArray.ipPortInfoArray[i].portNumber);
			MULTL_XMLAddNodeUINT(plXMLHolder, "bps", plMonitor->m_IPArray.ipPortInfoArray[i].portRate);
		}
		plXMLHolder = mxmlNewElement(plXMLRoot, "ip");
		MULTL_XMLAddNodeText(plXMLHolder, "ipv4", "0");
		MULTL_XMLAddNodeUINT(plXMLHolder, "protocol", 0);
		MULTL_XMLAddNodeUINT(plXMLHolder, "port", 0);
		MULTL_XMLAddNodeUINT(plXMLHolder, "bps", plMonitor->m_IPArray.totalRate);

		mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
		mxmlDelete(plXML);
		plXML = NULL;

	}
	else if (GLOBAL_STRCMP(pSubType, "refresh_syslog") == 0)
	{
#ifdef SUPPORT_SYSLOG_MODULE
		S32 lActLen;
		lActLen = MULT_SyslogGet(pHandle, pHandle->m_pReplayXMLBuf, MULT_MAX_XML_STRING_SIZE);
		pHandle->m_pReplayXMLBuf[lActLen] = 0;
#endif
	}

#ifdef MULT_TS_BACKUP_SUPPORT
	else if (GLOBAL_STRCMP(pSubType, "ts_backup_status") == 0)
	{
		MULT_BPSaveStatusXMLToBuf(pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf));
	}
#endif
	else
	{
		GLOBAL_ASSERT(0);
	}

}

void MULTL_WEBXMLStatusGet(MULT_Handle *pHandle, mxml_node_t* pBodyNode, CHAR_T* pSubType, CHAR_T *pParameter)
{
	MULT_Monitor *plMonitor;
	mxml_node_t *plXML;
	mxml_node_t *plXMLRoot;

	plXML = mxmlNewXML("1.0");
	plXMLRoot = mxmlNewElement(plXML, "root");

	plMonitor = &pHandle->m_Monitor;

	if (GLOBAL_STRCMP(pSubType, "status") == 0)
	{
		S32 i;
		S32 lLogNum, lTotalCount;
		MULT_SCS *plMultSCS;
		MULT_SCSSlot *plSCSSlot;
		mxml_node_t *plXMLHolder;
		mxml_node_t *plXMLSubHolder;

		plMultSCS = &pHandle->m_MultSCS;

		MULTL_XMLAddNodeINT(plXMLRoot, "busy_mark", pHandle->m_Monitor.m_BusyMark|| pHandle->m_Monitor.m_ApplyParametersBusyMark);
		lTotalCount = 0;
		lLogNum = CAL_LogGetLogCFGCount(plMonitor->m_LogHandle);
		for (i = 0; i < lLogNum; i++)
		{
#ifdef SUPPORT_NEW_ALARM_SYSTEM
			if (CAL_LogGetUsedMark(plMonitor->m_LogHandle, i))
			{
				lTotalCount += CAL_LogGetLogInfoCount(plMonitor->m_LogHandle, i);
			}
#endif
		}
		MULTL_XMLAddNodeINT(plXMLRoot, "alarm", lTotalCount);

		MULTL_XMLAddNodeFLOAT(plXMLRoot, "input_bitrate", plMonitor->m_TotalInBitrate);

		MULTL_XMLAddNodeFLOAT(plXMLRoot, "output_bitrate", plMonitor->m_TotalOutBitrate);

		MULTL_XMLAddNodeUINT(plXMLRoot, "inserter_bitrate", plMonitor->m_InserterBitrate);

		MULTL_XMLAddNodeINT(plXMLRoot, "save_mark", plMonitor->m_bSaveMark);

		MULTL_XMLAddNodeINT(plXMLRoot, "apply_mark", plMonitor->m_bApplyMark);

		plXMLHolder = mxmlNewElement(plXMLRoot, "scs_status");
		MULTL_XMLAddNodeINT(plXMLHolder, "scramble_status", plMultSCS->m_ScrambleStatus);
		MULTL_XMLAddNodeINT(plXMLHolder, "crypto_period_number", plMultSCS->m_CurrentCPNumber);
		MULTL_XMLAddNodeINT(plXMLHolder, "crypto_durationd", plMultSCS->m_CurrentCPDuration);
		MULTL_XMLAddNodeText(plXMLHolder, "ecm_update", plMultSCS->m_pECMTimeDataBuf);

		for (i = 0; i < plMultSCS->m_SCSCount; i++)
		{
			plSCSSlot = &plMultSCS->m_SCSSlot[i];
			plXMLSubHolder = mxmlNewElement(plXMLHolder, "scs");
			MULTL_XMLAddNodeINT(plXMLSubHolder, "active_mark", plSCSSlot->m_bActiveMark);
			MULTL_XMLAddNodeINT(plXMLSubHolder, "emm_status", plSCSSlot->m_EMMLinkStatus);
			MULTL_XMLAddNodeINT(plXMLSubHolder, "emm_bitrate", plSCSSlot->m_EMMCurBitrate);
			MULTL_XMLAddNodeINT(plXMLSubHolder, "ecm_status", plSCSSlot->m_ECMLinkStatus);
		}
	}
	else if (GLOBAL_STRCMP(pSubType, "card_status") == 0)
	{
		S32 i;
		S32 lLogNum, lTotalCount;
		mxml_node_t *plXMLHolder;
		mxml_node_t *plXMLSubHolder;


		MULTL_XMLAddNodeINT(plXMLRoot, "busy_mark", plMonitor->m_BusyMark || plMonitor->m_ApplyParametersBusyMark);

		lTotalCount = 0;
		lLogNum = CAL_LogGetLogCFGCount(plMonitor->m_LogHandle);
		for (i = 0; i < lLogNum; i++)
		{
#ifdef SUPPORT_NEW_ALARM_SYSTEM
			if (CAL_LogGetUsedMark(plMonitor->m_LogHandle, i))
#endif
			{
				lTotalCount += CAL_LogGetLogInfoCount(plMonitor->m_LogHandle, i);
			}
		}
		MULTL_XMLAddNodeINT(plXMLRoot, "alarm", lTotalCount);

		MULTL_XMLAddNodeFLOAT(plXMLRoot, "input_bitrate", plMonitor->m_TotalInBitrate);

		MULTL_XMLAddNodeFLOAT(plXMLRoot, "output_bitrate", plMonitor->m_TotalOutBitrate);

		MULTL_XMLAddNodeUINT(plXMLRoot, "inserter_bitrate", plMonitor->m_InserterBitrate);

		MULTL_XMLAddNodeINT(plXMLRoot, "save_mark", plMonitor->m_bSaveMark);

		MULTL_XMLAddNodeINT(plXMLRoot, "apply_mark", plMonitor->m_bApplyMark);

#ifdef ENCODER_CARD_PLATFORM
		CARD_XMLProcess(NULL, plXMLRoot, CARD_XML_OP_TYPE_STATUS_GET);
#endif
	}
	else if (GLOBAL_STRCMP(pSubType, "time") == 0)
	{
		MULTL_XMLAddNodeText(plXMLRoot, "time_date", plMonitor->m_pTimeDataBuf);

	}
	else if (GLOBAL_STRCMP(pSubType, "sfn_status") == 0)
	{
#ifdef SUPPORT_SFN_MODULATOR
		MULT_SFNXMLSave(plXMLRoot, TRUE);
#endif
	}
	else if (GLOBAL_STRCMP(pSubType, "sfna_status") == 0)
	{
#ifdef SUPPORT_SFN_ADAPTER
		MULT_SFNAXMLSave(plXMLRoot, TRUE);
#endif
	}
	else if (GLOBAL_STRCMP(pSubType, "gns_status") == 0)
	{
#ifdef SUPPORT_GNS_MODULE
		MULT_GNSXMLSave(pHandle,plXMLRoot, TRUE);
#endif
	}
#ifdef SUPPORT_PES_ANALYS_FUNCTION
	else if (GLOBAL_STRCMP(pSubType, "es_anaylse") == 0)
	{
		PES_XMLAcess(pBodyNode, plXMLRoot);
	}
#endif
	else if (GLOBAL_STRCMP(pSubType, "manual_reg") == 0)
	{
		U16 lChipID;
		S32 i, lBusWidth;
		U32 lAddr;
		CHAR_T *plTmpValue;
		plTmpValue = XML_WarpGetNodeText(pBodyNode, "cmd_type", "");

		XML_WarpAddNodeText(plXMLRoot, "cmd_type", plTmpValue);


		lChipID = XML_WarpGetNodeU32(pBodyNode, "chip_id", 16, 0);
		lBusWidth = XML_WarpGetNodeU32(pBodyNode, "bus_width", 10, 0);
		lAddr = XML_WarpGetNodeU32(pBodyNode, "addr", 16, 0);

		GLOBAL_TRACE(("ChipID = 0x%04X, BusWidth = %d, Address = 0x%08X\n", lChipID, lBusWidth, lAddr));
		if (GLOBAL_STRCMP(plTmpValue, "read") == 0)
		{
			S32 lReadSize;
			U32 *plTmpData;


			lReadSize = XML_WarpGetNodeU32(pBodyNode, "read_size", 10, 0);
			GLOBAL_TRACE(("Read Size = %d\n", lReadSize));

			plTmpData = (U32 *)GLOBAL_ZMALLOC(sizeof(U32) * lReadSize);
			if (plTmpData)
			{
				if (HWL_FPGARegProcess(0, lChipID, lAddr, plTmpData, lReadSize, TRUE, 1000))
				{
					CHAR_T *plTmpCSV;

					for (i = 0; i < lReadSize; i++)
					{
						if (lBusWidth == 32)
						{
							GLOBAL_TRACE(("Value[%d] = 0x%08X\n", i, plTmpData[i]));
						}
						else if (lBusWidth == 16)
						{
							GLOBAL_TRACE(("Value[%d] = 0x%04X\n", i, plTmpData[i] & 0xFFFF));
						}
						else if (lBusWidth == 8)
						{
							GLOBAL_TRACE(("Value[%d] = 0x%02X\n", i, plTmpData[i] & 0xFF));
						}
						else
						{
							GLOBAL_TRACE(("Value[%d] = 0x%08X Width Error!\n", i, plTmpData[i]));
						}
					}

					plTmpCSV = (CHAR_T *)GLOBAL_ZMALLOC(10 * (lReadSize + 1));
					if (plTmpCSV)
					{
						CAL_StringBinToCSV(plTmpData, lReadSize, CAL_STRING_CSV_BIN_COVERT_BIN_TYPE_U32_HEX, plTmpCSV, 10 * (lReadSize + 1), TRUE);

						GLOBAL_TRACE(("CSV = %s\n", plTmpCSV));

						XML_WarpAddNodeText(plXMLRoot, "read_value", plTmpCSV);

						GLOBAL_FREE(plTmpCSV);
					}


					XML_WarpAddNodeText(plXMLRoot, "cmd_status", "ok");
				}
				else
				{
					XML_WarpAddNodeText(plXMLRoot, "read_value", "NO Return");
					XML_WarpAddNodeText(plXMLRoot, "cmd_status", "error");
				}
				GLOBAL_FREE(plTmpData);
			}
			else
			{
				XML_WarpAddNodeText(plXMLRoot, "cmd_status", "error");
			}
		}
		else
		{
			CHAR_T *plHEXBuf;
			S32 i, lCount;

			plHEXBuf = XML_WarpGetNodeText(pBodyNode, "write_data", "");


			lCount = CAL_StringCSVToBinCount(plHEXBuf);

			if (lCount > 0)
			{
				U32 *plTmpData;

				plTmpData = (U32 *)GLOBAL_ZMALLOC(sizeof(U32) * lCount);
				if (plTmpData)
				{
					//GLOBAL_TRACE(("%s\n", plHEXBuf));
					CAL_StringCSVToBin(plHEXBuf, plTmpData, lCount, CAL_STRING_CSV_BIN_COVERT_BIN_TYPE_U32_HEX);

					for (i = 0; i < lCount; i++)
					{
						if (lBusWidth == 32)
						{
							GLOBAL_TRACE(("Value[%d] = 0x%08X\n", i, plTmpData[i]));
						}
						else if (lBusWidth == 16)
						{
							GLOBAL_TRACE(("Value[%d] = 0x%04X\n", i, plTmpData[i] & 0xFFFF));
						}
						else if (lBusWidth == 8)
						{
							GLOBAL_TRACE(("Value[%d] = 0x%02X\n", i, plTmpData[i] & 0xFF));
						}
						else
						{
							GLOBAL_TRACE(("Value[%d] = 0x%08X Width Error!\n", i, plTmpData[i]));
						}
					}

					if (HWL_FPGARegProcess(0, lChipID, lAddr, plTmpData, lCount, FALSE, 1000))
					{
						XML_WarpAddNodeText(plXMLRoot, "cmd_status", "ok");
					}

					GLOBAL_FREE(plTmpData);
				}
				else
				{
					XML_WarpAddNodeText(plXMLRoot, "cmd_status", "error");
				}
			}
			else
			{
				XML_WarpAddNodeText(plXMLRoot, "cmd_status", "error");
			}


		}

	}
	else
	{
		GLOBAL_ASSERT(0);
	}

	mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
	mxmlDelete(plXML);
	plXML = NULL;

}

void MULTL_WEBXMLUploadValidationGet(MULT_Handle *pHandle, mxml_node_t* pBodyNode, CHAR_T* pSubType, CHAR_T *pParameter)
{
	if (GLOBAL_STRCMP(pSubType, "check") == 0)
	{
		if (GLOBAL_STRCMP(pParameter, "ts") == 0)
		{
			S32 lFileSize;
			U8 *plData;
			BOOL lbValidation;
			U16 lPID;
			GLOBAL_FD lFD;
			mxml_node_t *plXML;
			mxml_node_t *plXMLRoot;

			plXML = mxmlNewXML("1.0");
			plXMLRoot = mxmlNewElement(plXML, "root");
			lFileSize = 0;
			lbValidation = FALSE;
			if ((lFD = GLOBAL_FOPEN(CGIC_UPLOAD_FILE_PATHNAME, "rb")) != NULL)
			{
				lFileSize = CAL_FileSize(lFD);

				if ((lFileSize % MPEG2_TS_PACKET_SIZE) == 0 && (lFileSize <= CGIC_UPLOAD_FILE_MAX_SIZE))
				{
					plData = (U8 *)GLOBAL_MALLOC(lFileSize);
					if (plData)
					{
						GLOBAL_FSEEK(lFD, 0, SEEK_SET);
						if (GLOBAL_FREAD(plData, lFileSize, 1, lFD) > 0)
						{
							if (MPEG2_TsSynByteCheck(plData, lFileSize))
							{
								lPID = MPEG2_TsGetPID(plData);
								if (MPEG2_DBMANGetFreeSpace(pHandle->m_DBSHandle) - lFileSize > 0) 
								{
									GLOBAL_TRACE(("Valid TS OK!\n"));
									lbValidation = TRUE;
								}
							}
						}
						GLOBAL_FREE(plData);
						plData = NULL;
					}
				}
				GLOBAL_FCLOSE(lFD);
				lFD = NULL;
			}

			if (lbValidation == TRUE)
			{
				MULTL_XMLAddNodeINT(plXMLRoot, "validation", 1);
				MULTL_XMLAddNodeUINT(plXMLRoot, "information", lPID);
			}
			else
			{
				MULTL_XMLAddNodeINT(plXMLRoot, "validation", 0);
				MULTL_XMLAddNodeText(plXMLRoot, "information", "---------");
			}

			mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
			mxmlDelete(plXML);
			plXML = NULL;
		}
		else if ((GLOBAL_STRCMP(pParameter, "software") == 0) || (GLOBAL_STRCMP(pParameter, "kernel") == 0))
		{
			BOOL bValid = FALSE;
			CHAR_T plInformation[1024], *plTmpInfo;
			GLOBAL_FD lFD;
			S32 lFileSize;
			CAL_FCAPHandle *plUPGHandle;
			mxml_node_t *plXML;
			mxml_node_t *plXMLRoot;

			plUPGHandle = NULL;
			GLOBAL_ZEROMEM(plInformation, sizeof(plInformation));
			lFD = GLOBAL_FOPEN(CGIC_UPLOAD_FILE_PATHNAME, "rb");
			if (lFD)
			{
				lFileSize = CAL_FileSize(lFD);
				plUPGHandle = UPG_PRGDecodeCreate(lFileSize);
				if (plUPGHandle)
				{
					GLOBAL_FREAD(plUPGHandle->m_pDataBuf, 1, lFileSize, lFD);
					if (UPG_PRGValidation(plUPGHandle, MULT_DEVICE_COMPLETE_TYPE) == TRUE)
					{
						bValid = TRUE;
						plTmpInfo = CAL_FCAPGetDescription(plUPGHandle);
						if (GLOBAL_STRLEN(plTmpInfo) < sizeof(plInformation))
						{
							GLOBAL_STRCPY(plInformation, plTmpInfo);
							GLOBAL_TRACE(("Valid %s OK! %s\n", pParameter, plInformation));
						}
					}
					else
					{
						GLOBAL_TRACE(("Valid %s Failed!\n", pParameter));
					}
					CAL_FCAPDestroy(plUPGHandle);
					plUPGHandle = NULL;
				}
				GLOBAL_FCLOSE(lFD);
			}


			plXML = mxmlNewXML("1.0");
			plXMLRoot = mxmlNewElement(plXML, "root");

			if (bValid == TRUE)
			{
				MULTL_XMLAddNodeINT(plXMLRoot, "validation", 1);
				MULTL_XMLAddNodeText(plXMLRoot, "information", plInformation);
			}
			else
			{
				MULTL_XMLAddNodeINT(plXMLRoot, "validation", 0);
				MULTL_XMLAddNodeText(plXMLRoot, "information", "---------");
			}

			mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
			mxmlDelete(plXML);
			plXML = NULL;
		}
#ifdef ENCODER_CARD_PLATFORM
		else if (GLOBAL_STRCMP(pParameter, "modules") == 0)
		{
			BOOL bValid = FALSE;
			CHAR_T plInformation[1024], *plTmpInfo;
			GLOBAL_FD lFD;
			S32 lFileSize;
			CAL_FCAPHandle *plUPGHandle;
			mxml_node_t *plXML;
			mxml_node_t *plXMLRoot;

			plUPGHandle = NULL;
			GLOBAL_ZEROMEM(plInformation, sizeof(plInformation));
			lFD = GLOBAL_FOPEN(CGIC_UPLOAD_FILE_PATHNAME, "rb");
			if (lFD)
			{
				lFileSize = CAL_FileSize(lFD);

				if (lFileSize < PAL_StorageGetPartitionFreeSpace(CARD_MODULE_FILE_STORAGE_PATHNAME))
				{
					plUPGHandle = UPG_MODULEDecodeCreate(lFileSize);
					if (plUPGHandle)
					{
						GLOBAL_FREAD(plUPGHandle->m_pDataBuf, 1, lFileSize, lFD);
						if (UPG_MODULEValidation(plUPGHandle, MULT_DEVICE_COMPLETE_TYPE) == TRUE)
						{
							bValid = TRUE;
							plTmpInfo = CAL_FCAPGetDescription(plUPGHandle);
							if (GLOBAL_STRLEN(plTmpInfo) < sizeof(plInformation))
							{
								GLOBAL_STRCPY(plInformation, plTmpInfo);
								GLOBAL_TRACE(("Valid %s OK! %s\n", pParameter, plInformation));
							}
						}
						else
						{
							GLOBAL_TRACE(("Valid %s Failed!\n", pParameter));
						}
						CAL_FCAPDestroy(plUPGHandle);
						plUPGHandle = NULL;
					}
				}
				else
				{
					GLOBAL_TRACE(("Not Storage Space!!!!\n"));
				}
				GLOBAL_FCLOSE(lFD);
			}


			plXML = mxmlNewXML("1.0");
			plXMLRoot = mxmlNewElement(plXML, "root");

			if (bValid == TRUE)
			{
				MULTL_XMLAddNodeINT(plXMLRoot, "validation", 1);
				MULTL_XMLAddNodeText(plXMLRoot, "information", plInformation);
			}
			else
			{
				MULTL_XMLAddNodeINT(plXMLRoot, "validation", 0);
				MULTL_XMLAddNodeText(plXMLRoot, "information", "---------");
			}

			mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
			mxmlDelete(plXML);
			plXML = NULL;
		}
#endif
		else if (GLOBAL_STRCMP(pParameter, "parameter") == 0)
		{
			BOOL bValid = FALSE;
			CHAR_T plInformation[1024], *plTmpInfo;
			GLOBAL_FD lFD;
			S32 lFileSize;
			CAL_FCAPHandle *plUPGHandle;
			mxml_node_t *plXML;
			mxml_node_t *plXMLRoot;

			plUPGHandle = NULL;
			lFD = GLOBAL_FOPEN(CGIC_UPLOAD_FILE_PATHNAME, "rb");
			if (lFD)
			{
				lFileSize = CAL_FileSize(lFD);
				plUPGHandle = CAL_FCAPDencodeCreate(lFileSize);
				if (plUPGHandle)
				{
					GLOBAL_FREAD(plUPGHandle->m_pDataBuf, 1, lFileSize, lFD);
					if (CAL_FCAPValidation(plUPGHandle, MULT_DEVICE_COMPLETE_TYPE, MULT_PARAMETER_DESCRIPTOR) == TRUE)
					{
						/*处理参数文件*/
						GLOBAL_TRACE(("Valid Parameter OK!\n"));
						bValid = TRUE;
						plTmpInfo = CAL_FCAPGetDescription(plUPGHandle);
						if (GLOBAL_STRLEN(plTmpInfo) < sizeof(plInformation))
						{
							GLOBAL_STRCPY(plInformation, plTmpInfo);
						}
					}
					CAL_FCAPDestroy(plUPGHandle);
					plUPGHandle = NULL;
				}
				GLOBAL_FCLOSE(lFD);
			}

			plXML = mxmlNewXML("1.0");
			plXMLRoot = mxmlNewElement(plXML, "root");

			if (bValid)
			{
				MULTL_XMLAddNodeINT(plXMLRoot, "validation", 1);
				MULTL_XMLAddNodeText(plXMLRoot, "information", plInformation);
			}
			else
			{
				MULTL_XMLAddNodeINT(plXMLRoot, "validation", 0);
				MULTL_XMLAddNodeText(plXMLRoot, "information", "---------");
			}

			mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
			mxmlDelete(plXML);
			plXML = NULL;
		}
		else if (GLOBAL_STRCMP(pParameter, "oem") == 0)
		{
			/*打开OEM文件校验并返回信息部分，这里返回的信息是OEM厂商的名称*/
			BOOL bValid = FALSE;
			CHAR_T plInformation[512];
			mxml_node_t *plXML;
			mxml_node_t *plXMLRoot;

			bValid = MULTL_LoadOEM(pHandle, CGIC_UPLOAD_FILE_PATHNAME, TRUE, plInformation, sizeof(plInformation));

			plXML = mxmlNewXML("1.0");
			plXMLRoot = mxmlNewElement(plXML, "root");

			if (bValid)
			{
				MULTL_XMLAddNodeINT(plXMLRoot, "validation", 1);
				MULTL_XMLAddNodeText(plXMLRoot, "information", plInformation);
			}
			else
			{
				MULTL_XMLAddNodeINT(plXMLRoot, "validation", 0);
				MULTL_XMLAddNodeText(plXMLRoot, "information", "---------");
			}

			mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
			mxmlDelete(plXML);
			plXML = NULL;
		}
		else if (GLOBAL_STRCMP(pParameter, "license") == 0)
		{
			BOOL bValid = FALSE;
			CHAR_T plInformation[512];
			mxml_node_t *plXML;
			mxml_node_t *plXMLRoot;

			bValid = MULTL_LoadLicense(pHandle, CGIC_UPLOAD_FILE_PATHNAME, TRUE, plInformation, sizeof(plInformation));

			plXML = mxmlNewXML("1.0");
			plXMLRoot = mxmlNewElement(plXML, "root");

			if (bValid)
			{
				MULTL_XMLAddNodeINT(plXMLRoot, "validation", 1);
				MULTL_XMLAddNodeText(plXMLRoot, "information", plInformation);
			}
			else
			{
				MULTL_XMLAddNodeINT(plXMLRoot, "validation", 0);
				MULTL_XMLAddNodeText(plXMLRoot, "information", "---------");
			}

			mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
			mxmlDelete(plXML);
			plXML = NULL;
		}
#if defined(SUPPORT_WATER_MARK)
		else if (GLOBAL_STRCMP(pParameter, "water_mark") == 0)
		{
			BOOL bValid = FALSE;
			GLOBAL_FD lFD;
			S32 lFileSize;
			mxml_node_t *plXML;
			mxml_node_t *plXMLRoot;

			lFD = GLOBAL_FOPEN(CGIC_UPLOAD_FILE_PATHNAME, "rb");
			if (lFD)
			{
				lFileSize = CAL_FileSize(lFD);
				if (lFileSize > 0)
				{
					if (IMA_WarpCheckPNG(CGIC_UPLOAD_FILE_PATHNAME))
					{
						bValid = TRUE;
					}
				}

				GLOBAL_TRACE((" Upload PNG file size:%d (bValid = %d)\r\n", lFileSize, bValid));

				GLOBAL_FCLOSE(lFD);
			}

			plXML = mxmlNewXML("1.0");
			plXMLRoot = mxmlNewElement(plXML, "root");

			if (bValid)
			{
				MULTL_XMLAddNodeINT(plXMLRoot, "validation", 1);
				MULTL_XMLAddNodeText(plXMLRoot, "information", "png file");
			}
			else
			{
				MULTL_XMLAddNodeINT(plXMLRoot, "validation", 0);
				MULTL_XMLAddNodeText(plXMLRoot, "information", "------");
			}

			mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
			mxmlDelete(plXML);
			plXML = NULL;
		}
#endif
		else
		{
			if (MULT_SubFlashXMLProcess(pHandle, pBodyNode, pParameter, MULT_SUB_FLASH_OP_TYPE_VALIDATION_GET) == FALSE)
			{
				GLOBAL_ASSERT(0); 
			}
		}
	}
}

#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
void MULTL_WEBXMLEdidCfgGet(MULT_Handle *pHandle, mxml_node_t* pBodyNode, CHAR_T* pSubType, CHAR_T *pParameter)
{
	CHAR_T *plTmpStr;
	mxml_node_t *plHolder;
	mxml_node_t *plXMLCurrent;
	S32 lIndex = 0;
	mxml_node_t *plXML;
	mxml_node_t *plXMLRoot;
	CHAR_T plTagStr[16];

	plXML = mxmlNewXML("1.0");
	plXMLRoot = mxmlNewElement(plXML, "root");

	plHolder = mxmlFindElement(pBodyNode, pBodyNode, "edid", NULL, NULL, MXML_DESCEND_FIRST);
	if (plHolder)
	{
		plXMLCurrent = plHolder;
		while(plXMLCurrent)
		{
			plTmpStr = MULTL_XMLGetNodeText(plXMLCurrent, "edid_type");
			if (plTmpStr)
			{
				HDMI_RxEdidType lEdidType;

				lEdidType = MULTL_XMLEdidTypeValueFromStr(plTmpStr);
				GLOBAL_TRACE(("DownLoad Edid! Index:%d, Type:%d\n", lIndex, lEdidType));
				GLOBAL_SNPRINTF((plTagStr, 16, "edid_status%d", lIndex));
				pHandle->m_EdidInfo.m_EdidType[lIndex] = lEdidType;
				if (HWL_HdmiRxDownloadEdid(lIndex, lEdidType)) {
					MULTL_XMLAddNodeText(plXMLRoot, plTagStr, "download_success");
				}
				else {
					MULTL_XMLAddNodeText(plXMLRoot, plTagStr, "download_failed");
				}
				
				lIndex++;
			}
			plXMLCurrent = mxmlFindElement(plXMLCurrent, pBodyNode, "edid", NULL, NULL, MXML_NO_DESCEND);
		}
	}
	MULTL_SaveEdidXML(pHandle);

	mxmlSaveString(plXML, pHandle->m_pReplayXMLBuf, sizeof(pHandle->m_pReplayXMLBuf), NULL);
	mxmlDelete(plXML);
	plXML = NULL;
}
#endif

void MULTL_WEBXMLGetProcess(MULT_Handle *pHandle, CHAR_T* pXMLStr, S32 StrBufSize)
{
	CHAR_T *plType, *plSubType, *plParameter;
	mxml_node_t *plXML;
	mxml_node_t *plRoot;
	mxml_node_t *plHolder;
	mxml_node_t *plBody;

	pHandle->m_pReplayXMLBuf[0] = 0;//确保没有任何东西的输出为空！
	plXML = mxmlLoadString(NULL, pXMLStr, MXML_OPAQUE_CALLBACK);
	if (plXML)
	{
		plRoot = mxmlFindElement(plXML, plXML, "root", NULL, NULL, MXML_DESCEND_FIRST);
		if (plRoot)
		{
			plHolder = mxmlFindElement(plRoot, plRoot, "post_head", NULL, NULL, MXML_DESCEND_FIRST); 
			if (plHolder)
			{
				plType = MULTL_XMLGetNodeText(plHolder, "type");
				plSubType = MULTL_XMLGetNodeText(plHolder, "subtype");
				plParameter = MULTL_XMLGetNodeText(plHolder, "parameter");

				plBody = mxmlFindElement(plRoot, plRoot, "post_body", NULL, NULL, MXML_DESCEND_FIRST);
				if (GLOBAL_STRCMP(plType, "monitor") == 0)
				{
					MULTL_WEBXMLMonitorGet(pHandle, plBody, plSubType, plParameter);
				}
				else if (GLOBAL_STRCMP(plType, "status") == 0)
				{
					MULTL_WEBXMLStatusGet(pHandle, plBody, plSubType, plParameter);
				}
				else if (GLOBAL_STRCMP(plType, "validation") == 0)
				{
					MULTL_WEBXMLUploadValidationGet(pHandle, plBody, plSubType, plParameter);
				}
				else if(GLOBAL_STRCMP(plType, "tuners") == 0)
				{
					MULTL_WEBXMLTunerGet(pHandle, plBody, plSubType, plParameter);

				}

#ifdef ENCODER_CARD_PLATFORM
				else if (GLOBAL_STRCMP(plType, "modules") == 0)
				{
					MULT_CARDModuleXMLGetProcess(pHandle, plRoot);
				}
#endif
#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
				else if (GLOBAL_STRCMP(plType, "edid_cfg") == 0) 
				{
					MULTL_WEBXMLEdidCfgGet(pHandle, plBody, plSubType, plParameter);
				}
#endif
				else
				{
					GLOBAL_ASSERT(0);
				}
			}
		}

		mxmlDelete(plXML);
		plXML = NULL;
	}


}


/*EOF*/
