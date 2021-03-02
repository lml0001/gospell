//
// Created by Administrator on 2021/2/23 0023.
//
//这个是有自己的线程和进程的的模块

/* Includes-------------------------------------------------------------------- */
#include "multi_m3u8.h"
#include "global_micros.h"
#include "libc_assist.h"
#include "platform_assist.h"
/* Global Variables (extern)--------------------------------------------------- */
/* Macro (local)--------------------------------------------------------------- */
#define M3U8TASK_STATCK_SIZE					(1024*1024)
#define M3U8_FILE_EXT_NAME							"m3u8"

/* Private Constants ---------------------------------------------------------- */
/* Private Types -------------------------------------------------------------- */

typedef struct
{
    HANDLE32			m_SegementNodeList;
    U32					m_ExtMediaSequence;//当删除一个Segment之后减一！
    CHAR_T				m_pMediaListFilePathName[GLOBAL_MAX_PATH];//MediaList的文件绝对路径
    CHAR_T				m_pMediaListFileMainName[GLOBAL_MAX_PATH];//MediaList的文件的主文件名例如：/tmp/200k.bps 则为200k
    CHAR_T				m_pSegmentFileBasePath[GLOBAL_MAX_PATH];//Segment名字。例如：AAA_1020.ts
}M3U8_MediaList;

typedef struct
{
    M3U8_PlayListParam	m_PlayListParam;
    S32					m_SegmentIndex;//当前最后一个（最新）分块的序号，用于生成推荐文件相对路径
    CHAR_T				m_pFileBasePath[GLOBAL_MAX_PATH];
    BOOL				m_bEnable;//TRUE 当前SLOT会参加更新
    M3U8_MediaList		m_pMediaListArray[M3U8_MAX_MULTI_BITRATE_NUMBER];
}M3U8_PlayList;

typedef struct
{
    M3U8_InitParam		m_InitParam;
    M3U8_PlayList		*m_pPlayListArray;
}M3U8_Handle;


/* Private Variables (static)-------------------------------------------------- */
static BOOL M3U8_LGenerate(M3U8_Handle *pHandle, S32 PlaySlot, BOOL bPlayList);
/* Private Function prototypes ------------------------------------------------ */
/* Functions ------------------------------------------------------------------ */
HANDLE32 M3U8_Create(M3U8_InitParam *pInitParam)
{
    M3U8_Handle *plHandle = (M3U8_Handle*) GLOBAL_ZMALLOC(sizeof(M3U8_Handle));
    if (plHandle)
    {
        GLOBAL_MEMCPY(&plHandle->m_InitParam, pInitParam , sizeof(M3U8_InitParam));

        if (plHandle->m_InitParam.m_MaxPlayListNum <= 0)
        {
            plHandle->m_InitParam.m_MaxPlayListNum = 1;
        }
        plHandle->m_pPlayListArray = (M3U8_PlayList *)GLOBAL_ZMALLOC(sizeof(M3U8_PlayList) * plHandle->m_InitParam.m_MaxPlayListNum);
        if (plHandle->m_pPlayListArray)
        {
            GLOBAL_TRACE(("%s Complete!\n", __FUNCTION__));
        }
        else
        {
            GLOBAL_TRACE(("%s Error!\n", __FUNCTION__));
            GLOBAL_SAFEFREE(plHandle);
        }
    }
    return plHandle;
}

