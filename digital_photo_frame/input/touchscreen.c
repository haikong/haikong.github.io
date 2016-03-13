#include <input_manager.h>
#include <disp_manager.h>
#include <stdlib.h>
#include <config.h>
#include <tslib.h>

static struct tsdev *g_tTSDev = NULL;
static int g_iXres;
static int g_iYres;
static int g_iBpp;

static int TouchScreenDeviceInit(void);

static int TouchScreenDeviceExit(void);

static int TouchScreenGetInputEvent(PT_InputEvent ptInputEvent);

static T_InputOpr   g_tTouchScreenOpr = {
    .name           = "TouchScreen",
    .DeviceInit     = TouchScreenDeviceInit,
    .DeviceExit     = TouchScreenDeviceExit,
    .GetInputEvent  = TouchScreenGetInputEvent,
};

/*由于要用到LCD分辨率，故此函数要在SelectAndInitDisplay之后调用*/
static int TouchScreenDeviceInit(void)
{
    char    *pcTSName = NULL;
    if((pcTSName = getenv("TSLIB_TSDEVICE")) != NULL){
        /*以阻塞方式打开*/
        g_tTSDev  = ts_open(pcTSName,0);

    }else{
        g_tTSDev  = ts_open("/dev/input/event0",0);
    }

    if(!g_tTSDev){
        DBG_PRINTF("ts_open error.!\n");
        return -1;
    }
    if(ts_config(g_tTSDev)){
        DBG_PRINTF("ts_config error.!\n");
        return -1;
    }
    if(GetDispResolution(&g_iXres,&g_iYres,&g_iBpp))
        return -1;

    return 0;
}

static int TouchScreenDeviceExit(void)
{

    return 0;
}


static int TouchScreenGetInputEvent(PT_InputEvent ptInputEvent)
{

    struct ts_sample tSamp;
    int iRet;
    while(1){
        iRet = ts_read(g_tTSDev,&tSamp,1);/*如果没有数据就休眠*/
        if(iRet == 1)
        {

            ptInputEvent->tTime     = tSamp.tv;
            ptInputEvent->iType     = INPUT_TYPE_TOUCHSCREEN;
            ptInputEvent->iX        = tSamp.x;
            ptInputEvent->iY        = tSamp.y;
            ptInputEvent->iPressure = tSamp.pressure;

            return 0;

        }
        else
        {
            return -1;
        }
    }

    return 0;
}

int TouchScreenInit(void)
{
    return RegisterInputOpr(&g_tTouchScreenOpr);
}
