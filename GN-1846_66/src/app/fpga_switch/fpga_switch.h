#ifndef _FPGA_SWITCH_H_
#define _FPGA_SWITCH_H_

/* Includes-------------------------------------------------------------------- */
#include "global_def.h"
/* Macro ---------------------------------------------------------------------- */
/* Types Define (struct typedef)----------------------------------------------- */

typedef struct
{
	S32				m_SendIntervalMS;
	S32				m_ResetIntervalMS;
}FPGASW_InitParam;


typedef struct
{
	U32				m_IPv4;
	U8				m_pMAC[GLOBAL_MAC_BUF_SIZE];//暂时没有使用！
}FPGASW_DataPortParam;

/* Functions prototypes ------------------------------------------------------- */
HANDLE32 FPGASW_Create(FPGASW_InitParam *pParam);
BOOL FPGASW_Probe(HANDLE32 Handle, S32 TimeoutMS);
void FPGASW_SetDataPortParam(HANDLE32 Handle, FPGASW_DataPortParam *pDataPortParam);
S32 FPGASW_GetSlot(HANDLE32 Handle);
BOOL FPGASW_CheckResetStatus(HANDLE32 Handle);
void FPGASW_Start(HANDLE32 Handle);
void FPGASW_Stop(HANDLE32 Handle);
void FPGASW_Access(HANDLE32 Handle, S32 Duration);
void FPGASW_Destroy(HANDLE32 Handle);


BOOL FPGASW_WarpInitiate(void);
void FPGASW_WarpProbe(S32 TimeoutMS);
S32 FPGASW_WarpGetSlot(void);
BOOL FPGASW_WarpCheckResetStatus(void);
void FPGASW_WarpSetDataPortParam(FPGASW_DataPortParam *pDataPortParam);
BOOL FPGASW_WarpTerminate(void);

#endif
/*EOF*/