void M3U8_SetupPlayList(HANDLE32 Handle, S32 PlaySlot, M3U8_PlayListParam *pStreamParam, BOOL bEnable)
{
    M3U8_Handle *plHandle = (M3U8_Handle *)Handle;
    if (plHandle)
    {
        if (GLOBAL_CHECK_INDEX(PlaySlot, plHandle->m_InitParam.m_MaxPlayListNum))
        {
            M3U8_PlayList *plPlayList = &plHandle->m_pPlayListArray[PlaySlot];
            if (pStreamParam)
            {
                M3U8_PlayListParam *plPlayParam;
                M3U8_MediaListParam *plMediaParam;
                plPlayParam = &plPlayList->m_PlayListParam;
                if (plPlayParam)
                {
                    S32 i;
                    M3U8_MediaList *plMediaList;

                    GLOBAL_MEMCPY(plPlayParam, pStreamParam, sizeof(M3U8_PlayListParam));

                    if (plPlayParam->m_SegementNum < 1)
                    {
                        plPlayParam->m_SegementNum = 1;
                    }
                    if (plPlayParam->m_MediaListNum < 1)
                    {
                        plPlayParam->m_MediaListNum = 1;
                    }
                    if (plPlayParam->m_MediaListNum > M3U8_MAX_MULTI_BITRATE_NUMBER)
                    {
                        plPlayParam->m_MediaListNum = M3U8_MAX_MULTI_BITRATE_NUMBER;
                    }
                    GLOBAL_STRCPY(plPlayList->m_pFileBasePath, plPlayParam->m_pPlayListFilePathName);
                    CAL_StringCutDirFromPathName(plPlayList->m_pFileBasePath);

                    for (i = 0; i < plPlayParam->m_MediaListNum; i++)
                    {
                        plMediaList = &plPlayList->m_pMediaListArray[i];
                        plMediaParam = &plPlayParam->m_pMediaListParam[i];
                        if (plMediaList)
                        {
                            plMediaList->m_ExtMediaSequence = 0;
                            plMediaList->m_SegementNodeList = CAL_ListCreate(sizeof(M3U8_Segment));

                            GLOBAL_STRCPY(plMediaList->m_pMediaListFilePathName, plPlayList->m_pFileBasePath);
                            GLOBAL_STRCAT(plMediaList->m_pMediaListFilePathName, plMediaParam->m_pMediaListRelativePathName);

                            GLOBAL_STRCPY(plMediaList->m_pMediaListFileMainName, CAL_StringGetFileNameFromPathName(plMediaParam->m_pMediaListRelativePathName));
                            CAL_StringCutExtFromPathName(plMediaList->m_pMediaListFileMainName);


                            GLOBAL_STRCPY(plMediaList->m_pSegmentFileBasePath, plMediaList->m_pMediaListFilePathName);
                            CAL_StringCutDirFromPathName(plMediaList->m_pSegmentFileBasePath);

                        }
                    }
                }
                plPlayList->m_SegmentIndex = 0;//序号从0开始
            }

            plPlayList->m_bEnable = bEnable;
        }
    }
}


BOOL M3U8_InsertSegment(HANDLE32 Handle, S32 PlaySlot, S32 MediaSlot, M3U8_Segment *pSegment)
{
    BOOL lRet = FALSE;
    M3U8_Handle *plHandle = (M3U8_Handle *)Handle;
    if (plHandle)
    {
        if (GLOBAL_CHECK_INDEX(PlaySlot, plHandle->m_InitParam.m_MaxPlayListNum))
        {
            M3U8_PlayList *plPlayList;
            M3U8_PlayListParam *plPlayParam;
            M3U8_MediaList *plMediaList;
            M3U8_MediaListParam *plMediaParam;
            plPlayList = &plHandle->m_pPlayListArray[PlaySlot];
            plPlayParam = &plPlayList->m_PlayListParam;
            plMediaList = &plPlayList->m_pMediaListArray[MediaSlot];
            plMediaParam = &plPlayParam->m_pMediaListParam[MediaSlot];

            {
                /*如果超过个数，则将头移除*/
                S32 lCurSegmentNum;

                lCurSegmentNum = CAL_ListGetCount(plMediaList->m_SegementNodeList);
                if (lCurSegmentNum >= plPlayParam->m_SegementNum)
                {
                    CAL_ListRemoveHead(plMediaList->m_SegementNodeList);
                    plMediaList->m_ExtMediaSequence ++;
                }
                /*在队尾插入*/
                lRet = CAL_ListInsertAtTail(plMediaList->m_SegementNodeList, pSegment);
            }
        }
    }
    return lRet;
}

