#include "spi_gpio.h"

#define LEVEL_HIGH 1
#define LEVEL_LOW 0

typedef struct {
	SPI_GpioInitParam m_InitParam;
} SPI_GpioHandle;

static void SPI_GpioInit(SPI_GpioHandle *pHandle)
{
	SPI_GpioInitParam *plInitParam = &pHandle->m_InitParam;

	plInitParam->m_GpioSetupCB(&plInitParam->m_SdaOutPin, TRUE, TRUE, plInitParam->m_pUserParam);
	plInitParam->m_GpioSetupCB(&plInitParam->m_CsPin, FALSE, TRUE, plInitParam->m_pUserParam);
	plInitParam->m_GpioSetupCB(&plInitParam->m_SdaInPin, FALSE, TRUE, plInitParam->m_pUserParam);
	plInitParam->m_GpioSetupCB(&plInitParam->m_SckPin, FALSE, TRUE, plInitParam->m_pUserParam);
}

S32 SPI_GpioRead(HANDLE32 Handle, U8 *pRegAddr, U32 RegAddrLen, char *pBuf, U32 BufSize)
{
	SPI_GpioHandle *plHandle = (SPI_GpioHandle *)Handle;
	SPI_GpioInitParam *pInitParam = &plHandle->m_InitParam;
	S32 i, j;
	U8 lCh;

	pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_HIGH, pInitParam->m_pUserParam);
	pInitParam->m_GpioSetValueCB(&pInitParam->m_CsPin, LEVEL_LOW, pInitParam->m_pUserParam);

	for (i = 0; i < RegAddrLen; i++) {
		lCh = pRegAddr[i];
		for(j = 0; j < 8; j++) { 	
			if( (lCh << j) & 0x80) {
				pInitParam->m_GpioSetValueCB(&pInitParam->m_SdaInPin, LEVEL_HIGH, pInitParam->m_pUserParam);
			}							
			else {
				pInitParam->m_GpioSetValueCB(&pInitParam->m_SdaInPin, LEVEL_LOW, pInitParam->m_pUserParam);
			}
			pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_LOW, pInitParam->m_pUserParam);
			pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_HIGH, pInitParam->m_pUserParam);
		}
	}

	for (i = 0; i < BufSize; i++) {
		lCh = 0;
		for (j = 0; j < 8; j++) {
			pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_LOW, pInitParam->m_pUserParam);
			pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_HIGH, pInitParam->m_pUserParam);
			lCh = (lCh << 1) | (0x01 & pInitParam->m_GpioGetValueCB(&pInitParam->m_SdaOutPin, pInitParam->m_pUserParam));
		}
		pBuf[i] = lCh;
	}

	pInitParam->m_GpioSetValueCB(&pInitParam->m_CsPin, LEVEL_HIGH, pInitParam->m_pUserParam);

	return BufSize;
}

S32 SPI_GpioWrite(HANDLE32 Handle, U8 *pRegAddr, U32 RegAddrLen, char *pBuf, U32 BufSize)
{
	SPI_GpioHandle *plHandle = (SPI_GpioHandle *)Handle;
	SPI_GpioInitParam *pInitParam = &plHandle->m_InitParam;
	S32 i, j;
	U8 lCh;

	pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_LOW, pInitParam->m_pUserParam);
	pInitParam->m_GpioSetValueCB(&pInitParam->m_CsPin, LEVEL_LOW, pInitParam->m_pUserParam);

	for (i = 0; i < RegAddrLen; i++) {
		lCh = pRegAddr[i];
		for(j = 0; j < 8; j++) { 	
			if( (lCh << j) & 0x80) {
				pInitParam->m_GpioSetValueCB(&pInitParam->m_SdaInPin, LEVEL_HIGH, pInitParam->m_pUserParam);
			}							
			else {
				pInitParam->m_GpioSetValueCB(&pInitParam->m_SdaInPin, LEVEL_LOW, pInitParam->m_pUserParam);	
			}

			pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_HIGH, pInitParam->m_pUserParam);
			pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_LOW, pInitParam->m_pUserParam);
		}
	}

	for (i = 0; i < BufSize; i++) {
		lCh = pBuf[i];
		for(j = 0; j < 8; j++) { 	
			if( (lCh << j) & 0x80) {
				pInitParam->m_GpioSetValueCB(&pInitParam->m_SdaInPin, LEVEL_HIGH, pInitParam->m_pUserParam);	
			}							
			else {
				pInitParam->m_GpioSetValueCB(&pInitParam->m_SdaInPin, LEVEL_LOW, pInitParam->m_pUserParam);	
			}

			pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_HIGH, pInitParam->m_pUserParam);	
			pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_LOW, pInitParam->m_pUserParam);	
		}
	}

	pInitParam->m_GpioSetValueCB(&pInitParam->m_CsPin, LEVEL_HIGH, pInitParam->m_pUserParam);	
	pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_HIGH, pInitParam->m_pUserParam);	

	return BufSize;
}

