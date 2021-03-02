

/* Includes-------------------------------------------------------------------- */
#include "multi_main_internal.h"

/* Global Variables (extern)--------------------------------------------------- */
/* Macro (local)--------------------------------------------------------------- */

/* Private Constants ---------------------------------------------------------- */
/* Private Types -------------------------------------------------------------- */
/* Private Variables (static)-------------------------------------------------- */
/* Private Function prototypes ------------------------------------------------ */
/* Functions ------------------------------------------------------------------ */
CHAR_T* MULTL_XMLMarkValueToStr(BOOL Value)
{
	if (Value == TRUE)
	{
		return "ON";
	}
	return "OFF";
}

BOOL MULTL_XMLMarkValueFromStr(CHAR_T* pString)
{
	if (pString)
	{
		if (GLOBAL_STRCMP(pString, "ON") == 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

CHAR_T* MULTL_XMLPolarMethodValueToStr(BOOL Value)
{
	switch (Value)
	{
	case GS_TUNER_PLOAR_NONE:	
		return "None";
		break;
	case GS_TUNER_PLOAR_HOR:
		return "HOR";
		break;
	case GS_TUNER_PLOAR_VER:
		return "VER";
		break;
	default:
		break;
	}
	return "None";
}


S32 MULTL_XMLPolarMethodValueFromStr(CHAR_T* pString)
{
	S32 lValue = 0;
	if (pString)
	{
		if (GLOBAL_STRCMP(pString, "None") == 0)
		{
			lValue = GS_TUNER_PLOAR_NONE;
		}
		else if (GLOBAL_STRCMP(pString, "HOR") == 0)
		{
			lValue = GS_TUNER_PLOAR_HOR;
		}
		else if (GLOBAL_STRCMP(pString, "VER") == 0)
		{
			lValue = GS_TUNER_PLOAR_VER;
		}
	}
	return lValue;

}

CHAR_T* MULTL_XMLReqTunerTypeValueToStr(BOOL Value)
{
	switch (Value)
	{
	case GS_TUNER_ANNEX_A:	
		return "ANNEX A";
		break;
	case GS_TUNER_ANNEX_B:
		return "ANNEX B";
		break;
	case GS_TUNER_ANNEX_C:
		return "ANNEX C";
		break;
	default:
		break;
	}
	return "ANNEX A";
	
}

S32 MULTL_XMLReqTunerTypeValueFromStr(CHAR_T* pString)
{
	S32 lValue = 0;
	if (pString)
	{
		if (GLOBAL_STRCMP(pString, "ANNEX A") == 0)
		{
			lValue = GS_TUNER_ANNEX_A;
		}
		else if (GLOBAL_STRCMP(pString, "ANNEX B") == 0)
		{
			lValue = GS_TUNER_ANNEX_B;
		}
		else if (GLOBAL_STRCMP(pString, "ANNEX C") == 0)
		{
			lValue = GS_TUNER_ANNEX_C;
		}
	}
	return lValue;
}

S32 MULTL_XMLTunerSpecinvValueFromStr(CHAR_T* pString)
{
	S32 lValue = 0;
	if (pString)
	{
		if (GLOBAL_STRCMP(pString, "ON") == 0)
		{
			lValue = GS_SPECINV_ON;
		}
		else if (GLOBAL_STRCMP(pString, "OFF") == 0)
		{
			lValue = GS_SPECINV_OFF;
		}
		else if (GLOBAL_STRCMP(pString, "AUTO") == 0)
		{
			lValue = GS_SPECINV_AUTO;
		}
	}
	return lValue;
}
CHAR_T* MULTL_XMLTunerSpecinvValueToStr(S32 Value)
{
	switch (Value)
	{
	case GS_SPECINV_ON:
		return "ON";
		break;
	case GS_SPECINV_OFF:
		return "OFF";
		break;
	case GS_SPECINV_AUTO:
		return "AUTO";
		break;
	default:
		break;
	}

	return "OFF";

}




/*		????   E3DS3    start  */
CHAR_T* MULTL_XMLE3DS3ValueToStr(S32 Value)
{
	switch (Value)
	{
	case GS_E3DS3_SELECT_E3:
		return "E3";
		break;
	case GS_E3DS3_SELECT_DS3:
		return "DS3";
		break;
	case GS_E3DS3_SELECT_AUTO:
		return "Auto";
		break;
	default:
		break;
	}

	return "E3";
}

#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
S32 MULTL_XMLIpOutputTypeValueFromStr(CHAR_T *pStr) 
{
	S32 lValue = 0;

	if (pStr)
	{
		if (GLOBAL_STRCMP(pStr, "SPTS") == 0) {
			lValue = IP_OUTPUT_SPTS;
		}
		else if (GLOBAL_STRCMP(pStr, "MPTS") == 0) {
			lValue = IP_OUTPUT_MPTS;
		}
		else {
			lValue = IP_OUTPUT_SPTS;
		}
	}
	else
	{
		lValue = IP_OUTPUT_SPTS;
	}

	return lValue;
}

CHAR_T* MULTL_XMLIpOutputTypeValueToStr(S32 Value)
{
	switch (Value)
	{
	case IP_OUTPUT_SPTS:
		return "SPTS";
	case IP_OUTPUT_MPTS:
		return "MPTS";
	default:
		break;
	}

	return "SPTS";
}

CHAR_T* MULTL_XMLIPv4NetworkportsettingValueToStr(S32 Value)
{
	switch (Value)
	{
	case Networkport_Auto:
		return "AUTO";
		
	case Networkport_100M_F:
		return "100M Full";
	default:
		break;
	}

	return "AUTO";
}

S32 MULTL_XMLIPv4NetworkportSettingValueFromStr(CHAR_T *pStr) 
{
	S32 lValue = 0;

	if (pStr)
	{
		if (GLOBAL_STRCMP(pStr, "AUTO") == 0) {
			lValue = Networkport_Auto;
		}
		else if (GLOBAL_STRCMP(pStr, "100M Full") == 0) {
			lValue = Networkport_100M_F;
		}
		else {
			lValue = Networkport_Auto;
		}
	}
	else
	{
		lValue = Networkport_Auto;
	}

	return lValue;
}


S32 MULTL_XMLEdidTypeValueFromStr(CHAR_T *pStr) 
{
	S32 lValue = 0;

	if (pStr)
	{
		if (GLOBAL_STRCMP(pStr, "DownMix") == 0) {
			lValue = HDMI_RX_EDID_PCM;
		}
		else if (GLOBAL_STRCMP(pStr, "AC3") == 0) {
			lValue = HDMI_RX_EDID_AC3;
		}
		else if (GLOBAL_STRCMP(pStr, "NONE") == 0) {
			lValue = HDMI_RX_EDID_NONE;
		}
		else {
			lValue = HDMI_RX_EDID_NONE;
		}
	}
	else
	{
		lValue = HDMI_RX_EDID_NONE;
	}

	return lValue;
}

CHAR_T* MULTL_XMLEdidTypeValueToStr(S32 Value)
{
	switch (Value)
	{
	case HDMI_RX_EDID_PCM:
		return "DownMix";
	case HDMI_RX_EDID_AC3:
		return "AC3";
	case HDMI_RX_EDID_NONE:
		return "NONE";
	default:
		break;
	}

	return "NONE";
}

S32 MULTL_XMLEncViModeValueFromStr(CHAR_T *pStr) 
{
	S32 lValue = 0;

	if (pStr)
	{
		if (GLOBAL_STRCMP(pStr, "PAL") == 0) {
			lValue = ENC_VI_MODE_PAL;
		}
		else if (GLOBAL_STRCMP(pStr, "NTSC") == 0) {
			lValue = ENC_VI_MODE_NTSC;
		}
		else if (GLOBAL_STRCMP(pStr, "720P50") == 0) {
			lValue = ENC_VI_MODE_720P_50;
		}
		else if (GLOBAL_STRCMP(pStr, "720P5994") == 0) {
			lValue = ENC_VI_MODE_720P_5994;
		}
		else if (GLOBAL_STRCMP(pStr, "720P60") == 0) {
			lValue = ENC_VI_MODE_720P_60;
		}
		else if (GLOBAL_STRCMP(pStr, "1080I50") == 0) {
			lValue = ENC_VI_MODE_1080I_50;
		}
		else if (GLOBAL_STRCMP(pStr, "1080I5994") == 0) {
			lValue = ENC_VI_MODE_1080I_5994;
		}
		else if (GLOBAL_STRCMP(pStr, "1080I60") == 0) {
			lValue = ENC_VI_MODE_1080I_60;
		}
		else if (GLOBAL_STRCMP(pStr, "1080P50") == 0) {
			lValue = ENC_VI_MODE_1080P_50;
		}
		else if (GLOBAL_STRCMP(pStr, "1080P5994") == 0) {
			lValue = ENC_VI_MODE_1080P_5994;
		}
		else if (GLOBAL_STRCMP(pStr, "1080P60") == 0) {
			lValue = ENC_VI_MODE_1080P_60;
		}
		else if (GLOBAL_STRCMP(pStr, "UHD4KP30") == 0) {
			lValue = ENC_VI_MODE_UHD4KP_30;
		}
		else {
			lValue = ENC_VI_MODE_AUTO;
		}
	}
	else
	{
		lValue = ENC_VI_MODE_AUTO;
	}
	return lValue;
}

CHAR_T* MULTL_XMLEncViModeValueToStr(S32 Value)
{
	switch (Value)
	{
	case ENC_VI_MODE_PAL:
		return "PAL";
	case ENC_VI_MODE_NTSC:
		return "NTSC";
	case ENC_VI_MODE_720P_50:
		return "720P50";
	case ENC_VI_MODE_720P_5994:
		return "720P5994";
	case ENC_VI_MODE_720P_60:
		return "720P60";
	case ENC_VI_MODE_1080I_50:
		return "1080I50";
	case ENC_VI_MODE_1080I_5994:
		return "1080I5994";
	case ENC_VI_MODE_1080I_60:
		return "1080I60";
	case ENC_VI_MODE_1080P_50:
		return "1080P50";
	case ENC_VI_MODE_1080P_5994:
		return "1080P5994";
	case ENC_VI_MODE_1080P_60:
		return "1080P60";
	case ENC_VI_MODE_UHD4KP_30:
		return "UHD4KP30";
	case ENC_VI_MODE_AUTO:
		return "AUTO";
	default:
		break;
	}

	return "--------";
}

S32 MULTL_XMLEncVoModeValueFromStr(CHAR_T *pStr) 
{
	S32 lValue = 0;

	if (pStr)
	{
		if (GLOBAL_STRCMP(pStr, "576P25") == 0) {
			lValue = ENC_VO_MODE_576P25;
		}
		else if (GLOBAL_STRCMP(pStr, "480P2997") == 0) {
			lValue = ENC_VO_MODE_480P2997;
		}
		else if (GLOBAL_STRCMP(pStr, "720P25") == 0) {
			lValue = ENC_VO_MODE_720P25;
		}
		else if (GLOBAL_STRCMP(pStr, "720P2997") == 0) {
			lValue = ENC_VO_MODE_720P2997;
		}
		else if (GLOBAL_STRCMP(pStr, "720P30") == 0) {
			lValue = ENC_VO_MODE_720P30;
		}
		else if (GLOBAL_STRCMP(pStr, "720P50") == 0) {
			lValue = ENC_VO_MODE_720P50;
		}
		else if (GLOBAL_STRCMP(pStr, "720P5994") == 0) {
			lValue = ENC_VO_MODE_720P5994;
		}
		else if (GLOBAL_STRCMP(pStr, "720P60") == 0) {
			lValue = ENC_VO_MODE_720P60;
		}
		else if (GLOBAL_STRCMP(pStr, "1080P25") == 0) {
			lValue = ENC_VO_MODE_1080P25;
		}
		else if (GLOBAL_STRCMP(pStr, "1080P2997") == 0) {
			lValue = ENC_VO_MODE_1080P2997;
		}
		else if (GLOBAL_STRCMP(pStr, "1080P30") == 0) {
			lValue = ENC_VO_MODE_1080P30;
		}
		else if (GLOBAL_STRCMP(pStr, "1080P50") == 0) {
			lValue = ENC_VO_MODE_1080P50;
		}
		else if (GLOBAL_STRCMP(pStr, "1080P5994") == 0) {
			lValue = ENC_VO_MODE_1080P5994;
		}
		else if (GLOBAL_STRCMP(pStr, "1080P60") == 0) {
			lValue = ENC_VO_MODE_1080P60;
		}
		else if (GLOBAL_STRCMP(pStr, "UHD4KP30") == 0) {
			lValue = ENC_VO_MODE_UHD4KP_30;
		}
		else if (GLOBAL_STRCMP(pStr, "AUTO") == 0) {
			lValue = ENC_VO_MODE_AUTO;
		}
		else {
			lValue = ENC_VO_MODE_AUTO;
		}
	}
	else
	{
		lValue = ENC_VO_MODE_AUTO;
	}
	return lValue;
}

CHAR_T* MULTL_XMLEncVoModeValueToStr(S32 Value)
{
	switch (Value)
	{
	case ENC_VO_MODE_576P25:
		return "576P25";
	case ENC_VO_MODE_480P2997:
		return "480P2997";
	case ENC_VO_MODE_720P25:
		return "720P25";
	case ENC_VO_MODE_720P2997:
		return "720P2997";
	case ENC_VO_MODE_720P30:
		return "720P30";
	case ENC_VO_MODE_720P50:
		return "720P50";
	case ENC_VO_MODE_720P5994:
		return "720P5994";
	case ENC_VO_MODE_720P60:
		return "720P60";
	case ENC_VO_MODE_1080P25:
		return "1080P25";
	case ENC_VO_MODE_1080P2997:
		return "1080P2997";
	case ENC_VO_MODE_1080P30:
		return "1080P30";
	case ENC_VO_MODE_1080P50:
		return "1080P50";
	case ENC_VO_MODE_1080P5994:
		return "1080P5994";
	case ENC_VO_MODE_1080P60:
		return "1080P60";
	case ENC_VO_MODE_UHD4KP_30:
		return "UHD4KP30";
	case ENC_VO_MODE_AUTO:
		return "AUTO";
	default:
		return "AUTO";
	}

	return "AUTO";
}

S32 MULTL_XMLEncBrModeValueFromStr(CHAR_T *pStr) 
{
	S32 lValue = 0;

	if (pStr)
	{
		if (GLOBAL_STRCMP(pStr, "VBR") == 0) {
			lValue = ENC_BR_MODE_VBR;
		}
		else if (GLOBAL_STRCMP(pStr, "CBR") == 0) {
			lValue = ENC_BR_MODE_CBR;
		}
		else {
			lValue = ENC_BR_MODE_CBR;
		}
	}
	else
	{
		lValue = ENC_BR_MODE_CBR;
	}
	return lValue;
}

CHAR_T* MULTL_XMLEncBrModeValueToStr(S32 Value)
{
	switch (Value)
	{
	case ENC_BR_MODE_VBR:
		return "VBR";
	case ENC_BR_MODE_CBR:
		return "CBR";
	default:
		return "CBR";
	}

	return "CBR";
}

S32 MULTL_XMLEncModeValueFromStr(CHAR_T *pStr) 
{
	S32 lValue = 0;

	if (pStr)
	{
		if (GLOBAL_STRCMP(pStr, "H264") == 0) {
			lValue = ENC_MODE_H264;
		}
		else if (GLOBAL_STRCMP(pStr, "H265") == 0) {
			lValue = ENC_MODE_H265;
		}
		else {
			lValue = ENC_MODE_H264;
		}
	}
	else
	{
		lValue = ENC_MODE_H264;
	}
	return lValue;
}

CHAR_T* MULTL_XMLEncModeValueToStr(S32 Value)
{
	switch (Value)
	{
	case ENC_MODE_H264:
		return "H264";
	case ENC_MODE_H265:
		return "H265";
	default:
		return "H264";
	}

	return "CBR";
}

S32 MULTL_XMLEncProfileValueFromStr(CHAR_T *pStr) 
{
	S32 lValue = 0;

	if (pStr)
	{
		if (GLOBAL_STRCMP(pStr, "BASE") == 0) {
			lValue = ENC_PROFILE_BASE;
		}
		else if (GLOBAL_STRCMP(pStr, "MP") == 0) {
			lValue = ENC_PROFILE_MP;
		}
		else if (GLOBAL_STRCMP(pStr, "HP") == 0) {
			lValue = ENC_PROFILE_HP;
		}
		else {
			lValue = ENC_PROFILE_MP;
		}
	}
	else
	{
		lValue = ENC_PROFILE_MP;
	}
	return lValue;
}

CHAR_T* MULTL_XMLEncProfileValueToStr(S32 Value)
{
	switch (Value)
	{
	case ENC_PROFILE_BASE:
		return "BASE";
	case ENC_PROFILE_MP:
		return "MP";
	case ENC_PROFILE_HP:
		return "HP";
	default:
		return "MP";
	}

	return "MP";
}

CHAR_T* MULTL_XMLEncMosaicSizeValueToStr(S32 Value)
{
	switch (Value)
	{
	case ENC_MOSAIC_BLK_SIZE_8:
		return "8*8";
		
	case ENC_MOSAIC_BLK_SIZE_16:
		return "16*16";
		
	case ENC_MOSAIC_BLK_SIZE_32:
		return "32*32";

	case ENC_MOSAIC_BLK_SIZE_64:
		return "64*64";

	default:
		return "16*16";
	}

	return "16*16";
}

S32 MULTL_XMLEncMosaicSizeValueFromStr(CHAR_T *pStr) 
{
	S32 lValue = 0;

	if (pStr)
	{
		if (GLOBAL_STRCMP(pStr, "8*8") == 0) {
			lValue = ENC_MOSAIC_BLK_SIZE_8;
		}
		else if (GLOBAL_STRCMP(pStr, "16*16") == 0) {
			lValue = ENC_MOSAIC_BLK_SIZE_16;
		}
		else if (GLOBAL_STRCMP(pStr, "32*32") == 0) {
			lValue = ENC_MOSAIC_BLK_SIZE_32;
		}
		else if (GLOBAL_STRCMP(pStr, "64*64") == 0) {
			lValue = ENC_MOSAIC_BLK_SIZE_64;
		}
		else {
			lValue = ENC_MOSAIC_BLK_SIZE_16;
		}
	}
	else
	{
		lValue = ENC_MOSAIC_BLK_SIZE_16;
	}
	return lValue;
}

S32 MULTL_XMLAudEncModeValueFromStr(CHAR_T *pStr) 
{
	S32 lValue = 0;

	if (pStr)
	{
		if (GLOBAL_STRCMP(pStr, "MPEG1-L2") == 0) {
			lValue = ENC_AUD_ENC_MODE_MPEG1_L2;
		}
		else if (GLOBAL_STRCMP(pStr, "Mode-AA") == 0) {
			lValue = ENC_AUD_ENC_MODE_LC_AAC;
		}
		else if (GLOBAL_STRCMP(pStr, "AC3-Bypass") == 0)
		{
			lValue = ENC_AUD_ENC_MODE_AC3;
		}
		else if (GLOBAL_STRCMP(pStr, "E-AC3-Bypass") == 0)
		{
			lValue = ENC_AUD_ENC_MODE_EAC3;
		}
		else {
			lValue = ENC_AUD_ENC_MODE_MPEG1_L2;
		}
	}
	else
	{
		lValue = ENC_AUD_ENC_MODE_MPEG1_L2;
	}
	return lValue;
}

CHAR_T* MULTL_XMLAudEncModeValueToStr(S32 Value)
{
	switch (Value)
	{
	case ENC_AUD_ENC_MODE_MPEG1_L2:
		return "MPEG1-L2";
	case ENC_AUD_ENC_MODE_LC_AAC:
		return "Mode-AA";
	case ENC_AUD_ENC_MODE_AC3:
		return "AC3-Bypass";
	case ENC_AUD_ENC_MODE_EAC3:
		return "E-AC3-Bypass";
	default:
		return "MPEG1-L2";
	}

	return "MPEG1-L2";
}

CHAR_T* MULTL_XMLAudGroupValueToStr(S32 Value)
{
	switch(Value)
	{
	case SDI_GRP1_CH_1_2:
		return "Group 1(Channel 1+2)";
		
	case SDI_GRP1_CH_3_4:
		return "Group 2(Channel 3+4)";
		
	case SDI_GRP2_CH_5_6:
		return "Group 3(Channel 5+6)";

	case SDI_GRP2_CH_7_8:
		return "Group 4(Channel 7+8)";

	case SDI_GRP3_CH_9_10:
		return "Group 5(Channel 9+10)";

	case SDI_GRP3_CH_11_12:
		return "Group 6(Channel 11+12)";
		
	case SDI_GRP4_CH_13_14:
		return "Group 7(Channel 13+14)";

	case SDI_GRP4_CH_15_16:
		return "Group 8(Channel 15+16)";
		
	default:
		return "Group 1(Channel 1+2)";
	}

	return "Group 1(Channel 1+2)";
}

S32 MULTL_XMLAudGroupValueFromStr(CHAR_T *pStr)
{
	S32 lValue = 0;

	if(pStr)
	{
		if (GLOBAL_STRCMP(pStr, "Group 1(Channel 1+2)") == 0) 
		{
			lValue = SDI_GRP1_CH_1_2;
		}
		else if (GLOBAL_STRCMP(pStr, "Group 2(Channel 3+4)") == 0) 
		{
			lValue = SDI_GRP1_CH_3_4;
		}
		else if (GLOBAL_STRCMP(pStr, "Group 3(Channel 5+6)") == 0)
		{
			lValue = SDI_GRP2_CH_5_6;
		}
		else if (GLOBAL_STRCMP(pStr, "Group 4(Channel 7+8)") == 0)
		{
			lValue = SDI_GRP2_CH_7_8;
		}
		else if (GLOBAL_STRCMP(pStr, "Group 5(Channel 9+10)") == 0)
		{
			lValue = SDI_GRP3_CH_9_10;
		}
		else if (GLOBAL_STRCMP(pStr, "Group 6(Channel 11+12)") == 0)
		{
			lValue = SDI_GRP3_CH_11_12;
		}
		else if (GLOBAL_STRCMP(pStr, "Group 7(Channel 13+14)") == 0)
		{
			lValue = SDI_GRP4_CH_13_14;
		}
		else if (GLOBAL_STRCMP(pStr, "Group 8(Channel 15+16)") == 0)
		{
			lValue = SDI_GRP4_CH_15_16;
		}
		else {
			lValue = SDI_GRP1_CH_1_2;
		}
	}
	else
	{
		lValue = SDI_GRP1_CH_1_2;
	}
	return lValue;
}

S32 MULTL_XMLAudBitrateValueFromStr(CHAR_T *pStr) 
{
	S32 lValue = 0;

	if (pStr)
	{
		if (GLOBAL_STRCMP(pStr, "16") == 0) {
			lValue = ENC_AUD_BR_16K;
		}
		else if (GLOBAL_STRCMP(pStr, "64") == 0) {
			lValue = ENC_AUD_BR_64K;
		}
		else if (GLOBAL_STRCMP(pStr, "96") == 0) {
			lValue = ENC_AUD_BR_96K;
		}
		else if (GLOBAL_STRCMP(pStr, "112") == 0) {
			lValue = ENC_AUD_BR_112K;
		}
		else if (GLOBAL_STRCMP(pStr, "128") == 0) {
			lValue = ENC_AUD_BR_128K;
		}
		else if (GLOBAL_STRCMP(pStr, "160") == 0) {
			lValue = ENC_AUD_BR_160K;
		}
		else if (GLOBAL_STRCMP(pStr, "192") == 0) {
			lValue = ENC_AUD_BR_192K;
		}
		else if (GLOBAL_STRCMP(pStr, "224") == 0) {
			lValue = ENC_AUD_BR_224K;
		}
		else if (GLOBAL_STRCMP(pStr, "256") == 0) {
			lValue = ENC_AUD_BR_256K;
		}
		else if (GLOBAL_STRCMP(pStr, "320") == 0) {
			lValue = ENC_AUD_BR_320K;
		}
		else if (GLOBAL_STRCMP(pStr, "384") == 0) {
			lValue = ENC_AUD_BR_384K;
		}
		else {
			lValue = ENC_AUD_BR_96K;
		}
	}
	else
	{
		lValue = ENC_AUD_BR_96K;
	}
	return lValue;
}

CHAR_T* MULTL_XMLAudBitrateValueToStr(S32 Value)
{
	switch (Value)
	{
	case ENC_AUD_BR_16K:
		return "16";
	case ENC_AUD_BR_64K:
		return "64";
	case ENC_AUD_BR_96K:
		return "96";
	case ENC_AUD_BR_112K:
		return "112";
	case ENC_AUD_BR_128K:
		return "128";
	case ENC_AUD_BR_160K:
		return "160";
	case ENC_AUD_BR_192K:
		return "192";
	case ENC_AUD_BR_224K:
		return "224";
	case ENC_AUD_BR_256K:
		return "256";
	case ENC_AUD_BR_384K:
		return "384";
	default:
		return "96";
	}

	return "96";
}

S32 MULTL_XMLAudSampValueFromStr(CHAR_T *pStr) 
{
	S32 lValue = 0;

	if (pStr)
	{
		if (GLOBAL_STRCMP(pStr, "32") == 0) {
			lValue = ENC_AUD_SAMP_32K;
		}
		else if (GLOBAL_STRCMP(pStr, "44.1") == 0) {
			lValue = ENC_AUD_SAMP_44_1K;
		}
		else if (GLOBAL_STRCMP(pStr, "48") == 0) {
			lValue = ENC_AUD_SAMP_48K;
		}
		else if (GLOBAL_STRCMP(pStr, "AUTO") == 0) {
			lValue = ENC_AUD_SAMP_AUTO;
		}
		else {
			lValue = ENC_AUD_SAMP_48K;
		}
	}
	else
	{
		lValue = ENC_AUD_SAMP_48K;
	}
	return lValue;
}

CHAR_T* MULTL_XMLAudSampValueToStr(S32 Value)
{
	switch (Value)
	{
	case ENC_AUD_SAMP_32K:
		return "32";
	case ENC_AUD_SAMP_44_1K:
		return "44.1";
	case ENC_AUD_SAMP_48K:
		return "48";
	case ENC_AUD_SAMP_AUTO:
		return "AUTO";
	default:
		return "48";
	}

	return "48";
}

#endif

S32 MULTL_XMLE3DS3ValueFromStr(CHAR_T* pString)
{
	S32 lValue = GS_E3DS3_SELECT_E3;
	if (pString)
	{
		if (GLOBAL_STRCMP(pString, "E3") == 0)
		{
			lValue = GS_E3DS3_SELECT_E3;
		}
		else if (GLOBAL_STRCMP(pString, "DS3") == 0)
		{
			lValue = GS_E3DS3_SELECT_DS3;
		}
		else if (GLOBAL_STRCMP(pString, "Auto") == 0)
		{
			lValue = GS_E3DS3_SELECT_AUTO;
		}
	}
	return lValue;
}


CHAR_T* MULTL_XMLBitOrderValueToStr(S32 Value)
{
	switch (Value)
	{
	case GS_E3DS3_BITORDER_MSB:
		return "MSB";
		break;
	case GS_E3DS3_BITORDER_LSB:
		return "LSB";
		break;
	default:
		break;
	}

	return "MSB";
}

S32 MULTL_XMLBitOrderValueFromStr(CHAR_T* pString)
{
	S32 lValue = GS_E3DS3_BITORDER_MSB;
	if (pString)
	{
		if (GLOBAL_STRCMP(pString, "MSB") == 0)
		{
			lValue = GS_E3DS3_BITORDER_MSB;
		}
		else if (GLOBAL_STRCMP(pString, "LSB") == 0)
		{
			lValue = GS_E3DS3_BITORDER_LSB;
		}
	}
	return lValue;
}

CHAR_T* MULTL_XMLFrameformValueToStr(S32 Value)
{
	switch (Value)
	{
	case GS_E3DS3_FRAMEFORM_NO:
		return "No Framed";
		break;
	case GS_E3DS3_FRAMEFORM_YES:
		return "Framed";
		break;
	default:
		break;
	}

	return "Framed";
}

S32 MULTL_XMLFrameformValueFromStr(CHAR_T* pString)
{
	S32 lValue = GS_E3DS3_FRAMEFORM_YES;
	if (pString)
	{
		if (GLOBAL_STRCMP(pString, "Framed") == 0)
		{
			lValue = GS_E3DS3_FRAMEFORM_YES;
		}
		else if (GLOBAL_STRCMP(pString, "No Framed") == 0)
		{
			lValue = GS_E3DS3_FRAMEFORM_NO;
		}
	}
	return lValue;
}

S32 MULTL_XMLPacketlengthformValueFromStr(CHAR_T* pString)
{
	S32 lValue = GS_E3DS3_PACKETLENGTH_204;
	if (pString)
	{
		if (GLOBAL_STRCMP(pString, "188") == 0)
		{
			lValue = GS_E3DS3_PACKETLENGTH_188;
		}
		else if (GLOBAL_STRCMP(pString, "204") == 0)
		{
			lValue = GS_E3DS3_PACKETLENGTH_204;
		}
	}
	return lValue;

}

CHAR_T* MULTL_XMLPacketlengthValueToStr(S32 Value)
{
	switch (Value)
	{
	case GS_E3DS3_PACKETLENGTH_188:
		return "188";
		break;
	case GS_E3DS3_PACKETLENGTH_204:
		return "204";
		break;
	default:
		break;
	}

	return "188";
}

/*		????   E3DS3    end  */

CHAR_T* MULTL_XMLITUCodingValueToStr(S32 Value)
{
	switch (Value)
	{
	case GS_MODULATOR_STANDARD_ANNEX_A:
		return "ANNEX A";
		break;
	case GS_MODULATOR_STANDARD_ANNEX_B:
		return "ANNEX B";
		break;
	case GS_MODULATOR_STANDARD_ANNEX_C:
		return "ANNEX C";
		break;
	default:
		break;
	}

	return "ANNEX A";
}

S32 MULTL_XMLITUCodingValueFromStr(CHAR_T* pString)
{
	S32 lValue = GS_MODULATOR_STANDARD_ANNEX_A;
	if (pString)
	{
		if (GLOBAL_STRCMP(pString, "ANNEX A") == 0)
		{
			lValue = GS_MODULATOR_STANDARD_ANNEX_A;
		}
		else if (GLOBAL_STRCMP(pString, "ANNEX B") == 0)
		{
			lValue = GS_MODULATOR_STANDARD_ANNEX_B;
		}
		else if (GLOBAL_STRCMP(pString, "ANNEX C") == 0)
		{
			lValue = GS_MODULATOR_STANDARD_ANNEX_C;
		}
	}
	return lValue;
}

CHAR_T* MULTL_XMLAnalogValueToStr(S32 Value)
{
	switch (Value)
	{
	case GS_MODULATOR_ANALOG_BAND_8M:
		return "8M";
		break;
	case GS_MODULATOR_ANALOG_BAND_6M:
		return "6M";
		break;
	default:
		break;
	}
	return "6M";
}


CHAR_T* MULTL_XMLFecEncodeValueToStr( S32 Value )
{

	switch (Value)
	{
	case GS_MODULATOR_FEC_ENCODE_1_2:
		return "1/2";
		break;

	case GS_MODULATOR_FEC_ENCODE_2_3:
		return "2/3";
		break;

	case GS_MODULATOR_FEC_ENCODE_3_4:
		return "3/4";
		break;

	case GS_MODULATOR_FEC_ENCODE_5_6:
		return "5/6";
		break;

	case GS_MODULATOR_FEC_ENCODE_7_8:
		return "7/8";
		break;

	default:
		break;
	}
	return "1/2";
}

S32 MULTL_XMLFecEncodeValueFromStr(CHAR_T* pString)
{
	S32 lValue = GS_MODULATOR_FEC_ENCODE_1_2;
	if (pString)
	{
		if (GLOBAL_STRCMP(pString, "1/2") == 0)
		{
			lValue = GS_MODULATOR_FEC_ENCODE_1_2;
		}
		else if (GLOBAL_STRCMP(pString, "2/3") == 0)
		{
			lValue = GS_MODULATOR_FEC_ENCODE_2_3;
		}
		else if (GLOBAL_STRCMP(pString, "3/4") == 0)
		{
			lValue = GS_MODULATOR_FEC_ENCODE_3_4;
		}
		else if (GLOBAL_STRCMP(pString, "5/6") == 0)
		{
			lValue = GS_MODULATOR_FEC_ENCODE_5_6;
		}
		else if (GLOBAL_STRCMP(pString, "7/8") == 0)
		{
			lValue = GS_MODULATOR_FEC_ENCODE_7_8;
		}
	}
	return lValue;
}


S32 MULTL_XMLAnalogValueFromStr(CHAR_T* pString)
{
	S32 lValue = GS_MODULATOR_ANALOG_BAND_8M;
	if (pString)
	{
		if (GLOBAL_STRCMP(pString, "8M") == 0)
		{
			lValue = GS_MODULATOR_ANALOG_BAND_8M;
		}
		else if (GLOBAL_STRCMP(pString, "6M") == 0)
		{
			lValue = GS_MODULATOR_ANALOG_BAND_6M;
		}
	}
	return lValue;
}


CHAR_T* MULTL_XMLQAMModeValueToStr(S32 Value)
{
	switch (Value)
	{
	case GS_MODULATOR_QAM_16:
		return "16QAM";
		break;
	case GS_MODULATOR_QAM_32:
		return "32QAM";
		break;
	case GS_MODULATOR_QAM_64:
		return "64QAM";
		break;
	case GS_MODULATOR_QAM_128:
		return "128QAM";
		break;
	case GS_MODULATOR_QAM_256:
		return "256QAM";
		break;
	case GS_MODULATOR_QAM_512:
		return "512QAM";
		break;
	case GS_MODULATOR_QAM_1024:
		return "1024QAM";
		break;
	case GS_MODULATOR_QAM_4:
		return "4QAM";
		break;
	case GS_MODULATOR_QAM_4NR:
		return "4QAM-NR";
		break;
	default:
		break;
	}
	return "UNKNOW";
}

S32 MULTL_XMLQAMModeValueFromStr(CHAR_T* pString)
{
	S32 lValue = GS_MODULATOR_QAM_64;
	if (pString)
	{
		if (GLOBAL_STRCMP(pString, "16QAM") == 0)
		{
			lValue = GS_MODULATOR_QAM_16;
		}
		else if (GLOBAL_STRCMP(pString, "32QAM") == 0)
		{
			lValue = GS_MODULATOR_QAM_32;
		}
		else if (GLOBAL_STRCMP(pString, "64QAM") == 0)
		{
			lValue = GS_MODULATOR_QAM_64;
		}
		else if (GLOBAL_STRCMP(pString, "128QAM") == 0)
		{
			lValue = GS_MODULATOR_QAM_128;
		}
		else if (GLOBAL_STRCMP(pString, "256QAM") == 0)
		{
			lValue = GS_MODULATOR_QAM_256;
		}
		else if (GLOBAL_STRCMP(pString, "512QAM") == 0)
		{
			lValue = GS_MODULATOR_QAM_512;
		}
		else if (GLOBAL_STRCMP(pString, "1024QAM") == 0)
		{
			lValue = GS_MODULATOR_QAM_1024;
		}
		else if (GLOBAL_STRCMP(pString, "4QAM") == 0)
		{
			lValue = GS_MODULATOR_QAM_4;
		}
		else if (GLOBAL_STRCMP(pString, "4QAM-NR") == 0)
		{
			lValue = GS_MODULATOR_QAM_4NR;
		}
	}
	return lValue;
}

/* -DTMB PN----------------------------------------------------------------------------------------------------------------------------------- */

CHAR_T* MULTL_XMLDTMBPNValueToStr(S32 Value)
{
	switch (Value)
	{
	case GS_MODULATOR_GUARD_INTERVAL_PN_420C:
		return "420";
		break;
	case GS_MODULATOR_GUARD_INTERVAL_PN_595:
		return "595";
		break;
	case GS_MODULATOR_GUARD_INTERVAL_PN_945C:
		return "945";
		break;
	case GS_MODULATOR_GUARD_INTERVAL_PN_420F:
		return "420F";
		break;
	case GS_MODULATOR_GUARD_INTERVAL_PN_945F:
		return "945F";
		break;
	default:
		break;
	}
	return "0";
}

S32 MULTL_XMLDTMBPNValueFromStr(CHAR_T* pString)
{
	S32 lValue = GS_MODULATOR_GUARD_INTERVAL_PN_420C;
	if (pString)
	{
		if (GLOBAL_STRCMP(pString, "420") == 0)
		{
			lValue = GS_MODULATOR_GUARD_INTERVAL_PN_420C;
		}
		else if (GLOBAL_STRCMP(pString, "595") == 0)
		{
			lValue = GS_MODULATOR_GUARD_INTERVAL_PN_595;
		}
		else if (GLOBAL_STRCMP(pString, "945") == 0)
		{
			lValue = GS_MODULATOR_GUARD_INTERVAL_PN_945C;
		}
		else if (GLOBAL_STRCMP(pString, "420F") == 0)
		{
			lValue = GS_MODULATOR_GUARD_INTERVAL_PN_420F;
		}
		else if (GLOBAL_STRCMP(pString, "945F") == 0)
		{
			lValue = GS_MODULATOR_GUARD_INTERVAL_PN_945F;
		}
	}
	return lValue;
}

/* -DTMB CodeRate----------------------------------------------------------------------------------------------------------------------------------- */
CHAR_T* MULTL_XMLDTMBCodeRateValueToStr(S32 Value)
{
	switch (Value)
	{
	case GS_MODULATOR_CODE_RATE_0_4:
		return "0.4";
		break;
	case GS_MODULATOR_CODE_RATE_0_6:
		return "0.6";
		break;
	case GS_MODULATOR_CODE_RATE_0_8:
		return "0.8";
		break;
	default:
		break;
	}
	return "0.0";
}

S32 MULTL_XMLDTMBCodeRateValueFromStr(CHAR_T* pString)
{
	S32 lValue = GS_MODULATOR_CODE_RATE_0_4;
	if (pString)
	{
		if (GLOBAL_STRCMP(pString, "0.4") == 0)
		{
			lValue = GS_MODULATOR_CODE_RATE_0_4;
		}
		else if (GLOBAL_STRCMP(pString, "0.6") == 0)
		{
			lValue = GS_MODULATOR_CODE_RATE_0_6;
		}
		else if (GLOBAL_STRCMP(pString, "0.8") == 0)
		{
			lValue = GS_MODULATOR_CODE_RATE_0_8;
		}
	}
	return lValue;
}


CHAR_T* MULTL_XMLDeliveryTypeToStr(S32 Value)
{
	switch (Value)
	{
	case MPEG2_PSI_SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR_TAG:
		return "SATELLITE";
		break;
	case MPEG2_PSI_CABLE_DELIVERY_SYSTEM_DESCRIPTOR_TAG:
		return "CABLE";
		break;
	case MPEG2_PSI_TERRESTRIAL_DELIVERY_SYSTEM_DESCRIPTOR_TAG:
		return "TERRESTRIAL";
		break;
	case MPEG2_PSI_S2_SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR_TAG:
		return "S2";
		break;
	default:
		break;
	}
	return "CABLE";
}

S32 MULTL_XMLDeliveryTypeFromStr(CHAR_T* pString)
{
	S32 lValue = MPEG2_PSI_SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR_TAG;
	if (pString)
	{
		if (GLOBAL_STRCMP(pString, "SATELLITE") == 0)
		{
			lValue = MPEG2_PSI_SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR_TAG;
		}
		else if (GLOBAL_STRCMP(pString, "CABLE") == 0)
		{
			lValue = MPEG2_PSI_CABLE_DELIVERY_SYSTEM_DESCRIPTOR_TAG;
		}
		else if (GLOBAL_STRCMP(pString, "TERRESTRIAL") == 0)
		{
			lValue = MPEG2_PSI_TERRESTRIAL_DELIVERY_SYSTEM_DESCRIPTOR_TAG;
		}
		else if (GLOBAL_STRCMP(pString, "S2") == 0)
		{
			lValue = MPEG2_PSI_S2_SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR_TAG;
		}
	}
	return lValue;
}

CHAR_T* MULTL_XMLCableDeliveryModeValueToStr(S32 Value)
{
	switch (Value)
	{
	case 0:
		return "Not Defined";
	case 1:
		return "16QAM";
		break;
	case 2:
		return "32QAM";
		break;
	case 3:
		return "64QAM";
		break;
	case 4:
		return "128QAM";
		break;
	case 5:
		return "256QAM";
		break;
	case 6:
		return "512QAM";
		break;
	default:
		break;
	}
	return "64QAM";
}


CHAR_T* MULTL_XMLChnTypeValueToStr(S32 Value)
{
	switch (Value)
	{
	case HWL_CHANNEL_TYPE_ASI:
		return MULT_CHANNEL_TYPE_ASI_NAME;
	case HWL_CHANNEL_TYPE_IP:
		return MULT_CHANNEL_TYPE_ETH_NAME;
		break;
	case HWL_CHANNEL_TYPE_IP_LOOP:
		return MULT_CHANNEL_TYPE_ETH_LOOP_NAME;
		break;
	case HWL_CHANNEL_TYPE_IP_DEP:
		return MULT_CHANNEL_TYPE_ETH_DEP_NAME;
		break;
	case HWL_CHANNEL_TYPE_IP_LOOP_DEP:
		return MULT_CHANNEL_TYPE_ETH_LOOP_DEP_NAME;
		break;
	case HWL_CHANNEL_TYPE_DVB_C_MODULATOR:
		return MULT_CHANNEL_TYPE_MODULATOR_NAME;
		break;

	case HWL_CHANNEL_TYPE_DVB_S_MODULATOR:
		return MULT_CHANNEL_TYPE_QPSK_NAME;
		break;
	case HWL_CHANNEL_TYPE_DTMB_MODULATOR:
		return MULT_CHANNEL_TYPE_DTMB_NAME;
		break;

	case HWL_CHANNEL_TYPE_E3DS3:
		return MULT_CHANNEL_TYPE_E3DS3_NAME;
		break;
	case HWL_CHANNEL_TYPE_TUNER_S:
		return MULT_CHANNEL_TYPE_TUNER_S_NAME;
		break;
	case HWL_CHANNEL_TYPE_TUNER_C:
		return MULT_CHANNEL_TYPE_TUNER_C_NAME;
		break;
	case HWL_CHANNEL_TYPE_TUNER_DTMB:
		return MULT_CHANNEL_TYPE_TUNER_DTMB_NAME;
		break;
#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
	case HWL_CHANNEL_TYPE_ENCODER:
		return MULT_CHANNEL_TYPE_ENCODER_NAME;
#endif
	default:
		break;
	}
	return "";
}


S32 MULTL_XMLChnTypeValueFromStr(CHAR_T* pString)
{
	S32 lValue = HWL_CHANNEL_TYPE_INVALID;
	if (pString)
	{
		if (GLOBAL_STRCMP(pString, MULT_CHANNEL_TYPE_ASI_NAME) == 0)
		{
			lValue = HWL_CHANNEL_TYPE_ASI;
		}
#if defined(GN1846) || defined(GN1866) || defined(GN1866B)
		else if (GLOBAL_STRCMP(pString, MULT_CHANNEL_TYPE_ENCODER_NAME) == 0)
		{
			lValue = HWL_CHANNEL_TYPE_ENCODER;
		}
#endif
		else if (GLOBAL_STRCMP(pString, MULT_CHANNEL_TYPE_ETH_NAME) == 0)
		{
			lValue = HWL_CHANNEL_TYPE_IP;
		}
		else if (GLOBAL_STRCMP(pString, MULT_CHANNEL_TYPE_ETH_LOOP_NAME) == 0)
		{
			lValue = HWL_CHANNEL_TYPE_IP_LOOP;
		}
		else if (GLOBAL_STRCMP(pString, MULT_CHANNEL_TYPE_ETH_LOOP_DEP_NAME) == 0)
		{
			lValue = HWL_CHANNEL_TYPE_IP_LOOP_DEP;
		}
		else if (GLOBAL_STRCMP(pString, MULT_CHANNEL_TYPE_ETH_DEP_NAME) == 0)
		{
			lValue = HWL_CHANNEL_TYPE_IP_DEP;
		}
		else if ((GLOBAL_STRCMP(pString, MULT_CHANNEL_TYPE_MODULATOR_NAME) == 0) || (GLOBAL_STRCMP(pString, MULT_CHANNEL_TYPE_MODULATOR_NAME2) == 0))
		{
			lValue = HWL_CHANNEL_TYPE_DVB_C_MODULATOR;
		}
		else if (GLOBAL_STRCMP(pString, MULT_CHANNEL_TYPE_QPSK_NAME) == 0)
		{
			lValue = HWL_CHANNEL_TYPE_DVB_S_MODULATOR;
		}
		else if (GLOBAL_STRCMP(pString, MULT_CHANNEL_TYPE_DTMB_NAME) == 0)
		{
			lValue = HWL_CHANNEL_TYPE_DTMB_MODULATOR;
		}
		else if (GLOBAL_STRCMP(pString, MULT_CHANNEL_TYPE_E3DS3_NAME) == 0)
		{
			lValue = HWL_CHANNEL_TYPE_E3DS3;
		}
		else if (GLOBAL_STRCMP(pString, MULT_CHANNEL_TYPE_TUNER_S_NAME) == 0)
		{
			lValue = HWL_CHANNEL_TYPE_TUNER_S;
		}
		else if (GLOBAL_STRCMP(pString, MULT_CHANNEL_TYPE_TUNER_C_NAME) == 0)
		{
			lValue = HWL_CHANNEL_TYPE_TUNER_C;
		}
		else if (GLOBAL_STRCMP(pString, MULT_CHANNEL_TYPE_TUNER_DTMB_NAME) == 0)
		{
			lValue = HWL_CHANNEL_TYPE_TUNER_DTMB;
		}
	}
	return lValue;
}



CHAR_T* MULTL_XMLChnSubTypeValueToStr(S32 Value)
{
	switch (Value)
	{
	case HWL_CHANNEL_SUBTYPE_ASI:
		return MULT_CHANNEL_SUB_TYPE_TS_OVER_ASI_NAME;
	case HWL_CHANNEL_SUBTYPE_IP:
		return MULT_CHANNEL_SUB_TYPE_TS_OVER_UDP_NAME;
		break;
	case HWL_CHANNEL_SUBTYPE_MODULATOR_AD9789:
		return MULT_MODULATOR_SUB_TYPE_AD9789_NAME;
		break;
	case HWL_CHANNEL_SUBTYPE_MODULATOR_BL85KMM:
		return MULT_MODULATOR_SUB_TYPE_BL85KMM_NAME;
		break;
	case HWL_CHANNEL_SUBTYPE_E3DS3:
		return MULT_CHANNEL_SBU_TYPE_E3DS3_NAME;
		break;
	case HWL_CHANNEL_SUBTYPE_TUNER_S_AVL6211:
		return MULT_CHANNEL_SBU_TYPE_TUNER_S_NAME;
		break;
	case HWL_CHANNEL_SUBTYPE_TUNER_C:
		return MULT_CHANNEL_SBU_TYPE_TUNER_C_NAME;
		break;
	case HWL_CHANNEL_SUBTYPE_TUNER_DTMB_ATBM8869:
		return MULT_CHANNEL_SBU_TYPE_TUNER_DTMB_NAME;
		break;
#if defined(GN1846)
	case HWL_CHANNEL_SUBTYPE_ENCODER_HI3531A:
		return MULT_CHANNEL_SUB_TYPE_ENCODER_HI3531A;
#endif
#if defined(GN1866B) || defined(GN1866)
	case HWL_CHANNEL_SUBTYPE_ENCODER_HI3519:
		return MULT_CHANNEL_SUB_TYPE_ENCODER_HI3519;
#endif

	default:
		break;
	}
	return "";
}


S32 MULTL_XMLChnSubTypeValueFromStr(CHAR_T* pString)
{
	S32 lValue = HWL_CHANNEL_SUBTYPE_INVALID;
	if (pString)
	{
		if (GLOBAL_STRCMP(pString, MULT_CHANNEL_SUB_TYPE_TS_OVER_ASI_NAME) == 0)
		{
			lValue = HWL_CHANNEL_SUBTYPE_ASI;
		}
		else if (GLOBAL_STRCMP(pString, MULT_CHANNEL_SUB_TYPE_TS_OVER_UDP_NAME) == 0)
		{
			lValue = HWL_CHANNEL_SUBTYPE_IP;
		}
		else if (GLOBAL_STRCMP(pString, MULT_MODULATOR_SUB_TYPE_AD9789_NAME) == 0)
		{
			lValue = HWL_CHANNEL_SUBTYPE_MODULATOR_AD9789;
		}
		else if (GLOBAL_STRCMP(pString, MULT_MODULATOR_SUB_TYPE_BL85KMM_NAME) == 0)
		{
			lValue = HWL_CHANNEL_SUBTYPE_MODULATOR_BL85KMM;
		}
		else if (GLOBAL_STRCMP(pString, MULT_CHANNEL_SBU_TYPE_E3DS3_NAME) == 0)
		{
			lValue = HWL_CHANNEL_SUBTYPE_E3DS3;
		}
		else if (GLOBAL_STRCMP(pString, MULT_CHANNEL_SBU_TYPE_TUNER_S_NAME) == 0)
		{
			lValue = HWL_CHANNEL_SUBTYPE_TUNER_S_AVL6211;
		}
		else if (GLOBAL_STRCMP(pString, MULT_CHANNEL_SBU_TYPE_TUNER_C_NAME) == 0)
		{
			lValue = HWL_CHANNEL_SUBTYPE_TUNER_C;
		}
		else if (GLOBAL_STRCMP(pString, MULT_CHANNEL_SBU_TYPE_TUNER_DTMB_NAME) == 0)
		{
			lValue = HWL_CHANNEL_SUBTYPE_TUNER_DTMB_ATBM8869;
		}
#if defined(GN1846)
		else if (GLOBAL_STRCMP(pString, MULT_CHANNEL_SUB_TYPE_ENCODER_HI3531A) == 0)
		{
			lValue = HWL_CHANNEL_SUBTYPE_ENCODER_HI3531A;
		}
#endif
#if defined(GN1866B) || defined(GN1866)
		else if (GLOBAL_STRCMP(pString, MULT_CHANNEL_SUB_TYPE_ENCODER_HI3519) == 0)
		{
			lValue = HWL_CHANNEL_SUBTYPE_ENCODER_HI3519;
		}
#endif
	}
	return lValue;
}


S32 MULTL_XMLCableDeliveryModeValueFromStr(CHAR_T* pString)
{
	S32 lValue = 3;
	if (pString)
	{
		if (GLOBAL_STRCMP(pString, "Not Defined") == 0)
		{
			lValue = 0;
		}
		if (GLOBAL_STRCMP(pString, "16QAM") == 0)
		{
			lValue = 1;
		}
		else if (GLOBAL_STRCMP(pString, "32QAM") == 0)
		{
			lValue = 2;
		}
		else if (GLOBAL_STRCMP(pString, "64QAM") == 0)
		{
			lValue = 3;
		}
		else if (GLOBAL_STRCMP(pString, "128QAM") == 0)
		{
			lValue = 4;
		}
		else if (GLOBAL_STRCMP(pString, "256QAM") == 0)
		{
			lValue = 5;
		}
	}
	return lValue;
}


CHAR_T *MULTL_XMLGetNodeText(mxml_node_t *pParent, CHAR_T* pTagname)
{
	mxml_node_t *plTmpNode;
	char *opaque;
	plTmpNode = mxmlFindElement(pParent, pParent, pTagname, NULL, NULL, MXML_DESCEND_FIRST);
	if (plTmpNode)
	{
		if (plTmpNode->child)
		{
			opaque = plTmpNode->child[0].value.opaque;
			if (GLOBAL_MEMCMP(opaque, "![CDATA[", GLOBAL_STRLEN("![CDATA[")) == 0)
			{
				opaque += GLOBAL_STRLEN("![CDATA[");
				opaque[GLOBAL_STRLEN(opaque) - 2] = 0;
			}
			return opaque;
		}
	}
	return NULL;
}

S32 MULTL_XMLGetNodeINT(mxml_node_t *pParent, CHAR_T* pTagname, S32 Radix)
{
	mxml_node_t *plTmpNode;
	plTmpNode = mxmlFindElement(pParent, pParent, pTagname, NULL, NULL, MXML_DESCEND_FIRST);
	if (plTmpNode)
	{
		if (plTmpNode->child)
		{
			if (plTmpNode->child[0].value.opaque)
			{
				return GLOBAL_STRTOL(plTmpNode->child[0].value.opaque, NULL, Radix);
			}
		}
	}
	return 0;
}

S32 MULTL_XMLGetNodeINTDefault(mxml_node_t *pParent, CHAR_T* pTagname, S32 Radix, S32 Default)
{
	mxml_node_t *plTmpNode;
	plTmpNode = mxmlFindElement(pParent, pParent, pTagname, NULL, NULL, MXML_DESCEND_FIRST);
	if (plTmpNode)
	{
		if (plTmpNode->child)
		{
			if (plTmpNode->child[0].value.opaque)
			{
				return GLOBAL_STRTOL(plTmpNode->child[0].value.opaque, NULL, Radix);
			}
		}
	}
	return Default;
}

U32 MULTL_XMLGetNodeUINT(mxml_node_t *pParent, CHAR_T* pTagname, S32 Radix)
{
	mxml_node_t *plTmpNode;
	plTmpNode = mxmlFindElement(pParent, pParent, pTagname, NULL, NULL, MXML_DESCEND_FIRST);
	if (plTmpNode)
	{
		if (plTmpNode->child)
		{
			if (plTmpNode->child[0].value.opaque)
			{
				return GLOBAL_STRTOUL(plTmpNode->child[0].value.opaque, NULL, Radix);
			}
		}
	}
	return 0;
}


F64 MULTL_XMLGetNodeFLOATDefault(mxml_node_t *pParent, CHAR_T* pTagname, F64 Default)
{
	mxml_node_t *plTmpNode;
	plTmpNode = mxmlFindElement(pParent, pParent, pTagname, NULL, NULL, MXML_DESCEND_FIRST);
	if (plTmpNode)
	{
		if (plTmpNode->child)
		{
			if (plTmpNode->child[0].value.opaque)
			{
				return GLOBAL_ATOF(plTmpNode->child[0].value.opaque);
			}
		}
	}
	return Default;
}

U32 MULTL_XMLGetNodeUINTDefault(mxml_node_t *pParent, CHAR_T* pTagname, S32 Radix, U32 Default)
{
	mxml_node_t *plTmpNode;
	plTmpNode = mxmlFindElement(pParent, pParent, pTagname, NULL, NULL, MXML_DESCEND_FIRST);
	if (plTmpNode)
	{
		if (plTmpNode->child)
		{
			if (plTmpNode->child[0].value.opaque)
			{
				return GLOBAL_STRTOUL(plTmpNode->child[0].value.opaque, NULL, Radix);
			}
		}
	}
	return Default;
}


BOOL MULTL_XMLGetNodeBOOL(mxml_node_t *pParent, CHAR_T* pTagname)
{
	mxml_node_t *plTmpNode;
	plTmpNode = mxmlFindElement(pParent, pParent, pTagname, NULL, NULL, MXML_DESCEND_FIRST);
	if (plTmpNode)
	{
		if (plTmpNode->child)
		{
			if (plTmpNode->child[0].value.opaque)
			{
				return (GLOBAL_STRTOL(plTmpNode->child[0].value.opaque, NULL, 10) > 0)?TRUE:FALSE;
			}
		}
	}
	return 0;
}



BOOL MULTL_XMLGetNodeMark(mxml_node_t *pParent, CHAR_T* pTagname)
{
	mxml_node_t *plTmpNode;
	plTmpNode = mxmlFindElement(pParent, pParent, pTagname, NULL, NULL, MXML_DESCEND_FIRST);
	if (plTmpNode)
	{
		if (plTmpNode->child)
		{
			return MULTL_XMLMarkValueFromStr(plTmpNode->child[0].value.opaque);
		}
	}
	return FALSE;
}

BOOL MULTL_XMLGetNodeMarkDefault(mxml_node_t *pParent, CHAR_T* pTagname, BOOL Default)
{
	mxml_node_t *plTmpNode;
	plTmpNode = mxmlFindElement(pParent, pParent, pTagname, NULL, NULL, MXML_DESCEND_FIRST);
	if (plTmpNode)
	{
		if (plTmpNode->child)
		{
			return MULTL_XMLMarkValueFromStr(plTmpNode->child[0].value.opaque);
		}
	}
	return Default;
}

U32 MULTL_XMLGetNodeIP(mxml_node_t *pParent, CHAR_T* pTagname)
{
	U32 lValue = 0;
	CHAR_T *plTmpStr;
	plTmpStr = MULTL_XMLGetNodeText(pParent, pTagname);
	if (plTmpStr)
	{
		lValue = PFC_SocketAToN(plTmpStr);
	}
	return lValue;
}



void MULTL_XMLAddNodeCDData(mxml_node_t *pParent, CHAR_T* pTagname, CHAR_T *pValue)
{
	mxmlNewCDATA(mxmlNewElement(pParent, pTagname), pValue);
}

void MULTL_XMLAddNodeText(mxml_node_t *pParent, CHAR_T* pTagname, CHAR_T *pValue)
{
	mxmlNewText(mxmlNewElement(pParent, pTagname), 0, pValue);
}

void MULTL_XMLAddNodeINT(mxml_node_t *pParent, CHAR_T* pTagname, S32 Value)
{
	mxmlNewTextf(mxmlNewElement(pParent, pTagname), 0, "%d", Value);
}

void MULTL_XMLAddNodeUINT(mxml_node_t *pParent, CHAR_T* pTagname, U32 Value)
{
	mxmlNewTextf(mxmlNewElement(pParent, pTagname), 0, "%u", Value);
}

void MULTL_XMLAddNodeFLOAT(mxml_node_t *pParent, CHAR_T* pTagname, F64 Value)
{
	mxmlNewTextf(mxmlNewElement(pParent, pTagname), 0, "%f", Value);
}

void MULTL_XMLAddNodeFLOATE(mxml_node_t *pParent, CHAR_T* pTagname, F64 Value)
{
	mxmlNewTextf(mxmlNewElement(pParent, pTagname), 0, "%E", Value);
}

void MULTL_XMLAddNodeHEX(mxml_node_t *pParent, CHAR_T* pTagname, U32 Value)
{
	mxmlNewTextf(mxmlNewElement(pParent, pTagname), 0, "%x", Value);
}

void MULTL_XMLAddNodeBOOL(mxml_node_t *pParent, CHAR_T* pTagname, BOOL Value)
{
	mxmlNewTextf(mxmlNewElement(pParent, pTagname), 0, "%d", ((Value > 0)?TRUE:FALSE));
}

void MULTL_XMLAddNodeMark(mxml_node_t *pParent, CHAR_T* pTagname, BOOL Value)
{
	mxmlNewText(mxmlNewElement(pParent, pTagname), 0, MULTL_XMLMarkValueToStr(Value));
}









void MULTL_GenerateMAC(U8 DeviceType, U8 DeviceSubType, S32 Index, U32 ChipIDSeries, U8 *pMAC, S32 BufSize)
{
	if (BufSize >= GLOBAL_MAC_BUF_SIZE)
	{
		U8 *plTmp ;
		pMAC[0] = 0x00;
		pMAC[1] = 0x5C;
		pMAC[2] = 0xB1;//??????
		pMAC[3] = Index & 0xFF;
		plTmp = &pMAC[4];
		GLOBAL_MSB16_E(plTmp, ChipIDSeries);
	}
}

S32 MULTL_GenerateSN(U8 DeviceType, U8 DeviceSubType, U32 ChipID, U8 *pSN, S32 BufSize)
{
	if (BufSize >= MULT_SN_STRING_BUF_LEN)
	{
		U8 *plTmp, plBinBuf[4];

		pSN[0] = 'T';
		pSN[1] = 'C';
		CAL_StringBinToHex(&DeviceType, 1, &pSN[2], 2, TRUE);
		pSN[4] = 'A';
		CAL_StringBinToHex(&DeviceSubType, 1, &pSN[5], 2, TRUE);
		pSN[7] = 'F';
		pSN[8] = 'F';
		pSN[9] = 'F';
		plTmp = &plBinBuf[0];
		GLOBAL_MSB32_E(plTmp, ChipID);
		CAL_StringBinToHex(plBinBuf, 4, &pSN[10], 8, TRUE);

		return MULT_SN_STRING_LEN;
	}
	return 0;
}

void MULTL_CreateIDs(MULT_IDsArray *pArray, S32 MaxNum)
{
	pArray->m_pIDArray = (MULT_IDs *)GLOBAL_ZMALLOC(sizeof(MULT_IDsArray) * MaxNum);
	pArray->m_CurNum = 0;
	pArray->m_MaxNum = MaxNum;
}


void MULTL_AddIDs(MULT_IDsArray *pArray, MULT_IDs *pACIDs)
{
	if (pArray->m_CurNum < pArray->m_MaxNum)
	{
		GLOBAL_MEMCPY(&pArray->m_pIDArray[pArray->m_CurNum], pACIDs, sizeof(MULT_IDs));
		pArray->m_CurNum++;
	}
}

U32 MULTL_GetNewIDs(MULT_IDsArray *pArray, U32 OriIDs)
{
	S32 i;
	for (i = 0; i < pArray->m_CurNum; i++)
	{
		if (pArray->m_pIDArray[i].m_ORIIDs == OriIDs)
		{
			return pArray->m_pIDArray[i].m_NewIDs;
		}
	}
	return 0;
}

void MULTL_DestroyIDs(MULT_IDsArray *pArray)
{
	GLOBAL_SAFEFREE(pArray->m_pIDArray);
}



void MULTL_XMLSaveDescriptors(MULT_Handle *pHandle, mxml_node_t *pXMLParent, U8 DescriptorType, U32 ParentIDs)
{
	U32 lItemIDs;
	CHAR_T plHexBuf[10240];
	S32 lHexLen;
	MPEG2_DBDescriptorInfo lDescInfo;
	mxml_node_t *plXMLData, *plXMLLoopNode;

	lItemIDs = 0;
	while((lItemIDs = MPEG2_DBGetDescriptorNextNode(pHandle->m_DBSHandle, lItemIDs, DescriptorType, ParentIDs)) != 0)
	{
		if (MPEG2_DBGetDescriptorInfo(pHandle->m_DBSHandle, lItemIDs, &lDescInfo) == TRUE)
		{
			plXMLLoopNode = mxmlNewElement(pXMLParent, "descriptor");

			plXMLData = mxmlNewElement(plXMLLoopNode, "desc_ids");
			mxmlNewTextf(plXMLData, 0, "%d", lItemIDs);

			plXMLData = mxmlNewElement(plXMLLoopNode, "data");
			lHexLen = CAL_StringBinToHex(lDescInfo.m_pDescriptorData, lDescInfo.m_DescriptorDataSize, plHexBuf, sizeof(plHexBuf), TRUE);
			plHexBuf[lHexLen] = 0;
			mxmlNewText(plXMLData, 0, plHexBuf);

			plXMLData = mxmlNewElement(plXMLLoopNode, "out_mark");
			mxmlNewText(plXMLData, 0, MULTL_XMLMarkValueToStr(lDescInfo.m_OutputMark));
		}
		else
		{
			GLOBAL_ASSERT(0);
		}
	}
}


void MULTL_XMLLoadDescriptors(MULT_Handle *pHandle, mxml_node_t *pXMLParent, U8 DescriptorType, U32 ParentIDs)
{
	CHAR_T *plTmpBuf;
	MPEG2_DBDescriptorInfo lDescInfo;
	mxml_node_t *plXMLLoopNode;

	GLOBAL_ZEROMEM(&lDescInfo, sizeof(lDescInfo));
	plXMLLoopNode = mxmlFindElement(pXMLParent, pXMLParent, "descriptor", NULL, NULL, MXML_DESCEND_FIRST);
	while(plXMLLoopNode != NULL)
	{
		plTmpBuf = MULTL_XMLGetNodeText(plXMLLoopNode, "data");
		lDescInfo.m_DescriptorDataSize = CAL_StringHexToBin(plTmpBuf, lDescInfo.m_pDescriptorData, sizeof(lDescInfo.m_pDescriptorData));
		if (lDescInfo.m_DescriptorDataSize > 0)
		{
			lDescInfo.m_OutputMark = MULTL_XMLGetNodeMark(plXMLLoopNode, "out_mark");
			MPEG2_DBAddDescriptor(pHandle->m_DBSHandle, ParentIDs, DescriptorType, &lDescInfo);
		}
		plXMLLoopNode = mxmlFindElement(plXMLLoopNode, pXMLParent, "descriptor", NULL, NULL, MXML_NO_DESCEND);
	}
}



void MULTL_XMLSaveMuxCAs(MULT_Handle *pHandle, mxml_node_t *pXMLParent, U8 CAType, U32 ParentIDs)
{
	U32 lItemIDs;
	MPEG2_DBCaInInfo lCAInInfo;
	MPEG2_DBCaOutInfo lCAOutInfo;
	mxml_node_t *plXMLData, *plXMLLoopNode, *plInfoNode;

	lItemIDs = 0;
	while((lItemIDs = MPEG2_DBGetMuxCANextNode(pHandle->m_DBSHandle, lItemIDs, CAType, ParentIDs)) != 0)
	{
		//GLOBAL_TRACE(("Mux CA ID = 0x%.8X, For ParentID = 0x%.8X, Type = %d\n", lItemIDs, ParentIDs, CAType));
		if ( MPEG2_DBGetMuxCAInInfo(pHandle->m_DBSHandle, lItemIDs, &lCAInInfo) == TRUE)
		{
			MPEG2_DBGetMuxCAOutInfo(pHandle->m_DBSHandle, lItemIDs, &lCAOutInfo);
			plXMLLoopNode = mxmlNewElement(pXMLParent, "mux_ca");

			plXMLData = mxmlNewElement(plXMLLoopNode, "mux_ca_ids");
			mxmlNewTextf(plXMLData, 0, "%d", lItemIDs);

			plInfoNode = mxmlNewElement(plXMLLoopNode, "in_ca_info");
			plXMLData = mxmlNewElement(plInfoNode, "cas_id");
			mxmlNewTextf(plXMLData, 0, "%d", lCAInInfo.m_CaSystemID);
			plXMLData = mxmlNewElement(plInfoNode, "ca_pid");
			mxmlNewTextf(plXMLData, 0, "%d", lCAInInfo.m_CaPID);

			plInfoNode = mxmlNewElement(plXMLLoopNode, "out_ca_info");
			plXMLData = mxmlNewElement(plInfoNode, "ca_pid");
			mxmlNewTextf(plXMLData, 0, "%d", lCAOutInfo.m_CaPID);
			plXMLData = mxmlNewElement(plInfoNode, "out_ts_index");
			mxmlNewTextf(plXMLData, 0, "%d", lCAOutInfo.m_SPInfo.m_OutTsIndex);
			plXMLData = mxmlNewElement(plInfoNode, "out_mark");
			mxmlNewText(plXMLData, 0, MULTL_XMLMarkValueToStr(lCAOutInfo.m_OutputMark));


			//???MUX CA??????????
			{
				S32 lHexLen;
				CHAR_T plHexBuf[1024];
				MPEG2_DBCaPrivateInfo lCAPrivateInfo;
				if (MPEG2_DBMuxCAPrivateInfoProc(pHandle->m_DBSHandle, lItemIDs, &lCAPrivateInfo, TRUE))
				{
					plInfoNode = mxmlNewElement(plXMLLoopNode, "ca_private_info");
					lHexLen = CAL_StringBinToHex(lCAPrivateInfo.m_pPrivateData, lCAPrivateInfo.m_PrivateDataLen, plHexBuf, sizeof(plHexBuf), TRUE);
					plHexBuf[lHexLen] = 0;
					MULTL_XMLAddNodeText(plInfoNode, "data", plHexBuf);
					MULTL_XMLAddNodeMark(plInfoNode, "out_mark", lCAPrivateInfo.m_OutputMark);
				}
			}


			/*PID?????? XJ 20130807*/
			MULTL_XMLAddNodeMark(plInfoNode, "ps_m", lCAOutInfo.m_PIDShareMark);//?ò???????????

		}
		else
		{
			GLOBAL_ASSERT(0);
		}
	}
}

void MULTL_XMLLoadMuxCAs(MULT_Handle *pHandle, mxml_node_t *pXMLParent, U8 CAType, U32 ParentIDs)
{
	U32 lItemIDs;
	MPEG2_DBCaInInfo lCAInInfo;
	MPEG2_DBCaOutInfo lCAOutInfo;
	mxml_node_t *plXMLLoopNode, *plInfoNode;

	GLOBAL_ZEROMEM(&lCAInInfo, sizeof(lCAInInfo));
	GLOBAL_ZEROMEM(&lCAOutInfo, sizeof(lCAOutInfo));
	plXMLLoopNode = mxmlFindElement(pXMLParent, pXMLParent, "mux_ca", NULL, NULL, MXML_DESCEND_FIRST);
	while(plXMLLoopNode != NULL)
	{
		lItemIDs = MPEG2_DBAddMuxCA(pHandle->m_DBSHandle, CAType, ParentIDs);
		if (lItemIDs > 0)
		{
			plInfoNode = mxmlFindElement(plXMLLoopNode, plXMLLoopNode, "in_ca_info", NULL, NULL, MXML_DESCEND_FIRST);
			if (plInfoNode)
			{
				MPEG2_DBGetMuxCAInInfo(pHandle->m_DBSHandle, lItemIDs, &lCAInInfo);
				lCAInInfo.m_CaSystemID = MULTL_XMLGetNodeUINT(plInfoNode, "cas_id", 10);
				lCAInInfo.m_CaPID = MULTL_XMLGetNodeUINT(plInfoNode, "ca_pid", 10);
				MPEG2_DBSetMuxCAInInfo(pHandle->m_DBSHandle, lItemIDs, &lCAInInfo);
			}

			plInfoNode = mxmlFindElement(plXMLLoopNode, plXMLLoopNode, "out_ca_info", NULL, NULL, MXML_DESCEND_FIRST);
			if (plInfoNode)
			{
				MPEG2_DBGetMuxCAOutInfo(pHandle->m_DBSHandle, lItemIDs, &lCAOutInfo);
				lCAOutInfo.m_CaPID = MULTL_XMLGetNodeUINT(plInfoNode, "ca_pid", 10);
				lCAOutInfo.m_SPInfo.m_OutTsIndex = MULTL_XMLGetNodeINT(plInfoNode, "out_ts_index", 10);
				lCAOutInfo.m_OutputMark = MULTL_XMLGetNodeMark(plInfoNode, "out_mark");
				MPEG2_DBSetMuxCAOutInfo(pHandle->m_DBSHandle, lItemIDs, &lCAOutInfo);
			}

			//???MUX CA??????????
			plInfoNode = mxmlFindElement(plXMLLoopNode, plXMLLoopNode, "ca_private_info", NULL, NULL, MXML_DESCEND_FIRST);
			if (plInfoNode)
			{
				MPEG2_DBCaPrivateInfo lCAPrivateInfo;
				if (MPEG2_DBMuxCAPrivateInfoProc(pHandle->m_DBSHandle, lItemIDs, &lCAPrivateInfo, TRUE))
				{
					lCAPrivateInfo.m_PrivateDataLen = CAL_StringHexToBin(MULTL_XMLGetNodeText(plInfoNode, "data"), lCAPrivateInfo.m_pPrivateData, sizeof(lCAPrivateInfo.m_pPrivateData));
					lCAPrivateInfo.m_OutputMark = MULTL_XMLGetNodeMark(plInfoNode, "out_mark");
					MPEG2_DBMuxCAPrivateInfoProc(pHandle->m_DBSHandle, lItemIDs, &lCAPrivateInfo, FALSE);
				}
			}
		}
		plXMLLoopNode = mxmlFindElement(plXMLLoopNode, pXMLParent, "mux_ca", NULL, NULL, MXML_NO_DESCEND);
	}

}


void MULTL_XMLSaveSCSCAs(MULT_Handle *pHandle, mxml_node_t *pXMLParent, U8 CAType, U32 ParentIDs)
{
	S32 i, lSCSCANum;
	U32 lItemIDs;
	MPEG2_DBSCSCAInfo lSCSCAInfo;
	mxml_node_t *plXMLData, *plXMLLoopNode;

	lSCSCANum = MPEG2_DBGetSCSCount(pHandle->m_DBSHandle);//20120919 ????BUG
	lItemIDs = 0;
	while((lItemIDs = MPEG2_DBGetSCSCANextNode(pHandle->m_DBSHandle, lItemIDs, CAType, ParentIDs)) != 0)
	{
		if (MPEG2_DBGetSCSCAInfo(pHandle->m_DBSHandle, lItemIDs, &lSCSCAInfo) == TRUE)
		{
			plXMLData = mxmlNewElement(pXMLParent, "scs_ca_ids");
			mxmlNewTextf(plXMLData, 0, "%d", lItemIDs);
			for (i = 0; i < lSCSCANum; i++)
			{
				plXMLLoopNode = mxmlNewElement(pXMLParent, "scs_ca_info");

				plXMLData = mxmlNewElement(plXMLLoopNode, "ca_pid");
				mxmlNewTextf(plXMLData, 0, "%d", lSCSCAInfo.m_pOutputCaInfo[i].m_CaPID);
				plXMLData = mxmlNewElement(plXMLLoopNode, "ac_ids");
				mxmlNewTextf(plXMLData, 0, "%.8X", lSCSCAInfo.m_pOutputCaInfo[i].m_SPInfo.m_ACIDs);
				plXMLData = mxmlNewElement(plXMLLoopNode, "out_mark");
				mxmlNewText(plXMLData, 0, MULTL_XMLMarkValueToStr(lSCSCAInfo.m_pOutputCaInfo[i].m_OutputMark));


				//XUEJING???????????????CA_PRIVATE_INFO???????2016-4-6
				{
					CHAR_T plStrBuf[256 * 2 + 1];
					S32 lStrLen;
					lStrLen = CAL_StringBinToHex(lSCSCAInfo.m_pCAPrivateData[i].m_pPrivateData, lSCSCAInfo.m_pCAPrivateData[i].m_PrivateDataLen, plStrBuf, sizeof(plStrBuf), TRUE);
					plStrBuf[lStrLen] = 0;
					MULTL_XMLAddNodeCDData(plXMLLoopNode, "ca_private_data", plStrBuf);
					MULTL_XMLAddNodeMark(plXMLLoopNode, "ca_private_mark", lSCSCAInfo.m_pCAPrivateData[i].m_OutputMark);
				}

			}

		}
		else
		{
			GLOBAL_ASSERT(0);
		}
	}
}





void MULTL_XMLLoadSCSCAs(MULT_Handle *pHandle, mxml_node_t *pXMLParent, U8 CAType, U32 ParentIDs, MULT_IDsArray *pACArray)
{
	S32 lSCSCAIndex, lSCSCANum;
	U32 lItemIDs;
	MPEG2_DBSCSCAInfo lSCSCAInfo;
	mxml_node_t *plXMLLoopNode;

	lSCSCANum = MPEG2_DBGetSCSCount(pHandle->m_DBSHandle);//20120919 ????BUG

	GLOBAL_ZEROMEM(&lSCSCAInfo, sizeof(lSCSCAInfo));

	lSCSCAIndex = 0;
	plXMLLoopNode = mxmlFindElement(pXMLParent, pXMLParent, "scs_ca_info", NULL, NULL, MXML_DESCEND_FIRST);
	while(plXMLLoopNode != NULL)
	{
		lSCSCAIndex++;
		plXMLLoopNode = mxmlFindElement(plXMLLoopNode, pXMLParent, "scs_ca_info", NULL, NULL, MXML_NO_DESCEND);
	}

	if (lSCSCAIndex > 0)
	{
// 		lItemIDs = MPEG2_DBAddSCSCA(pHandle->m_DBSHandle, CAType, ParentIDs);
		lItemIDs = MPEG2_DBGetSCSCANextNode(pHandle->m_DBSHandle, 0, CAType, ParentIDs);
		if (lItemIDs > 0)
		{
			if (MPEG2_DBGetSCSCAInfo(pHandle->m_DBSHandle, lItemIDs, &lSCSCAInfo))
			{
				lSCSCAIndex = 0;
				plXMLLoopNode = mxmlFindElement(pXMLParent, pXMLParent, "scs_ca_info", NULL, NULL, MXML_DESCEND_FIRST);
				while(plXMLLoopNode != NULL)
				{
					if (lSCSCAIndex < lSCSCANum)
					{
						lSCSCAInfo.m_pOutputCaInfo[lSCSCAIndex].m_CaPID = MULTL_XMLGetNodeUINT(plXMLLoopNode, "ca_pid", 10);
						lSCSCAInfo.m_pOutputCaInfo[lSCSCAIndex].m_SPInfo.m_ACIDs = MULTL_GetNewIDs(pACArray, MULTL_XMLGetNodeUINT(plXMLLoopNode, "ac_ids", 16));
						lSCSCAInfo.m_pOutputCaInfo[lSCSCAIndex].m_OutputMark = MULTL_XMLGetNodeMark(plXMLLoopNode, "out_mark");

						//XUEJING???????????????CA_PRIVATE_INFO???????2016-4-6
						{
							lSCSCAInfo.m_pCAPrivateData[lSCSCAIndex].m_PrivateDataLen = CAL_StringHexToBin(MULTL_XMLGetNodeText(plXMLLoopNode, "ca_private_data"), lSCSCAInfo.m_pCAPrivateData[lSCSCAIndex].m_pPrivateData, sizeof(lSCSCAInfo.m_pCAPrivateData[lSCSCAIndex].m_pPrivateData));
							lSCSCAInfo.m_pCAPrivateData[lSCSCAIndex].m_OutputMark = MULTL_XMLGetNodeMark(plXMLLoopNode, "ca_private_mark");
						}

						lSCSCAIndex++;
					}
					plXMLLoopNode = mxmlFindElement(plXMLLoopNode, pXMLParent, "scs_ca_info", NULL, NULL, MXML_NO_DESCEND);
				}
				MPEG2_DBSetSCSCAInfo(pHandle->m_DBSHandle, lItemIDs, &lSCSCAInfo);
			}
		}
	}
}

void MULTL_XMLSaveMonitorLimitInfo(MULT_Handle *pHandle, mxml_node_t *pXMLParent, MULT_MonitorLimitInfo *pInfo)
{
	mxml_node_t *plXMLHolder;

	plXMLHolder = mxmlNewElement(pXMLParent, "limit");

	MULTL_XMLAddNodeINT(plXMLHolder, "low", pInfo->m_LowLimit);
	MULTL_XMLAddNodeINT(plXMLHolder, "high", pInfo->m_HighLimit);
	MULTL_XMLAddNodeMark(plXMLHolder, "mark", pInfo->m_Mark);
}

void MULTL_XMLLoadMonitorLimitInfo(MULT_Handle *pHandle, mxml_node_t *pXMLParent, MULT_MonitorLimitInfo *pInfo)
{
	mxml_node_t *plXMLHolder;

	plXMLHolder = mxmlFindElement(pXMLParent, pXMLParent, "limit", NULL, NULL, MXML_DESCEND_FIRST);
	pInfo->m_HighLimit = MULTL_XMLGetNodeINT(plXMLHolder, "high", 10);
	pInfo->m_LowLimit = MULTL_XMLGetNodeINT(plXMLHolder, "low", 10);
	pInfo->m_Mark = MULTL_XMLGetNodeMark(plXMLHolder, "mark");
}



void MULTL_LoadChannelInfoXML(MULT_Handle *pHandle, MULT_ChannelNode *pChnNode, mxml_node_t *pXMLParent)
{
	switch (pChnNode->m_ChannelType)
	{
	case HWL_CHANNEL_TYPE_ASI:
		{
		}
		break;
	case HWL_CHANNEL_TYPE_TUNER_S:
		{
		}
		break;
	case HWL_CHANNEL_TYPE_TUNER_C:
		{
		}
		break;
	case HWL_CHANNEL_TYPE_TUNER_DTMB:
		{
		}
		break;
	case HWL_CHANNEL_TYPE_E3DS3:
		{
		}
		break;
	case HWL_CHANNEL_TYPE_IP:
		{
			pChnNode->m_ChannelInfo.m_IPInfo.m_IPAddress = PFC_SocketAToN(MULTL_XMLGetNodeText(pXMLParent, "ipv4_address"));
			pChnNode->m_ChannelInfo.m_IPInfo.m_IPMask = PFC_SocketAToN(MULTL_XMLGetNodeText(pXMLParent, "ipv4_mask"));
			pChnNode->m_ChannelInfo.m_IPInfo.m_IPGate = PFC_SocketAToN(MULTL_XMLGetNodeText(pXMLParent, "ipv4_gate"));
			pChnNode->m_ChannelInfo.m_IPInfo.m_IPMark = MULTL_XMLGetNodeMarkDefault(pXMLParent, "eth_mark", TRUE);
			CAL_StringMACToBin(MULTL_XMLGetNodeText(pXMLParent, "ip_mac"), pChnNode->m_ChannelInfo.m_IPInfo.m_MAC, sizeof(pChnNode->m_ChannelInfo.m_IPInfo.m_MAC));
		}
		break;
	case HWL_CHANNEL_TYPE_IP_DEP:
		{
		}
		break;
	case HWL_CHANNEL_TYPE_DVB_C_MODULATOR:
	case HWL_CHANNEL_TYPE_DVB_S_MODULATOR:
	case HWL_CHANNEL_TYPE_DTMB_MODULATOR:
		{
			GLOBAL_TRACE(( " Load QPSK/DVB-C/DTMB Channel Param! Type = %d\n", pChnNode->m_ChannelType));
			pChnNode->m_ChannelInfo.m_ModulatorInfo.m_ExAttLevel = MULTL_XMLGetNodeINT(pXMLParent, "ex_att_level", 10);
#ifdef SUPPORT_ALC
			pChnNode->m_ChannelInfo.m_ModulatorInfo.m_ALCMark = MULTL_XMLGetNodeMarkDefault(pXMLParent, "alc_mark", FALSE);
#endif
#ifdef SUPPORT_MODULATOR_FREQ_1_HZ
			pChnNode->m_ChannelInfo.m_ModulatorInfo.m_FreqAdj = MULTL_XMLGetNodeINT(pXMLParent, "freq_adj", 10);
			GLOBAL_TRACE(("FreADJ = %d\n", pChnNode->m_ChannelInfo.m_ModulatorInfo.m_FreqAdj));
#else
			pChnNode->m_ChannelInfo.m_ModulatorInfo.m_FreqAdj = 0;
#endif
		}
		break;

	default:
		break;
	}
}

void MULTL_LoadSubChannelInfoXML(MULT_Handle *pHandle, MULT_ChannelNode *pChnNode, MULT_SubChannelNode *pSubNode, mxml_node_t *pXMLParent)
{
	mxml_node_t *plXMLSubNode;

	pSubNode->m_ActiveMark = MULTL_XMLGetNodeMark(pXMLParent, "active_mark");

	switch (pChnNode->m_SubType)
	{
	case HWL_CHANNEL_SUBTYPE_ASI:
		{
			pSubNode->m_Bitrate = MULTL_XMLGetNodeINT( pXMLParent, "bitrate", 10 );
		}
		break;
#if defined(GN1846) || defined(GN1866)  || defined(GN1866B)
		case HWL_CHANNEL_SUBTYPE_ENCODER_HI3531A:
		case HWL_CHANNEL_SUBTYPE_ENCODER_HI3519:
		{
			CHAR_T *plTmpStr;
			MULT_SubENCODERInfo *plEncInfo = &pSubNode->m_SubInfo.m_SubENCODERInfo;

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "vi_mode");
			if (plTmpStr) {
				plEncInfo->m_ViMode = MULTL_XMLEncViModeValueFromStr(plTmpStr);
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "vo_mode");
			if (plTmpStr) {
				plEncInfo->m_VoMode = MULTL_XMLEncVoModeValueFromStr(plTmpStr);
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "br_mode");
			if (plTmpStr) {
				plEncInfo->m_BrMode = MULTL_XMLEncBrModeValueFromStr(plTmpStr);
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "enc_mode");
			if (plTmpStr) {
				plEncInfo->m_EncMode = MULTL_XMLEncModeValueFromStr(plTmpStr);
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "profile");
			if (plTmpStr) {
				plEncInfo->m_Profile = MULTL_XMLEncProfileValueFromStr(plTmpStr);
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "bitrate");
			if (plTmpStr) {
				plEncInfo->m_Bitrate = MULTL_XMLGetNodeUINT(pXMLParent, "bitrate", 10);
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "prog_bitrate");
			if (plTmpStr) {
				plEncInfo->m_ProgBitrate = MULTL_XMLGetNodeUINT(pXMLParent, "prog_bitrate", 10);
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "gop");
			if (plTmpStr) {
				plEncInfo->m_Gop = MULTL_XMLGetNodeUINT(pXMLParent, "gop", 10);
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "audio_group");
			if (plTmpStr) {
				plEncInfo->m_AudioGroup = MULTL_XMLAudGroupValueFromStr(plTmpStr);
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "aud_enc_mode");
			if (plTmpStr) {
				plEncInfo->m_AudEncMode = MULTL_XMLAudEncModeValueFromStr(plTmpStr);
			}
			if(pHandle->m_Information.m_LicenseMode != pHandle->m_AuthMode)
			{
				plEncInfo->m_AudEncMode = MULTL_XMLAudEncModeValueFromStr("MPEG1-L2");
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "aud_bitrate");
			if (plTmpStr) {
				plEncInfo->m_AudBitrate = MULTL_XMLAudBitrateValueFromStr(plTmpStr);
			}

#if defined(SUPPORT_WATER_MARK)
			plEncInfo->m_x_coordinate = MULTL_XMLGetNodeUINTDefault(pXMLParent, "x_coordinate", 10, 0);
			plEncInfo->m_y_coordinate = MULTL_XMLGetNodeUINTDefault(pXMLParent, "y_coordinate", 10, 0);
			plEncInfo->m_transparent = MULTL_XMLGetNodeUINTDefault(pXMLParent, "transparent", 10, 0);
			plEncInfo->m_enable = MULTL_XMLGetNodeMarkDefault(pXMLParent, "enable", FALSE);
#endif	

#if defined(SUPPORT_MOSIAC)
			plEncInfo->m_Mosaic_Param.m_x_coordinate = MULTL_XMLGetNodeUINTDefault(pXMLParent, "mosaic_x_coordinate", 10, 0);
			plEncInfo->m_Mosaic_Param.m_y_coordinate = MULTL_XMLGetNodeUINTDefault(pXMLParent, "mosaic_y_coordinate", 10, 0);
			plEncInfo->m_Mosaic_Param.m_width = MULTL_XMLGetNodeUINTDefault(pXMLParent, "mosaic_width", 10, 8);
			plEncInfo->m_Mosaic_Param.m_height = MULTL_XMLGetNodeUINTDefault(pXMLParent, "mosaic_height", 10, 8);
			plEncInfo->m_Mosaic_Param.m_blk_size = MULTL_XMLEncMosaicSizeValueFromStr(plTmpStr);
			plEncInfo->m_Mosaic_Param.m_enable = MULTL_XMLGetNodeMarkDefault(pXMLParent, "mosaic_enable", FALSE);
#endif					

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "aud_sample");
			if (plTmpStr) {
				plEncInfo->m_AudSample = MULTL_XMLAudSampValueFromStr(plTmpStr);
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "volume");
			if (plTmpStr) {
				plEncInfo->m_Volume = MULTL_XMLGetNodeINT(pXMLParent, "volume", 10);
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "prog_number");
			if (plTmpStr) {
				plEncInfo->m_ProgNumber = MULTL_XMLGetNodeUINT(pXMLParent, "prog_number", 10);
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "prog_name");
			if (plTmpStr) {
				GLOBAL_STRCPY(plEncInfo->m_pProgName, plTmpStr);
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "vid_pid");
			if (plTmpStr) {
				plEncInfo->m_VidPid = MULTL_XMLGetNodeUINT(pXMLParent, "vid_pid", 10);
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "aud_pid");
			if (plTmpStr) {
				plEncInfo->m_AudPid = MULTL_XMLGetNodeUINT(pXMLParent, "aud_pid", 10);
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "pcr_pid");
			if (plTmpStr) {
				plEncInfo->m_PcrPid = MULTL_XMLGetNodeUINT(pXMLParent, "pcr_pid", 10);
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "pmt_pid");
			if (plTmpStr) {
				plEncInfo->m_PmtPid = MULTL_XMLGetNodeUINT(pXMLParent, "pmt_pid", 10);
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "protocol");
			if (plTmpStr) {
				plEncInfo->m_Protocol = MULTL_XMLGetNodeUINT(pXMLParent, "protocol", 10);
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "ipv4_des");
			if (plTmpStr) {
				plEncInfo->m_IPv4Addr = PFC_SocketAToN(MULTL_XMLGetNodeText(pXMLParent, "ipv4_des"));
			}

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "ipv4_des_port");
			if (plTmpStr) {
				plEncInfo->m_IPv4Port = MULTL_XMLGetNodeUINT(pXMLParent, "ipv4_des_port", 10);
			}

			GLOBAL_TRACE(("EncChn: ip-%#x, port-%d, protocol-%d\n", plEncInfo->m_IPv4Addr, plEncInfo->m_IPv4Port, plEncInfo->m_Protocol));

			plTmpStr = MULTL_XMLGetNodeText(pXMLParent, "active");
			if (plTmpStr) {
				plEncInfo->m_ActiveMark = MULTL_XMLGetNodeMark(pXMLParent, "active");
			}
		}
		break;
#endif
	case HWL_CHANNEL_SUBTYPE_E3DS3:
		{
			pSubNode->m_SubInfo.m_SubESDS3Info.m_E3DS3 = MULTL_XMLE3DS3ValueFromStr(MULTL_XMLGetNodeText(pXMLParent, "e3ds3_mark"));
			pSubNode->m_SubInfo.m_SubESDS3Info.m_Bitorder = MULTL_XMLBitOrderValueFromStr(MULTL_XMLGetNodeText(pXMLParent, "bit_order"));
			pSubNode->m_SubInfo.m_SubESDS3Info.m_Frameform = MULTL_XMLFrameformValueFromStr(MULTL_XMLGetNodeText(pXMLParent, "fream_from"));
			pSubNode->m_SubInfo.m_SubESDS3Info.m_Packetlength = MULTL_XMLPacketlengthformValueFromStr(MULTL_XMLGetNodeText(pXMLParent, "packet_length"));
			pSubNode->m_SubInfo.m_SubESDS3Info.m_InterweaveCoding = MULTL_XMLGetNodeMark(pXMLParent, "interweave_code");
			pSubNode->m_SubInfo.m_SubESDS3Info.m_Scramble = MULTL_XMLGetNodeMark(pXMLParent, "scramble_changer");
			pSubNode->m_SubInfo.m_SubESDS3Info.m_RSCoding = MULTL_XMLGetNodeMark(pXMLParent, "rs_code");
			pSubNode->m_SubInfo.m_SubESDS3Info.m_RateRecover = MULTL_XMLGetNodeMark(pXMLParent, "code_rate_recover");
		}
		break;
	case HWL_CHANNEL_SUBTYPE_TUNER_S_AVL6211:
		{
			pSubNode->m_SubInfo.m_SubTUNERInfo.m_InputFreq = MULTL_XMLGetNodeUINT(pXMLParent, "in_freq", 10);
			pSubNode->m_SubInfo.m_SubTUNERInfo.m_LocalFreq = MULTL_XMLGetNodeUINT(pXMLParent, "local_freq", 10);
			pSubNode->m_SubInfo.m_SubTUNERInfo.m_SymbolRate = MULTL_XMLGetNodeUINT(pXMLParent, "sym_rate", 10);
			pSubNode->m_SubInfo.m_SubTUNERInfo.m_PolarMethod = MULTL_XMLPolarMethodValueFromStr(MULTL_XMLGetNodeText(pXMLParent, "polar_method"));
			pSubNode->m_SubInfo.m_SubTUNERInfo.m_22kSwitch = MULTL_XMLGetNodeMark(pXMLParent, "switch_22k");
			pSubNode->m_SubInfo.m_SubTUNERInfo.m_Specinv = MULTL_XMLTunerSpecinvValueFromStr(MULTL_XMLGetNodeText(pXMLParent, "spec_inv"));
		}
		break;
	case HWL_CHANNEL_SUBTYPE_TUNER_C:
		{
			pSubNode->m_SubInfo.m_SubTUNERInfo.m_InputFreq = MULTL_XMLGetNodeUINT(pXMLParent, "in_freq", 10);			
			pSubNode->m_SubInfo.m_SubTUNERInfo.m_SymbolRate = MULTL_XMLGetNodeUINT(pXMLParent, "sym_rate", 10);
			pSubNode->m_SubInfo.m_SubTUNERInfo.m_Modulation = MULTL_XMLQAMModeValueFromStr(MULTL_XMLGetNodeText(pXMLParent, "modulation_mode"));
			pSubNode->m_SubInfo.m_SubTUNERInfo.m_Reqtype = MULTL_XMLReqTunerTypeValueFromStr( MULTL_XMLGetNodeText(pXMLParent, "req_type"));
			pSubNode->m_SubInfo.m_SubTUNERInfo.m_Specinv = MULTL_XMLTunerSpecinvValueFromStr(MULTL_XMLGetNodeText(pXMLParent, "spec_inv"));
		}
		break;
	case HWL_CHANNEL_SUBTYPE_TUNER_DTMB_ATBM8869:
		{
			pSubNode->m_SubInfo.m_SubTUNERInfo.m_InputFreq = MULTL_XMLGetNodeUINT(pXMLParent, "in_freq", 10);			
			pSubNode->m_SubInfo.m_SubTUNERInfo.m_Specinv = MULTL_XMLTunerSpecinvValueFromStr(MULTL_XMLGetNodeText(pXMLParent, "spec_inv"));
		}
		break;
	case HWL_CHANNEL_SUBTYPE_IP:
		{
			pSubNode->m_SubInfo.m_SubIPInfo.m_IPv4Addr = PFC_SocketAToN(MULTL_XMLGetNodeText(pXMLParent, "ipv4_des"));
			pSubNode->m_SubInfo.m_SubIPInfo.m_IPv4Port = MULTL_XMLGetNodeUINT(pXMLParent, "ipv4_des_port", 10);
			pSubNode->m_SubInfo.m_SubIPInfo.m_IPv4LimitAddr = PFC_SocketAToN(MULTL_XMLGetNodeText(pXMLParent, "ipv4_src"));
			pSubNode->m_SubInfo.m_SubIPInfo.m_IPv4LimitPort = MULTL_XMLGetNodeUINT(pXMLParent, "ipv4_src_port", 10);
			pSubNode->m_SubInfo.m_SubIPInfo.m_IPv4LimitMark = PFC_SocketAToN(MULTL_XMLGetNodeText(pXMLParent, "ipv4_src_limit_mark"));
			pSubNode->m_SubInfo.m_SubIPInfo.m_Protocol = MULTL_XMLGetNodeUINT(pXMLParent, "protocol", 10);
			pSubNode->m_Bitrate = MULTL_XMLGetNodeINT( pXMLParent, "bitrate", 10 );
		}
		break;
	case HWL_CHANNEL_SUBTYPE_MODULATOR_AD9789:
	case HWL_CHANNEL_SUBTYPE_MODULATOR_BL85KMM:
		{
			pSubNode->m_SubInfo.m_SubModulatorInfo.m_ITUCoding = MULTL_XMLITUCodingValueFromStr(MULTL_XMLGetNodeText(pXMLParent, "itu_coding"));
			pSubNode->m_SubInfo.m_SubModulatorInfo.m_AnalogBand = MULTL_XMLAnalogValueFromStr(MULTL_XMLGetNodeText(pXMLParent, "analog_band"));
			pSubNode->m_SubInfo.m_SubModulatorInfo.m_CenterFreq = MULTL_XMLGetNodeUINT(pXMLParent, "center_frequence", 10);
			pSubNode->m_SubInfo.m_SubModulatorInfo.m_SymbolRate = MULTL_XMLGetNodeUINT(pXMLParent, "symbol_rate", 10);
			pSubNode->m_SubInfo.m_SubModulatorInfo.m_Mode = MULTL_XMLQAMModeValueFromStr(MULTL_XMLGetNodeText(pXMLParent, "modulation_mode"));
			pSubNode->m_SubInfo.m_SubModulatorInfo.m_SpectInv = MULTL_XMLGetNodeMark(pXMLParent, "spectrum_invert");
			pSubNode->m_SubInfo.m_SubModulatorInfo.m_GainLevel = MULTL_XMLGetNodeUINT(pXMLParent, "gain_level", 10);
			pSubNode->m_SubInfo.m_SubModulatorInfo.m_Modulation = MULTL_XMLGetNodeMark(pXMLParent, "modulation_mark");
			pSubNode->m_SubInfo.m_SubModulatorInfo.m_FecEncode = MULTL_XMLFecEncodeValueFromStr( MULTL_XMLGetNodeText(pXMLParent, "fec_encode") );
#if defined(GQ3760A) || defined(LR1800S) || defined(GQ3760B) || defined(GQ3763) || defined(GQ3760) || defined(GQ3768) || defined(GQ3765)
			pSubNode->m_SubInfo.m_SubModulatorInfo.m_CarrierMode = MULTL_XMLGetNodeINT(pXMLParent, "carrier", 10);
			pSubNode->m_SubInfo.m_SubModulatorInfo.m_PNMode = MULTL_XMLDTMBPNValueFromStr(MULTL_XMLGetNodeText(pXMLParent, "pn_mode"));
			pSubNode->m_SubInfo.m_SubModulatorInfo.m_CodeRate = MULTL_XMLDTMBCodeRateValueFromStr(MULTL_XMLGetNodeText(pXMLParent, "code_rate"));
			pSubNode->m_SubInfo.m_SubModulatorInfo.m_TimeInterleave = MULTL_XMLGetNodeINT(pXMLParent, "time_interleaver", 10);
			pSubNode->m_SubInfo.m_SubModulatorInfo.m_DoublePilot = MULTL_XMLGetNodeMark(pXMLParent, "double_pilot");
#endif
			GLOBAL_TRACE(("Load MODULATOR_PARAM!\n"));

		}
		break;
	default:
		break;
	}
}


