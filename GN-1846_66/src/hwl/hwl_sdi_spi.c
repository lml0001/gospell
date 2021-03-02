/******************************************************************************

                  ��Ȩ���� (C), 2005-2018, GOSPELL���޹�˾

 ******************************************************************************
  �� �� ��   : hwl_sdi_spi.c
  �� �� ��   : 1.0
  ��    ��   : ���
  ��������   : 2018��5��15��
  ����޸�   :
  ��������   :
  �����б�   :
  �޸���ʷ   :

   1.��    ��          : 2018��5��15��
      ��    ��          : ���
      �޸�����   : �����ļ�

******************************************************************************/
#include "hwl_sdi_spi.h"

/*****************************************************************************
* FUNCTION:GS2970_EnvParaCheck
*
* DESCRIPTION: Check input paraments
*	
* INPUT:
*	  
* OUTPUT:
*	
*
* RETURN:
*
* NOTES:
*   
* HISTORY:
*	
*	Review: 
******************************************************************************/
static BOOL GS2970_EnvParaCheck(HWL_Gs2970EnvPara *pEnvPara)
{
	if (pEnvPara == NULL) 
	{
		printf ("[%s][%s] env_para can not null!\n", __FILE__, __FUNCTION__);
		return FALSE;
	}

	if (pEnvPara->m_Gs2970ResetPulseWidth < MIN_RESET_PULSE_WIDTH) 
	{
		pEnvPara->m_Gs2970ResetPulseWidth = MIN_RESET_PULSE_WIDTH;
	}

	if (pEnvPara->m_ReadGpioCB == NULL || pEnvPara->m_WriteGpioCB == NULL
		 || pEnvPara->m_ReadRegCB == NULL   ||pEnvPara->m_WriteRegCB == NULL) 
	{
			printf ("[%s][%s] Call Back Functions can not null!\n", __FILE__, __FUNCTION__);
			return FALSE;
	}

	return TRUE;
}


/*****************************************************************************
* FUNCTION:SDI_Create
*
* DESCRIPTION:
*	
* INPUT:
*	  
* OUTPUT:
*	
*
* RETURN:
*
* NOTES:
*   
* HISTORY:
*	
*	Review: 
******************************************************************************/
HANDLE32 SDI_Create(HWL_Gs2970EnvPara *pEnvPara)
{
	GS2970_Handle *plHandle = NULL;
	
	if (GS2970_EnvParaCheck(pEnvPara) == FALSE) 
	{
		return NULL;
	}

	plHandle = (GS2970_Handle *)malloc(sizeof(GS2970_Handle)); 
	if (plHandle == NULL) 
	{
		perror("malloc err!");
		return NULL;
	}

	GLOBAL_MEMSET(plHandle, 0x0, sizeof(GS2970_Handle));
	
	GLOBAL_MEMCPY(&plHandle->m_EnvPara, pEnvPara, sizeof(HWL_Gs2970EnvPara));

    if (!plHandle->m_GS2970SpiHandle) 
	{
        if ((plHandle->m_GS2970SpiHandle = SPI_GpioCreate(&pEnvPara->m_InitParam.m_InitSpiParam)) == NULL) 
		 {
            GLOBAL_TRACE(("SPI_GpioCreate Failed!\n"));
            return NULL;
        }
    }

    return plHandle;
}


/*****************************************************************************
* FUNCTION:GS2970_Reset
*
* DESCRIPTION:
*	
* INPUT:
*	  
* OUTPUT:
*	
*
* RETURN:
*
* NOTES:
*   
* HISTORY:
*	
*	Review: 
******************************************************************************/
static BOOL GS2970_Reset(HANDLE32 Handle, void *pUserPara)
{
	GS2970_Handle *plHandle = (GS2970_Handle *)Handle;

	 /* ��λ�ǵ���Ч */
	plHandle->m_EnvPara.m_WriteGpioCB(plHandle->m_EnvPara.m_InitParam.m_ResetPin, LEVEL_LOW, pUserPara); 
	PL_TaskSleepUS(plHandle->m_EnvPara.m_Gs2970ResetPulseWidth * 1000);
	plHandle->m_EnvPara.m_WriteGpioCB(plHandle->m_EnvPara.m_InitParam.m_ResetPin, LEVEL_HIGH, pUserPara);

	/* ��λ����ʱ���Ĵ�����datasheetûҪ��Ϊ���ȶ��������10ms */
	PL_TaskSleepUS(10000); 

	return TRUE;
}

/*****************************************************************************
* FUNCTION: HWL_Gs2970Destroy
*
* DESCRIPTION:
*	
* INPUT:
*	  
* OUTPUT:
*	
*
* RETURN:
*
* NOTES:
*
*	Review: 
******************************************************************************/
BOOL HWL_Gs2970Destroy(HANDLE32 Handle)
{
	GS2970_Handle *plHandle = (GS2970_Handle *)Handle;
	if (plHandle)
	{
		GLOBAL_SAFEFREE(plHandle->m_GS2970SpiHandle);
		GLOBAL_SAFEFREE(Handle);		
	}
}

