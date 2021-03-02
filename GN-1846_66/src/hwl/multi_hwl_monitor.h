#ifndef _MULTI_HWL_MONITOR_H_
#define _MULTI_HWL_MONITOR_H_
/* Includes-------------------------------------------------------------------- */
#include "global_def.h"
#include "multi_hwl.h"
/* Macro ---------------------------------------------------------------------- */
#define HWL_PID_STATISTICS_MAX_NUM			250
#define HWL_IP_STATISTICS_MAX_NUM			250
/* Types Define (struct typedef)----------------------------------------------- */
typedef struct  
{
	U16	m_PID;
	U32	m_Bitrate;//bps
}HWL_MonitorPIDStatNode;

typedef struct  
{
	HWL_MonitorPIDStatNode		m_pNode[HWL_PID_STATISTICS_MAX_NUM];
	S32							m_Number;
	S32							m_Bitrate;
}HWL_MonitorPIDStatArray;


typedef struct  
{
	U32 m_IPAddr;
	U16 m_Port;
	U8	m_Protocol;
	S32	m_Bitrate;
}HWL_MonitorIPStatNode;

typedef struct  
{
	HWL_MonitorIPStatNode	m_pNode[HWL_IP_STATISTICS_MAX_NUM];
	S32				m_Number;
	S32				m_Bitrate;//Total bps
}HWL_MonitorIPStatArray;

/* Functions prototypes ------------------------------------------------------- */

/*ģ���ʼ��������*/
void HWL_MonitorInitate(void);
/*��ѯ������Ԥ��1S��ѯһ��*/
void HWL_MonitorAccess(S32 Duration);
/*������Ϣ��ȡ����������TRUEʱ��ʾ�������Ѿ�������*/
BOOL HWL_MonitorParser(U8 *pData, S32 DataLen);

/*PID���ʲ�ѯ����*/
void HWL_MonitorPIDStatisticConfig(S16 ChnIndex, S16 SubIndex, BOOL bInput);
/*PID������������*/
void HWL_MonitorPIDStatisticResultReq(void);
/*���PID������Ϣ*/
void HWL_MonitorPIDStatisticResultClean(void);
/*��ȡPID������Ϣ*/
void HWL_MonitorPIDStatisticResultGet(HWL_MonitorPIDStatArray *pArray);


/*������Ҫͳ�Ƶ�����˿ںţ������������ͬʱ��ǰ��ͳ�ƽ���ᱻ�Զ����*/
void HWL_MonitorIPStatisticConfig(S16 ChnIndex, BOOL bInput);
/*��ѯ��������*/
void HWL_MonitorIPStatisticResultReq(void);
/*���IP������Ϣ*/
void HWL_MonitorIPStatisticResultClean(void);
/*��ȡIP������Ϣ*/
void HWL_MonitorIPStatisticResultGet(HWL_MonitorIPStatArray *pArray);

void HWL_MonitorHeartBeateSend(void);

/*��ȡ��������*/
BOOL HWL_MonitorGetHeartBeatError(void);
/*Ӳ����Ϣ��ȡ�ӿ�*/
void HWL_MonitorHWInfoGet(HWL_HWInfo *pHWInfo);
/*��ȡ��̫���ĸ�ʽ*/
S32 HWL_MonitorHWInfoETHChnNum(HWL_HWInfo *pHWInfo);
/*��ȡͨ��/��ͨ��������Ϣ����λbps��ChnIndex == -1ʱΪ��ǰ�Ӱ�������ʣ�SubIndex == -1ʱΪ��ǰͨ��������*/
U32 HWL_MonitorChnBitrateInfoGet(S16 ChnIndex, S16 SubIndex, BOOL bInput);

/*ISRͳ��*/
void HWL_MonitorPlusInserterPacketNum(S32 Num);
/*��ȡ����������*/
U32 HWL_MonitorInserterBitrateGet(void);

/*�ڲ�ͨѶͳ��*/
void HWL_MonitorPlusICPByteNum(S32 ByteNum, BOOL bFPGAToCPU);
/*��ȡ�ڲ�ͨѶ����*/
S32 HWL_MonitorInternalCOMBitrateGet(BOOL bFPGAToCPU);

/*��ȡ��̫������״̬���ɰ汾��*/
BOOL HWL_MonitorGetETHLinkStatus(S32 Slot);
/*��ȡDDR����״̬*/
U32 HWL_MonitorGetMainFPGAModuleStatus(void);
/*ģ�鸴λ״̬���*/
void HWL_MonitorModuleResetSend(U32 ModuleResetMark);










void HWL_DVBMuxRouteApply(S32 InCHN, S32 MaxSubNum);
void HWL_DVBMuxRouteSet(S32 InTsInd, S32 OutTsInd, BOOL bRoute, BOOL RouteWithNullPacket, BOOL EnablePCRCorrection);

#endif
/*EOF*/