HANDLE32 SPI_GpioCreate(SPI_GpioInitParam *pInitPara)
{
	SPI_GpioHandle *plHandle;

	if (!pInitPara) {
		GLOBAL_TRACE(("pInitPara is NULL!\n"));
		return NULL;
	}

	if (!pInitPara->m_GpioGetValueCB || !pInitPara->m_GpioSetupCB
		|| !pInitPara->m_GpioSetValueCB) {
		GLOBAL_TRACE(("pInitPara Param Error!\n"));
		return NULL;
	}

	plHandle = (SPI_GpioHandle *)GLOBAL_MALLOC(sizeof(SPI_GpioHandle));
	if (!plHandle) {
		GLOBAL_TRACE(("malloc error!\n"));
		return NULL;
	}

	GLOBAL_MEMCPY(&plHandle->m_InitParam, pInitPara, sizeof(SPI_GpioInitParam));

	SPI_GpioInit(plHandle);

	return plHandle;
}

void SPI_GpioDestroy(HANDLE32 Handle)
{
	SPI_GpioHandle *plHandle = (SPI_GpioHandle *)Handle;

	if (plHandle) {
		free(plHandle);
		plHandle = NULL;
	}
}

/*ndelay (1), n = ns *167; ==>It 's 1Us*/
static void udelay(ns)
{
 	volatile U32 n;
	
 	n = ns *167;

 	while(n--)
 	{
 	}
}

static void spi_start(HANDLE32 Handle)
{
	SPI_GpioHandle *plHandle = (SPI_GpioHandle *)Handle;
	SPI_GpioInitParam *pInitParam = &plHandle->m_InitParam;

	pInitParam->m_GpioSetValueCB(&pInitParam->m_CsPin, LEVEL_HIGH, pInitParam->m_pUserParam);

	/* 在cs高情况下，拉低sclk线防止在开始时产生时钟下降沿 */
	pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_LOW, pInitParam->m_pUserParam);

	pInitParam->m_GpioSetValueCB(&pInitParam->m_CsPin, LEVEL_LOW, pInitParam->m_pUserParam);
	udelay (1); 
}

static void spi_stop(HANDLE32 Handle)
{
	SPI_GpioHandle *plHandle = (SPI_GpioHandle *)Handle;
	SPI_GpioInitParam *pInitParam = &plHandle->m_InitParam;

	pInitParam->m_GpioSetValueCB(&pInitParam->m_CsPin, LEVEL_HIGH, pInitParam->m_pUserParam);
	pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_HIGH, pInitParam->m_pUserParam);
}

