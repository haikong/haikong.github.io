#include <s3c2440_addr.h>
#include <dma.h>
#include <serial.h>

#if 0
/*
* DMA initialize
*
*/
void dma_init(void)
{
   //配置DMA
   rDISRC2 = (U32)buffer;                     //DMA的源基址为音频数据数组的首地址
   /*DMA 2初始源控制寄存器
   *	位[1]是用于源位置的选择;
   *		0：源在系统总线（AHB）上
   *		1：源在外设总线（APB）上
   *	位[0]是用于地址增加的选择
   *		0：增加
   *		1：固定
   */
   rDISRCC2 = (0<<1)|(0<<0);        //AHB，源地址递增
   rDIDST2 = (U32)IISFIFO;          //DMA的目的基址为IIS的FIFO
   /*DMA 0初始目标控制寄存器
   	*	[2]当设置了自动再加载时发生中断的时间选择
   	*		0：在TC到达0时发生中断
   	*		1：在执行完自动再加载后发生中断
    *	[1]是用于目标位置的选择
    *		0：目标在系统总线（AHB）上
    *		1：目标在外设总线（APB）上
    *	[0]是用于地址增加的选择
    *		0：增加
    *		1：固定
    */
   rDIDSTC2 = (0<<2)| (1<<1)|(1<<0);          //当传输计数值为0时中断，APB，目的地址不变
   if (result == 0)                     //所传输的字节数没有超出DMA的最大传输范围
   {
          flag = 0;                //清标志，表示没有超出范围，进入DMA中断后结束DMA操作
          //握手模式，PCLK同步，传输计数中断，单元传输，单步服务模式，IISSDO，
          //硬件请求模式，非自动重载，半字，
          rDCON2 = (1<<31) | (0<<30) | (1<<29) | (0<<28) | (0<<27) | (0<<24) | (1<<23) | (1<<22) | (1<<20) | (remainder);

   }
   else                       //所传输的字节数超出了DMA的最大传输范围
   {
          flag = 1;                //置标志，表示超出范围
          rDCON2 = (1<<31) | (0<<30) | (1<<29) | (0<<28) | (0<<27) | (0<<24) | (1<<23) | (1<<22) | (1<<20) | (0xfffff);
   }
   rDMASKTRIG2=(0<<2)|(1<<1)|0;      //不停止DMA，DMA通道开启，非软件触发
}

void dma_irq_Handle(void)
{
	rSRCPND |= 0x1<<19;
	rINTPND |= 0x1<<19;

	if (flag == 0)                //DMA传输完毕
	{
	   	rIISCON = 0x0;            //关闭IIS
		rIISFCON = 0x0;          //清IIS的FIFO
	    rDMASKTRIG2=1<<2;               //停止DMA
	    play_state = 0;                     //清播放标志
	}
	else                       //DMA没有传输完毕，继续传输
	{
       result --;         //商递减
       rDISRC2 += 0x200000;               //DMA源基址递增。因为传输的数据是半字，所以这里递增0x200000
       if (result == 0 )             //只剩下余数部分需要传输
       {
          rDCON2=(rDCON2&(~0xfffff))|(remainder);            //需要重新设置传输计数值
          flag=0;           //清标志
       }
       rDMASKTRIG2=(0<<2)|(1<<1)|0;      //需要重新设置DMA通道的开启
	}
}
#endif

void DMA_memcpy(void* p_dst,const void* p_src,int len)
{
	/*DMA 0初始源寄存器 [30:0]*/
    DISRC0  = ((int)p_src & 0x7FFFFFFF);
   /*DMA 0初始源控制寄存器
   *	位[1]是用于源位置的选择;
   *		0：源在系统总线（AHB）上
   *		1：源在外设总线（APB）上
   *	位[0]是用于地址增加的选择
   *		0：增加
   *		1：固定
   */
    DISRCC0 = 0;
    /*DMA 0初始目标寄存器 [30:0]*/
    DIDST0  = ((int)p_dst & 0x7FFFFFFF);
   /*DMA 0初始目标控制寄存器
   	*	[2]当设置了自动再加载时发生中断的时间选择
   	*		0：在TC到达0时发生中断
   	*		1：在执行完自动再加载后发生中断
    *	[1]是用于目标位置的选择
    *		0：目标在系统总线（AHB）上
    *		1：目标在外设总线（APB）上
    *	[0]是用于地址增加的选择
    *		0：增加
    *		1：固定
    */
    DIDSTC0 = 0;
   	/*DMA 0控制寄存器
   	*	[31]查询模式和握手模式之间选择其中之一:
   	*		0：选择查询模式 ;
   	*		1：选择握手模式
   	*	[30]DREQ/DACK的同步化选择:
   	*		0：DREQ和DACK同步于PCLK（APB时钟）
   	*		1：DREQ和DACK同步于HCLK（AHB时钟）
   	*	[29]CURR_TC (终点计数)的中断使能/禁止设置:
   	*		0：禁止CURR_TC中断。用户必须观察状态寄存器的传输计数（即定时查询）
   	*		1：当所有传输完成产生中断请求（即CURR_TC变为0）
   	*	[27]单服务模式和全服务模式之间的服务模式选择:
   	*		0：选择每次原子传输（单次或突发4）后DMA停止和等待其它DMA请求的单服务模式
   	*		1：选择传输计数达到0前重复请求得到原子传输的全服务模式。此模式不需要额外请求
   	*	[22]设置再加载开/关选项
   	*		0：当传输计数的当前值变为0时（即执行了所有请求的传输）执行自动再加载
   	*		1：当传输计数的当前值变为0时DMA通道（DMA  REQ）关闭。设置通道开/关位（DMASKTRIGn[1]）为0（DREQ关闭）来预防非预定的新DMA操作的进一步开始
   	*	[21:20]要传输的数据大小
   	*		00 = 字节  01 = 半字  10 = 字  11 = 保留
   	*	[19:0]初始传输计数（或传输节拍）
   	*		注意其实际传输的字节数由以下等式计算得到：SZ×TSZ×TC。此处
	*		DSZ，TSZ（1或4）和TC分别代表数据大小DCONn[21:20]，传输大
	*		小DCONn[28]和初始传输计数。当且仅当CURR_TC为0并且DMA
	*		ACK为1时该值将被加载到CURR_TC。
   	*/
    DCON0 = (len & 0xFFFFF) + (2 << 20) + (1 << 22) + (1 << 27) + (1 << 29) + (1 << 30) + (1U << 31);
    /*DMA 0触发屏蔽寄存器
    *	[2]停止DMA的运行
    *		1：一旦结束了当前的原子传输就停止DMA。如果当前无运行的原子传输，立即停止DMA。
	*			CURR_TC，CURR_SRC和CURR_DST将变为0。
    *	[1]DMA通道开/关位
    *		0：关闭DMA通道。（忽略此通道DMA请求）
    *		1：打开DMA通道并且处理此通道DMA请求。
    *	[0]软件请求模式中触发DMA通道
    *		1：为此控制器请求DMA操作
    */
    DMASKTRIG0 = 0x03;
    /*中断屏蔽（INTMSK）寄存器决定屏蔽哪个中断源。被屏蔽的中断源将不会服务
    *	0 = 中断服务可用  1 = 屏蔽中断服务
    */
    INTMSK &= ~(1 << 17);//enable the dma0 interrupt
}


