#include <pic_operation.h>
#include <config.h>
#include <stdlib.h>
#include <string.h>
#include <picfmt_manager.h>
#include <file.h>

#pragma pack(push)
#pragma pack(1)

typedef struct tagBITMAPFILEHEADER { /* bmfh */
    unsigned short bfType;
    unsigned long  bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned long  bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER { /* bmih */
    unsigned long   biSize;
    unsigned long   biWidth;
    unsigned long   biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount;
    unsigned long   biCompression;
    unsigned long   biSizeImage;
    unsigned long   biXPelsPerMeter;
    unsigned long   biYPelsPerMeter;
    unsigned long   biClrUsed;
    unsigned long   biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)


static int   isBMPFormat(PT_FileMap ptFileMap);
static int   GetPixelDatasFrmBMP(PT_FileMap ptFileMap,PT_PixelDatas ptPixelDatas);
static int   FreePixelDatasForBMP(PT_PixelDatas ptPixelDatas);


static T_PicFileParser g_tBMPParser = {
    .name           = "bmp",
    .isSupport      = isBMPFormat,
    .GetPixelDatas  = GetPixelDatasFrmBMP,
    .FreePixelDatas = FreePixelDatasForBMP,
};


static int   isBMPFormat(PT_FileMap ptFileMap)
{
    unsigned char *aFileHead = ptFileMap->pucFileMapMem;
    DBG_PRINTF("aFileHead[0] = %x,aFileHead[1] = %x\n",aFileHead[0],aFileHead[1]);
    if((aFileHead[0] != 0x42) || (aFileHead[1] != 0x4d))
        return 0;
    else
        return 1;

}

static int CovertOneLine(int iWidth,int iSrcBpp,int iDestBpp,unsigned char* pucSrcDatas,unsigned char* pucDestDatas )
{
    int i,pos = 0;
    unsigned int dwRed;
    unsigned int dwDreen;
    unsigned int dwBlue;
    unsigned int dwColor;
    unsigned short *pwDstDatas16bpp = (unsigned short *)pucDestDatas;
    unsigned int *pwDstDatas32bpp = (unsigned int *)pucDestDatas;


    if(iSrcBpp != 24)
        return -1;
    if(iDestBpp == 24){
        memcpy(pucDestDatas,pucSrcDatas,iWidth * 3);
    }
    else{
        for(i = 0;i < iWidth;i++){
            dwBlue  = pucSrcDatas[pos++];
            dwDreen = pucSrcDatas[pos++];
            dwRed   = pucSrcDatas[pos++];
            switch(iDestBpp){
                case 32:{
                    dwColor = (dwRed << 16) | (dwDreen << 8) | (dwBlue << 0);
                    *pwDstDatas32bpp = dwColor;
                    pwDstDatas32bpp++;
                }
                    break;
                case 16:{
                    dwBlue  = dwBlue >> 3;
                    dwDreen = dwDreen >> 2;
                    dwRed   = dwRed >> 3;
                    dwColor = (dwRed << 11) | (dwDreen << 5) | (dwBlue);
                    *pwDstDatas16bpp = dwColor;
                    pwDstDatas16bpp++;
                }
                    break;
                default:
                    return -1;
                    break;
            }
        }
    }
    return  0;
}
/*
*ptPixelDatas->iBpp 输入的参数可转换
*/
static int   GetPixelDatasFrmBMP(PT_FileMap ptFileMap,PT_PixelDatas ptPixelDatas)
{
    BITMAPFILEHEADER* ptBITMAPFILEHEADER ;
    BITMAPINFOHEADER* ptBITMAPINFOHEADER ;
    int iWidth;
    int iHeight;
    int iBMPBpp;
    int iRet,y;
    unsigned char* pucSrc;
    unsigned char* pucDest;
    int iLineWidthAlign;
    int iLineWidthReal;
    unsigned char *aFileHead;

    aFileHead = ptFileMap->pucFileMapMem;

    ptBITMAPFILEHEADER = (BITMAPFILEHEADER*)aFileHead;
    ptBITMAPINFOHEADER = (BITMAPINFOHEADER*)(aFileHead + sizeof(BITMAPFILEHEADER));

    iWidth = ptBITMAPINFOHEADER->biWidth;
    iHeight = ptBITMAPINFOHEADER->biHeight;
    iBMPBpp = ptBITMAPINFOHEADER->biBitCount;

    if(iBMPBpp != 24)
        return -1;
    ptPixelDatas->iWith = iWidth;
    ptPixelDatas->iHeight = iHeight;
    //ptPixelDatas->iBpp = iBMPBpp;
    ptPixelDatas->iLineByteCnt = iWidth * ptPixelDatas->iBpp /8;
    ptPixelDatas->iTotalBytes   = ptPixelDatas->iHeight * ptPixelDatas->iLineByteCnt;
	ptPixelDatas->aucPixelDatas = malloc(ptPixelDatas->iTotalBytes);

    if(NULL == ptPixelDatas->aucPixelDatas)
        return -1;

    iLineWidthReal = iWidth * iBMPBpp /8;
    iLineWidthAlign = (iLineWidthReal + 3) & ~0x3;/*向4取整*/
    pucSrc = aFileHead + ptBITMAPFILEHEADER->bfOffBits;
    pucSrc = pucSrc + (iHeight - 1) * iLineWidthAlign;
    pucDest = ptPixelDatas->aucPixelDatas;
    for(y = 0 ;y < iHeight;y++)
    {
        //memcpy(pucDest,pucSrc,iLineWidthReal);
        iRet = CovertOneLine(iWidth,iBMPBpp,ptPixelDatas->iBpp,pucSrc,pucDest);
        if(iRet){
            DBG_PRINTF("CovertOneLine error.\n");
            free(ptPixelDatas->aucPixelDatas);
            return -1;
        }
        pucSrc  -= iLineWidthAlign;
        pucDest += ptPixelDatas->iLineByteCnt ;
    }
    return 0;
}

static int   FreePixelDatasForBMP(PT_PixelDatas ptPixelDatas)
{
    free(ptPixelDatas->aucPixelDatas);
    return 0;
}

int BMPParserInit(void)
{
	return RegisterPicFileParser(&g_tBMPParser);
}