/*****************************************************************************
* FUNCTION: HWL_Gs2970DevInit
*
* DESCRIPTION:
*	
* INPUT:
*	  
* OUTPUT:
*	
*
* RETURN:
*
* NOTES:
*    2970�ĳ�ʼ����������ֵ��ʼ������λ���жϸ�оƬ�Ƿ���� 
*     (1) GS2970 �豸���и�λ�����ϵ�����³�ʼ��GS2970оƬ��ȷ��
*				оƬ�Ĺ�������
*
*     (2) ��ȡ�Ĵ���0x6D������Ӳ���������ù̶��ˣ����ԸüĴ���
*				��ֵ�̶�Ϊ0x2B�����ö���ֵ�ķ�ʽ�ж��Ƿ�GS2970����
*
*     (3) ����λ��ȷ�����ж�GS2970���ڣ��򷵻�TRUE�����򷵻�FALSE
* HISTORY:
*	
*	Review: 
******************************************************************************/
BOOL HWL_Gs2970DevInit(HANDLE32 Handle, void *pUserPara)
{
	GS2970_Handle *plHandle = (GS2970_Handle *)Handle;
	U16 lRegData = 0;
	S32 lCounter = GS2970_CHECK_COUNT;

	if (plHandle == NULL)
	{
		GLOBAL_TRACE(("[ERR] Please First Carry Out HWL_Gs2970EnvInit!\n"));
		return FALSE;
	}
		
	/* Configure GPIO of  Reset and 20-10bit */
	plHandle->m_EnvPara.m_SetupGpioCB(plHandle->m_EnvPara.m_InitParam.m_ResetPin, 1, NULL);
	plHandle->m_EnvPara.m_SetupGpioCB(plHandle->m_EnvPara.m_InitParam.m_20_10BitPin, 1, NULL);
	
	if (GS2970_Reset(Handle, pUserPara) == FALSE)
	{
		return FALSE;
	}
	
	while (lCounter --)
	{	
		plHandle->m_EnvPara.m_ReadRegCB(Handle, GS2970_REG_IO_DRIVE_STRENGTH, &lRegData, 2, pUserPara);
		if (lRegData == GS2970_REG_IO_DRIVE_STRENGTH_CONST_VAL)
		{
			break;
		}
		else
		{
			GLOBAL_TRACE(("[INFO] Register: 0x%x  Data: 0x%x\n", GS2970_REG_IO_DRIVE_STRENGTH, lRegData));
		}
	}
	
	if (lCounter < 0)
	{
		GLOBAL_TRACE(("[INFO] GS2970 Not Exist!\n"));
		return FALSE;
	}

	/* ������δ�����ֲ��GN8214Q�����庬�岻��� */
	lRegData = 0xFEFF;
	plHandle->m_EnvPara.m_WriteRegCB(Handle, GS2970_REG_IOPROC_1, lRegData, 2, pUserPara);
	lRegData = 0xDEFF;
	plHandle->m_EnvPara.m_WriteRegCB(Handle, GS2970_REG_IOPROC_1, lRegData, 2, pUserPara);
	
	return TRUE;
}

/*****************************************************************************
* FUNCTION: HWL_Probe_Gs2970
*
* DESCRIPTION: ̽��GS2970�Ƿ����.
*	
* INPUT:
*	  
* OUTPUT:
*	
*
* RETURN:
*
* NOTES:
*
*	Review: 
******************************************************************************/
BOOL HWL_Probe_Gs2970(HANDLE32 Handle, void *pUserPara)
{
	GS2970_Handle *plHandle = (GS2970_Handle *)Handle;
	U16 lRegData = 0;
	S32 lCounter = GS2970_CHECK_COUNT;
	
	if (plHandle == NULL)
	{
		GLOBAL_TRACE(("[ERR] Please First Carry Out HWL_Gs2970EnvInit!\n"));
		return FALSE;
	}

	while (lCounter --)
	{	
		plHandle->m_EnvPara.m_ReadRegCB(Handle, GS2970_REG_IO_DRIVE_STRENGTH, &lRegData, 2, pUserPara);
		if (lRegData == GS2970_REG_IO_DRIVE_STRENGTH_CONST_VAL)
		{
			break;
		}
		else
		{
			GLOBAL_TRACE(("[INFO] Register: 0x%x  Data: 0x%x\n", GS2970_REG_IO_DRIVE_STRENGTH, lRegData));
		}
	}
	
	if (lCounter < 0)
	{
		GLOBAL_TRACE(("[INFO] GS2970 Not Exist!\n"));
		return FALSE;
	}
	
	return TRUE;
}