void MULTL_SetRemuxApplyMark(MULT_Handle *pHandle, BOOL bClear)
{
	if (bClear)
	{
		pHandle->m_Monitor.m_bApplyMark = FALSE;
	}
	else
	{
		GLOBAL_TRACE(("Apply Remux Param!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"));
		MULTL_ApplyRemuxParameter(pHandle);//????????ò???
#ifdef SUPPORT_SFN_MODULATOR
		MULT_SFNApplyByQAMModule();
#endif
#ifdef SUPPORT_SFN_ADAPTER
		MULT_SFNAApplyParameter();
#endif
		MULTL_SaveParameterXML(pHandle);
		pHandle->m_Monitor.m_bApplyMark = TRUE;
	}
}


void MULTL_SetSaveMark(MULT_Handle *pHandle, BOOL bClear)
{
	if (bClear)
	{
		pHandle->m_Monitor.m_bSaveMark = FALSE;
	}
	else
	{
		pHandle->m_Monitor.m_bSaveMark = TRUE;
	}
}


void MULTL_ResetAlarmCount(MULT_Handle *pHandle, S32 AlarmIndex)
{
	S32 i, lAlarmCount, lAlarmIndex;
	U32 lAlarmID;
	MULT_Monitor *plMonitor;
	plMonitor = &pHandle->m_Monitor;

	lAlarmCount = CAL_LogGetLogCFGCount(plMonitor->m_LogHandle);
	for (i = 0; i < lAlarmCount; i++)
	{
		if ((i == AlarmIndex) || (-1 == AlarmIndex))
		{
			CAL_LogClearLogInfo(plMonitor->m_LogHandle, i);
		}
	}
}


