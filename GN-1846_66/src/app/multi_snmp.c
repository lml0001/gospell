
/* Includes-------------------------------------------------------------------- */
#include "global_micros.h"
#include "platform_assist.h"
#include "multi_main_internal.h"
#include "net_snmp_warp.h"
/* Global Variables (extern)--------------------------------------------------- */
/* Macro (local)--------------------------------------------------------------- */
//#define MULT_SNMP_CONF_DIR	("/tmp/snmp")
//#define MULT_SNMP_CONF_NAME	("snmp.conf")
//# 
/* Private Constants ---------------------------------------------------------- */
/* Private Types -------------------------------------------------------------- */
/* Private Variables (static)-------------------------------------------------- */
/* Private Function prototypes ------------------------------------------------ */
/* Functions ------------------------------------------------------------------ */

//#ifdef SUPPORT_1846_SNMP
#if 1
#endif

/*回调函数*/
S32 MULTL_SNMPCallbackFunc(HANDLE32 UserHandle, U32 ID, S32 ValueType, void *pValue, S32 ValueSize, BOOL bRead)
{
	S32 lRetValueSize;
	MULT_Handle *plHandle = (MULT_Handle*)UserHandle;
	MULT_Monitor *plMonitorInfo;
	plMonitorInfo = &plHandle->m_Monitor;
	lRetValueSize = 0;
	if (plHandle)
	{
		GLOBAL_TRACE(("ID = %d, Type = %d, Size = %d\n", ID, ValueType, ValueSize));
		if (ID == SNMP_WARP_MIB2_SYSTEM_ID_DESC)
		{
			MULT_Information *plInfo;
			plInfo = &plHandle->m_Information;

			lRetValueSize = GLOBAL_STRLEN(plInfo->m_pWEBENG);
			if (ValueSize > lRetValueSize)
			{
				GLOBAL_STRCPY((CHAR_T *)pValue, plInfo->m_pWEBENG);
			}
			else
			{
				lRetValueSize = 0;
			}
		}
		else if (ID == SNMP_WARP_MIB2_SYSTEM_ID_OID)
		{
			lRetValueSize = SNMP_WarpOIDString2Bin(SNMP_GOSPELL_DEVICE_BASE_OID_STR, pValue, ValueSize);
		}
		else if (ID == SNMP_WARP_MIB2_SYSTEM_ID_UPTIME)
		{
			MULT_Monitor *plInfo;
			plInfo = &plHandle->m_Monitor;
			(*(U32 *)pValue) = (U32)(plInfo->m_CurDuration * 100);
			lRetValueSize = 4;
		}
		else if (ID == SNMP_WARP_MIB2_SYSTEM_ID_CONTACT)
		{
			MULT_Information *plInfo;
			plInfo = &plHandle->m_Information;

			lRetValueSize = GLOBAL_STRLEN(plInfo->m_pManufacter);
			if (ValueSize > lRetValueSize)
			{
				GLOBAL_STRCPY((CHAR_T *)pValue, plInfo->m_pManufacter);
			}
			else
			{
				lRetValueSize = 0;
			}
		}
		else if (ID == SNMP_WARP_MIB2_SYSTEM_ID_NAME)
		{
			SNMP_InitParam *plInfo;
			plInfo = &plHandle->m_Configuration.m_SNMPInitParam;

			lRetValueSize = GLOBAL_STRLEN(plInfo->m_pDeviceName);
			if (ValueSize > lRetValueSize)
			{
				GLOBAL_STRCPY((CHAR_T *)pValue, plInfo->m_pDeviceName);
			}
			else
			{
				lRetValueSize = 0;
			}
		}
		else if (ID == SNMP_WARP_MIB2_SYSTEM_ID_LOCATION)
		{
			SNMP_InitParam *plInfo;
			plInfo = &plHandle->m_Configuration.m_SNMPInitParam;

			lRetValueSize = GLOBAL_STRLEN(plInfo->m_pDeviceLocation);
			if (ValueSize > lRetValueSize)
			{
				GLOBAL_STRCPY((CHAR_T *)pValue, plInfo->m_pDeviceLocation);
			}
			else
			{
				lRetValueSize = 0;
			}
		}
		else if (ID == SNMP_WARP_MIB2_SYSTEM_ID_SERVICE)
		{
			(*(U32 *)pValue) = 4;
			lRetValueSize = 4;
		}
		/*下面是设备节点*/
		else if (ID == SNMP_WARP_GOSDEVICE_INFO_SN)
		{
			MULT_Information *plInfo;
			plInfo = &plHandle->m_Information;

			lRetValueSize = GLOBAL_STRLEN(plInfo->m_pSNString);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_STRCPY((CHAR_T *)pValue, plInfo->m_pSNString);
			}
			else
			{
				lRetValueSize = 0;
			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_INFO_SOFTWARE_VERSION)
		{
			MULT_Information *plInfo;
			plInfo = &plHandle->m_Information;

			lRetValueSize = GLOBAL_STRLEN(plInfo->m_pSoftVersion);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_STRCPY((CHAR_T *)pValue, plInfo->m_pSoftVersion);
			}
			else
			{
				lRetValueSize = 0;
			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_INFO_HARDWARE_VERSION)
		{
			MULT_Information *plInfo;
			plInfo = &plHandle->m_Information;

			lRetValueSize = GLOBAL_STRLEN(plInfo->m_pHardVersion);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_STRCPY((CHAR_T *)pValue, plInfo->m_pHardVersion);
			}
			else
			{
				lRetValueSize = 0;
			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_INFO_SOFTWARE_RELEASE)
		{
			MULT_Information *plInfo;
			plInfo = &plHandle->m_Information;

			lRetValueSize = GLOBAL_STRLEN(plInfo->m_pSoftRelease);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_STRCPY((CHAR_T *)pValue, plInfo->m_pSoftRelease);
			}
			else
			{
				lRetValueSize = 0;
			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_INFO_FPGA_RELEASE)
		{
			MULT_Information *plInfo;
			plInfo = &plHandle->m_Information;

			lRetValueSize = GLOBAL_STRLEN(plInfo->m_pFPGARelease);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_STRCPY((CHAR_T *)pValue, plInfo->m_pFPGARelease);
			}
			else
			{
				lRetValueSize = 0;
			}
		}
		/*下面是非通用设备节点*/
		else if (ID == SNMP_WARP_GOSDEVICE_ALARM_TEMP)
		{
			S32 lErrorCount;
			MULT_Monitor *plMonitorInfo;
			plMonitorInfo = &plHandle->m_Monitor;
			lErrorCount = CAL_LogGetLogInfoCount(plMonitorInfo->m_LogHandle, 0);
			lRetValueSize = sizeof(lErrorCount);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;

			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_ALARM_FPGA)
		{	
			S32 lErrorCount = 0;			
			lRetValueSize = sizeof(lErrorCount);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;

			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_ALARM_NTP)
		{
			S32 lErrorCount;
			MULT_Monitor *plMonitorInfo;
			plMonitorInfo = &plHandle->m_Monitor;
			lErrorCount = CAL_LogGetLogInfoCount(plMonitorInfo->m_LogHandle, 5);

			lRetValueSize = sizeof(plMonitorInfo->m_TempErrorCount);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;

			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_ALARM_SCS_EMM)
		{

			S32 lErrorCount;
			MULT_Monitor *plMonitorInfo;
			plMonitorInfo = &plHandle->m_Monitor;
			lErrorCount = CAL_LogGetLogInfoCount(plMonitorInfo->m_LogHandle, 6);

			lRetValueSize = sizeof(plMonitorInfo->m_TempErrorCount);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;
			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_ALARM_SCS_ECM)
		{

			S32 lErrorCount;
			MULT_Monitor *plMonitorInfo;
			plMonitorInfo = &plHandle->m_Monitor;
			lErrorCount = CAL_LogGetLogInfoCount(plMonitorInfo->m_LogHandle, 7);

			lRetValueSize = sizeof(plMonitorInfo->m_TempErrorCount);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;
			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_ALARM_ETH_LINK)
		{
			S32 lErrorCount = 0;			
			lRetValueSize = sizeof(lErrorCount);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;

			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_ALARM_PLL)
		{
			S32 lErrorCount;
			MULT_Monitor *plMonitorInfo;
			plMonitorInfo = &plHandle->m_Monitor;
			lErrorCount = CAL_LogGetLogInfoCount(plMonitorInfo->m_LogHandle, 2);

			lRetValueSize = sizeof(plMonitorInfo->m_TempErrorCount);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;
			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_ALARM_CHANNEL_IN)
		{

			S32 lErrorCount;
			MULT_Monitor *plMonitorInfo;
			plMonitorInfo = &plHandle->m_Monitor;
			lErrorCount = CAL_LogGetLogInfoCount(plMonitorInfo->m_LogHandle, 3);

			lRetValueSize = sizeof(plMonitorInfo->m_TempErrorCount);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;
			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_ALARM_CHANNEL_OUT)
		{


			S32 lErrorCount;
			MULT_Monitor *plMonitorInfo;
			plMonitorInfo = &plHandle->m_Monitor;
			lErrorCount = CAL_LogGetLogInfoCount(plMonitorInfo->m_LogHandle, 4);

			lRetValueSize = sizeof(plMonitorInfo->m_TempErrorCount);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;
			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_ALARM_BUFFER_STATUS)
		{
			S32 lErrorCount;
			lErrorCount = CAL_LogGetLogInfoCount(plMonitorInfo->m_LogHandle, 8);
			lErrorCount += CAL_LogGetLogInfoCount(plMonitorInfo->m_LogHandle, 9);
			lErrorCount += CAL_LogGetLogInfoCount(plMonitorInfo->m_LogHandle, 10);
			lErrorCount += CAL_LogGetLogInfoCount(plMonitorInfo->m_LogHandle, 11);
			if (lErrorCount > 255)
			{
				lErrorCount =255;

			}

			lRetValueSize = sizeof(plMonitorInfo->m_TempErrorCount);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;
			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_ALARM_FAN_FAILURE)
		{
			S32 lErrorCount = 0;			
			lRetValueSize = sizeof(lErrorCount);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;

			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_ALARM_POWER_FAILURE)
		{
			S32 lErrorCount = 0;			
			lRetValueSize = sizeof(lErrorCount);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;

			}

		}
		else if (ID == SNMP_WARP_GOSDEVICE_STATUS_CPU_USEAGE)
		{
            S32 lCPUUsage = PL_CPUAVGUsageCAL();
            PL_CPUAVGUsageSet();

            GLOBAL_TRACE(("CPU Usage = %d !!!\n", lCPUUsage));

			S32 lErrorCount = lCPUUsage;
			lRetValueSize = sizeof(lErrorCount);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;

			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_STATUS_MEM_USEAGE)
		{
            S32 lTotal, lCurrent;
            S32 lMEMUsage;
            PL_CPUAVGUsageSet();
            PL_MEMUsageGet(&lTotal, &lCurrent);
            if (lTotal > 0) {
                lMEMUsage = (S32)(((F32)(lCurrent) / lTotal) * 100);
                GLOBAL_TRACE(("MEM Usage = %d !!!\n", lMEMUsage));
            }

			S32 lErrorCount = lMEMUsage;
			lRetValueSize = sizeof(lErrorCount);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;

			}
		}		
		else if (ID == SNMP_WARP_GOSDEVICE_STATUS_CPU_TEMPERATURE)
		{
            U32 Temp = 0;
            S32 lCpuCurrentTemp = 0;
            if(PFC_PhyAddrMapDirectAccess(0x120E0000, 4*1024, 0x0114, &Temp, 1, TRUE))
            {
                lCpuCurrentTemp = ((Temp&0x000000FF)*180/256) - 40;
                GLOBAL_TRACE(("CpuTemperature : %d !\n", lCpuCurrentTemp));
            }
            else{
                GLOBAL_TRACE((" Error read cpu temperature ! \n" ));
            }

			S32 lErrorCount = lCpuCurrentTemp;
			lRetValueSize = sizeof(lErrorCount);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;

			}
		}
