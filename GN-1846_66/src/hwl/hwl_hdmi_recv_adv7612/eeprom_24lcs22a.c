/*
EEPROM 24LCS22A��д�ļ������ڴ洢�����ʽ�ļ�EDID��ͨ��IIC����
1 ����24LCS22Aʱ��ÿ��Ϊ��ȷ��д��ɹ�������������жԱ�

2014-08-22
1 ����24LCS22A������������������£�д����û�����⣻�ڶ���ʱ�򾭲����������㣬ԭ��Ŀǰ���꣬ȡ��CM2031оƬͨ��������
2 ����IICʱ��ʱ��Ŀ������⣬���ڲ���24LCS22A��Ƶ�ʵͣ���Ҫ��֤���ݣ�������ʱ�������Ա�֤��д��ȷ��
3 дҳд���ֽ�д���ַ�ʽ��һ�����ҳд��ʽ��
4 ����ʱ��scl��sda���øߣ�ԭ�����������������õͺĵ磬Э��Ҳ�涨�øߡ�
5 �����ݷ�ʽ���������������ʽ��
*/

#include "hdmi_rx_inter.h"
#define DEV_ADDR_24LCS22A	0xA0

#define EEPROM_24LCS22A_PAGE_SIZE 8
	
/* ��ģʽ�л���˫��ģʽ��ԭ�����ϵ�Ĭ��ģʽ��transtionģʽ���μ�24LC22AоƬ��datasheet */
static void EEPROM_ChangeModeToBidirection(HDMI_RxHandle *pHandle)
{
	S32 i;
	HDMI_RxInitParam *plInitParam = &pHandle->m_InitParam;
	
	plInitParam->m_GpioSetupCB(&plInitParam->m_EepromI2cSclPin, FALSE, TRUE, plInitParam->m_pUserParam);
	plInitParam->m_GpioSetupCB(&plInitParam->m_EepromI2cSdaPin, FALSE, TRUE, plInitParam->m_pUserParam);

	plInitParam->m_GpioSetValueCB(&plInitParam->m_EepromWpPin, LEVEL_HIGH, plInitParam->m_pUserParam);
	plInitParam->m_GpioSetValueCB(&plInitParam->m_EepromI2cSclPin, LEVEL_LOW, plInitParam->m_pUserParam);
	plInitParam->m_GpioSetValueCB(&plInitParam->m_EepromWpPin, LEVEL_LOW, plInitParam->m_pUserParam);
	plInitParam->m_GpioSetValueCB(&plInitParam->m_EepromI2cSclPin, LEVEL_HIGH, plInitParam->m_pUserParam);
	for (i=0; i<2; i++) {
		plInitParam->m_GpioSetValueCB(&plInitParam->m_EepromWpPin, LEVEL_HIGH, plInitParam->m_pUserParam);
		plInitParam->m_GpioSetValueCB(&plInitParam->m_EepromWpPin, LEVEL_LOW, plInitParam->m_pUserParam);
	}

	/* ��Ϊ����Bidirectionģʽ */
	if (I2C_GpioWrite(pHandle->m_EdidI2cHandle, DEV_ADDR_24LCS22A, NULL, 0, NULL, 0, FALSE, 0) == -1) {
		pHandle->m_HwErrMark = TRUE;
	}
	else {
		pHandle->m_HwErrMark = FALSE;
	}
	plInitParam->m_GpioSetupCB(&plInitParam->m_EepromI2cSclPin, TRUE, TRUE, plInitParam->m_pUserParam);
	plInitParam->m_GpioSetupCB(&plInitParam->m_EepromI2cSdaPin, TRUE, TRUE, plInitParam->m_pUserParam);
}

BOOL EEPROM_DownLoadEdid(HDMI_RxHandle *pHandle, U8 *pBuffer, S32 BufLen)
{
	S32 i;
	U8 plRecvBuffer[256];
	HDMI_RxInitParam *plInitParam = &pHandle->m_InitParam;
	U8 lRegAddr = 0;
	BOOL lRet = TRUE;

	plInitParam->m_GpioSetValueCB(&plInitParam->m_EepromWpPin, LEVEL_HIGH, plInitParam->m_pUserParam); /* ����д�����ͷ� */
	plInitParam->m_GpioSetupCB(&plInitParam->m_EepromI2cSclPin, FALSE, TRUE, plInitParam->m_pUserParam);
	plInitParam->m_GpioSetupCB(&plInitParam->m_EepromI2cSdaPin, FALSE, TRUE, plInitParam->m_pUserParam);

	if (I2C_GpioWrite(pHandle->m_EdidI2cHandle, DEV_ADDR_24LCS22A, &lRegAddr, 1, pBuffer, BufLen, TRUE, EEPROM_24LCS22A_PAGE_SIZE) == -1) {
		pHandle->m_HwErrMark = TRUE;
		return FALSE;
	}
	else {
		pHandle->m_HwErrMark = FALSE;
	}
	if (I2C_GpioRead(pHandle->m_EdidI2cHandle, DEV_ADDR_24LCS22A, &lRegAddr, 1, plRecvBuffer, BufLen) == -1) {
		pHandle->m_HwErrMark = TRUE;
		return FALSE;
	}
	else {
		pHandle->m_HwErrMark = FALSE; 
	}
	if (memcmp(plRecvBuffer, pBuffer, BufLen) != 0) {
		GLOBAL_TRACE (("download EDID error!\n"));
		CAL_PrintDataBlock("Expect Data", pBuffer, BufLen);
		CAL_PrintDataBlock("Real Data", plRecvBuffer, BufLen);
		lRet = FALSE;
	}

	plInitParam->m_GpioSetValueCB(&plInitParam->m_EepromWpPin, LEVEL_LOW, plInitParam->m_pUserParam);
	plInitParam->m_GpioSetupCB(&plInitParam->m_EepromI2cSclPin, TRUE, TRUE, plInitParam->m_pUserParam); /* I2C������ɺ�����Ϊ���裬��Ӱ��HDMI�Ķ�д */
	plInitParam->m_GpioSetupCB(&plInitParam->m_EepromI2cSdaPin, TRUE, TRUE, plInitParam->m_pUserParam);

	return lRet;
}

void EEPROM_Init(HDMI_RxHandle *pHandle)
{
	EEPROM_ChangeModeToBidirection(pHandle);
}
/* EOF */
