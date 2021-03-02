/*
FPGA操作加密卡的读写，包括加密卡ID、加密卡非易失性块的读写操作
*/


#include "fpga_encry_card.h"
#include "multi_drv.h"
#include <unistd.h> 

#define FPGA_ENCRY_DATA_ADDR 0x300
#define FPGA_ENCRY_ADDR_ADDR 0x301
#define FPGA_ENCRY_LBLOCK_ADDR 0x3F0 /* 大块块号 */
#define FPGA_ENCRY_SBLOCK_ADDR 0x3F1 /* 小块块号 */
#define FPGA_ENCRY_DATA_SIZE_ADDR 0x3F2 /* 数据长度（字节数）<= 100 */
#define FPGA_ENCRY_CMD_FLAG_ADDR 0x3F3 /* 命令序列 */
#define FPGA_ENCRY_ACK_FLAG_ADDR 0x3F4 /* 应答序列 */
#define FPGA_ENCRY_STATE_ADDR 0x3F5 /* FPGA操作是否成功 */

#define FPGA_ENCRY_RW_BIT (1 << 4)

#define FPGA_ENCRY_RW_TIME_OUT (1000) /* ms */

/* 返回TRUE，执行成功，返回FALSE，表示超时 */
static BOOL ED_WaitAck(void)
{
	static U8 lFlag = 0; 
	U32 lPreTime = 0;
	U8 lRetVal;

	lFlag ++;
	DRL_FspiWrite(FPGA_ENCRY_CMD_FLAG_ADDR, &lFlag, 1);
	lPreTime = PL_GetTickCount();
	while (1)
	{
		DRL_FspiRead(FPGA_ENCRY_ACK_FLAG_ADDR, &lRetVal, 1);
		if (lRetVal == lFlag)
			break;

		if (PL_GetTickCount() - lPreTime >= FPGA_ENCRY_RW_TIME_OUT)
		{
			GLOBAL_TRACE(("Fpga Read Ack Time Out!\n"));
			return FALSE;
		}

		usleep(1000);
	}
	DRL_FspiRead(FPGA_ENCRY_STATE_ADDR, &lRetVal, 1);
	if (lRetVal == 0x00)
	{
		GLOBAL_TRACE(("Fpga Operate Encryption Card Failed!\n"));
		return FALSE;
	}

	return TRUE;
}

/* 读写触发 */
static BOOL ED_RwTrig(S32 DataLen)
{
	U8 lValue;
	lValue = DataLen & 0xFF;
	DRL_FspiWrite(FPGA_ENCRY_DATA_SIZE_ADDR, &lValue, 1);
	if (ED_WaitAck() == FALSE)
		return FALSE;

	return TRUE;
}

BOOL ED_ReadBlock(S32 LBlockNum, S32 SBlcokNum, U8 *pData, S32 DataLen)
{
	U8 i = 0;
	U8 lValue;
	lValue = LBlockNum & 0xFF;
	DRL_FspiWrite(FPGA_ENCRY_LBLOCK_ADDR, &lValue, 1);
	lValue = ~FPGA_ENCRY_RW_BIT & (SBlcokNum & 0x0F);
	DRL_FspiWrite(FPGA_ENCRY_SBLOCK_ADDR, &lValue, 1);
	if (ED_RwTrig(DataLen) == FALSE)
	{
		return FALSE;
	}

	for (i=0; i<DataLen; i++) /* 读操作 */
	{
		DRL_FspiWrite(FPGA_ENCRY_ADDR_ADDR, &i, 1);
		DRL_FspiRead(FPGA_ENCRY_DATA_ADDR, &pData[i], 1);
	}
	

	return TRUE;
}

BOOL ED_WriteBlock(S32 LBlockNum, S32 SBlcokNum, U8 *pData, S32 DataLen)
{
	U8 i = 0;
	U8 lValue;
	U8 plData[2] = {0};

	/* 写数据 */
	for (i=0; i<DataLen; i++)
	{
		DRL_FspiWrite(FPGA_ENCRY_ADDR_ADDR, &i, 1);
		DRL_FspiWrite(FPGA_ENCRY_DATA_ADDR, &pData[i], 1);
	}
	lValue = LBlockNum & 0xFF;
	DRL_FspiWrite(FPGA_ENCRY_LBLOCK_ADDR, &lValue, 1);
	lValue = FPGA_ENCRY_RW_BIT | (SBlcokNum & 0x0F);
	DRL_FspiWrite(FPGA_ENCRY_SBLOCK_ADDR, &lValue, 1);
	if (ED_RwTrig(DataLen) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

/* 读加密卡ID号 */
BOOL ED_ReadId(U32 *pEncryId)
{
	U8 plData[16] = {0};
	U8 i =0;
	if (ED_ReadBlock(FPGA_ENCRY_BIG_BLOCK_ID, 0, plData, 16) == FALSE)
		return FALSE;
	/*for (i = 0;i < 16; i ++)
	{
		GLOBAL_TRACE(("Chip Data[%d] = 0x%X\n",i, plData[i]));
	}*/

	*pEncryId = ((U32)plData[9] << 24) | ((U32)plData[10] << 16) | ((U32)plData[11] << 8) | (U32)plData[12];

	return TRUE;
}