//#ifdef SUPPORT_1846_SNMP
#if 1
        else if (ID == (SNMP_WARP_ID_USER_START + 1080))
        {
            HDMI_RxStatusParam lHdmiRxStatus;
            HWL_HdmiRxGetStatus(0, &lHdmiRxStatus);
            S32 lErrorCount;
            if(lHdmiRxStatus.m_SignalIsLocked)
            {
                if(plHandle->m_Parameter.m_pInChannel[0].m_pSubChannelNode[0].m_SubInfo.m_SubENCODERInfo.m_ActiveMark)
                {
                    lErrorCount = 1;
                }
                else
                {
                    lErrorCount = 2;
                }
            }
            else
            {
                lErrorCount = 0;
            }
            lRetValueSize = sizeof(lErrorCount);
            if (ValueSize >= lRetValueSize)
            {
                GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
            }
            else
            {
                lRetValueSize = 0;

            }
        }
        else if (ID == (SNMP_WARP_ID_USER_START + 1081))
        {
            HDMI_RxStatusParam lHdmiRxStatus;
            HWL_HdmiRxGetStatus(1, &lHdmiRxStatus);
            S32 lErrorCount;
            if(lHdmiRxStatus.m_SignalIsLocked)
            {
                if(plHandle->m_Parameter.m_pInChannel[0].m_pSubChannelNode[1].m_SubInfo.m_SubENCODERInfo.m_ActiveMark)
                {
                    lErrorCount = 1;
                }
                else
                {
                    lErrorCount = 2;
                }
            }
            else
            {
                lErrorCount = 0;
            }
            lRetValueSize = sizeof(lErrorCount);
            if (ValueSize >= lRetValueSize)
            {
                GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
            }
            else
            {
                lRetValueSize = 0;

            }
        }
        else if (ID == (SNMP_WARP_ID_USER_START + 1082))
        {
            HDMI_RxStatusParam lHdmiRxStatus;
            HWL_HdmiRxGetStatus(2, &lHdmiRxStatus);
            S32 lErrorCount;
            if(lHdmiRxStatus.m_SignalIsLocked)
            {
                if(plHandle->m_Parameter.m_pInChannel[0].m_pSubChannelNode[2].m_SubInfo.m_SubENCODERInfo.m_ActiveMark)
                {
                    lErrorCount = 1;
                }
                else
                {
                    lErrorCount = 2;
                }
            }
            else
            {
                lErrorCount = 0;
            }
            lRetValueSize = sizeof(lErrorCount);
            if (ValueSize >= lRetValueSize)
            {
                GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
            }
            else
            {
                lRetValueSize = 0;
            }
        }
        else if (ID == (SNMP_WARP_ID_USER_START + 1083))
        {
            HDMI_RxStatusParam lHdmiRxStatus;
            HWL_HdmiRxGetStatus(3, &lHdmiRxStatus);
            S32 lErrorCount;
            if(lHdmiRxStatus.m_SignalIsLocked)
            {
                if(plHandle->m_Parameter.m_pInChannel[0].m_pSubChannelNode[3].m_SubInfo.m_SubENCODERInfo.m_ActiveMark)
                {
                    lErrorCount = 1;
                }
                else
                {
                    lErrorCount = 2;
                }
            }
            else
            {
                lErrorCount = 0;
            }
            lRetValueSize = sizeof(lErrorCount);
            if (ValueSize >= lRetValueSize)
            {
                GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
            }
            else
            {
                lRetValueSize = 0;

            }
        }
        else if (ID == (SNMP_WARP_ID_USER_START + 1090))
        {
            S32 Rate = 0;
            Rate =  plHandle->m_Monitor.m_pOutChnArray[0].m_pSubArray[0].m_CurValue;


            S32 lErrorCount = Rate;
            lRetValueSize = sizeof(lErrorCount);
            if (ValueSize >= lRetValueSize)
            {
                GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
            }
            else
            {
                lRetValueSize = 0;

            }
        }
        else if (ID == (SNMP_WARP_ID_USER_START + 1091))
        {
            S32 Rate = 0;
            Rate =  plHandle->m_Monitor.m_pOutChnArray[0].m_pSubArray[1].m_CurValue;

            S32 lErrorCount = Rate;
            lRetValueSize = sizeof(lErrorCount);
            if (ValueSize >= lRetValueSize)
            {
                GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
            }
            else
            {
                lRetValueSize = 0;

            }
        }
        else if (ID == (SNMP_WARP_ID_USER_START + 1092))
        {
            S32 Rate = 0;
            Rate =  plHandle->m_Monitor.m_pOutChnArray[0].m_pSubArray[2].m_CurValue;

            S32 lErrorCount = Rate;
            lRetValueSize = sizeof(lErrorCount);
            if (ValueSize >= lRetValueSize)
            {
                GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
            }
            else
            {
                lRetValueSize = 0;
            }
        }
        else if (ID == (SNMP_WARP_ID_USER_START + 1093))
        {
            S32 Rate = 0;
            Rate =  plHandle->m_Monitor.m_pOutChnArray[0].m_pSubArray[3].m_CurValue;

            S32 lErrorCount = Rate;
            lRetValueSize = sizeof(lErrorCount);
            if (ValueSize >= lRetValueSize)
            {
                GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
            }
            else
            {
                lRetValueSize = 0;

            }
        }
        else if (ID == (SNMP_WARP_ID_USER_START + 1094))
        {
            U32 Temp = 0;
            S32 lErrorCount = plHandle->m_Monitor.m_CurrentTemp;
            lRetValueSize = sizeof(lErrorCount);
            if (ValueSize >= lRetValueSize)
            {
                GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
            }
            else
            {
                lRetValueSize = 0;

            }
        }
