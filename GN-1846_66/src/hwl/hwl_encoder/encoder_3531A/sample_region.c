/******************************************************************************
  A simple program of Hisilicon HI3531 osd implementation.
  the flow as follows:
    1) init mpp system.
    2) start vi ( internal isp, ViDev 0, 2 vichn)                  
    3) start venc
    4) osd process, you can see video from some H264 streams files. the video will show as follows step:
        4.1) create some cover/osd regions
        4.2) display  cover/osd regions ( One Region -- Multi-VencGroup )
        4.3) change all vencGroups Regions' Layer
        4.4) change all vencGroups Regions' position
        4.5) change all vencGroups Regions' color
        4.6) change all vencGroups Regions' alpha (front and backgroud)
        4.7) load bmp form bmp-file to Region-0
        4.8) change BmpRegion-0
    6) stop venc
    7) stop vi and system.
  Copyright (C), 2010-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
    Modification:  2011-2 Created
******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "loadbmp.h"
#include "sample_comm.h"
#include "hi_tde_api.h"
#include "hi_tde_type.h"

/*Support load PNG image */
#define SAMPLE_RGN_SUPPORT_PNG				1

static HI_S32 SAMPLE_RGN_TransToTransparence(HI_S32 transparency)
{
	HI_S32 s32Transparence = 0;

	if ((transparency < 0) || (transparency > 100))
	{
		return 128;
	}
	
	s32Transparence = 128 - transparency;
	return s32Transparence;
}

static HI_S32 SAMPLE_RGN_DestroyRegion(RGN_HANDLE Handle, VENC_CHN lVencChn)
{
    HI_S32 s32Ret;
    MPP_CHN_S stChn;
    stChn.enModId  = HI_ID_VENC;
    stChn.s32DevId = 0;
    stChn.s32ChnId = lVencChn;
	
    s32Ret = HI_MPI_RGN_DetachFromChn(Handle, &stChn);
    if (s32Ret != HI_SUCCESS)
    {
        printf("region detach to chn %d fail. value=0x%x.\r\n", Handle, s32Ret);
    }

    s32Ret = HI_MPI_RGN_Destroy(Handle);
    if (s32Ret != HI_SUCCESS)
    {
        printf("destroy  pciv chn %d region fail. value=0x%x.\r\n", Handle, s32Ret);
    }       

    return HI_SUCCESS;
}

static HI_S32 SAMPLE_RGN_DestroyMosaicRegion(RGN_HANDLE Handle, VPSS_GRP lVpssGrp)
{
    HI_S32 s32Ret;
    MPP_CHN_S stChn;
    stChn.enModId  = HI_ID_VPSS;
    stChn.s32DevId = lVpssGrp;
    stChn.s32ChnId = 0;
	
    s32Ret = HI_MPI_RGN_DetachFromChn(Handle, &stChn);
    if (s32Ret != HI_SUCCESS)
    {
        printf("region detach to chn %d fail. value=0x%x.\r\n", Handle, s32Ret);
    }

    s32Ret = HI_MPI_RGN_Destroy(Handle);
    if (s32Ret != HI_SUCCESS)
    {
        printf("destroy  pciv chn %d region fail. value=0x%x.\r\n", Handle, s32Ret);
    }       

    return HI_SUCCESS;
}

/******************************************************************************
* funciton : osd region change color
******************************************************************************/
HI_S32 SAMPLE_RGN_ChgColor(RGN_HANDLE RgnHandle, HI_U32 u32Color)
{
    HI_S32 s32Ret;
    RGN_ATTR_S stRgnAttr;

    s32Ret = HI_MPI_RGN_GetAttr(RgnHandle, &stRgnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_RGN_GetAttr (%d)) failed with %#x!\n", RgnHandle, s32Ret);
        return HI_FAILURE;
    }

    stRgnAttr.unAttr.stOverlay.u32BgColor = u32Color;

    s32Ret = HI_MPI_RGN_SetAttr(RgnHandle, &stRgnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_RGN_SetAttr (%d)) failed with %#x!\n", RgnHandle, s32Ret);
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}
 