/*****************************************************************************
* FUNCTION:
*
* DESCRIPTION:
*	
* INPUT:
*	  
* OUTPUT:
*	
*
* RETURN:
*
* NOTES:
*   
* HISTORY:
*	
*	Review: 
******************************************************************************/
/* ���ò�����䣬���Ϊ�̶�Ĭ��ֵ�ģ��Դ���Ĳ������ǣ�����û�и�ֵ���ǵĲ�����ʹ���ⲿ����Ĳ��� */
static void GS2970_FillConfigPara(HWL_Gs2970ConfigPara *pCfgPara)
{
	pCfgPara->m_AudOutEndian.m_AoutEndian12 = GS2970_AOUT_MSB;
	pCfgPara->m_AudOutEndian.m_AoutEndian34 = GS2970_AOUT_MSB;
	pCfgPara->m_AudOutEndian.m_AoutEndian56 = GS2970_AOUT_MSB;
	pCfgPara->m_AudOutEndian.m_AoutEndian78 = GS2970_AOUT_MSB;

	pCfgPara->m_AudOutMute.m_AoutMute1 = GS2970_AOUT_NO_MUTE;
	pCfgPara->m_AudOutMute.m_AoutMute2 = GS2970_AOUT_NO_MUTE;
	pCfgPara->m_AudOutMute.m_AoutMute3 = GS2970_AOUT_NO_MUTE;
	pCfgPara->m_AudOutMute.m_AoutMute4 = GS2970_AOUT_NO_MUTE;
	pCfgPara->m_AudOutMute.m_AoutMute5 = GS2970_AOUT_NO_MUTE;
	pCfgPara->m_AudOutMute.m_AoutMute6 = GS2970_AOUT_NO_MUTE;
	pCfgPara->m_AudOutMute.m_AoutMute7 = GS2970_AOUT_NO_MUTE;
	pCfgPara->m_AudOutMute.m_AoutMute8 = GS2970_AOUT_NO_MUTE;

	// pCfgPara->m_AudOutWordLen ���ⲿ�������
	// pCfgPara->m_AudEmbedGroup ���ⲿ�������
	// pCfgPara->m_AudOutFormat ���ⲿ�������
	// pCfgPara->m_AudOutSrc ���ⲿ�������
	// pCfgPara->m_VidOutFormatSel ���ⲿ�������
}

/* ����2970�Ĳ�����������Ҫ����ƵǶ�����
	1) ��Ƶϵͳʱ������Ϊ12.288MHZ
	2) �ж������ź��������Ž�����ƵǶ�����������
	3) ������Ƶ��ʽ��Ȼ����ж�Ӧ�ļĴ���������
*/