#endif

#ifdef SUPPORT_SFN_MODULATOR
		/*单频网专用报警*/
		else if (ID == SNMP_WARP_GOSDEVICE_ALARM_1PPS_FAILURE)
		{
			S32 lErrorCount = 0;			
			lRetValueSize = sizeof(lErrorCount);
			lErrorCount += CAL_LogGetLogInfoCount(plMonitorInfo->m_LogHandle, MULT_MONITOR_TYPE_INT_1PPS_LOST);
			lErrorCount += CAL_LogGetLogInfoCount(plMonitorInfo->m_LogHandle, MULT_MONITOR_TYPE_EXT_1PPS_LOST);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;

			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_ALARM_REF_CLK_FAILURE)
		{
			S32 lErrorCount = 0;			
			lRetValueSize = sizeof(lErrorCount);
			lErrorCount += CAL_LogGetLogInfoCount(plMonitorInfo->m_LogHandle, MULT_MONITOR_TYPE_INT_10M_LOST);
			lErrorCount += CAL_LogGetLogInfoCount(plMonitorInfo->m_LogHandle, MULT_MONITOR_TYPE_EXT_10M_LOST);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;

			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_ALARM_SIP_ERROR)
		{
			S32 lErrorCount = 0;			
			lRetValueSize = sizeof(lErrorCount);
			lErrorCount += CAL_LogGetLogInfoCount(plMonitorInfo->m_LogHandle, MULT_MONITOR_TYPE_SFN_SIP_ERROR);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;

			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_ALARM_SIP_CRC_ERROR)
		{
			S32 lErrorCount = 0;			
			lRetValueSize = sizeof(lErrorCount);
			lErrorCount += CAL_LogGetLogInfoCount(plMonitorInfo->m_LogHandle, MULT_MONITOR_TYPE_SFN_SIP_CRC32_ERROR);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;

			}
		}
		else if (ID == SNMP_WARP_GOSDEVICE_ALARM_SFN_FAILURE)
		{
			S32 lErrorCount = 0;			
			lRetValueSize = sizeof(lErrorCount);
			lErrorCount += CAL_LogGetLogInfoCount(plMonitorInfo->m_LogHandle, MULT_MONITOR_TYPE_SFN_ERROR);
			if (ValueSize >= lRetValueSize)
			{
				GLOBAL_MEMCPY((S32 *)pValue, &lErrorCount, lRetValueSize);
			}
			else
			{
				lRetValueSize = 0;

			}
		}
