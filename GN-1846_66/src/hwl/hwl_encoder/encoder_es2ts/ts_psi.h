/******************************************************************************

                  ��Ȩ���� (C), 2005-2017, GOSPELL���޹�˾

 ******************************************************************************
  �� �� ��   : ts.h
  �� �� ��   : ����
  ��    ��   : ���
  ��������   : 2017��7��14��
  ����޸�   :
  ��������   : ts.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :

   1.��    ��          : 2017��7��14��
      ��    ��          : ���
      �޸�����   : �����ļ�

******************************************************************************/
#ifndef __TS_PSI_H__
#define __TS_PSI_H__

#include <stdio.h>
#include <stdlib.h>		/* calloc realloc abs div*/
#include <string.h>		/* memcpy memcmp */
#include <stdint.h>		/* uint8_t ... */
#include <stdbool.h>	/* bool true false */
#include <ctype.h>		/* toupper isxdigit isspace */
#include <math.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>		/* time_t clock_t tm */
#include "mpeg2_micro.h"
#include "mpeg2.h"

#define GN_ENCODE_CH_NUM			4   /* ����ͨ����Ŀ */

#define MPEG2_STREAM_TYPE_H264_VIDEO					MPEG2_STREAM_TYPE_MPEG4_AVC_H264_VIDEO 
#define MPEG2_STREAM_TYPE_HEVC_VIDEO					MPEG2_STREAM_TYPE_HEVC_H265

/* ���뽨��ӿ� */
typedef enum
{
	PSI_VID_ENC_MODE_AVS = 0,
	PSI_VID_ENC_MODE_AVS_PLUS /* AVS+ */
}HWL_PsiVidEncMode; 

typedef enum
{
	PSI_AUD_ENC_MODE_MPEG1_L2 = 0,
	PSI_AUD_ENC_MODE_DRA_2_0,
	PSI_AUD_ENC_MODE_DRA_5_1,
	PSI_AUD_ENC_MODE_AAC,
	PSI_AUD_ENC_MODE_AC3,
	PSI_AUD_ENC_MODE_EAC3
}HWL_PsiAudEncMode; 

//edit by leonli
typedef enum
{
	ENCODE_MODE_H264,
	ENCODE_MODE_H265,
	ENCODE_MODE_NUM
} ENC_EncodeMode;

typedef struct
{
	uint8_t		m_SyncByteReplaceChar; /* ͬ���ֽ�ָ����ָ��ͬ���ֽڵ�ԭ�����ڵ���SPTSģʽʱ��FPGAͨ��ͬ���ֽ�ȷ����ų���IP�˿� */
	uint8_t		m_Version;

	uint32_t	m_ProgramNum; /* ��Ŀ�� */
	char		m_pProgramName[MPEG2_DB_MAX_SERVICE_NAME_BUF_LEN]; /* ��Ŀ�� */
	uint32_t	m_PmtPid; /* PMT��PID */

	uint32_t	m_PcrPid;

	uint32_t	m_VidPid;
	int32_t  	m_VidEncMode; /* ��Ƶ����ģʽ */

	uint32_t	m_AudPid;
	int32_t 	m_AudEncMode; /* ��Ƶ����ģʽ */

	ENC_EncodeMode		m_EncMode;/* H264/H265 */ //edit by leonli
}HWL_EncPmtInfo;

typedef struct  
{
	uint32_t	m_TsId;
	uint32_t	m_OnId; /* original_network_id */
	uint8_t	   m_Version;
	uint8_t	   m_SyncByteReplaceChar; /* ͬ���ֽ�ָ����ָ��ͬ���ֽڵ�ԭ�����ڵ���SPTSģʽʱ��FPGAͨ��ͬ���ֽ�ȷ����ų���IP�˿� */

	int32_t 	m_Charset;
	int32_t	   m_CharsetMark;

	struct 
	{
		uint32_t	m_ProgramNum; /* ��Ŀ�� */
		char	   m_pProgramName[MPEG2_DB_MAX_SERVICE_NAME_BUF_LEN]; /* ��Ŀ�� */
		uint32_t	m_PmtPid; /* PMT��PID */
	}m_ProgramInfo[GN_ENCODE_CH_NUM];
	
	uint32_t	m_ProgramLen; /* ��Ŀ��Ŀ */
}HWL_EncSdtInfo;