void MULTL_ParameterReset(MULT_Handle *pHandle)
{
	CHAR_T plCMD[1024];
	GLOBAL_TRACE(("Parameter Reset!!!!!!\n"));

	/*???????IP?????DEVICE_SETTING*/
	PFC_System("mv -f %s%s %s%s__\n", MULT_XML_BASE_DIR, MULT_DEVICE_PARAMETER_XML, MULT_XML_BASE_DIR, MULT_DEVICE_PARAMETER_XML);

	/*???monitor_setting.xml??system_paramter.xml*/
	PFC_System("rm -rf %s%s\n", MULT_XML_BASE_DIR, MULT_SYSTEM_PARAMETER_XML);
	PFC_System("rm -rf %s%s\n", MULT_XML_BASE_DIR, MULT_MONITOR_SETTING_XML);
#ifdef ENCODER_CARD_PLATFORM
	PFC_System("rm -rf %s%s\n", MULT_XML_BASE_DIR, CARD_MODULE_PARAMETER_XML_FILE);
#endif

	/*??м???OEM????????????????е????????????OEM??????????????????????*/
	GLOBAL_SPRINTF((plCMD, "%s%s", MULT_STORAGE_BASE_DIR, MULT_PARAMETER_DEF_PATHNAME));
	MULTL_LoadParamterFromStorage(pHandle, plCMD);

	///*???????IP?????DEVICE_SETTING*/
	PFC_System("mv -f %s%s__ %s%s\n", MULT_XML_BASE_DIR, MULT_DEVICE_PARAMETER_XML, MULT_XML_BASE_DIR, MULT_DEVICE_PARAMETER_XML);

	/*???浽FLASH*/
	MULTL_SaveParamterToStorage(pHandle);
}