#endif
	}

	return lRetValueSize;
}


S32 MULTL_SNMPWarpTableFunc(HANDLE32 UserHandle, U32 ID, S32 Row, S32 Col, S32 ValueType, void *pValue, S32 ValueSize, BOOL bRead)
{
	S32 lRetValueSize = 0;
	MULT_Handle *plHandle = (MULT_Handle*)UserHandle;

#ifdef MULT_TS_BACKUP_SUPPORT
	if (ID == (SNMP_WARP_ID_USER_START + MULT_TS_BACKUP_OUT_TS_SNUM_USER_ID_OFFSET))
	{

		if (pValue == NULL)
		{
			if (Row < MPEG2_DBGetTsCount(plHandle->m_DBSHandle, FALSE))//这里判断有多少行，当行序号是有效的时候，返回1，反之返回0
			{
				lRetValueSize = 1;
			}
			//GLOBAL_TRACE(("Get RowInd = %d, Ret = %d\n", Row, lRetValueSize));
		}
		else
		{
			//GLOBAL_TRACE(("bRead = %d, Row = %d, Col = %d\n", bRead, Row, Col));
			if (Row < MPEG2_DBGetTsCount(plHandle->m_DBSHandle, FALSE))
			{
				if (bRead)
				{
					MULT_BPTsOutBackupInfo lOutInfo;
					MULT_BPProcOutTsBackupInfo(Row, &lOutInfo, TRUE);
					if (Col == 0)//Out Ts Index
					{
						*(int*)pValue  = Row + 1;
						lRetValueSize = 4;
					}
					else if (Col == 1)
					{
						*(int*)pValue  = lOutInfo.m_BackupTsInd + 1;
						lRetValueSize = 4;
					}
					else if (Col == 2)
					{
						GLOBAL_STRCPY((char*)pValue, MULT_BPModeValueToStr(MULT_BPGetBackupStatus(Row, FALSE)));
						lRetValueSize = GLOBAL_STRLEN((char*)pValue);
					}
					else if (Col == 3)
					{
						GLOBAL_STRCPY((char*)pValue, MULT_BPModeValueToStr(lOutInfo.m_Mode));
						lRetValueSize = GLOBAL_STRLEN((char*)pValue);
					}
					else if (Col == 4)
					{
						*(int*)pValue  = (MULT_BPGetBackupStatus(Row, FALSE) == MULT_BP_TS_BACKUP_MODE_BACKUP)?0:1;
						lRetValueSize = 4;
					}
					else
					{
						GLOBAL_TRACE(("UnSupported Col = %d\n", Col));
					}
				}
				else
				{
					if (Col == 4)
					{
						if ((*(int*)pValue) == 0)
						{
							MULT_BPSetOutputRemoteControl(Row, FALSE);
						}
						else
						{
							MULT_BPSetOutputRemoteControl(Row, TRUE);
						}
						lRetValueSize = 1;
					}
					else
					{
						GLOBAL_TRACE(("UnSupported Col = %d\n", Col));
					}
				}
			}
			else
			{
				GLOBAL_TRACE(("UnSupported Row = %d\n", Row));
			}
		}
	}
#endif
	return lRetValueSize;
}