/******************************************************************************
* funciton : load bmp from file
******************************************************************************/
HI_S32 SAMPLE_RGN_LoadBmp(const char *filename, BITMAP_S *pstBitmap, HI_BOOL bFil, HI_U32 u16FilColor)
{
    OSD_SURFACE_S Surface;
    OSD_BITMAPFILEHEADER bmpFileHeader;
    OSD_BITMAPINFO bmpInfo;
    HI_U32 u32BytePerPix = 0;
    
    if(GetBmpInfo(filename, &bmpFileHeader, &bmpInfo) < 0)
    {
        printf("GetBmpInfo err!\n");
        return HI_FAILURE;
    }

    Surface.enColorFmt = OSD_COLOR_FMT_RGB1555;
    u32BytePerPix      = 2;
    
    pstBitmap->pData = malloc(u32BytePerPix * (bmpInfo.bmiHeader.biWidth) * (bmpInfo.bmiHeader.biHeight));
	
    if(NULL == pstBitmap->pData)
    {
        printf("malloc osd memroy err!\n");        
        return HI_FAILURE;
    }
    
    CreateSurfaceByBitMap(filename, &Surface, (HI_U8*)(pstBitmap->pData));
	
    pstBitmap->u32Width      = Surface.u16Width;
    pstBitmap->u32Height     = Surface.u16Height;
    pstBitmap->enPixelFormat = PIXEL_FORMAT_RGB_1555;
    
    
    int i,j;
    HI_U16 *pu16Temp;
    pu16Temp = (HI_U16*)pstBitmap->pData;
    
    if (bFil)
    {
        for (i=0; i<pstBitmap->u32Height; i++)
        {
            for (j=0; j<pstBitmap->u32Width; j++)
            {
                if (u16FilColor == *pu16Temp)
                {
                    *pu16Temp &= 0x7FFF;
                }
                pu16Temp++;
            }
        }
    }
        
    return HI_SUCCESS;
}