void MULTL_FactoryPreset(MULT_Handle *pHandle)
{
	CHAR_T plCMD[1024];
	GLOBAL_TRACE(("Factory Preset!!!!!!\n"));
	/*??????parameter.bin???????????*/
	GLOBAL_SPRINTF((plCMD, "rm -rf %s%s\n", MULT_STORAGE_BASE_DIR, MULT_PARAMETER_FILE_PATHNAME));
	PFC_System(plCMD);
	GLOBAL_SPRINTF((plCMD, "rm -rf %s%s\n", MULT_STORAGE_BASE_DIR, MULT_WEB_USER_MANAGE_FILE_PATHNAME));
	PFC_System(plCMD);
	/*??????????????????*/
	GLOBAL_SPRINTF((plCMD, "cp -f %s%s %s%s", MULT_STORAGE_BASE_DIR, MULT_PARAMETER_DEF_PATHNAME, MULT_STORAGE_BASE_DIR, MULT_PARAMETER_FILE_PATHNAME));
	PFC_System(plCMD);
}
#if defined(CPU_DATA_PORT)
#if defined(INDEPENDENT_DATA_IP_SUPPORT)
#else
static BOOL s_EthSetFlag = TRUE;
BOOL MULTL_GetEthSetFlag(void) 
{
	return s_EthSetFlag;
}
#endif
#endif