#define ENC_MAX_PSI_PACKET_NUM 50 /* ������������PSI���� */

typedef struct  
{
	uint8_t		m_pPsiPacket[ENC_MAX_PSI_PACKET_NUM][MPEG2_TS_PACKET_SIZE];
	uint32_t	m_PsiPacketCounter; /* PSI������ */	
}HWL_EncPsiInfo;

typedef struct  
{
	uint32_t	m_TsId;
	uint8_t	   m_Version;
	uint8_t	   m_SyncByteReplaceChar; /* ͬ���ֽ�ָ����ָ��ͬ���ֽڵ�ԭ�����ڵ���SPTSģʽʱ��FPGAͨ��ͬ���ֽ�ȷ����ų���IP�˿� */

	struct 
	{
		uint32_t	m_ProgramNum; /* ��Ŀ�� */
		uint32_t	m_PmtPid; /* PMT��PID */
	}m_ProgramInfo[GN_ENCODE_CH_NUM];

	uint32_t	m_ProgramLen; /* ��Ŀ��Ŀ */
}HWL_EncPatInfo;

typedef struct  
{
	uint8_t		pmt_num; /* PMT ����Ŀ */

   	HWL_EncPatInfo pat_Info; /* PAT infomation*/
   HWL_EncPmtInfo pmt_Info[GN_ENCODE_CH_NUM];  /* PMT infomation*/
   HWL_EncSdtInfo sdt_Info; /* SDT infomation*/
	uint8_t	   pat_continuity_counter; /* PAT continuity counter (from 0 to 15)*/
	uint8_t	   pmt_continuity_counter[GN_ENCODE_CH_NUM]; /* PMT continuity counter (from 0 to 15)*/
	uint8_t	   sdt_continuity_counter; /* SDT continuity counter (from 0 to 15)*/
	uint8_t	   cat_continuity_counter; /* CAT continuity counter (from 0 to 15)*/
	void		*m_pUserParam;
	void (*tx_psi_call)(uint8_t *data, uint32_t len, void *pUserParam); /* Transmit callback */
}HWL_PSI_OPTION_T;

/*****************************************************************************
* FUNCTION:ts_psi_CreatePat()
*
* DESCRIPTION:
*	    
* INPUT:
*	
* OUTPUT:
*	None.
*
* RETURN:
*
* NOTES:
*
* HISTORY:
*	
*	Review: 
******************************************************************************/
void ts_psi_CreatePat(HWL_EncPatInfo *pat_Info, uint8_t	 *pat_continuity_counter, HWL_EncPsiInfo *pPsiInfo);

void ts_psi_CreateSdt(HWL_EncSdtInfo *sdt_Info, uint8_t	 *sdt_continuity_counter, HWL_EncPsiInfo *pPsiInfo);

void ts_psi_CreateNullCat(uint8_t	 *cat_continuity_counter, uint8_t *pPsiPacket);

/*****************************************************************************
* FUNCTION:ts_psi_CreatePmt()
*
* DESCRIPTION:
*	    
* INPUT:
*	
* OUTPUT:
*	None.
*
* RETURN:
*
* NOTES:
*
* HISTORY:
*	
*	Review: 
******************************************************************************/
void ts_psi_CreatePmt(HWL_EncPmtInfo *pPmtInfo, uint8_t  *pmt_continuity_counter, HWL_EncPsiInfo *pPsiInfo);

#endif /*__TS_PSI_H__ */

