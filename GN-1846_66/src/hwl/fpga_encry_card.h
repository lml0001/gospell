#ifndef FPGA_ENCRY_CARD_H
#define FPGA_ENCRY_CARD_H

#include "global_def.h"
#include "global_micros.h"

#define FPGA_ENCRY_BIG_BLOCK_1 0x0A
#define FPGA_ENCRY_BIG_BLOCK_2 0x0B
#define FPGA_ENCRY_BIG_BLOCK_3 0x0C
#define FPGA_ENCRY_BIG_BLOCK_ID 0x00

BOOL ED_ReadId(U32 *pEncryId);
BOOL ED_WriteBlock(S32 LBlockNum, S32 SBlcokNum, U8 *pData, S32 DataLen);
BOOL ED_ReadBlock(S32 LBlockNum, S32 SBlcokNum, U8 *pData, S32 DataLen);

#endif /* FPGA_ENCRY_CARD_H */
/* EOF */
