#include <config.h>
#include <page_manager.h>

static void ExplorePageRun(void);
static int  ExplorePageGetInputEvent(PT_InputEvent inputevent);
static int  ExplorePagePrepare(void);


static T_PageAction g_tExplorePageAction = {
    .name           = "explore",
    .Run            = ExplorePageRun,
    .GetInputEvent  = ExplorePageGetInputEvent,
    .Prepare        = ExplorePagePrepare,
};

static void ExplorePageRun(void)
{
    /*显示页面*/

    /*创建prepare线程*/

    /*调用GetinputEvent获得输入事件，进而处理*/
    while(1){
        InputEvent = ExplorePageGetInputEvent();
        switch(InputEvent){
            case "向上":{
                /*判断是否是顶层*/
                if(isTopLevel)
                    return 0;
                else{
                    /*显示上一个目录*/
                }
                break;
            }
            case "选择":{
               if(isSelectDir)
               {
                    /*显示下一个目录*/
               }
               else
               {
                    /*保存当前页面*/
                    StorePage();
                    Page("browse")->Run();
                    /*恢复之前页面*/
                    RestorePage();
               }
                break;
            }
            case "上页":{
                /*显示上一页*/
                break;
            }
            case "下页":{
                /*显示下一页*/
                break;
            }
            default:
                break;
        }

    }
}

int ExplorePageInit(void)
{
    return RegisterPageAction(&g_tExplorePageAction);
}