#if SAMPLE_RGN_SUPPORT_PNG
HI_S32 SAMPLE_RGN_UpdateCanvas(const char *filename, 
																					BITMAP_S *pstBitmap, 
																					HI_BOOL bFil, 
    																				HI_U32 u16FilColor, 
    																				SIZE_S *pstSize, 
    																				HI_U32 u32Stride, 
    																				PIXEL_FORMAT_E enPixelFmt)
{
    OSD_SURFACE_S Surface;
    OSD_BITMAPFILEHEADER bmpFileHeader;
    OSD_BITMAPINFO bmpInfo;


    if (PIXEL_FORMAT_RGB_1555 == enPixelFmt)
    {
        Surface.enColorFmt = OSD_COLOR_FMT_RGB1555;
    }
    else if (PIXEL_FORMAT_RGB_4444 == enPixelFmt)
    {
        Surface.enColorFmt = OSD_COLOR_FMT_RGB4444;
    }
    else if (PIXEL_FORMAT_RGB_8888 == enPixelFmt)
    {
        Surface.enColorFmt = OSD_COLOR_FMT_RGB8888;
    }
    else
    {
        printf("Pixel format is not support!\n");        
        return HI_FAILURE;
    }
	
    if(NULL == pstBitmap->pData)
    {
        printf("malloc osd memroy err!\n");        
        return HI_FAILURE;
    }
		
    CreateSurfaceByCanvas(filename, &Surface, (HI_U8*)(pstBitmap->pData), pstSize->u32Width, pstSize->u32Height, u32Stride);
	
    pstBitmap->u32Width  = Surface.u16Width;
    pstBitmap->u32Height = Surface.u16Height;
    
    if (PIXEL_FORMAT_RGB_1555 == enPixelFmt)
    {
        pstBitmap->enPixelFormat = PIXEL_FORMAT_RGB_1555;
    }
    else if (PIXEL_FORMAT_RGB_4444 == enPixelFmt)
    {
        pstBitmap->enPixelFormat = PIXEL_FORMAT_RGB_4444;
    }
    else if (PIXEL_FORMAT_RGB_8888 == enPixelFmt)
    {
        pstBitmap->enPixelFormat = PIXEL_FORMAT_RGB_8888;
    }
    
    return HI_SUCCESS;
}
#else
HI_S32 SAMPLE_RGN_UpdateCanvas(const char *filename, 
																					BITMAP_S *pstBitmap, 
																					HI_BOOL bFil, 
    																				HI_U32 u16FilColor, 
    																				SIZE_S *pstSize, 
    																				HI_U32 u32Stride, 
    																				PIXEL_FORMAT_E enPixelFmt)
{
    OSD_SURFACE_S Surface;
    OSD_BITMAPFILEHEADER bmpFileHeader;
    OSD_BITMAPINFO bmpInfo;

    if(GetBmpInfo(filename, &bmpFileHeader, &bmpInfo) < 0)
    {
        printf("GetBmpInfo err!\n");
        return HI_FAILURE;
    }

#if 1//xub added for debug
	printf("Read File:%s, File lengh = %d, offset = %d\r\n", 
				filename, 
				bmpFileHeader.bfSize,
				bmpFileHeader.bfOffBits);

	printf("Bitmap header info:\r\n");
	
	printf("Size = %d, Width = %d, Height = %d, Planes = %d, BitCount = %d, Compression = %d, SizeImage = %d, X = %d, Y = %d, ClrUsed = %d, ClrImportant = %d\r\n", 
				bmpInfo.bmiHeader.biSize,
				bmpInfo.bmiHeader.biWidth,
				bmpInfo.bmiHeader.biHeight,
				bmpInfo.bmiHeader.biPlanes,
				bmpInfo.bmiHeader.biBitCount,
				bmpInfo.bmiHeader.biCompression,
				bmpInfo.bmiHeader.biSizeImage,
				bmpInfo.bmiHeader.biXPelsPerMeter,
				bmpInfo.bmiHeader.biYPelsPerMeter,
				bmpInfo.bmiHeader.biClrUsed,
				bmpInfo.bmiHeader.biClrImportant);
#endif

    if (PIXEL_FORMAT_RGB_1555 == enPixelFmt)
    {
        Surface.enColorFmt = OSD_COLOR_FMT_RGB1555;
    }
    else if (PIXEL_FORMAT_RGB_4444 == enPixelFmt)
    {
        Surface.enColorFmt = OSD_COLOR_FMT_RGB4444;
    }
    else if (PIXEL_FORMAT_RGB_8888 == enPixelFmt)
    {
        Surface.enColorFmt = OSD_COLOR_FMT_RGB8888;
    }
    else
    {
        printf("Pixel format is not support!\n");        
        return HI_FAILURE;
    }
	
    if(NULL == pstBitmap->pData)
    {
        printf("malloc osd memroy err!\n");        
        return HI_FAILURE;
    }
		
    CreateSurfaceByCanvas(filename, &Surface, (HI_U8*)(pstBitmap->pData), pstSize->u32Width, pstSize->u32Height, u32Stride);
	
    pstBitmap->u32Width  = Surface.u16Width;
    pstBitmap->u32Height = Surface.u16Height;
    
    if (PIXEL_FORMAT_RGB_1555 == enPixelFmt)
    {
        pstBitmap->enPixelFormat = PIXEL_FORMAT_RGB_1555;
    }
    else if (PIXEL_FORMAT_RGB_4444 == enPixelFmt)
    {
        pstBitmap->enPixelFormat = PIXEL_FORMAT_RGB_4444;
    }
    else if (PIXEL_FORMAT_RGB_8888 == enPixelFmt)
    {
        pstBitmap->enPixelFormat = PIXEL_FORMAT_RGB_8888;
    }

    int i,j;
    HI_U16 *pu16Temp;
    pu16Temp = (HI_U16*)pstBitmap->pData;
    
    if (bFil)
    {
        for (i=0; i<pstBitmap->u32Height; i++)
        {
            for (j=0; j<pstBitmap->u32Width; j++)
            {
                if (u16FilColor == *pu16Temp)
                {
                    *pu16Temp &= 0x7FFF;
                }
                pu16Temp++;
            }
        }
    }
        
    return HI_SUCCESS;
}
#endif

