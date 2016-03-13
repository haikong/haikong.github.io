#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <config.h>
#include <encoding_manager.h>
#include <fonts_manager.h>
#include <disp_manager.h>
#include <string.h>
#include <input_manager.h>
#include <sys/time.h>
#include <tslib.h>
#include <sys/stat.h>
#include <debug_manager.h>
#include <page_manager.h>
#include <pic_operation.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <picfmt_manager.h>

/* ./digital <bmpfile>*/
int main(int argc, char **argv)
{
    int iRet;


    iRet = DebugInit();
    if(iRet)
    {
        DBG_PRINTF("DebugInit error.\n");
        return -1;
    }

    iRet = InitDebugChanel();

    if(iRet)
    {
        DBG_PRINTF("InitDebugChanel error.\n");
        return -1;
    }
	if (argc != 2)
	{
		DBG_PRINTF("Usage:\n");
		DBG_PRINTF("%s <freetype_file>\n", argv[0]);
		return 0;
	}

    iRet = DisplayInit();

    if(iRet)
    {
        DBG_PRINTF("DisplayInit error.\n");
        return -1;
    }
    iRet = SelectAndInitDefaultDisplayDev("fb");
    if(iRet)
    {
        DBG_PRINTF("SelectAndInitDefaultDisplayDev error.\n");
        return -1;
    }

    iRet = AllocVideoMem(5);

    if(iRet)
    {
        DBG_PRINTF("AllocVideoMem error.\n");
        return -1;
    }
    iRet = InitDebugChanel();

    if(iRet)
    {
        DBG_PRINTF("InitDebugChanel error.\n");
        return -1;
    }
    iRet = InputInit();
    if(iRet)
    {
        DBG_PRINTF("InputInit error.\n");
        return -1;
    }

    iRet = ALLInputDevicesInit();

    if(iRet)
    {
        DBG_PRINTF("ALLInputDevicesInit error.\n");
        return -1;
    }
    /*×¢²á±àÂëÎÄ¼þ*/
    EncodingInit();

	iRet = FontsInit();
	if (iRet)
	{
		DBG_PRINTF("FontsInit error!\n");
        return -1;
	}
    ShowFontOpr();
	iRet = SetFontsDetail("freetype", argv[1], 32);
	if (iRet)
	{
		DBG_PRINTF("SetFontsDetail error!\n");
        return -1;
	}

    iRet = PagesInit();

    if(iRet)
    {
        DBG_PRINTF("PagesInit error.\n");
        return -1;
    }
    PicFmtsInit();
    Page("main")->Run(NULL);
    return 0;
}