void MULTL_ManagePortConfig(MULT_Handle *pHandle)
{
	CHAR_T plCMD[1024];
	MULT_Config *plConfig;
	plConfig = &pHandle->m_Configuration;

#if defined(INDEPENDENT_DATA_IP_SUPPORT)
	HWL_ENCODER_StopChn(0);//这里偷懒只使用了一个通道
#endif

#if defined(CPU_DATA_PORT)
#if defined(INDEPENDENT_DATA_IP_SUPPORT)
#else
	s_EthSetFlag = TRUE;
#endif
#endif
	GLOBAL_SPRINTF((plCMD, "ifconfig eth0 down\n"));
	PFC_System(plCMD);

	GLOBAL_SPRINTF((plCMD, "ifconfig eth0 hw ether %.2x%.2x%.2x%.2x%.2x%.2x\n", plConfig->m_pMAC[0], plConfig->m_pMAC[1], plConfig->m_pMAC[2], plConfig->m_pMAC[3], plConfig->m_pMAC[4], plConfig->m_pMAC[5]));
	PFC_System(plCMD);

	GLOBAL_SPRINTF((plCMD,"ifconfig eth0 %s netmask ", PFC_SocketNToA(plConfig->m_ManageIPv4Addr)));
	GLOBAL_STRCAT(plCMD, PFC_SocketNToA(plConfig->m_ManageIPv4Mask));
	GLOBAL_STRCAT(plCMD, "\n\0");
	GLOBAL_TRACE((plCMD));
	PFC_System(plCMD);

	GLOBAL_SPRINTF((plCMD, "ifconfig eth0 up\n"));
	PFC_System(plCMD);

	GLOBAL_TRACE(("Send Gratuitous ARP\n"));
	GLOBAL_SPRINTF((plCMD,"arping -A %s -c 1 \n\0", PFC_SocketNToA(plConfig->m_ManageIPv4Addr)));
	PFC_System(plCMD);

	sprintf(plCMD,"route add default gw %s dev eth0\n",  PFC_SocketNToA(plConfig->m_ManageIPv4Gate));
	GLOBAL_TRACE((plCMD));
	PFC_System(plCMD);
	
#if defined(CPU_DATA_PORT)
#if defined(INDEPENDENT_DATA_IP_SUPPORT)
#else
	s_EthSetFlag = FALSE;
#endif
#endif


#if defined(INDEPENDENT_DATA_IP_SUPPORT)
	HWL_ENCODER_ReSetChnIPParam(0);//这里偷懒只使用了一个通道
#endif
}


