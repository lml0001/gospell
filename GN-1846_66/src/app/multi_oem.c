/* Includes-------------------------------------------------------------------- */
#include "global_micros.h"
#include "libc_assist.h"
#include "crypto.h"
#include "platform_assist.h"
#include "multi_main_internal.h"
/* Global Variables (extern)--------------------------------------------------- */
/* Macro (local)--------------------------------------------------------------- */
/* Private Constants ---------------------------------------------------------- */
/* Private Types -------------------------------------------------------------- */
/* Private Variables (static)-------------------------------------------------- */
/* Private Function prototypes ------------------------------------------------ */
/* Functions ------------------------------------------------------------------ */
BOOL MULTL_LoadOEM(MULT_Handle *pHandle, CHAR_T *pPathname, BOOL bCheckValid, CHAR_T* pInformation, S32 BufSize)
{
	BOOL lRet = FALSE;
	GLOBAL_FD lFD;
	CHAR_T plUTF8Buf[512], *plTmpStr;
	S32 lFileSize, lUTF8Len;
	CAL_FCAPHandle *plUPGHandle;
	MULT_Information *plInfo;
	plInfo = &pHandle->m_Information;
	plUPGHandle = NULL;
	lFD = GLOBAL_FOPEN(pPathname, "rb");
	if (lFD)
	{
		lFileSize = CAL_FileSize(lFD);
		plUPGHandle = UPG_OEMDecodeCreate(lFileSize);
		if (plUPGHandle)
		{
			GLOBAL_FREAD(plUPGHandle->m_pDataBuf, 1, lFileSize, lFD);
			if (UPG_OEMValidation(plUPGHandle, MULT_DEVICE_COMPLETE_TYPE) == TRUE)
			{
				if (bCheckValid)
				{
					if (pInformation && BufSize > 0)
					{
						plTmpStr = CAL_FCAPGetDescription(plUPGHandle);
						if (GLOBAL_STRLEN(plTmpStr) < BufSize)
						{
							GLOBAL_STRCPY(pInformation, plTmpStr);
						}
					}
					lRet = TRUE;
				}
				else
				{
					GLOBAL_FD lNewFD;
					U8 *plNewBuf;
					S32 lNewSize;
					plNewBuf = UPG_OEMGetData(plUPGHandle, UPG_DATA_TYPE_LOGO, &lNewSize);
					if (plNewBuf && (lNewSize > 0))
					{
						lNewFD = GLOBAL_FOPEN(MULT_LOGO_FILE_WEB_PATH, "wb");
						if (lNewFD)
						{
							GLOBAL_FWRITE(plNewBuf, 1, lNewSize, lNewFD);
							GLOBAL_FCLOSE(lNewFD);
							lNewFD = NULL;
						}
					}

					plNewBuf = UPG_OEMGetData(plUPGHandle, UPG_DATA_TYPE_INTRO, &lNewSize);
					if (plNewBuf && (lNewSize > 0))
					{
						lNewFD = GLOBAL_FOPEN(MULT_INTRO_FILE_WEB_PATH, "wb");
						if (lNewFD)
						{
							GLOBAL_FWRITE(plNewBuf, 1, lNewSize, lNewFD);
							GLOBAL_FCLOSE(lNewFD);
							lNewFD = NULL;
						}
					}

					plNewBuf = UPG_OEMGetData(plUPGHandle, UPG_DATA_TYPE_MANUAL, &lNewSize);
					if (plNewBuf && (lNewSize > 0))
					{
						lNewFD = GLOBAL_FOPEN(MULT_MANUAL_FILE_WEB_PATH, "wb");
						if (lNewFD)
						{
							plInfo->m_bHaveManual = TRUE;
							GLOBAL_FWRITE(plNewBuf, 1, lNewSize, lNewFD);
							GLOBAL_FCLOSE(lNewFD);
							lNewFD = NULL;
						}
					}

					plNewBuf = UPG_OEMGetData(plUPGHandle, UPG_DATA_TYPE_OVERRIDE, &lNewSize);
					if (plNewBuf && (lNewSize > 0))
					{
						lNewFD = GLOBAL_FOPEN(MULT_OVERRIDE_FILE_WEB_PATH, "wb");
						if (lNewFD)
						{
							GLOBAL_FWRITE(plNewBuf, 1, lNewSize, lNewFD);
							GLOBAL_FCLOSE(lNewFD);
							lNewFD = NULL;

							GLOBAL_TRACE(("Override File Size = %d\n", lNewSize));

							PFC_System("tar -xzf %s", MULT_OVERRIDE_FILE_WEB_PATH);
							PFC_System("rm %s", MULT_OVERRIDE_FILE_WEB_PATH);
						}
					}
					/*END*/

					CAL_CovertGB2312ToUTF8Str(UPG_OEMGetModel(plUPGHandle), plInfo->m_pModelName, sizeof(plInfo->m_pModelName));
					CAL_CovertGB2312ToUTF8Str(UPG_OEMGetWebNameENG(plUPGHandle), plInfo->m_pWEBENG, sizeof(plInfo->m_pWEBENG));
					CAL_CovertGB2312ToUTF8Str(UPG_OEMGetWebNameCHN(plUPGHandle), plInfo->m_pWEBCHN, sizeof(plInfo->m_pWEBCHN));
					GLOBAL_STRCPY(plInfo->m_pFRPENG, UPG_OEMGetLCDNameENG(plUPGHandle));
					GLOBAL_STRCPY(plInfo->m_pFRPCHN, UPG_OEMGetLCDNameCHN(plUPGHandle));
					CAL_CovertGB2312ToUTF8Str(UPG_OEMGetManufacturyName(plUPGHandle), plInfo->m_pManufacter, sizeof(plInfo->m_pManufacter));
					CAL_CovertGB2312ToUTF8Str(UPG_OEMGetManufacturyWebAddr(plUPGHandle), plInfo->m_pManufacterWEBADDR, sizeof(plInfo->m_pManufacterWEBADDR));
					lRet = TRUE;
				}
			}
			else
			{
				GLOBAL_TRACE(("Validation OEM Filed!!\n"));
			}
			CAL_FCAPDestroy(plUPGHandle);
			plUPGHandle = NULL;
		}
		else {
			GLOBAL_TRACE(("UPG_OEMDecodeCreate Failed!\n"));
		}
		GLOBAL_FCLOSE(lFD);
	}
	else
	{
		GLOBAL_TRACE(("No OEM File!\n"));
	}
	return lRet;
}


/*EOF*/
