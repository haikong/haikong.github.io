#include <config.h>
#include <debug_manager.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

static int StdutDbgInit(void);
static int StdutDbgExit(void);
static int StdoutDebugPrint(char* strData);


static T_DebugOpr g_tStdoutDbgOpr = {
	.name           = "stdout",
	.isCanUse       = 1,
	.DebugInit      = StdutDbgInit,
	.DebugExit      = StdutDbgExit,
	.DebugPrint     = StdoutDebugPrint,
};

static int StdutDbgInit(void)
{
    return 0;
}

static int StdutDbgExit(void)
{
    return 0;
}

static int StdoutDebugPrint(char* strData)
{
    /*直接使用printf打印信息就可以*/
    printf("%s",strData);
    return strlen(strData);
}

int StdoutInit(void)
{
	return RegisterDebugOpr(&g_tStdoutDbgOpr);
}