void MULTL_RebootThread(void *pParam)
{
	MULT_Handle *plHandle = (MULT_Handle*)pParam;
	if (plHandle)
	{
		/*??????????????????*/
#ifdef SUPPORT_NEW_FRP_MENU_MODULE
		MULT_FRPMenuADVShowRebootProgressString();
#else
		FRP_MenuSetRebootShow();
#endif

		/*????????Ч??*/
		PFC_TaskSleep(2000);

#ifdef zynq
		PFC_TaskSleep(10000);
#endif

#ifdef USE_NEW_QAM_SETTING_FUNCTIONS
		{
			HWL_QAMForceOutputProgressChange(0);
		}
#endif


#if 0
		plHandle->m_RetCode = 1;
		plHandle->m_TaskMark = MULT_TASK_MARK_WAIT_STOP;
#else
		/*??????????*/
		PFC_System("reboot");
#endif
	}
}

void MULTL_RebootSequence(MULT_Handle *pHandle)
{
	GLOBAL_SYSLOG(( PL_SYSLOG_PRIORITY_NOTICE, "Ready To Reboot!!\n"));
	PFC_TaskCreate("Reboot", 40* 1024, MULTL_RebootThread, 1, pHandle);
}

#if defined(GN1846) || defined(GN1866)
void MULTL_Reboot_SDI(MULT_Handle *pHandle)
{
	Manual_Reboot_SDI();
}

