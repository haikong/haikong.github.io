#include <input_manager.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <config.h>

static int StdinDeviceInit(void);

static int StdinDeviceExit(void);

static int StdinGetInputEvent(PT_InputEvent ptInputEvent);

static T_InputOpr   g_tStdinOpr = {
    .name           = "stdin",
    .DeviceInit     = StdinDeviceInit,
    .DeviceExit     = StdinDeviceExit,
    .GetInputEvent  = StdinGetInputEvent,
};

static int StdinDeviceInit(void)
{
    struct termios tTTYState;
    //get the terminal state
    tcgetattr(STDIN_FILENO,&tTTYState);
    //turn off canonical mode
    tTTYState.c_lflag &= ~ICANON;
    //minimum of number input read,only a char return
    tTTYState.c_cc[VMIN] = 1;
    //set the terminal attributes
    tcsetattr(STDIN_FILENO,TCSANOW,&tTTYState);

    return 0;
}

static int StdinDeviceExit(void)
{

    struct termios tTTYState;
    //get the terminal state
    tcgetattr(STDIN_FILENO,&tTTYState);
    //turn on canonical mode
    tTTYState.c_lflag |= ICANON;

    //set the terminal attributes
    tcsetattr(STDIN_FILENO,TCSANOW,&tTTYState);
    return 0;
}

static int StdinGetInputEvent(PT_InputEvent ptInputEvent)
{
    /*��������ݾͶ�ȡ�����ط��򷵻ش���*/
    char c;

	DBG_PRINTF("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    /*��������*/
    ptInputEvent->iType  = INPUT_TYPE_STDIN;
    /*fgetc�����ߣ�ֱ��������Ϊֹ*/
    c = fgetc(stdin);
    gettimeofday(&ptInputEvent->tTime,NULL);
#if 0
    switch(c){

    case 'u':
        ptInputEvent->iValue  = INPUT_VALUE_UP;
        break;
    case 'n':
        ptInputEvent->iValue  = INPUT_VALUE_DOWN;
        break;
    case 'q':
        ptInputEvent->iValue  = INPUT_VALUE_EXIT;
        break;
    default:
        ptInputEvent->iValue  = INPUT_VALUE_UNKOWN;
        break;

    }
 #endif
    return 0;
}



int StdinInit(void)
{
    return RegisterInputOpr(&g_tStdinOpr);
}
