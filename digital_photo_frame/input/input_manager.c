#include <pthread.h>
#include <config.h>
#include <input_manager.h>
#include <string.h>

static PT_InputOpr g_ptInputOprHead;
static T_InputEvent g_tInputEvent;

static pthread_mutex_t g_tMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_tConVar = PTHREAD_COND_INITIALIZER;

int RegisterInputOpr(PT_InputOpr ptInputOpr)
{
	PT_InputOpr ptTmp;

	if (!g_ptInputOprHead)
	{
		g_ptInputOprHead   = ptInputOpr;
		ptInputOpr->ptNext = NULL;
	}
	else
	{
		ptTmp = g_ptInputOprHead;
		while (ptTmp->ptNext)
		{
			ptTmp = ptTmp->ptNext;
		}
		ptTmp->ptNext	  = ptInputOpr;
		ptInputOpr->ptNext = NULL;
	}

	return 0;
}


void ShowInputOpr(void)
{
	int i = 0;
	PT_InputOpr ptTmp = g_ptInputOprHead;

	while (ptTmp)
	{
		printf("%02d %s\n", i++, ptTmp->name);
		ptTmp = ptTmp->ptNext;
	}
}

static void* InputEventThreadFunction(void *pVoid)
{
    int iError;
    T_InputEvent tInputEvent;
    /*���庯��ָ��*/
    int  (*GetInputEvent)(PT_InputEvent inputevent);
    GetInputEvent = (int (*)(PT_InputEvent))pVoid;
    while(1){
       if (0 == (iError =  GetInputEvent(&tInputEvent)))
       {
            /*�������߳�,��tInputEvent��ֵ��ȫ�ֱ������������*/
            pthread_mutex_lock(&g_tMutex);
            g_tInputEvent = tInputEvent;
            pthread_cond_signal(&g_tConVar);
            pthread_mutex_unlock(&g_tMutex);
       }
    }
    return NULL;
}


int ALLInputDevicesInit(void)
{

    int ierror = 0;
    PT_InputOpr ptTmp = g_ptInputOprHead;

    while (ptTmp)
    {
        if(ierror |= ptTmp->DeviceInit())
            return ierror;
        /*�������߳�*/
        pthread_create(&ptTmp->tThreadID,NULL,InputEventThreadFunction,ptTmp->GetInputEvent);
        ptTmp = ptTmp->ptNext;
    }
    return ierror;
}

int GetInputEvent(PT_InputEvent ptInputEvent)
{
    /*����*/

    pthread_mutex_lock(&g_tMutex);
    pthread_cond_wait(&g_tConVar,&g_tMutex);

    /*�����Ѻ󣬷�������*/
    *ptInputEvent = g_tInputEvent;
    pthread_mutex_unlock(&g_tMutex);
    return 0;
}

int InputInit(void)
{
	int iError;

	iError = StdinInit();
    iError |= TouchScreenInit();

	return iError;
}


