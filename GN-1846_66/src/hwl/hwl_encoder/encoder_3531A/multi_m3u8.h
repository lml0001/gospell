//
// Created by Administrator on 2021/2/23 0023.
//

#ifndef GN_1846_66_MULTI_M3U8_H
#define GN_1846_66_MULTI_M3U8_H
/* Includes-------------------------------------------------------------------- */
#include "global_def.h"
/* Macro ---------------------------------------------------------------------- */
#define M3U8_MAX_MULTI_BITRATE_NUMBER					(8)
#define M3U8_MAX_NAME_BUF_SIZE							(64)
/* Types Define (struct typedef)----------------------------------------------- */
typedef struct
{
    CHAR_T				m_pSegmentFileName[GLOBAL_MAX_PATH];//Segment名字。例如：AAA_1020.ts
    F32					m_Duration;
}M3U8_Segment;

typedef struct
{
    S32					m_MaxPlayListNum;
}M3U8_InitParam;

typedef struct
{
    CHAR_T				m_pMediaListRelativePathName[M3U8_MAX_NAME_BUF_SIZE];//第二级索引相对路径（例如/2000K/2000kbps.m3u8）则推荐的分块文件名 2000kbps-0001.ts
    S32					m_BandwidthBPS;//当前MediaList的目标带宽
    S32					m_ResolutionWidth;//分辨率，用于显示
    S32					m_ResolutionHeight;
}M3U8_MediaListParam;

typedef struct
{
    CHAR_T				m_pPlayListFilePathName[GLOBAL_MAX_PATH];//m3u8文件系统绝对路径，包括扩展名。例如：/tmp/web/tmp/1.m3u8
    S32					m_SegementNum;//保持的分块的个数，当前分块个数大于这个数时，则会删除最先加入的分块
    S32					m_SegementDurationS;//单个切片文件的最大时长 单位S

    M3U8_MediaListParam	m_pMediaListParam[M3U8_MAX_MULTI_BITRATE_NUMBER];
    S32					m_MediaListNum;//多码率的个数
}M3U8_PlayListParam;
/* Functions prototypes ------------------------------------------------------- */

HANDLE32 M3U8_Create(M3U8_InitParam *pInitParam);
void M3U8_SetupPlayList(HANDLE32 Handle, S32 PlaySlot, M3U8_PlayListParam *pStreamParam, BOOL bEnable);
BOOL M3U8_InsertSegment(HANDLE32 Handle, S32 PlaySlot, S32 MediaSlot, M3U8_Segment *pSegment);
BOOL M3U8_GetSegementFileName(HANDLE32 Handle, S32 PlaySlot, S32 MediaSlot, CHAR_T *pPathName, BOOL bURL);
BOOL M3U8_UpdateList(HANDLE32 Handle, S32 PlaySlot, BOOL bPlayList);
void M3U8_Destroy(HANDLE32 Handle);
#endif //GN_1846_66_MULTI_M3U8_H