/*****************************************************************************
* FUNCTION:HWL_Gs2970SetPara
*
* DESCRIPTION:
*	
* INPUT:
*	  
* OUTPUT:
*	
*
* RETURN:
*
* NOTES:
*   
* HISTORY:
*	
*	Review: 
******************************************************************************/
BOOL HWL_Gs2970SetPara(HANDLE32 Handle, HWL_Gs2970ConfigPara *pCfgPara, void *pUserPara)
{
	S32 lRegData;
	S32 lSdiType = GS2970_SDI_SD_STA;

	GS2970_Handle *plHandle = (GS2970_Handle *)Handle;
	
	if (plHandle == NULL)
	{
		GLOBAL_TRACE(("[ERR] Please First Carry Out HWL_Gs2970EnvInit!\n"));
		return FALSE;
	}

	lRegData = GS2970_AMCLK_SEL & GS2970_AMCLK_256FS; /* fs = 48KHz, 256 * 48 = 12.288MHz */
	plHandle->m_EnvPara.m_WriteRegCB(Handle, GS2970_REG_AGC_CTRL, lRegData, 2, pUserPara);

	if (pCfgPara == NULL)
	{
		GLOBAL_TRACE(("[ERR] Gs2970 para cannot null!\n"));
		return FALSE;
	}

	GS2970_FillConfigPara(pCfgPara);

	if ((pCfgPara->m_VidOutFormatSel >= GS2970_VID_FMT_720_576_50I) && (pCfgPara->m_VidOutFormatSel <= GS2970_VID_FMT_320_240_60I)) 
	{
		lSdiType = GS2970_SDI_SD_STA;
	}
	else if ((pCfgPara->m_VidOutFormatSel >= GS2970_VID_FMT_1920_1080_50I) && (pCfgPara->m_VidOutFormatSel <= GS2970_VID_FMT_1280_720_60P)) 
	{
		lSdiType = GS2970_SDI_HD_STA;
	}
	else if ((pCfgPara->m_VidOutFormatSel >= GS2970_VID_FMT_1920_1080_50P) && (pCfgPara->m_VidOutFormatSel <= GS2970_VID_FMT_1920_1080_60P)) 
	{
		lSdiType = GS2970_SDI_3G_STA;
	}
	else 
	{
		GLOBAL_TRACE (("[ERR] Gs2970 video format para error!\n"));
		return FALSE;
	}

	GLOBAL_SDI_DEBUG(("Sdi Type = %d (VidOutFormatSel = %d)\n", lSdiType, pCfgPara->m_VidOutFormatSel), SDI_DEBUG);

	/* ����10Bit/20Bit */
	if (lSdiType == GS2970_SDI_SD_STA) /* SD �������10λ */
	{
		plHandle->m_EnvPara.m_WriteGpioCB(plHandle->m_EnvPara.m_InitParam.m_20_10BitPin, LEVEL_LOW, pUserPara);
		GLOBAL_SDI_DEBUG(("Sdi 20_10Bit Set Low \n"), SDI_DEBUG);
	}
	else /* HD/3G �������20λ */
	{
		plHandle->m_EnvPara.m_WriteGpioCB(plHandle->m_EnvPara.m_InitParam.m_20_10BitPin, LEVEL_HIGH, pUserPara);
		GLOBAL_SDI_DEBUG(("Sdi 20_10Bit Set High \n"), SDI_DEBUG);
	}


	/* ��ƵǶ������˵��
		1) ĿǰӲ���������AOUT_1/2����������ʹ�ã�����I2S������룻
			AOUT_7/8�����FPGA������AC3����ͨ(AES/EBU)
		2) GS2970��4����Ƶ�飬��ֻ��Ƕ��������Ƶ�鵽�����Ƕ���������
			��Ƶ����4����Ƶ���н���ѡ�񣬲���������ͬ
		3) GS2970��4��I2S������ֱ�ΪAOUT_1/2, AOUT_3/4, AOUT_5/6, AOUT_7/8,
			������AOUT_1/2��ΪI2S�ı������룬�������������8����Ƶ�������ѡ��
			��������ֱ�Ϊ������1(1/2, 3/4), ������2(1/2, 3/4), ������3(1/2, 3/4), 
			������4(1/2, 3/4)
		4) ͨ�����ϵķ���������������Ƶ��Ƕ�룬�������£�
			1> ����ѡ������Ƕ�����Ƶ��
			2> �����������Ƶ��
	*/
	/* set register 0x400/0x200 */
	if (lSdiType == GS2970_SDI_SD_STA)
	{
		lRegData = (pCfgPara->m_AudOutEndian.m_AoutEndian12 << 4) & GS2970_LSB_FIRSTA_SD;
		lRegData |= (pCfgPara->m_AudOutEndian.m_AoutEndian34 << 5) & GS2970_LSB_FIRSTB_SD;
		lRegData |= (pCfgPara->m_AudOutEndian.m_AoutEndian56 << 6) & GS2970_LSB_FIRSTC_SD;
		lRegData |= (pCfgPara->m_AudOutEndian.m_AoutEndian78 << 7) & GS2970_LSB_FIRSTD_SD;
		lRegData |= (pCfgPara->m_AudEmbedGroup.m_GrpA) & GS2970_IDA;
		lRegData |= (pCfgPara->m_AudEmbedGroup.m_GrpB << 2) & GS2970_IDB;
		plHandle->m_EnvPara.m_WriteRegCB(Handle, GS2970_REG_CFG_AUD_SD, lRegData, 2, pUserPara);
	}
	else /* HD/3G */
	{
		lRegData = (pCfgPara->m_AudOutWordLen.m_AoutWordLen56 << 10) & GS2970_ASWLB_HD; /* ��ǰ����ͨ�����õĸ�ʽΪ׼ */
		lRegData |= (pCfgPara->m_AudOutWordLen.m_AoutWordLen12 << 8) & GS2970_ASWLA_HD;
		lRegData |= (pCfgPara->m_AudOutFormat.m_AoutFmt56 << 6) & GS2970_AMB_HD; /* ��ǰ����ͨ�����õĸ�ʽΪ׼ */
		lRegData |= (pCfgPara->m_AudOutFormat.m_AoutFmt12 << 4) & GS2970_AMA_HD;
		lRegData |= (pCfgPara->m_AudEmbedGroup.m_GrpA) & GS2970_IDA;
		lRegData |= (pCfgPara->m_AudEmbedGroup.m_GrpB << 2) & GS2970_IDB;
		plHandle->m_EnvPara.m_WriteRegCB(Handle, GS2970_REG_CFG_AUD_HD_3G, lRegData, 2, pUserPara);
	}

	/* set register 0x405/0x205 */
	lRegData = pCfgPara->m_AudOutMute.m_AoutMute1 | (pCfgPara->m_AudOutMute.m_AoutMute2 << 1) 
					| (pCfgPara->m_AudOutMute.m_AoutMute3 << 2) | (pCfgPara->m_AudOutMute.m_AoutMute4 << 3) 
					| (pCfgPara->m_AudOutMute.m_AoutMute5 << 4) | (pCfgPara->m_AudOutMute.m_AoutMute6 << 5) 
					| (pCfgPara->m_AudOutMute.m_AoutMute7 << 6) | (pCfgPara->m_AudOutMute.m_AoutMute8 << 7);
	if (lSdiType == GS2970_SDI_SD_STA)
	{
		plHandle->m_EnvPara.m_WriteRegCB(Handle, GS2970_REG_CH_MUTE_SD, lRegData, 2, pUserPara);
	}
	else /* HD/3G */
	{
		plHandle->m_EnvPara.m_WriteRegCB(Handle, GS2970_REG_CH_MUTE_HD_3G, lRegData, 2, pUserPara);
	}

	/* set register 0x408 */
	if (lSdiType == GS2970_SDI_SD_STA)
	{
		lRegData = (pCfgPara->m_AudOutWordLen.m_AoutWordLen12 << 8) & GS2970_ASWLA_SD;
		lRegData |= (pCfgPara->m_AudOutWordLen.m_AoutWordLen34 << 10) & GS2970_ASWLB_SD;
		lRegData |= (pCfgPara->m_AudOutWordLen.m_AoutWordLen56 << 12) & GS2970_ASWLC_SD;
		lRegData |= (pCfgPara->m_AudOutWordLen.m_AoutWordLen78 << 14) & GS2970_ASWLD_SD;
		lRegData |= (pCfgPara->m_AudOutFormat.m_AoutFmt12) & GS2970_AMA_SD;
		lRegData |= (pCfgPara->m_AudOutFormat.m_AoutFmt34 << 2) & GS2970_AMB_SD;
		lRegData |= (pCfgPara->m_AudOutFormat.m_AoutFmt56 << 4) & GS2970_AMC_SD;
		lRegData |= (pCfgPara->m_AudOutFormat.m_AoutFmt78 << 6) & GS2970_AMD_SD;
		plHandle->m_EnvPara.m_WriteRegCB(Handle, GS2970_REG_CFG_OUTPUT_SD, lRegData, 2, pUserPara);
	}

	/* set register 0x409/0x40A/0x20A/0x20B */
	lRegData = (pCfgPara->m_AudOutSrc.m_AoutSrc1) & GS2970_OP1_SRC;
	lRegData |= (pCfgPara->m_AudOutSrc.m_AoutSrc2 << 3) & GS2970_OP2_SRC;
	lRegData |= (pCfgPara->m_AudOutSrc.m_AoutSrc3 << 6) & GS2970_OP3_SRC;
	lRegData |= (pCfgPara->m_AudOutSrc.m_AoutSrc4 << 9) & GS2970_OP4_SRC;
	if (lSdiType == GS2970_SDI_SD_STA)
	{
		plHandle->m_EnvPara.m_WriteRegCB(Handle, GS2970_REG_OUTPUT_SEL_1_SD, lRegData, 2, pUserPara);
	}
	else /* HD/3G */
	{
		plHandle->m_EnvPara.m_WriteRegCB(Handle, GS2970_REG_OUTPUT_SEL_1_HD_3G, lRegData, 2, pUserPara);
	}
	lRegData = (pCfgPara->m_AudOutSrc.m_AoutSrc5) & GS2970_OP5_SRC;
	lRegData |= (pCfgPara->m_AudOutSrc.m_AoutSrc6 << 3) & GS2970_OP6_SRC;
	lRegData |= (pCfgPara->m_AudOutSrc.m_AoutSrc7 << 6) & GS2970_OP7_SRC;
	lRegData |= (pCfgPara->m_AudOutSrc.m_AoutSrc8 << 9) & GS2970_OP8_SRC;
	
	if (lSdiType == GS2970_SDI_SD_STA)
	{
		plHandle->m_EnvPara.m_WriteRegCB(Handle, GS2970_REG_OUTPUT_SEL_2_SD, lRegData, 2, pUserPara);
	}
	else /* HD/3G */
	{
		plHandle->m_EnvPara.m_WriteRegCB(Handle, GS2970_REG_OUTPUT_SEL_2_HD_3G, lRegData, 2, pUserPara);
	}

	return TRUE;
}