/*初始化SNMP系统*/
void MULT_SNMPInitiate(MULT_Handle *pHandle)
{
	SNMP_WarpInitParam lParam;

//	lParam.m_MaxMIBNodeNum = 32;
	lParam.m_MaxMIBNodeNum = 256;
	lParam.m_MaxTrapNodeNum = 32;
	lParam.m_MaxTrapTimes = 1;
	lParam.m_NormalTrapInterval = pHandle->m_Configuration.m_SNMPInitParam.m_NormalTrapInterval;
	lParam.m_TRAPGlobalMark = pHandle->m_Configuration.m_SNMPInitParam.m_TRAPGlobalMark;
	lParam.m_TRAPIPAddress = pHandle->m_Configuration.m_SNMPInitParam.m_TRAPIPAddress;
	lParam.m_TRAPPort =  pHandle->m_Configuration.m_SNMPInitParam.m_TRAPPort ;
	lParam.m_bOpenSyslog = pHandle->m_Configuration.m_SNMPGlobalMark;
	//GLOBAL_TRACE(("lParam.m_TRAPPort == %d\n" , lParam.m_TRAPPort));
	//GLOBAL_TRACE(("lParam.m_NormalTrapInterval == %d\n" , lParam.m_NormalTrapInterval));
	//GLOBAL_TRACE(("lParam.m_TRAPIPAddress == 0x%x\n" , lParam.m_TRAPIPAddress));

	//GLOBAL_TRACE(("lParam.m_TRAPGlobalMark == %d\n" , lParam.m_TRAPGlobalMark));


	//GLOBAL_STRCPY(lParam.m_pROCommunity, SNMP_WARP_DEFAULT_RO_COMMUNITY);
	//GLOBAL_STRCPY(lParam.m_pRWCommunity, SNMP_WARP_DEFAULT_RW_COMMUNITY);

	GLOBAL_STRCPY(lParam.m_pROCommunity, pHandle->m_Configuration.m_SNMPInitParam.m_pROCommunity);
	GLOBAL_STRCPY(lParam.m_pRWCommunity, pHandle->m_Configuration.m_SNMPInitParam.m_pRWCommunity);
	GLOBAL_STRCPY(lParam.m_pSNMPConfDir, SNMP_WARP_DEFAULT_CONF_DIR);
	GLOBAL_STRCPY(lParam.m_pSNMPConfName, SNMP_WARP_DEFAULT_CONF_NAME);

	PFC_System("mkdir -p /tmp/snmp");

	SNMP_WarpInitiate(&lParam);

	SNMP_WarpRegisterMIB2System(pHandle, MULTL_SNMPCallbackFunc);
	SNMP_WarpRegisterGosDevice(pHandle, MULTL_SNMPCallbackFunc, TRUE);

//#ifdef SUPPORT_1846_SNMP
#if 1
    {
        SNMP_WarpMibReg lReg;

        GLOBAL_ZEROMEM(&lReg, sizeof(lReg));
        lReg.m_BaseOIDLen = SNMP_WarpOIDString2Bin(SNMP_GOSPELL_DEVICE_STATUS_BASE_OID_STR, lReg.m_pBaseOID, SNMP_WARP_MAX_OID_LEN);
        lReg.m_pUserHandle = pHandle;
        lReg.m_pCB = MULTL_SNMPCallbackFunc;
        lReg.m_SubOIDLen = 1;
        //1.3.6.1.4.1.21571.1000.2.500
        lReg.m_Access = SNMP_WARP_ACCESS_READ_ONLY;
        lReg.m_pSubOID[0] = 500;
        lReg.m_ValueType = SNMP_WARP_VALUE_INTEGER;
        lReg.m_ID = SNMP_WARP_ID_USER_START + 1080;
        SNMP_WarpRegisterMIBNode(&lReg);

        GLOBAL_ZEROMEM(&lReg, sizeof(lReg));
        lReg.m_BaseOIDLen = SNMP_WarpOIDString2Bin(SNMP_GOSPELL_DEVICE_STATUS_BASE_OID_STR, lReg.m_pBaseOID, SNMP_WARP_MAX_OID_LEN);
        lReg.m_pUserHandle = pHandle;
        lReg.m_pCB = MULTL_SNMPCallbackFunc;
        lReg.m_SubOIDLen = 1;
        //1.3.6.1.4.1.21571.1000.2.501
        lReg.m_Access = SNMP_WARP_ACCESS_READ_ONLY;
        lReg.m_pSubOID[0] = 501;
        lReg.m_ValueType = SNMP_WARP_VALUE_INTEGER;
        lReg.m_ID = SNMP_WARP_ID_USER_START + 1081;
        SNMP_WarpRegisterMIBNode(&lReg);

        GLOBAL_ZEROMEM(&lReg, sizeof(lReg));
        lReg.m_BaseOIDLen = SNMP_WarpOIDString2Bin(SNMP_GOSPELL_DEVICE_STATUS_BASE_OID_STR, lReg.m_pBaseOID, SNMP_WARP_MAX_OID_LEN);
        lReg.m_pUserHandle = pHandle;
        lReg.m_pCB = MULTL_SNMPCallbackFunc;
        lReg.m_SubOIDLen = 1;
        //1.3.6.1.4.1.21571.1000.2.502
        lReg.m_Access = SNMP_WARP_ACCESS_READ_ONLY;
        lReg.m_pSubOID[0] = 502;
        lReg.m_ValueType = SNMP_WARP_VALUE_INTEGER;
        lReg.m_ID = SNMP_WARP_ID_USER_START + 1082;
        SNMP_WarpRegisterMIBNode(&lReg);

        GLOBAL_ZEROMEM(&lReg, sizeof(lReg));
        lReg.m_BaseOIDLen = SNMP_WarpOIDString2Bin(SNMP_GOSPELL_DEVICE_STATUS_BASE_OID_STR, lReg.m_pBaseOID, SNMP_WARP_MAX_OID_LEN);
        lReg.m_pUserHandle = pHandle;
        lReg.m_pCB = MULTL_SNMPCallbackFunc;
        lReg.m_SubOIDLen = 1;
        //1.3.6.1.4.1.21571.1000.2.503
        lReg.m_Access = SNMP_WARP_ACCESS_READ_ONLY;
        lReg.m_pSubOID[0] = 503;
        lReg.m_ValueType = SNMP_WARP_VALUE_INTEGER;
        lReg.m_ID = SNMP_WARP_ID_USER_START + 1083;
        SNMP_WarpRegisterMIBNode(&lReg);

        GLOBAL_ZEROMEM(&lReg, sizeof(lReg));
        lReg.m_BaseOIDLen = SNMP_WarpOIDString2Bin(SNMP_GOSPELL_DEVICE_STATUS_BASE_OID_STR, lReg.m_pBaseOID, SNMP_WARP_MAX_OID_LEN);
        lReg.m_pUserHandle = pHandle;
        lReg.m_pCB = MULTL_SNMPCallbackFunc;
        lReg.m_SubOIDLen = 1;
        //1.3.6.1.4.1.21571.1000.2.600
        lReg.m_Access = SNMP_WARP_ACCESS_READ_ONLY;
        lReg.m_pSubOID[0] = 600;
        lReg.m_ValueType = SNMP_WARP_VALUE_INTEGER;
        lReg.m_ID = SNMP_WARP_ID_USER_START + 1090;
        SNMP_WarpRegisterMIBNode(&lReg);

        GLOBAL_ZEROMEM(&lReg, sizeof(lReg));
        lReg.m_BaseOIDLen = SNMP_WarpOIDString2Bin(SNMP_GOSPELL_DEVICE_STATUS_BASE_OID_STR, lReg.m_pBaseOID, SNMP_WARP_MAX_OID_LEN);
        lReg.m_pUserHandle = pHandle;
        lReg.m_pCB = MULTL_SNMPCallbackFunc;
        lReg.m_SubOIDLen = 1;
        //1.3.6.1.4.1.21571.1000.2.601
        lReg.m_Access = SNMP_WARP_ACCESS_READ_ONLY;
        lReg.m_pSubOID[0] = 601;
        lReg.m_ValueType = SNMP_WARP_VALUE_INTEGER;
        lReg.m_ID = SNMP_WARP_ID_USER_START + 1091;
        SNMP_WarpRegisterMIBNode(&lReg);

        GLOBAL_ZEROMEM(&lReg, sizeof(lReg));
        lReg.m_BaseOIDLen = SNMP_WarpOIDString2Bin(SNMP_GOSPELL_DEVICE_STATUS_BASE_OID_STR, lReg.m_pBaseOID, SNMP_WARP_MAX_OID_LEN);
        lReg.m_pUserHandle = pHandle;
        lReg.m_pCB = MULTL_SNMPCallbackFunc;
        lReg.m_SubOIDLen = 1;
        //1.3.6.1.4.1.21571.1000.2.602
        lReg.m_Access = SNMP_WARP_ACCESS_READ_ONLY;
        lReg.m_pSubOID[0] = 602;
        lReg.m_ValueType = SNMP_WARP_VALUE_INTEGER;
        lReg.m_ID = SNMP_WARP_ID_USER_START + 1092;
        SNMP_WarpRegisterMIBNode(&lReg);

        GLOBAL_ZEROMEM(&lReg, sizeof(lReg));
        lReg.m_BaseOIDLen = SNMP_WarpOIDString2Bin(SNMP_GOSPELL_DEVICE_STATUS_BASE_OID_STR, lReg.m_pBaseOID, SNMP_WARP_MAX_OID_LEN);
        lReg.m_pUserHandle = pHandle;
        lReg.m_pCB = MULTL_SNMPCallbackFunc;
        lReg.m_SubOIDLen = 1;
        //1.3.6.1.4.1.21571.1000.2.603
        lReg.m_Access = SNMP_WARP_ACCESS_READ_ONLY;
        lReg.m_pSubOID[0] = 603;
        lReg.m_ValueType = SNMP_WARP_VALUE_INTEGER;
        lReg.m_ID = SNMP_WARP_ID_USER_START + 1093;
        SNMP_WarpRegisterMIBNode(&lReg);

        GLOBAL_ZEROMEM(&lReg, sizeof(lReg));
        lReg.m_BaseOIDLen = SNMP_WarpOIDString2Bin(SNMP_GOSPELL_DEVICE_STATUS_BASE_OID_STR, lReg.m_pBaseOID, SNMP_WARP_MAX_OID_LEN);
        lReg.m_pUserHandle = pHandle;
        lReg.m_pCB = MULTL_SNMPCallbackFunc;
        lReg.m_SubOIDLen = 1;
        //1.3.6.1.4.1.21571.1000.2.603
        lReg.m_Access = SNMP_WARP_ACCESS_READ_ONLY;
        lReg.m_pSubOID[0] = 604;
        lReg.m_ValueType = SNMP_WARP_VALUE_INTEGER;
        lReg.m_ID = SNMP_WARP_ID_USER_START + 1094;
        SNMP_WarpRegisterMIBNode(&lReg);
    }
#endif

//#ifdef SUPPORT_SFN_MODULATOR
	//{
	//	SNMP_WarpMibReg lMibReg;

	//	SNMP_WarpRegisterMIBNode(&lMibReg);
	//	lMibReg.m_Access = SNMP_WARP_ACCESS_READ_ONLY;
	//	lMibReg.m_BaseOIDLen =  SNMP_WarpOIDString2Bin(SNMP_GOSPELL_DEVICE_INFO_BASE_OID_STR, lMibReg.m_pBaseOID, SNMP_WARP_MAX_OID_LEN);

	//	lMibReg.m_pSubOID[0] = 1;
	//	lMibReg.m_SubOIDLen = 1;
	//	lMibReg.m_ValueType = SNMP_WARP_VALUE_OCTET_STR;
	//}
//#endif

	///*注册MIB节点*/
	//{
	//	SNMP_WarpMibReg lMibReg;

	//	/*INFO节点*/
	//	lMibReg.m_Access = SNMP_WARP_ACCESS_READ_ONLY;
	//	lMibReg.m_BaseOIDLen =  SNMP_WarpOIDString2Bin(SNMP_GOSPELL_DEVICE_INFO_BASE_OID_STR, lMibReg.m_pBaseOID, SNMP_WARP_MAX_OID_LEN);

	//	lMibReg.m_pSubOID[0] = 1;
	//	lMibReg.m_SubOIDLen = 1;
	//	lMibReg.m_ValueType = SNMP_WARP_VALUE_OCTET_STR;
	//	lMibReg.m_ID = 

	//}

	///*注册TRAP节点*/
	//{
	//	SNMP_WarpTrapReg lTrapReg;
	//	lTrapReg.m_TrapOIDLen = SNMP_WarpOIDString2Bin(SNMP_GOSPELL_DEVICE_TRAP_OID_STR, lTrapReg.m_pTrapOID, SNMP_WARP_MAX_OID_LEN);
	//	lTrapReg.m_DescrOIDLen = SNMP_WarpOIDString2Bin(SNMP_GOSPELL_DEVICE_TRAP_OID_STR, lTrapReg.m_pDescrOID, SNMP_WARP_MAX_OID_LEN);
	//	GLOBAL_SPRINTF((lTrapReg.m_pDescr, "jlasjdklfjas:1"));
	//	lTrapReg.m_DescrLen = GLOBAL_STRLEN(lTrapReg.m_pDescr);

	//	SNMP_WarpRegisteTrapNode(0, &lTrapReg);
	//}

#ifdef MULT_TS_BACKUP_SUPPORT
	{
		SNMP_WarpMibReg lReg;
		GLOBAL_ZEROMEM(&lReg, sizeof(lReg));

		lReg.m_pUserHandle = pHandle;
		lReg.m_pTableCB = MULTL_SNMPWarpTableFunc;
		lReg.m_Access = SNMP_WARP_ACCESS_READ_WRITE;


		lReg.m_BaseOIDLen = SNMP_WarpOIDString2Bin(SNMP_GOSPELL_DEVICE_TS_BACKUP_BASE, lReg.m_pBaseOID, SNMP_WARP_MAX_OID_LEN);
		lReg.m_pSubOID[0] = 2;/*Input .1 Output .2*/
		lReg.m_SubOIDLen = 1;

		lReg.m_ID = SNMP_WARP_ID_USER_START + MULT_TS_BACKUP_OUT_TS_SNUM_USER_ID_OFFSET;
		lReg.m_pTableValueType[0] = SNMP_WARP_VALUE_INTEGER;
		lReg.m_pTableValueType[1] = SNMP_WARP_VALUE_INTEGER;
		lReg.m_pTableValueType[2] = SNMP_WARP_VALUE_OCTET_STR;
		lReg.m_pTableValueType[3] = SNMP_WARP_VALUE_OCTET_STR;
		lReg.m_pTableValueType[4] = SNMP_WARP_VALUE_INTEGER;
		lReg.m_ColNum = 5;

		SNMP_WarpRegisterTableNode(&lReg);
	}
#endif

	SNMP_WarpStart();
}