void MULTL_Print_SDI_Reg(MULT_Handle *pHandle)
{
	Manual_Print_SDI_Reg();
}
#endif

void MULTL_SaveMaintenaceToStorage(MULT_Handle *pHandle)
{
	GLOBAL_FD lReadFD, lWriteFD;
	CHAR_T plCMD[1024];
	U8 *plPayLoadStart;
	S32 lFileSize, lActSize;
	CAL_FCAPHandle *plFCAHandle;

	/*??????? MULT_DEVICE_MAINTENANCE_XML ??????*/
	GLOBAL_SPRINTF((plCMD, "cd %s\n tar -czf %s ./%s\n", MULT_XML_BASE_DIR, MULT_IO_TMP_FILE_PATHNAME, MULT_DEVICE_MAINTENANCE_XML));//??????????????????????????????????????????????????棡????????
	PFC_System(plCMD);

	lReadFD = GLOBAL_FOPEN(MULT_IO_TMP_FILE_PATHNAME, "rb");
	if (lReadFD)
	{
		lFileSize = CAL_FileSize(lReadFD);
		if (lFileSize > 0)
		{
			/*???ó????????????*/
			GLOBAL_TRACE(("Tar Size = %d\n", lFileSize));
			plFCAHandle = CAL_FCAPEncodeCreate(MULT_DEVICE_COMPLETE_TYPE, MULT_MAINTANCE_DESCRIPTOR, lFileSize);
			if (plFCAHandle)
			{
				plPayLoadStart = CAL_FCAPGetPayloadPtr(plFCAHandle);
				if (plPayLoadStart)
				{
					lActSize = GLOBAL_FREAD(plPayLoadStart, 1, lFileSize, lReadFD);
					CAL_FCAPFinalize(plFCAHandle, lFileSize);
					if (lActSize == lFileSize)
					{
						GLOBAL_SPRINTF((plCMD, "%s%s", MULT_STORAGE_BASE_DIR, MULT_MAINTENACE_FILE_PATHNAME));
						lWriteFD = GLOBAL_FOPEN(plCMD, "wb");
						if (lWriteFD)
						{
							lActSize = GLOBAL_FWRITE(plFCAHandle->m_pDataBuf, 1, plFCAHandle->m_DataSize, lWriteFD);
							GLOBAL_FCLOSE(lWriteFD);
							if (lActSize == plFCAHandle->m_DataSize)
							{
								GLOBAL_TRACE(("Save Paramters Successful, BIN Size = %d,\n", lActSize));
							}
							else
							{
								GLOBAL_TRACE(("Save Paramters Failed\n"));
							}
						}
						else
						{
							GLOBAL_TRACE(("Open %s for Write Failed!!!\n", MULT_IO_TMP_FILE_PATHNAME));
						}
					}
					else
					{
						GLOBAL_TRACE(("Read File File Error Info: %d/%d\n", lActSize, lFileSize));
					}

				}
				else
				{
					GLOBAL_TRACE(("Get PTR Failed\n"));
				}
				CAL_FCAPDestroy(plFCAHandle);
				plFCAHandle = NULL;
			}
			else
			{
				GLOBAL_TRACE(("Create Handle Failed\n"));
			}
		
		}
		else
		{
			GLOBAL_TRACE(("%s File Size = \n", lFileSize));
		}


		GLOBAL_FCLOSE(lReadFD);
	}
	else
	{
		GLOBAL_TRACE(("Open %s Failed!!!\n", MULT_IO_TMP_FILE_PATHNAME));
	}


	/*?????????*/
	GLOBAL_SPRINTF((plCMD, "rm -r %s", MULT_IO_TMP_FILE_PATHNAME));
 	PFC_System(plCMD);
}


BOOL MULTL_LoadMaintenaceFromStorage(MULT_Handle *pHandle, CHAR_T *pFilePathName)
{
	BOOL lRet = FALSE;
	GLOBAL_FD lReadFD, lWriteFD;
	CHAR_T plCMD[1024];
	U8 *plPayLoadStart;
	S32 lFileSize, lActSize;
	CAL_FCAPHandle *plFCAHandle;

	if (pFilePathName)
	{
		lReadFD = GLOBAL_FOPEN(pFilePathName, "rb");
		if (lReadFD)
		{
			lFileSize = CAL_FileSize(lReadFD);
			if (lFileSize > 0)
			{
				/*???ó????????????*/
				plFCAHandle = CAL_FCAPDencodeCreate(lFileSize);
				if (plFCAHandle)
				{
					lActSize = GLOBAL_FREAD(plFCAHandle->m_pDataBuf, 1, lFileSize, lReadFD);
					if (lActSize == lFileSize)
					{
						GLOBAL_TRACE(("Load Paramters BIN FileSize = %d\n", lFileSize));
						if (CAL_FCAPValidation(plFCAHandle, MULT_DEVICE_COMPLETE_TYPE, MULT_MAINTANCE_DESCRIPTOR))
						{
							GLOBAL_SPRINTF((plCMD, "%s", MULT_IO_TMP_FILE_PATHNAME));
							lWriteFD = GLOBAL_FOPEN(plCMD, "wb");
							if (lWriteFD)
							{
								plPayLoadStart = CAL_FCAPGetPayloadPtr(plFCAHandle);
								lFileSize = CAL_FCAPGetPayloadSize(plFCAHandle);
								lActSize = GLOBAL_FWRITE(plPayLoadStart, 1, lFileSize, lWriteFD);
								GLOBAL_FCLOSE(lWriteFD);
								if (lActSize == lFileSize)
								{
									GLOBAL_SPRINTF((plCMD, "cd %s\ntar -xzf %s\n", MULT_XML_BASE_DIR, MULT_IO_TMP_FILE_PATHNAME));//??????????????????????????????????????????????????棡????????
									PFC_System(plCMD);

									/*?????????*/
 									GLOBAL_SPRINTF((plCMD, "rm -r %s\n", MULT_IO_TMP_FILE_PATHNAME));
 									PFC_System(plCMD);

									GLOBAL_TRACE(("Load Paramters From Successful, Tar Size = %d\n", lFileSize, pFilePathName));
									lRet = TRUE;
								}
								else
								{
									GLOBAL_TRACE(("Write Tmp File Failed\n"));
								}
							}
							else
							{
								GLOBAL_TRACE(("Open %s for Write Failed!!!\n", plCMD));
							}

						}
						else
						{
							GLOBAL_TRACE(("Parameter %s File Validation Failed\n", pFilePathName));
						}
					}
					else
					{
						GLOBAL_TRACE(("Read File File Error Info: %d/%d\n", lActSize, lFileSize));
					}

					CAL_FCAPDestroy(plFCAHandle);
					plFCAHandle = NULL;
				}
				else
				{
					GLOBAL_TRACE(("Create Handle Failed\n"));
				}
			}
			else
			{
				GLOBAL_TRACE(("%s File Size = \n", lFileSize));
			}
			GLOBAL_FCLOSE(lReadFD);
		}
		else
		{
			GLOBAL_TRACE(("Open %s Failed!!!\n", pFilePathName));
		}

	}
	return lRet;
}