/*****************************************************************************
* FUNCTION:HWL_Gs2970GetPara
*
* DESCRIPTION:
*	
* INPUT:
*	  
* OUTPUT:
*	
*
* RETURN:
*
* NOTES:
*   
* HISTORY:
*	
*	Review: 
******************************************************************************/
BOOL HWL_Gs2970GetPara(HANDLE32 Handle, HWL_Gs2970StatusPara *pStatusPara, void *pUserPara)
{
	U16 lRegData = 0;
	GS2970_Handle *plHandle = (GS2970_Handle *)Handle;
	
	if (plHandle == NULL)
	{
		GLOBAL_TRACE(("[ERR] Please First Carry Out HWL_Gs2970EnvInit!\n"));
		return FALSE;
	}

	plHandle->m_EnvPara.m_ReadRegCB(Handle, GS2970_REG_RASTER_STRUC_4, &lRegData, 2, pUserPara);
	
	if (lRegData & GS2970_SDT_LOCK) /* Lock */
	{
		int lSdiType;

		pStatusPara->m_LockStatus = GS2970_LOCKED;
		switch ((lRegData >> 14) & 0x0003) /* ��Ƶ��ʽ */
		{
		case 0:
			lSdiType = GS2970_SDI_HD_STA;
			break;
		case 2:
			lSdiType = GS2970_SDI_3G_STA;
			break;
		default:
			lSdiType = GS2970_SDI_SD_STA;
			break;
		}

		if (lSdiType == GS2970_SDI_SD_STA) /* SD */
		{
			/* ��������Ƶ��� */
			plHandle->m_EnvPara.m_ReadRegCB(Handle, GS2970_REG_DATA_FORMAT_DS1, &lRegData, 2, pUserPara);
			
			switch ((lRegData >> 8) & 0x3F) /* VD_STD_DS1 */
			{
			case 0x18:
			case 0x1A:           
				pStatusPara->m_VidFormat = GS2970_VID_FMT_720_576_50I;   	
				break;
			case 0x16:
			case 0x17:                                               
			case 0x19:                                                 
			case 0x1B:    
				pStatusPara->m_VidFormat = GS2970_VID_FMT_720_480_60I;     	
				break;	
			default :
				pStatusPara->m_VidFormat = GS2970_VID_FMT_NO_IDENTIFY;     	
				break;			       		
			}

			/* ��������Ƶ���, ������Ƿ�����ƵǶ�� */
			plHandle->m_EnvPara.m_ReadRegCB(Handle, GS2970_REG_AUD_DET_SD, &lRegData, 2, pUserPara);
			pStatusPara->m_AudEmbedStatus.m_AudGrp1Status = (((lRegData >> 4) & 0x01) ? GS2970_AUD_EMBED : GS2970_AUD_NO_EMBED);
			pStatusPara->m_AudEmbedStatus.m_AudGrp2Status = (((lRegData >> 5) & 0x01) ? GS2970_AUD_EMBED : GS2970_AUD_NO_EMBED);
			pStatusPara->m_AudEmbedStatus.m_AudGrp3Status = (((lRegData >> 6) & 0x01) ? GS2970_AUD_EMBED : GS2970_AUD_NO_EMBED);
			pStatusPara->m_AudEmbedStatus.m_AudGrp4Status = (((lRegData >> 7) & 0x01) ? GS2970_AUD_EMBED : GS2970_AUD_NO_EMBED);
		}
		else /* HD/3G */
		{
			int lFreqType;

			/* ��������Ƶ��� */
			if ((lRegData >> 13) & 0x0001) /* M Value */
			{
				lFreqType = GS2970_59_94_FPS; /* 1 / 1.001 */
			}
			else
			{
				lFreqType = GS2970_60_FPS; /* 1 / 1.000 */
			}

			plHandle->m_EnvPara.m_ReadRegCB(Handle, GS2970_REG_DATA_FORMAT_DS1, &lRegData, 2, pUserPara);
			
			switch ((lRegData >> 8) & 0x3F) /* VD_STD_DS1 */
			{
			case 0x2D:
				pStatusPara->m_VidFormat = GS2970_VID_FMT_1920_1080_50P;
				break;
			case 0x2B:
				if(lFreqType == GS2970_59_94_FPS)
				{
					pStatusPara->m_VidFormat = GS2970_VID_FMT_1920_1080_59_94P;  	
				}
				else
				{
					pStatusPara->m_VidFormat = GS2970_VID_FMT_1920_1080_60P;  		    	
				}
				break;
			case 0x14:
			case 0x0C:
				pStatusPara->m_VidFormat = GS2970_VID_FMT_1920_1080_50I;  	
				break;
			case 0x0A:
				if(lFreqType == GS2970_59_94_FPS)
				{
					pStatusPara->m_VidFormat = GS2970_VID_FMT_1920_1080_59_94I;  	
				}
				else
				{
					pStatusPara->m_VidFormat = GS2970_VID_FMT_1920_1080_60I;  		    	
				}
				break;
			case 0x04:
			case 0x05:
				pStatusPara->m_VidFormat = GS2970_VID_FMT_1280_720_50P;  	
				break;
			case 0x00:
			case 0x01:
				if(lFreqType == GS2970_59_94_FPS)
				{
					pStatusPara->m_VidFormat = GS2970_VID_FMT_1280_720_59_94P;  	
				}
				else
				{
					pStatusPara->m_VidFormat = GS2970_VID_FMT_1280_720_60P;  		    	
				}
				break;
			default:
				pStatusPara->m_VidFormat = GS2970_VID_FMT_NO_IDENTIFY;    	
				break;
			}

			/* ��������Ƶ���, ������Ƿ�����ƵǶ�� */
			plHandle->m_EnvPara.m_ReadRegCB(Handle, GS2970_REG_AUD_DET1_HD_3G, &lRegData, 2, pUserPara);
			pStatusPara->m_AudEmbedStatus.m_AudGrp1Status = (((lRegData >> 1) & 0x01) ? GS2970_AUD_EMBED : GS2970_AUD_NO_EMBED);
			pStatusPara->m_AudEmbedStatus.m_AudGrp2Status = (((lRegData >> 2) & 0x01) ? GS2970_AUD_EMBED : GS2970_AUD_NO_EMBED);
			pStatusPara->m_AudEmbedStatus.m_AudGrp3Status = (((lRegData >> 3) & 0x01) ? GS2970_AUD_EMBED : GS2970_AUD_NO_EMBED);
			pStatusPara->m_AudEmbedStatus.m_AudGrp4Status = (((lRegData >> 4) & 0x01) ? GS2970_AUD_EMBED : GS2970_AUD_NO_EMBED);
		}
	}
	else
	{
		pStatusPara->m_LockStatus = GS2970_UNLOCKED;
		pStatusPara->m_VidFormat = GS2970_VID_FMT_NO_IDENTIFY;
	}

	return TRUE;
}

