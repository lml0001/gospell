#ifndef __TUNERIIC_H__
#define __TUNERIIC_H__

#include "global_def.h"

enum
{
	DTMB=0,
	DVB_C,
	DVB_S,
	DVB_T,
	ISDB_T,
	ATSC,
	DVB_C_2,
	NONE,
};


typedef void (*Tuner_Notify)(U8 ChannelNo, U8 Reset,U8 LNBPower);

typedef enum
{
	NORMAL=0,
	NO_ACK,
	CHIP_CHANNEL_INVALID,
	DATA_INVALID
}TUNER_Error_Type;
#define TUNER_ENABLE 0
#define TUNER_DISENABLE 1

#define TUNER_PIN_HIGH 1
#define TUNER_PIN_LOW 0
#define MaxAccessTimes 5

typedef struct tagTunerPara
{

	U32 Frequency ;   //unit 10k Hz
	U32 lnbSymbol ;   //unit 1kBand
	U32 LocalFreq;
	U8  modulation;	        //the modulation mode of the signal
	U8 TuneReqType; //TRT_DVBC TRT_DCABLE 
	U8 lnbPower;
	U8 lnb22K;
	U8  IQInvert ; //A2108_RA_Normal = 0,	///< = 0  The normal way. Increase when signal turns to stronger
	//A2108_RA_Invert = 1		///< = 1  The invert way. Increase when signal turns to weaker. 

	U8 bandwith; //8,7,6
}TunerPara;

/********************
 *�������ܣ���ȡ�豸����
 *��    ����U8 ChipAddrΪ6��֮�е�ĳ��IICоƬ��ַ��
 *          		 U8 RegAddr,�Ĵ�����ַ
 *			 U8 *DataBuffer,��������Ч����
 *��    �أ�NORMAL����
 *          CHIP_INVALID:����һ����Ч��Ƭѡ
 *          CHIP_CHANNEL_INVALID:����һ����Ч��ͨ��
 *          DATA_INVALID:û�л��һ����Ч����
 ********************/
extern TUNER_Error_Type TUNER_Write(U8 ChipAddr,U8 RegAddr, U8*DataBuffer, U8 len,U8 StopFlag);
extern TUNER_Error_Type TUNER_Read(U8 ChipAddr, U8 *DataBuffer, U8 len);
void InitTunerIIC();
void CloseTuner();

void Delay(unsigned int count);
void ResetTuner(U16 Channel);
void EnablePower(void) ;
void	L14VPower(void) ;
void L18VPower(void) ;
void	DisEnablePower(void) ;
void delay_ms(U32 dMilliSeconds);
#endif