HI_S32 SAMPLE_RGN_CreateOverlayForVenc(RGN_HANDLE Handle, 
																									SIZE_S region_size,
																									VENC_CHN lVencChn, 
																									HI_S32 s32X, 
																									HI_S32 s32Y,
																									HI_S32 transparence,
																									HI_BOOL enable)
{
    HI_S32 s32Ret;
    MPP_CHN_S stChn;
    RGN_ATTR_S stRgnAttr;
    RGN_CHN_ATTR_S stChnAttr;

	SAMPLE_PRT("SAMPLE_RGN_CreateOverlayForVenc(), Handle = %d, lVencChn = %d\r\n", Handle, lVencChn);
	
    /* Add cover to vpss group */
    stChn.enModId  = HI_ID_VENC;
    stChn.s32DevId = 0;
    stChn.s32ChnId = lVencChn;
        
    stRgnAttr.enType = OVERLAY_RGN;
    stRgnAttr.unAttr.stOverlay.enPixelFmt       = PIXEL_FORMAT_RGB_1555;
    stRgnAttr.unAttr.stOverlay.stSize.u32Width  = region_size.u32Width;
    stRgnAttr.unAttr.stOverlay.stSize.u32Height = region_size.u32Height;
    stRgnAttr.unAttr.stOverlay.u32BgColor       = 0x00007c00;

    s32Ret = HI_MPI_RGN_Create(Handle, &stRgnAttr);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_RGN_Create failed! s32Ret: 0x%x.\n", s32Ret);
        return HI_FAILURE;
    }

    stChnAttr.bShow  = enable;
    stChnAttr.enType = OVERLAY_RGN;
    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = s32X;
    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = s32Y;
    stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha   = 0;
    stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha   = transparence;
    stChnAttr.unChnAttr.stOverlayChn.u32Layer     = 0;

	 stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bQpDisable = HI_FALSE;
    stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
    stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;

    stChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Height = 128;
    stChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Width  = 128;
    stChnAttr.unChnAttr.stOverlayChn.stInvertColor.u32LumThresh = 128;
    stChnAttr.unChnAttr.stOverlayChn.stInvertColor.enChgMod     = LESSTHAN_LUM_THRESH;
    stChnAttr.unChnAttr.stOverlayChn.stInvertColor.bInvColEn    = HI_FALSE;

    s32Ret = HI_MPI_RGN_AttachToChn(Handle, &stChn, &stChnAttr);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_RGN_AttachToChn failed! s32Ret: 0x%x.(Chn = %d)\n", s32Ret, Handle);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
    
}

