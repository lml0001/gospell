/* Includes-------------------------------------------------------------------- */
#include "global_micros.h"
#include "libc_assist.h"
#include "crypto.h"
#include "platform_assist.h"
#include "multi_main_internal.h"
/* Global Variables (extern)--------------------------------------------------- */
/* Macro (local)--------------------------------------------------------------- */
#define MULT_NEW_KERNEL_VERSION_STRING			""
/* Private Constants ---------------------------------------------------------- */
/* Private Types -------------------------------------------------------------- */
/* Private Variables (static)-------------------------------------------------- */
/* Private Function prototypes ------------------------------------------------ */
/* Functions ------------------------------------------------------------------ */

void MULT_FRPImageUpdateThread(void* pUserParam)
{
	GLOBAL_TRACE(("Erase FLASH MTD1\n"));
	PFC_System("/tmp/mtd_debug erase /dev/mtd1 0 5242880");
	GLOBAL_TRACE(("Write Kernel Image On MTD1\n"));
	PFC_System("/tmp/mtd_debug write /dev/mtd1 0 %d %s", CAL_FileSizePath(MULT_NEW_KERNEL_FILE_PATHNAME), MULT_NEW_KERNEL_FILE_PATHNAME);
	GLOBAL_TRACE(("Remove Kernel Image\n"));
	PFC_System("rm -r %s", MULT_NEW_KERNEL_FILE_PATHNAME);
}

void MULT_KernelAutoUpdate(void)
{
	CHAR_T *plVersion;

	plVersion = PFC_GetSystemVersion();

	if (GLOBAL_STRCMP(plVersion, MULT_NEW_KERNEL_VERSION_STRING) != 0)
	{
		GLOBAL_TRACE(("Version Different! [%s] - [%s]\n", plVersion, MULT_NEW_KERNEL_VERSION_STRING));

		/*��鵱ǰ�Ƿ���Ҫ�����ں�*/
		if (GLOBAL_ACCESS(MULT_NEW_KERNEL_FILE_PATHNAME, R_OK) == 0)
		{
			S32 lTimeTick;
			HANDLE32 lThreadHandle;

			GLOBAL_TRACE(("Have File to Update Kernel!!!\n"));

			MULT_FRPWarpPreInitateSetText(0, "Start Update Kernel.");
			MULT_FRPWarpPreInitateSetText(1, "Avoid PowerOff!");

			lThreadHandle = PFC_TaskCreate(" ", 1024 * 1024, MULT_FRPImageUpdateThread, 0, NULL);

			lTimeTick = PFC_GetTickCount();
			while(TRUE)
			{
				if (PFC_TaskWait(lThreadHandle, 1000))
				{
					GLOBAL_TRACE(("Kernel Update Complete!!!!!!! Duration = %f S\n", ((F64)PAL_TimeEnd(&lTimeTick)) / 1000));
					PFC_System("reboot");
					break;
				}
				else
				{
					GLOBAL_TRACE(("Kernel In Progress Duration = %f S", ((F64)PAL_TimeEnd(&lTimeTick)) / 1000));
				}
			}

		}
		else
		{
			GLOBAL_TRACE(("No File to Update Kernel!!! System May Run with Critical Problem!\n"));
		}
	}
}
/*EOF*/