BOOL M3U8_GetSegementFileName(HANDLE32 Handle, S32 PlaySlot, S32 MediaSlot, CHAR_T *pPathName, BOOL bURL)
{
    BOOL lRet = FALSE;
    M3U8_Handle *plHandle = (M3U8_Handle *)Handle;
    if (plHandle)
    {
        if (GLOBAL_CHECK_INDEX(PlaySlot, plHandle->m_InitParam.m_MaxPlayListNum))
        {
            if (pPathName)
            {
                M3U8_PlayList *plPlayList = &plHandle->m_pPlayListArray[PlaySlot];
                M3U8_MediaListParam *plMediaParam = &plPlayList->m_PlayListParam.m_pMediaListParam[MediaSlot];
                M3U8_MediaList *plMediaList = &plPlayList->m_pMediaListArray[MediaSlot];
                if (bURL)
                {
                    GLOBAL_SPRINTF(((pPathName), "%s-%03d.ts", plMediaList->m_pMediaListFileMainName, plPlayList->m_SegmentIndex));
                }
                else
                {
                    GLOBAL_SPRINTF(((pPathName), "%s%s-%03d.ts", plMediaList->m_pSegmentFileBasePath, plMediaList->m_pMediaListFileMainName, plPlayList->m_SegmentIndex));
                }
                lRet = TRUE;
            }
        }
    }
    return lRet;
}


BOOL M3U8_UpdateList(HANDLE32 Handle, S32 PlaySlot, BOOL bPlayList)
{
    BOOL lRet = FALSE;
    M3U8_Handle *plHandle = (M3U8_Handle *)Handle;
    if (plHandle)
    {
        if (GLOBAL_CHECK_INDEX(PlaySlot, plHandle->m_InitParam.m_MaxPlayListNum))
        {
            M3U8_PlayList *plPlayList = &plHandle->m_pPlayListArray[PlaySlot];
            if (plPlayList->m_bEnable)
            {
                lRet = M3U8_LGenerate(plHandle, PlaySlot, bPlayList);
                if (lRet)
                {
                    if (bPlayList)
                    {
                        plPlayList->m_SegmentIndex = 0;
                    }
                    else
                    {
                        plPlayList->m_SegmentIndex++;
                    }
                }
            }
            else
            {
                GLOBAL_TRACE(("PlayList Not Enabled!\n"));
            }
        }
    }
    return lRet;
}

void M3U8_Destroy(HANDLE32 Handle)
{
    M3U8_Handle *plHandle = (M3U8_Handle *)Handle;
    if (plHandle)
    {
        S32 i, PlayInd;
        M3U8_PlayListParam *plPlayParam;
        M3U8_MediaList *plMediaList;
        M3U8_PlayList *plPlayList;

        for (PlayInd = 0; PlayInd < plHandle->m_InitParam.m_MaxPlayListNum; PlayInd++)
        {
            plPlayList = &plHandle->m_pPlayListArray[PlayInd];

            plPlayParam = &plPlayList->m_PlayListParam;

            for (i = 0; i < plPlayParam->m_MediaListNum; i++)
            {
                plMediaList = &plPlayList->m_pMediaListArray[i];
                if (plMediaList->m_SegementNodeList)
                {
                    CAL_ListDestroy(plMediaList->m_SegementNodeList);
                    plMediaList->m_SegementNodeList = NULL;
                }
            }
        }

        GLOBAL_SAFEFREE(plHandle->m_pPlayListArray);
        GLOBAL_SAFEFREE(plHandle);
        GLOBAL_TRACE(("%s Complete!\n", __FUNCTION__));
    }
}