BOOL HWL_Gs2970_GPIO_Debug(HANDLE32 Handle, void *pUserPara)
{
	GS2970_Handle *plHandle = (GS2970_Handle *)Handle;
	S32 lCounter = GS2970_CHECK_COUNT;

	if (plHandle == NULL)
	{
		GLOBAL_TRACE(("[ERR] Please First Carry Out HWL_Gs2970EnvInit!\n"));
		return FALSE;
	}

	GLOBAL_TRACE((" ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n")); 
	GLOBAL_TRACE((" m_ResetPin = (%d-%d), m_20_10BitPin = (%d-%d) (Handle = 0x%x)\n", 
									plHandle->m_EnvPara.m_InitParam.m_ResetPin.m_GpioIndex,
									plHandle->m_EnvPara.m_InitParam.m_ResetPin.m_Pin,
									plHandle->m_EnvPara.m_InitParam.m_20_10BitPin.m_GpioIndex,
									plHandle->m_EnvPara.m_InitParam.m_20_10BitPin.m_Pin,
									Handle));
	GLOBAL_TRACE((" ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n")); 


	/* Configure GPIO of  Reset and 20-10bit */
	plHandle->m_EnvPara.m_SetupGpioCB(plHandle->m_EnvPara.m_InitParam.m_ResetPin, 1, NULL);
	plHandle->m_EnvPara.m_SetupGpioCB(plHandle->m_EnvPara.m_InitParam.m_20_10BitPin, 1, NULL);

	plHandle->m_EnvPara.m_WriteGpioCB(plHandle->m_EnvPara.m_InitParam.m_20_10BitPin, LEVEL_LOW, pUserPara);
	usleep(5000); 
	plHandle->m_EnvPara.m_WriteGpioCB(plHandle->m_EnvPara.m_InitParam.m_20_10BitPin, LEVEL_HIGH, pUserPara);
	usleep(5000);  
	
	if (GS2970_Reset(Handle, pUserPara) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL HWL_Gs2970_Reset_Debug(HANDLE32 Handle, void *pUserPara)
{
	GS2970_Handle *plHandle = (GS2970_Handle *)Handle;
	S32 lCounter = GS2970_CHECK_COUNT;

	if (plHandle == NULL)
	{
		GLOBAL_TRACE(("[ERR] Please First Carry Out HWL_Gs2970EnvInit!\n"));
		return FALSE;
	}

	/* Configure GPIO of  Reset and 20-10bit */
	plHandle->m_EnvPara.m_SetupGpioCB(plHandle->m_EnvPara.m_InitParam.m_ResetPin, 1, NULL);
	plHandle->m_EnvPara.m_SetupGpioCB(plHandle->m_EnvPara.m_InitParam.m_20_10BitPin, 1, NULL);

	if (GS2970_Reset(Handle, pUserPara) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL HWL_Gs2970_SPI_Read_Debug(HANDLE32 Handle, void *pUserPara)
{
	GS2970_Handle *plHandle = (GS2970_Handle *)Handle;
	U16 lRegData = 0;
	S32 read_Data = 0;

	if (plHandle == NULL)
	{
		GLOBAL_TRACE(("[ERR] Please First Carry Out HWL_Gs2970EnvInit!\n"));
		return FALSE;
	}

#if 0
	plHandle->m_EnvPara.m_ReadRegCB(Handle, GS2970_REG_IO_DRIVE_STRENGTH, &lRegData, 2, pUserPara);
	if (lRegData == GS2970_REG_IO_DRIVE_STRENGTH_CONST_VAL)
	{
		GLOBAL_TRACE(("\r\n\r\n Read Reg:0x06D data = 0x%x(------Read Test OK!----)\r\n", lRegData));
	}
	else
	{
		GLOBAL_TRACE(("\r\n Read Reg:0x06D data = 0x%x(!!!Read Test Fail!!!)\n", lRegData));
	}
#else
	plHandle->m_EnvPara.m_ReadRegCB(Handle, GS2970_REG_OUTPUT_SEL_2_HD_3G, &lRegData, 2, pUserPara);
   GLOBAL_TRACE(("Read Reg = 0x%x, data = 0x%x \r\n", GS2970_REG_OUTPUT_SEL_2_HD_3G, lRegData));
#endif
	return TRUE;
}

BOOL HWL_Gs2970_SPI_Write_Debug(HANDLE32 Handle, void *pUserPara)
{
	GS2970_Handle *plHandle = (GS2970_Handle *)Handle;
	U16 read_old_RegData = 0;
	U16 read_RegData = 0;
	U16 write_RegData = 0;

	if (plHandle == NULL)
	{
		GLOBAL_TRACE(("[ERR] Please First Carry Out HWL_Gs2970EnvInit!\n"));
		return FALSE;
	}

	plHandle->m_EnvPara.m_ReadRegCB(Handle, GS2970_REG_CH_MUTE_HD_3G, &read_old_RegData, 2, pUserPara);
	
	write_RegData = 0x11; 
	plHandle->m_EnvPara.m_WriteRegCB(Handle, GS2970_REG_CH_MUTE_HD_3G, write_RegData, 2, pUserPara);

	read_RegData = 0;
	plHandle->m_EnvPara.m_ReadRegCB(Handle, GS2970_REG_CH_MUTE_HD_3G, &read_RegData, 2, pUserPara);

	if (write_RegData == read_RegData)
	{
		GLOBAL_TRACE(("---Write Test OK---, (write_RegData = read_RegData = 0x%x) \n", read_RegData));
	}
	else
	{
		GLOBAL_TRACE(("---Write Test Fail---, (write_RegData = 0x%x, read_RegData = 0x%x (old data = 0x%x)\n", write_RegData, read_RegData, read_old_RegData));
		return FALSE;
	}

	return TRUE;
}

BOOL HWL_Gs2970_SPI_Read_All_Reg(HANDLE32 Handle, void *pUserPara, S32 chn)
{
	GS2970_Handle *plHandle = (GS2970_Handle *)Handle;
	U16	RetAddr = 0;
	U16 lRegData = 0;

	if (plHandle == NULL)
	{
		GLOBAL_TRACE(("[ERR] Please First Carry Out HWL_Gs2970EnvInit!\n"));
		return FALSE;
	}

	GLOBAL_TRACE(("------------Chn = %d-----------------------\n", chn));
	GLOBAL_TRACE(("Video Core Configuration and Status Registers\n"));
	for (RetAddr = GS2970_REG_IOPROC_1; RetAddr <= GS2970_REG_IO_DRIVE_STRENGTH; RetAddr++)
	{
		plHandle->m_EnvPara.m_ReadRegCB(Handle, RetAddr, &lRegData, 2, pUserPara);
   		GLOBAL_TRACE(("Read Reg = 0x%x, data = 0x%x \r\n", RetAddr, lRegData));
	}

	GLOBAL_TRACE(("SD Audio Core Registers\n"));
	for (RetAddr = GS2970_REG_CFG_AUD_SD; RetAddr <= GS2970_REG_ACSR_BYTE_22_SD; RetAddr++)
	{
		plHandle->m_EnvPara.m_ReadRegCB(Handle, RetAddr, &lRegData, 2, pUserPara);
   		GLOBAL_TRACE(("Read Reg = 0x%x, data = 0x%x \r\n", RetAddr, lRegData));
	}

	GLOBAL_TRACE(("HD and 3G Audio Core Registers\n"));
	for (RetAddr = GS2970_REG_CFG_AUD_HD_3G; RetAddr <= GS2970_REG_ACSR_BYTE_22_HD_3G; RetAddr++)
	{
		plHandle->m_EnvPara.m_ReadRegCB(Handle, RetAddr, &lRegData, 2, pUserPara);
   		GLOBAL_TRACE(("Read Reg = 0x%x, data = 0x%x \r\n", RetAddr, lRegData));
	}
	GLOBAL_TRACE(("--------------------------------------------\n"));
	
	return TRUE;
}


/* Eof */
