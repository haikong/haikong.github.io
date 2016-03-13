
#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <sys/time.h>
#include <pthread.h>

#define INPUT_TYPE_STDIN        0
#define INPUT_TYPE_TOUCHSCREEN  1

#define INPUT_VALUE_UP              0
#define INPUT_VALUE_DOWN            1
#define INPUT_VALUE_EXIT            2
#define INPUT_VALUE_UNKOWN            -1

typedef struct _InputEvent{
    struct  timeval tTime;
    int     iType;  /*stdin,touchscreen*/
    int     iX;
    int     iY;
    int     iKey;
    int     iPressure;
}T_InputEvent,*PT_InputEvent;

typedef struct _InputOpr{
    char    *name;
    pthread_t tThreadID;
    int     (*DeviceInit)(void);
    int     (*DeviceExit)(void);
    int     (*GetInputEvent)(PT_InputEvent inputevent);
    struct _InputOpr *ptNext;
}T_InputOpr,*PT_InputOpr;

int ALLInputDevicesInit(void);
int InputInit(void);
void ShowInputOpr(void);
int RegisterInputOpr(PT_InputOpr ptInputOpr);
int GetInputEvent(PT_InputEvent ptInputEvent);
int StdinInit(void);
int TouchScreenInit(void);

#endif /* _INPUT_MANAGER_H */

