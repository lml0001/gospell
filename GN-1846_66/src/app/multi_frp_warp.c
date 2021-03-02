/* Includes-------------------------------------------------------------------- */
#include "global_micros.h"
#include "libc_assist.h"
#include "crypto.h"
#include "platform_assist.h"
#include "multi_main_internal.h"
#include "frp_device.h"
#include "frp_card.h"
/* Global Variables (extern)--------------------------------------------------- */
/* Macro (local)--------------------------------------------------------------- */
/* Private Constants ---------------------------------------------------------- */
/* Private Types -------------------------------------------------------------- */
/* Private Variables (static)-------------------------------------------------- */
static BOOL s_bNEWFRPCARD = FALSE;
/* Private Function prototypes ------------------------------------------------ */
/* Functions ------------------------------------------------------------------ */
void MULT_FRPWarpPreInitate(MULT_Handle *pHandle)
{
	BOOL lRet;
	BOOL lFRPCardPlatFormatType = FRPCARD_PLATFORM_DEFAULT;

#if (defined(GN1846) || defined(GN1866) || defined(GN1866B))
	GLOBAL_TRACE(("Force NEW FRP\n"));
	lFRPCardPlatFormatType = FRPCARD_PLATFORM_GN1846_1866;
	s_bNEWFRPCARD = TRUE;
#elif (0)
	GLOBAL_TRACE(("Force OLD FRP\n"));
	s_bNEWFRPCARD = FALSE;
#else
	lRet = FRP_DeviceDetectFRPType(FRP_UART_INDEX, 2, 500);
	if (lRet)
	{
		GLOBAL_TRACE(("NEW CARD FRP\n"));
		s_bNEWFRPCARD = TRUE;
	}
	else
	{
		GLOBAL_TRACE(("OLD FRP\n"));
		s_bNEWFRPCARD = FALSE;
	}
#endif

	if (s_bNEWFRPCARD)
	{
		MULT_FRPCARDInitate(pHandle, lFRPCardPlatFormatType);
	}
	else
	{
		MULT_FRPPreConsoleIntiate();
	}
}

void MULT_FRPWarpPreInitateSetText(S32 Line, CHAR_T *pText)
{
	if (s_bNEWFRPCARD)
	{

	}
	else
	{
		MULT_FRPPreConsoleSetText(Line, pText);
	}
}

void MULT_FRPWarpMainInitate(MULT_Handle *pHandle)
{
	if (s_bNEWFRPCARD)
	{
		MULT_FRPCARDAfterInitiate(pHandle);
	}
	else
	{
		MULT_FRPPreConsoleTerminate();
		MULT_FRPInitiate(pHandle);
	}
}

void MULT_FRPWarpAccess(MULT_Handle *pHandle, S32 Duration)
{
	if (s_bNEWFRPCARD)
	{
		MULT_FRPCARDAccess(pHandle, Duration);
	}
	else
	{
		MULT_FRPAccess(pHandle, Duration);
	}
}

void MULT_FRPWarpAlarmClear(S32 ModuleInd, S32 Index)
{
	if (s_bNEWFRPCARD)
	{
		MULT_FRPCARDClearAlarm(ModuleInd);
	}
	else
	{
		MULT_FRPClearAlarm(Index);
	}

}

void MULT_FRPWarpRebootShow(void)
{
	if (s_bNEWFRPCARD)
	{
		MULT_FRPCARDRebootShow();
	}
	else
	{
		MULT_FRPRebootShow();
	}
}

void MULT_FRPWarpSetManagePortRelatedData(MULT_Handle *pHandle)
{
	if (s_bNEWFRPCARD)
	{
	}
	else
	{
		MULT_FRPSetManagePortRelatedData(pHandle);
	}
}


BOOL MULT_FRPWarpIsNewFRP(void)
{
	return s_bNEWFRPCARD;
}

void MULT_FRPWarpTerminate(void)
{
	if (s_bNEWFRPCARD)
	{
		MULT_FRPCARDTerminate();
	}
	else
	{
		MULT_FRPTerminate();
	}
}

/*EOF*/