BOOL MULTL_GetTsMark(MULT_Handle *pHandle, S32 TsIndex, BOOL bInput)
{
	BOOL lRet = FALSE;
	S32 i, k;
	MULT_Parameter *plSystemParam;
	MULT_ChannelNode *plChnNode;
	MULT_SubChannelNode *plSubNode;

	plSystemParam = &pHandle->m_Parameter;

	if (bInput)
	{
		for (i = 0; i < plSystemParam->m_InChannelNumber; i++)
		{
			plChnNode = &plSystemParam->m_pInChannel[i];
			for (k = 0; k < plChnNode->m_SubChannelNumber; k++)
			{
				plSubNode = &plChnNode->m_pSubChannelNode[k];
				if (plSubNode->m_CorrespondTsIndex == TsIndex)
				{
					lRet = plSubNode->m_ActiveMark;
					break;
				}
			}
		}
	}
	else
	{
		for (i = 0; i < plSystemParam->m_OutChannelNumber; i++)
		{
			plChnNode = &plSystemParam->m_pOutChannel[i];
			if ((plChnNode->m_ChannelType != HWL_CHANNEL_TYPE_IP_LOOP_DEP) && (plChnNode->m_ChannelType != HWL_CHANNEL_TYPE_IP_LOOP_DEP))
			{
				for (k = 0; k < plChnNode->m_SubChannelNumber; k++)
				{
					plSubNode = &plChnNode->m_pSubChannelNode[k];
					if (plSubNode->m_CorrespondTsIndex == TsIndex)
					{
						lRet = plSubNode->m_ActiveMark;
						break;
					}
				}
			}
		}
	}
	return lRet;
}


void MULTL_GenerateServiceXML(MULT_Handle *pHandle, CHAR_T *pFilePathName)
{
	S32 i, k;
	S8 plBuf[1024];
	FILE *pFD;

	S32 lInTsNum, lOutTsNum, lSCSCANum;
	U32 lInTsIDs, lOutTsIDs, lSCSIDs, lServiceIDs, lEsIDs;

	U32 lItemIDs;
	U16 lCASystemID[4];
	S32 *plTmpMark;
	MPEG2_DBTsInInfo	lInTsInfo;
	MPEG2_DBTsOutInfo	lOutTsInfo;
	MPEG2_DBTsRouteInfo	lRouteInfo;

	MULT_SubModulatorInfo *plSubInfo;

	MPEG2_DBCaOutInfo lCAOutInfo;
	MPEG2_DBCaInInfo lCAInInfo;
	MPEG2_DBSCSCAInfo lSCSCAInfo;
	MPEG2_DBSCSInfo lSCSInfo;
	MPEG2_DBServiceOutInfo lServiceOutInfo;
	MPEG2_DBEsOutInfo lEsOutInfo;


	pFD = fopen(pFilePathName, "wb");
	if (pFD)
	{
		fprintf(pFD, "<?xml version=\"1.0\" encoding=\"utf-8\" ?>");

		fprintf(pFD, "<root>");
		fprintf(pFD, "<networks>");
		fprintf(pFD, "<network>");

		lInTsNum = MPEG2_DBGetTsCount(pHandle->m_DBSHandle, TRUE);

		plTmpMark = (S32 *)GLOBAL_ZMALLOC(sizeof(S32) * lInTsNum);

		for (k = 0; k < lInTsNum; k++)
		{
			plTmpMark[k] = GLOBAL_INVALID_INDEX;

			lInTsIDs = MPEG2_DBGetTsIDs(pHandle->m_DBSHandle, FALSE, k);
			lItemIDs = 0;
			while((lItemIDs = MPEG2_DBGetMuxCANextNode(pHandle->m_DBSHandle, lItemIDs, MPEG2_DB_CA_TYPE_TS, lInTsIDs)) != 0)
			{
				plTmpMark[k] = TRUE;
				break;
			}
		}

		lOutTsNum = MPEG2_DBGetTsCount(pHandle->m_DBSHandle, FALSE);
		for (i = 0; i < lOutTsNum; i++)
		{
			if (MPEG2_DBGetTsValid(pHandle->m_DBSHandle, i, FALSE) == FALSE)
			{
				continue;
			}

			lOutTsIDs = MPEG2_DBGetTsIDs(pHandle->m_DBSHandle, FALSE, i);
			if(MPEG2_DBGetTsOutInfo(pHandle->m_DBSHandle, lOutTsIDs, &lOutTsInfo))
			{
				fprintf(pFD, "<ts>");


				fprintf(pFD, "<ts_ind>%d</ts_ind>", i);
				fprintf(pFD, "<delivery_system>");
				fprintf(pFD, "<delivery_system_type>%s</delivery_system_type>", "cable");
				fprintf(pFD, "<delivery_system_info>");
#if defined(GQ3650DS) || defined(GQ3650DR) || defined(GQ3655)
				plSubInfo = &pHandle->m_Parameter.m_pOutChannel[0].m_pSubChannelNode[i].m_SubInfo.m_SubModulatorInfo;
				fprintf(pFD, "<frequency>%d</frequency>", plSubInfo->m_CenterFreq);
				fprintf(pFD, "<modulation_type>%d</modulation_type>", plSubInfo->m_Mode + 1);
				fprintf(pFD, "<symbol_rate>%d</symbol_rate>", plSubInfo->m_SymbolRate);
#endif
				
#if defined(GQ3710A) || defined(GQ3710B) || defined(GQ3760A) || defined(LR1800S) || defined(GQ3760B) || defined(GQ3763) || defined(GQ3760) || defined(GQ3768) || defined(GQ3765)
				plSubInfo = &pHandle->m_Parameter.m_pOutChannel[0].m_pSubChannelNode[i].m_SubInfo.m_SubModulatorInfo;
				fprintf(pFD, "<frequency>%d</frequency>", plSubInfo->m_CenterFreq);
				fprintf(pFD, "<modulation_type>%d</modulation_type>", 0);
				fprintf(pFD, "<symbol_rate>%d</symbol_rate>", plSubInfo->m_SymbolRate);
#endif

#if defined(GM2730X) || defined(GM2730S) || defined(GM2730F) || defined(GM4500) || defined(GN2000) || defined(GC1815B) || defined(GC1804C) || defined(GC1804B) || defined(GN1866B) || defined(GN1846) || defined(GN1866)
				fprintf(pFD, "<frequency>%d</frequency>", 0);
				fprintf(pFD, "<modulation_type>%d</modulation_type>",0);
				fprintf(pFD, "<symbol_rate>%d</symbol_rate>",0);
#endif

				fprintf(pFD, "</delivery_system_info>");
				fprintf(pFD, "</delivery_system>");


				fprintf(pFD, "<original_network_id>%d</original_network_id>", lOutTsInfo.m_ONID);
				fprintf(pFD, "<ts_id>%d</ts_id>", lOutTsInfo.m_TsID);
				fprintf(pFD, "<pat_version>%d</pat_version>", lOutTsInfo.m_PATVersion);
				fprintf(pFD, "<sdt_version>%d</sdt_version>", lOutTsInfo.m_SDTVersion);
				fprintf(pFD, "<cat_version>%d</cat_version>", lOutTsInfo.m_CATVersion);

				for (k = 0; k < lInTsNum; k++)
				{

					if (MPEG2_DBGetTsValid(pHandle->m_DBSHandle, k, TRUE) == FALSE)
					{
						continue;
					}

					if (plTmpMark[k] == TRUE)
					{
						lInTsIDs = MPEG2_DBGetTsIDs(pHandle->m_DBSHandle, FALSE, k);

						lItemIDs = 0;
						while((lItemIDs = MPEG2_DBGetMuxCANextNode(pHandle->m_DBSHandle, lItemIDs, MPEG2_DB_CA_TYPE_TS, lInTsIDs)) != 0)
						{
							if (plTmpMark[k] == GLOBAL_INVALID_INDEX)
							{
								plTmpMark[k] = TRUE;
							}

							if ( MPEG2_DBGetMuxCAInInfo(pHandle->m_DBSHandle, lItemIDs, &lCAInInfo) == TRUE)
							{
								MPEG2_DBGetMuxCAOutInfo(pHandle->m_DBSHandle, lItemIDs, &lCAOutInfo);
								if (lCAOutInfo.m_OutputMark && (lCAOutInfo.m_SPInfo.m_OutTsIndex == i))
								{
									fprintf(pFD, "<emm>");
									fprintf(pFD, "<cas_id>%d</cas_id>", lCAInInfo.m_CaSystemID);
									fprintf(pFD, "<pid>%d</pid>", lCAOutInfo.m_CaPID);
									fprintf(pFD, "</emm>");
								}
							}
						}
					}

				}


				lSCSCANum = MPEG2_DBGetSCSCount(pHandle->m_DBSHandle);
				for (k = 0; k < lSCSCANum; k++)
				{
					lSCSIDs = MPEG2_DBGetSCSIDs(pHandle->m_DBSHandle, k);
					if (MPEG2_DBProcSCSInfo(pHandle->m_DBSHandle, lSCSIDs, &lSCSInfo, TRUE))
					{
						if (lSCSInfo.m_ActiveMark)
						{
							lCASystemID[k] = lSCSInfo.m_CASystemID;
						}
						else
						{
							lCASystemID[k] = 0;
						}
					}
				}

				lItemIDs = 0;
				while((lItemIDs = MPEG2_DBGetSCSCANextNode(pHandle->m_DBSHandle, lItemIDs, MPEG2_DB_CA_TYPE_TS, lOutTsIDs)) != 0)
				{
					if (MPEG2_DBGetSCSCAInfo(pHandle->m_DBSHandle, lItemIDs, &lSCSCAInfo) == TRUE)
					{
						for (k = 0; k < lSCSCANum; k++)
						{
							if (lSCSCAInfo.m_pOutputCaInfo[k].m_OutputMark && (lCASystemID[k] != 0))
							{
								fprintf(pFD, "<emm>");
								fprintf(pFD, "<cas_id>%d</cas_id>", lCASystemID[k]);
								fprintf(pFD, "<pid>%d</pid>", lSCSCAInfo.m_pOutputCaInfo[k].m_CaPID);
								fprintf(pFD, "</emm>");
							}
						}
					}
				}

				lServiceIDs = 0;
				while((lServiceIDs = MPEG2_DBGetServiceNextNode(pHandle->m_DBSHandle, lServiceIDs)) != 0)
				{
					MPEG2_DBGetServiceOutInfo(pHandle->m_DBSHandle, lServiceIDs, &lServiceOutInfo);
					if (MPEG2_DBGetServicTsIndex(pHandle->m_DBSHandle, lServiceIDs, FALSE) == i)
					{
						fprintf(pFD, "<service>");
						fprintf(pFD, "<service_id>%d</service_id>", lServiceOutInfo.m_ServiceID);
						fprintf(pFD, "<service_type>%d</service_type>", lServiceOutInfo.m_ServiceType);
						fprintf(pFD, "<pmt_pid>%d</pmt_pid>",lServiceOutInfo.m_PMTPID);
						fprintf(pFD, "<pmt_version>%d</pmt_version>", lServiceOutInfo.m_PMTVersion);
						fprintf(pFD, "<pcr_pid>%d</pcr_pid>", lServiceOutInfo.m_PCRPID);

						fprintf(pFD, "<name><![CDATA[%s]]></name>", lServiceOutInfo.m_ServiceName);
						fprintf(pFD, "<provider><![CDATA[%s]]></provider>", lServiceOutInfo.m_ServiceProviderName);

						lItemIDs = 0;
						while((lItemIDs = MPEG2_DBGetMuxCANextNode(pHandle->m_DBSHandle, lItemIDs, MPEG2_DB_CA_TYPE_SERVICE, lServiceIDs)) != 0)
						{
							if ( MPEG2_DBGetMuxCAInInfo(pHandle->m_DBSHandle, lItemIDs, &lCAInInfo) == TRUE)
							{
								MPEG2_DBGetMuxCAOutInfo(pHandle->m_DBSHandle, lItemIDs, &lCAOutInfo);
								if (lCAOutInfo.m_OutputMark)
								{
									fprintf(pFD, "<emm>");
									fprintf(pFD, "<cas_id>%d</cas_id>", lCAInInfo.m_CaSystemID);
									fprintf(pFD, "<pid>%d</pid>", lCAOutInfo.m_CaPID);
									fprintf(pFD, "</emm>");
								}
							}
						}

						lItemIDs = 0;
						while((lItemIDs = MPEG2_DBGetSCSCANextNode(pHandle->m_DBSHandle, lItemIDs, MPEG2_DB_CA_TYPE_SERVICE, lServiceIDs)) != 0)
						{
							if (MPEG2_DBGetSCSCAInfo(pHandle->m_DBSHandle, lItemIDs, &lSCSCAInfo) == TRUE)
							{
								for (k = 0; k < lSCSCANum; k++)
								{
									if (lSCSCAInfo.m_pOutputCaInfo[k].m_OutputMark  && (lCASystemID[k] != 0))
									{
										fprintf(pFD, "<emm>");
										fprintf(pFD, "<cas_id>%d</cas_id>", lCASystemID[k]);
										fprintf(pFD, "<pid>%d</pid>", lSCSCAInfo.m_pOutputCaInfo[k].m_CaPID);
										fprintf(pFD, "</emm>");
									}
								}
							}
						}


						lEsIDs = 0;
						while((lEsIDs = MPEG2_DBGetEsNextNode(pHandle->m_DBSHandle, lEsIDs, lServiceIDs)) != 0)
						{

							MPEG2_DBGetEsOutInfo(pHandle->m_DBSHandle, lEsIDs, &lEsOutInfo);
							if (lEsOutInfo.m_OutputMark)
							{
								fprintf(pFD, "<es>");
								fprintf(pFD, "<es_type>%d</es_type>", lEsOutInfo.m_EsType);
								fprintf(pFD, "<pid>%d</pid>", lEsOutInfo.m_EsPID);
								fprintf(pFD, "</es>");

							}
						}

						fprintf(pFD, "</service>");       

					}
				}

				fprintf(pFD, "</ts>");
			}
		}
		fprintf(pFD, "</network>");
		fprintf(pFD, "</networks>");
		fprintf(pFD, "</root>");
		fclose(pFD);
		pFD = NULL;

		GLOBAL_SAFEFREE(plTmpMark);
	}
}


void MULT_SetApplyBusyMark(MULT_Handle *pHandle, BOOL bActive)
{
	pHandle->m_Monitor.m_ApplyParametersBusyMark = bActive;
}


#ifdef SUPPORT_SYSLOG_MODULE
void MULTL_XMLLoadSyslog(MULT_Handle *pHandle, mxml_node_t *pXMLHolder)
{
	CHAR_T *plTmpStr;
	MULT_Syslog *plTmpSyslog;
	plTmpSyslog = &pHandle->m_SyslogParam;
	if (pXMLHolder)
	{
		plTmpStr = MULTL_XMLGetNodeText(pXMLHolder, "syslog_active_mark");
		if (plTmpStr)
		{
			plTmpSyslog->m_SyslogGlobalMark = MULTL_XMLMarkValueFromStr(plTmpStr);
		}
		plTmpStr = MULTL_XMLGetNodeText(pXMLHolder, "remote_active_mark");
		if (plTmpStr)
		{
			plTmpSyslog->m_RemoteMark = MULTL_XMLMarkValueFromStr(plTmpStr);
		}
		plTmpStr = MULTL_XMLGetNodeText(pXMLHolder, "remote_ipv4_addr");
		if (plTmpStr)
		{
			plTmpSyslog->m_RemoteAddr = PFC_SocketAToN(plTmpStr);
		}
		plTmpStr = MULTL_XMLGetNodeText(pXMLHolder, "remote_ip_port");
		if (plTmpStr)
		{
			plTmpSyslog->m_RemotePort = GLOBAL_STRTOL(plTmpStr, NULL, 10);
		}
		plTmpStr = MULTL_XMLGetNodeText(pXMLHolder, "log_level");
		if (plTmpStr)
		{
			plTmpSyslog->m_LogLevel = GLOBAL_STRTOL(plTmpStr, NULL, 10);
		}
	}
}
#endif


/*????????????????б?*/
void MULTL_XMLGetOutServces(MULT_Handle *pHandle, HANDLE32 lXMLHolder)
{
	S32 i, k;



}
/*EOF*/