static void spi_send_cmd_word(HANDLE32 Handle, U16 cmd_word)
{
	S32 i = 0;
	SPI_GpioHandle *plHandle = (SPI_GpioHandle *)Handle;
	SPI_GpioInitParam *pInitParam = &plHandle->m_InitParam;


	for (i = 15; i >= 0; i--) 
	{
		if ((cmd_word >> i ) & 0x01) 
		{
			pInitParam->m_GpioSetValueCB(&pInitParam->m_SdaInPin, LEVEL_HIGH, pInitParam->m_pUserParam);
		}
		else 
		{
			pInitParam->m_GpioSetValueCB(&pInitParam->m_SdaInPin, LEVEL_LOW, pInitParam->m_pUserParam);
		}
		
		pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_HIGH, pInitParam->m_pUserParam);

		udelay(1);

		pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_LOW, pInitParam->m_pUserParam);
	}
}

static void spi_receive_data_word(HANDLE32 Handle, U16 *pdata_word)
{
	S32 i = 0;
	U16 val = 0;
	SPI_GpioHandle *plHandle = (SPI_GpioHandle *)Handle;
	SPI_GpioInitParam *pInitParam = &plHandle->m_InitParam;


	for (i = 15; i >= 0; i--) 
	{
		/* 上升沿，主设备读取数据 */
		pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_HIGH, pInitParam->m_pUserParam);

		val |= (pInitParam->m_GpioGetValueCB(&pInitParam->m_SdaOutPin, pInitParam->m_pUserParam) << i);
		udelay(1); 
		
		/* 下降沿，从设备改变数据 */
		pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_LOW, pInitParam->m_pUserParam);
	}
	
	*pdata_word = val;
}

static void spi_send_data_word(HANDLE32 Handle, unsigned short data_word)
{
	S32 i = 0;
	SPI_GpioHandle *plHandle = (SPI_GpioHandle *)Handle;
	SPI_GpioInitParam *pInitParam = &plHandle->m_InitParam;

	for (i = 15; i >= 0; i--) 
	{
		if((data_word >> i ) & 0x01) 
		{
			pInitParam->m_GpioSetValueCB(&pInitParam->m_SdaInPin, LEVEL_HIGH, pInitParam->m_pUserParam);
		}
		else 
		{
			pInitParam->m_GpioSetValueCB(&pInitParam->m_SdaInPin, LEVEL_LOW, pInitParam->m_pUserParam);
		}
		
		pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_HIGH, pInitParam->m_pUserParam);
		
		udelay(1);
		
		pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_LOW, pInitParam->m_pUserParam);     
	}
}

S32 SPI_SDI_GpioRead(HANDLE32 Handle, U16 RegAddr, U32 RegAddrLen, U16 *pRegData)
{
	U16 data_word = 0;
	SPI_GpioHandle *plHandle = (SPI_GpioHandle *)Handle;
	SPI_GpioInitParam *pInitParam = &plHandle->m_InitParam;

	spi_start(Handle);
	spi_send_cmd_word(Handle, RegAddr);

	pInitParam->m_GpioSetValueCB(&pInitParam->m_SdaInPin, LEVEL_LOW, pInitParam->m_pUserParam);
	udelay (1); 
	spi_receive_data_word(Handle, &data_word);
	spi_stop(Handle);
	*pRegData = data_word;
	
	return sizeof(RegAddr); /* 返回数据的长度 */
}

S32 SPI_SDI_GpioWrite(HANDLE32 Handle, U16 RegAddr, U32 RegAddrLen, U16 RegData)
{
	spi_start(Handle);
	spi_send_cmd_word(Handle, RegAddr);
	udelay (1); 
	spi_send_data_word(Handle, RegData);
	spi_stop(Handle);

	return sizeof(RegData);
}

void usleep_test(HANDLE32 Handle)
{
	SPI_GpioHandle *plHandle = (SPI_GpioHandle *)Handle;
	SPI_GpioInitParam *pInitParam = &plHandle->m_InitParam;

	pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_HIGH, pInitParam->m_pUserParam);
	udelay (1); 
	pInitParam->m_GpioSetValueCB(&pInitParam->m_SckPin, LEVEL_LOW, pInitParam->m_pUserParam);
	udelay (1);
}
	
/* EOF */