static CHAR_T *s_pTrapStrings[] = 
{
	"Temperature Error", 
	"Hardware Error",
	"PLL Lock Lost",
	"Input Channel Error", 
	"Output Channel Error",	
	"NTP Access Failure", 
	"EMM Error", 
	"ECM Error"
};

/*存放在主循环当中，检查是否有需要TRAP的对象！*/
void MULT_SNMPAccess(MULT_Handle *pHandle, S32 Duration)
{
	S32 i, k;
	CAL_LogConfig lLogConfig;
	MULT_Monitor *plMonitor;
	CHAR_T plString[SNMP_WARP_MAX_DESCR];

	plMonitor = &pHandle->m_Monitor;
	if ((pHandle->m_Configuration.m_SNMPGlobalMark == TRUE) && (pHandle->m_Configuration.m_SNMPInitParam.m_TRAPGlobalMark == TRUE))
	{

		{
			BOOL blNewTrap;
			U32 lTrapID;
			for (i = 0; i < MULT_MONITOR_TYPE_NUM - HWL_TUNER_MAX_NUM - 3; i++)
			{
				CAL_LogProcConfig(plMonitor->m_LogHandle, i, &lLogConfig, TRUE);
				if (lLogConfig.m_bTrap)
				{
					blNewTrap = CAL_LogCheckNewAlarm(plMonitor->m_LogHandle, i);
					if (i == 8)
					{
						blNewTrap |= CAL_LogGetLogInfoCount(plMonitor->m_LogHandle, i);
						blNewTrap |= CAL_LogGetLogInfoCount(plMonitor->m_LogHandle, i + 1);
						blNewTrap |= CAL_LogGetLogInfoCount(plMonitor->m_LogHandle, i + 2);
						blNewTrap |= CAL_LogGetLogInfoCount(plMonitor->m_LogHandle, i + 3);

					}

					if (blNewTrap)
					{
						if (i < sizeof(s_pTrapStrings) / sizeof(CHAR_T*))
						{
							GLOBAL_SPRINTF((plString, "%s:%d", s_pTrapStrings[i], lLogConfig.m_LogLevel));
						}
						else
						{
							GLOBAL_SPRINTF((plString, "No Strings"));
						}

						lTrapID = MULT_MONITOR_TYPE_TEMP;

						if (i == MULT_MONITOR_TYPE_TEMP)
						{
							lTrapID = SNMP_WARP_GOSDEVICE_ALARM_TEMP;
						}
						else if (i == MULT_MONITOR_TYPE_FPGA_HEART_BEAT)
						{
							lTrapID = SNMP_WARP_GOSDEVICE_ALARM_FPGA;
						}
						else if (i == MULT_MONITOR_TYPE_PLL)
						{
							lTrapID = SNMP_WARP_GOSDEVICE_ALARM_PLL;
						}
						else if (i == MULT_MONITOR_TYPE_CHANNEL_IN)
						{
							lTrapID = SNMP_WARP_GOSDEVICE_ALARM_CHANNEL_IN;
						}
						else if (i == MULT_MONITOR_TYPE_CHANNEL_OUT)
						{
							lTrapID = SNMP_WARP_GOSDEVICE_ALARM_CHANNEL_OUT;
						}
						else if (i == MULT_MONITOR_TYPE_NTP)
						{
							lTrapID = SNMP_WARP_GOSDEVICE_ALARM_NTP;
						}
						else if (i == MULT_MONITOR_TYPE_SCS_EMM)
						{
							lTrapID = SNMP_WARP_GOSDEVICE_ALARM_SCS_EMM;
						}
						else if (i == MULT_MONITOR_TYPE_SCS_ECM)
						{
							lTrapID = SNMP_WARP_GOSDEVICE_ALARM_SCS_ECM;
						}
						SNMP_WarpSetTrap(lTrapID, plString, GLOBAL_STRLEN(plString));
					}
				}
			}
		}
	}
}


void MULT_SNMPTerminate(MULT_Handle *pHandle)
{
	SNMP_WarpStop();

	SNMP_WarpTerminate();
}

/*EOF*/