/**************************************************************
*              DMA初始化
*p_dst:DMA transfer destination address
*p_src:DMA transfer source address
*len  :size
*mode :transfer mode (ie:Uart IIS...)
*m_flag:transfer mode flag (ie:Uart IIS...source)
**************************************************************/
void Dma_init(void* p_dst,const void* p_src,unsigned int len,unsigned char mode,unsigned char m_flag)
{
  DISRC0    =(unsigned long)p_src & 0x7FFFFFFF;					//数据source地址
  DISRCC0   |=((0<<1)|(0<<0));				                    //[1]系统总线,[0]地址将根据单次和突发模式中每次传输后其数据大小而增加
  DIDST0    =(unsigned long)p_dst & 0x7FFFFFFF;					//传输目标地址
  DIDSTC0   |=((0<<2)|(1<<1)|(1<<0));		                    //[2]在TC到达0时发生中断, [1] APB(外设总线上)  , [0]传输后地址总线不变

  DCON0     |=  (1U<<31)|(0<<30)|(1<<29)|(0<<28)|(0<<27)|(mode << 24)|(m_flag << 23)|(1<<22)|(0<<20)|(len & 0xFFFFF);
  //[31] 握手模式; [30]同步PCLK=50M(APB时钟< 外设 >); [29]当所有传输完成产生中断请求（即CURR_TC变为0）;[28]执行一个单元传输;
  //[27]选择每次原子传输（单次或突发4）后DMA停止和等待其它DMA请求的单服务模式; [24]UART0; [23]选择DMA源触发DMA操作;
  //[22]当传输计数的当前值变为0时DMA通道（DMA  REQ）关闭; [20]字节(每次一个字节传输);[19:0]初始传输计数15，15=SendBuffer字符数组长度;

  DMASKTRIG0=(0<<2)|(1<<1)|(0<<0);  //[1]打开DMA通道并且处理此通道DMA请求
}


/**************************************************************
|						  DMA子函数
| setting DMA0 UART0
**************************************************************/
void DMA_UART(void* p_dst,const void* p_src,unsigned int len,unsigned char mode,unsigned char m_flag)
{
  	Dma_init(p_dst,p_src,len,mode,m_flag);//DMA初始化
  	/*enable the dma interruput*/
	INTMSK &= ~(1 << 17);
}


#if 0

void Main(void)
{
  U32 mpll_val = 0,consoleNum;
  Port_Init();

  mpll_val = (92<<12)|(1<<4)|(1);

  //init FCLK=400M,
  ChangeMPllValue((mpll_val>>12)&0xff, (mpll_val>>4)&0x3f, mpll_val&3);
  ChangeClockDivider(14, 12);   //the result of rCLKDIVN [0:1:0:1] 3-0 bit
  cal_cpu_bus_clk();			//HCLK=100M   PCLK=50M

  consoleNum = 0;   // Uart 1 select for debug.
  Uart_Init( 0,115200 );
  Uart_Select( consoleNum );

  MMU_Init();//中断映射地址初始化
  Beep(2000, 100);

  DMA_UART();				  //DMA方式实现Uart（串口）通信

}
DMA_uart.c


/**************************************************************
              DMA中断
**************************************************************/
void __irq Dma0_isr(){
  rSRCPND|=0x1<<17;   //清除中断挂起状态
  rINTPND|=0x1<<17;
  Uart_Printf("\n	   ***DMA TO Uart finished***\n");
  Beep(2000,100);
  rGPBCON=0x015400;
  rGPBDAT=0x6<<5;
}

/**************************************************************
|						  DMA子函数
|  需设置2440lib.c里   rUCON0  |=((1<<0) | (1<<3) | (2<<10) );
**************************************************************/
void DMA_UART(){
  Uart_Printf("\n	   ***HELLO DMA TO Uart ***\n");
  Delay(1000);

  Dma_init();//DMA初始化
  rINTMSK &=~(1<<17);  //开 dma0 中断
  pISR_DMA0=(U32)Dma0_isr;


  while(1){
  }
}
#endif