/******************************************************************************************
    function :  Venc OSD
    process steps:                                                          
      1) create some cover/osd regions                                  
      2) load bmp form bmp-file to Region
******************************************************************************************/
HI_S32 SAMPLE_RGN_AddOsdToVenc(VENC_CHN lVencChn,
																					  SIZE_S region_size,
																					  HI_S32 s32X, 
																					  HI_S32 s32Y,
																					  HI_S32 transparence,
																					  const char *filename,
																					  HI_BOOL enable)
{
    HI_S32 s32Ret = HI_SUCCESS;
    RGN_HANDLE OverlayHandle;
    RGN_ATTR_S stRgnAttrSet;
    RGN_CANVAS_INFO_S stCanvasInfo;
    BITMAP_S stBitmap;
    SIZE_S stSize;
	 HI_S32 s32Real_Transparence = 0;

	if (!filename)
	{
	     SAMPLE_PRT("SAMPLE_RGN_AddOsdToVenc failed! FileName is NULL.\n");
        return HI_FAILURE;		
	}

	s32Real_Transparence = SAMPLE_RGN_TransToTransparence(transparence);
	OverlayHandle    = lVencChn + 255;

#if 0	
	SAMPLE_PRT("--------Begin---------\r\n");
   SAMPLE_PRT("Chn = %d\r\n", lVencChn);
	SAMPLE_PRT("Display Size: Height = %d, Width = %d\r\n", region_size.u32Height, region_size.u32Width);
	SAMPLE_PRT("X = %d\r\n", s32X);
	SAMPLE_PRT("Y = %d\r\n", s32Y);
	SAMPLE_PRT("Transparence= %d\r\n", s32Real_Transparence);
	SAMPLE_PRT("enable= %d\r\n", enable);
   SAMPLE_PRT("----------------------\r\n");
#endif

	 /*************************************************
    step 1: Destroy region.
    *************************************************/
	SAMPLE_RGN_DestroyRegion(OverlayHandle, lVencChn);

    /*************************************************
    step 2: create region and attach to venc
    *************************************************/
    
    s32Ret = SAMPLE_RGN_CreateOverlayForVenc(OverlayHandle, region_size, lVencChn, s32X, s32Y, s32Real_Transparence, enable);
    if(HI_SUCCESS != s32Ret)
    {
	     SAMPLE_PRT("SAMPLE_RGN_CreateOverlayForVenc failed! s32Ret: 0x%x.\n", s32Ret);
        return HI_FAILURE;
    }

		
    /*************************************************
    step 3: load bitmap to region
    *************************************************/
    s32Ret = HI_MPI_RGN_GetAttr(OverlayHandle, &stRgnAttrSet);
    if(HI_SUCCESS != s32Ret)
    {
   	    SAMPLE_PRT("HI_MPI_RGN_GetAttr failed! s32Ret: 0x%x.\n", s32Ret);
    	 return HI_FAILURE;
    }
    
    s32Ret = HI_MPI_RGN_GetCanvasInfo(OverlayHandle, &stCanvasInfo);
    if(HI_SUCCESS != s32Ret)
    {
         SAMPLE_PRT("HI_MPI_RGN_GetCanvasInfo failed! s32Ret: 0x%x.\n", s32Ret);
    	  return HI_FAILURE;
    }
        
    stBitmap.pData   = (HI_VOID *)stCanvasInfo.u32VirtAddr;
    stSize.u32Width  = stCanvasInfo.stSize.u32Width;
    stSize.u32Height = stCanvasInfo.stSize.u32Height;

    SAMPLE_PRT("u32Width = %d, u32Height = %d (Virt Addr = 0x%x, u32Stride = %d)\n", 
    							stCanvasInfo.stSize.u32Width,
    							stCanvasInfo.stSize.u32Height,
    							stCanvasInfo.u32VirtAddr,
    							stCanvasInfo.u32Stride);

    s32Ret = SAMPLE_RGN_UpdateCanvas(filename, 
																		&stBitmap, 
																		HI_FALSE, 
																		0, 
																		&stSize, 
																		stCanvasInfo.u32Stride, 
        																stRgnAttrSet.unAttr.stOverlay.enPixelFmt);
    if(HI_SUCCESS != s32Ret)
    {
         SAMPLE_PRT("UpdateCanvas failed! s32Ret: 0x%x.\n", s32Ret);
    	  return HI_FAILURE;
    }

    s32Ret = HI_MPI_RGN_UpdateCanvas(OverlayHandle);
    if(HI_SUCCESS != s32Ret)
    {
	     SAMPLE_PRT("HI_MPI_RGN_UpdateCanvas failed! s32Ret: 0x%x.\n", s32Ret);
    	  return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 SAMPLE_RGN_CreateMosaicForVpss(RGN_HANDLE Handle, 
																									VPSS_GRP lVpssGrp,
																							 		HI_S32 s32Width,
																							 		HI_S32 s32Height,
																									HI_S32 s32X, 
																									HI_S32 s32Y,
																									MOSAIC_BLK_SIZE_E BlkSize,
																									HI_BOOL enable)
{
    HI_S32 i;
    HI_S32 s32Ret;
    MPP_CHN_S stChn;
    RGN_ATTR_S stRgnAttr;
    RGN_CHN_ATTR_S stChnAttr;
    
    /* Add Mosaic to vpss group */
    stChn.enModId  = HI_ID_VPSS;
    stChn.s32DevId = lVpssGrp;
    stChn.s32ChnId = 0;
        
    /* Create Mosaic and attach to vpss group */
	stRgnAttr.enType = MOSAIC_RGN;

	s32Ret = HI_MPI_RGN_Create(Handle, &stRgnAttr);
	if(s32Ret != HI_SUCCESS)
	{
	    printf("HI_MPI_RGN_Create for Mosaic fail! s32Ret: 0x%x.(vpss group = %d)\n", s32Ret, lVpssGrp);
	    return s32Ret;
	}

	stChnAttr.bShow  = enable;
	stChnAttr.enType = MOSAIC_RGN;
	stChnAttr.unChnAttr.stMosaicChn.enBlkSize = BlkSize;
	stChnAttr.unChnAttr.stMosaicChn.stRect.s32X      = s32X;
	stChnAttr.unChnAttr.stMosaicChn.stRect.s32Y      = s32Y;
	stChnAttr.unChnAttr.stMosaicChn.stRect.u32Height = s32Height ;
	stChnAttr.unChnAttr.stMosaicChn.stRect.u32Width  = s32Width ;
	stChnAttr.unChnAttr.stMosaicChn.u32Layer         = 0;

	s32Ret = HI_MPI_RGN_AttachToChn(Handle, &stChn, &stChnAttr);
	if(s32Ret != HI_SUCCESS)
	{
	    printf("HI_MPI_RGN_AttachToChn for Mosaic fail! s32Ret: 0x%x. (vpss group = %d)\n", s32Ret, lVpssGrp);
	    return s32Ret;
	}

    return HI_SUCCESS;
}

HI_S32 SAMPLE_RGN_AddMosaicToVpss(VPSS_GRP lVpssGrp,
																							 HI_S32 s32Width,
																							 HI_S32 s32Height,
																							 HI_S32 s32X, 
																							 HI_S32 s32Y,
																							 MOSAIC_BLK_SIZE_E BlkSize,
																							 HI_BOOL enable)
{
	HI_S32	 result;
	RGN_HANDLE MosaicHandle;

	MosaicHandle = 275 + lVpssGrp;

	SAMPLE_RGN_DestroyMosaicRegion(MosaicHandle, lVpssGrp);

	if (enable == 1)
	{
		SAMPLE_PRT("*********Begin********\r\n");
		SAMPLE_PRT("Chn = %d (handle = %d)\r\n", lVpssGrp, MosaicHandle);
		SAMPLE_PRT("Display Size: Height = %d, Width = %d\r\n", s32Height, s32Width);
		SAMPLE_PRT("X = %d\r\n", s32X);
		SAMPLE_PRT("Y = %d\r\n", s32Y);
		SAMPLE_PRT("Block size = %d\r\n", BlkSize);
		SAMPLE_PRT("enable= %d\r\n", enable);
		SAMPLE_PRT("**********************\r\n");
	}
	 
	result = SAMPLE_RGN_CreateMosaicForVpss(MosaicHandle, lVpssGrp, s32Width, s32Height, s32X, s32Y, BlkSize, enable);	
	return result;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
