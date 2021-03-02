#ifndef HDMI_RX_H
#define HDMI_RX_H

#include "global_def.h"
#include "platform_assist.h"
#include "libc_assist.h" 

typedef enum {
	HDMI_RX_EDID_PCM = 0,              
	HDMI_RX_EDID_AC3,   
	HDMI_RX_EDID_NONE,
	HDMI_RX_EDID_NUM
} HDMI_RxEdidType;

typedef enum {
	HDMI_RX_GPIO_CPU = 0,
	HDMI_RX_GPIO_FPGA_EXT,
	HDMI_RX_GPIO_NUM
} HDMI_RxGpioType;

typedef struct {
	HDMI_RxGpioType m_GpioType; /* GPIO ���ͣ�CPU �Դ�������չ */
	U32 m_GpioIndex;
	U32 m_Pin;
} HDMI_RxGpioPin;

typedef struct {
	void (*m_GpioSetValueCB)(HDMI_RxGpioPin *pPinAddr, S32 Value, void *pUserParam);
	S32 (*m_GpioGetValueCB)(HDMI_RxGpioPin *pPinAddr, void *pUserParam);
	void (*m_GpioSetupCB)(HDMI_RxGpioPin *pPinAddr, BOOL IsInput, BOOL IsPullUp, void *pUserParam);

	void *m_pUserParam;

	/* ������Ϣ */
	HDMI_RxGpioPin m_HdmiHpdPin; /* �Ȱβ����� */
	HDMI_RxGpioPin m_Adv7612I2cSdaPin;
	HDMI_RxGpioPin m_Adv7612I2cSclPin;
	HDMI_RxGpioPin m_Adv7612RstPin; /* HDMI ��λ���� */
	HDMI_RxGpioPin m_EepromI2cSdaPin;
	HDMI_RxGpioPin m_EepromI2cSclPin;
	HDMI_RxGpioPin m_EepromWpPin;
} HDMI_RxInitParam;

typedef struct {
	S32	m_Brightness;	/* ���� */
	S32	m_Contrast;		/* �Աȶ� */
	S32	m_Saturation;	/* ���Ͷ� */
	S32	m_Hue;			/* ɫ�� */

	BOOL m_IsAc3Bypass;
} HDMI_RxCfgParam;

typedef struct {
	BOOL m_HdmiIsConnected;	/* HDMI �Ƿ����ӣ����ӽ���⵽ 5V �ĵ�ѹ */
	BOOL m_SignalIsLocked;	/* �ź��Ƿ����� TRUE: ���� FALSE: δ���� */
	S32 m_VideoStandard;	/* ��Ƶ��ʽ */
	S32 m_AudioSample;		/* ��Ƶ������ */
} HDMI_RxStatusParam;

HANDLE32 HDMI_RxCreate(HDMI_RxInitParam *pInitParam); /* HDMI ����ģ�鴴�� */
void HDMI_RxDestroy(HANDLE32 Handle); /* HDMI ����ģ������ */
BOOL HDMI_RxSetParam(HANDLE32 Handle, HDMI_RxCfgParam *pCfgParam); /* ���� HDMI ����ģ��Ĳ��� */
BOOL HDMI_RxGetStatus(HANDLE32 Handle, HDMI_RxStatusParam *pStatusParam); /* ��ȡ HDMI ����ģ���״ֵ̬ */
BOOL HDMI_RxDownloadEdid(HANDLE32 Handle, HDMI_RxEdidType EdidType); /* ���� EDID �� EEPROM оƬ */
BOOL HDMI_RxGetHwIsOk(HANDLE32 Handle); /* Ѳ��Ӳ��ͨ���Ƿ���� */

#endif /* HDMI_RX_H */