/*本地函数*/
BOOL M3U8_LGenerate(M3U8_Handle *pHandle, S32 PlaySlot, BOOL bPlayList)
{
    BOOL lRet = FALSE;
    M3U8_Handle *plHandle = pHandle;
    if (plHandle)
    {
        if (GLOBAL_CHECK_INDEX(PlaySlot, plHandle->m_InitParam.m_MaxPlayListNum))
        {
            M3U8_PlayList *plPlayList = &plHandle->m_pPlayListArray[PlaySlot];
            if (plPlayList)
            {
                S32 i;
                M3U8_PlayListParam *plPlayParam;
                M3U8_MediaListParam *plMediaParam;
                CHAR_T plStringBuf[1024];
                CHAR_T plFormatBuf[1024];
                GLOBAL_FD lListFD;

                plPlayParam = &plPlayList->m_PlayListParam;

                if (bPlayList)
                {
                    GLOBAL_STRCPY(plStringBuf, "#EXTM3U\r\n");
                    GLOBAL_STRCAT(plStringBuf, "#EXT-X-VERSION:3\r\n");
                    for (i = 0; i < plPlayParam->m_MediaListNum; i++)
                    {
                        plMediaParam = &plPlayParam->m_pMediaListParam[i];
                        GLOBAL_SPRINTF((plFormatBuf, "#EXT-X-STREAM-INF:PROGRAM-ID=%d,", PlaySlot));
                        GLOBAL_STRCAT(plStringBuf, plFormatBuf);
                        GLOBAL_SPRINTF((plFormatBuf, "BANDWIDTH=%d,RESOLUTION=%dx%d\r\n", plMediaParam->m_BandwidthBPS, plMediaParam->m_ResolutionWidth, plMediaParam->m_ResolutionHeight));
                        GLOBAL_STRCAT(plStringBuf, plFormatBuf);
                        GLOBAL_SPRINTF((plFormatBuf, "%s\r\n", plMediaParam->m_pMediaListRelativePathName));
                        GLOBAL_STRCAT(plStringBuf, plFormatBuf);
                    }

                    lListFD = GLOBAL_FOPEN(plPlayParam->m_pPlayListFilePathName, "wb");
                    if (lListFD)
                    {
                        GLOBAL_FWRITE(plStringBuf, 1, GLOBAL_STRLEN(plStringBuf), lListFD);
                        GLOBAL_FCLOSE(lListFD);
                        lRet = TRUE;
                    }
                    else
                    {
                        GLOBAL_TRACE(("Error! Open File [%s] Failed!!!\n", plPlayParam->m_pPlayListFilePathName));
                    }
                }
                else
                {
                    M3U8_MediaList *plMediaList;
                    M3U8_Segment lSegment;
                    for (i = 0; i < plPlayParam->m_MediaListNum; i++)
                    {
                        plMediaList = &plPlayList->m_pMediaListArray[i];
                        GLOBAL_STRCPY(plStringBuf, "#EXTM3U\r\n");
                        GLOBAL_STRCAT(plStringBuf, "#EXT-X-VERSION:3\r\n");
                        GLOBAL_SPRINTF((plFormatBuf, "#EXT-X-MEDIA-SEQUENCE:%d\r\n", plMediaList->m_ExtMediaSequence));
                        GLOBAL_STRCAT(plStringBuf, plFormatBuf);
                        GLOBAL_SPRINTF((plFormatBuf, "#EXT-X-TARGETDURATION:%d\r\n", plPlayParam->m_SegementDurationS));
                        GLOBAL_STRCAT(plStringBuf, plFormatBuf);

                        /*遍历LIST 生成segment列表*/
                        {
                            POSITION32 lPOS;
                            if (CAL_ListGetPos(plMediaList->m_SegementNodeList, &lPOS, TRUE))
                            {
                                while(lPOS != NULL)
                                {
                                    if (CAL_ListGetNext(plMediaList->m_SegementNodeList, &lPOS, &lSegment))
                                    {
                                        GLOBAL_SPRINTF((plFormatBuf, "#EXTINF:%.3f,\r\n", lSegment.m_Duration));
                                        GLOBAL_STRCAT(plStringBuf, plFormatBuf);
                                        GLOBAL_SPRINTF((plFormatBuf, "%s\r\n", lSegment.m_pSegmentFileName));
                                        GLOBAL_STRCAT(plStringBuf, plFormatBuf);
                                    }
                                }
                            }
                        }

                        lListFD = GLOBAL_FOPEN(plMediaList->m_pMediaListFilePathName, "wb");
                        if (lListFD)
                        {
                            GLOBAL_FWRITE(plStringBuf, 1, GLOBAL_STRLEN(plStringBuf), lListFD);
                            GLOBAL_FCLOSE(lListFD);
                            lRet = TRUE;
                        }
                        else
                        {
                            GLOBAL_TRACE(("Error! Open File [%s] Failed!!!\n", plMediaList->m_pMediaListFilePathName));
                        }
                    }
                }
            }
        }
    }
    return lRet;
}


/*EOF*/
