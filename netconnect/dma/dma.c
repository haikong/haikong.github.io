#include <s3c2440_addr.h>
#include <dma.h>
#include <serial.h>
#include "stdio.h"
#include <intirq.h>

#if 0
/*
* DMA initialize
*
*/
void dma_init(void)
{
   //����DMA
   rDISRC2 = (U32)buffer;                     //DMA��Դ��ַΪ��Ƶ����������׵�ַ
   /*DMA 2��ʼԴ���ƼĴ���
   *	λ[1]������Դλ�õ�ѡ��;
   *		0��Դ��ϵͳ���ߣ�AHB����
   *		1��Դ���������ߣ�APB����
   *	λ[0]�����ڵ�ַ���ӵ�ѡ��
   *		0������
   *		1���̶�
   */
   rDISRCC2 = (0<<1)|(0<<0);        //AHB��Դ��ַ����
   rDIDST2 = (U32)IISFIFO;          //DMA��Ŀ�Ļ�ַΪIIS��FIFO
   /*DMA 0��ʼĿ����ƼĴ���
   	*	[2]���������Զ��ټ���ʱ�����жϵ�ʱ��ѡ��
   	*		0����TC����0ʱ�����ж�
   	*		1����ִ�����Զ��ټ��غ����ж�
    *	[1]������Ŀ��λ�õ�ѡ��
    *		0��Ŀ����ϵͳ���ߣ�AHB����
    *		1��Ŀ�����������ߣ�APB����
    *	[0]�����ڵ�ַ���ӵ�ѡ��
    *		0������
    *		1���̶�
    */
   rDIDSTC2 = (0<<2)| (1<<1)|(1<<0);          //���������ֵΪ0ʱ�жϣ�APB��Ŀ�ĵ�ַ����
   if (result == 0)                     //��������ֽ���û�г���DMA������䷶Χ
   {
          flag = 0;                //���־����ʾû�г�����Χ������DMA�жϺ����DMA����
          //����ģʽ��PCLKͬ������������жϣ���Ԫ���䣬��������ģʽ��IISSDO��
          //Ӳ������ģʽ�����Զ����أ����֣�
          rDCON2 = (1<<31) | (0<<30) | (1<<29) | (0<<28) | (0<<27) | (0<<24) | (1<<23) | (1<<22) | (1<<20) | (remainder);

   }
   else                       //��������ֽ���������DMA������䷶Χ
   {
          flag = 1;                //�ñ�־����ʾ������Χ
          rDCON2 = (1<<31) | (0<<30) | (1<<29) | (0<<28) | (0<<27) | (0<<24) | (1<<23) | (1<<22) | (1<<20) | (0xfffff);
   }
   rDMASKTRIG2=(0<<2)|(1<<1)|0;      //��ֹͣDMA��DMAͨ�����������������
}

void dma_irq_Handle(void)
{
	rSRCPND |= 0x1<<19;
	rINTPND |= 0x1<<19;

	if (flag == 0)                //DMA�������
	{
	   	rIISCON = 0x0;            //�ر�IIS
		rIISFCON = 0x0;          //��IIS��FIFO
	    rDMASKTRIG2=1<<2;               //ֹͣDMA
	    play_state = 0;                     //�岥�ű�־
	}
	else                       //DMAû�д�����ϣ���������
	{
       result --;         //�̵ݼ�
       rDISRC2 += 0x200000;               //DMAԴ��ַ��������Ϊ����������ǰ��֣������������0x200000
       if (result == 0 )             //ֻʣ������������Ҫ����
       {
          rDCON2=(rDCON2&(~0xfffff))|(remainder);            //��Ҫ�������ô������ֵ
          flag=0;           //���־
       }
       rDMASKTRIG2=(0<<2)|(1<<1)|0;      //��Ҫ��������DMAͨ���Ŀ���
	}
}
#endif

/*dma0 interrupt handler*/
void Dma0_Handle(unsigned int vector)
{
	printf("finish the transport.\n");
	/*set beep ring*/
	GPBDAT = 0b1001 << 5;
}


void DMA_memcpy(void* p_dst,const void* p_src,int len)
{
	/*DMA 0��ʼԴ�Ĵ��� [30:0]*/
    DISRC0  = ((int)p_src & 0x7FFFFFFF);
   /*DMA 0��ʼԴ���ƼĴ���
   *	λ[1]������Դλ�õ�ѡ��;
   *		0��Դ��ϵͳ���ߣ�AHB����
   *		1��Դ���������ߣ�APB����
   *	λ[0]�����ڵ�ַ���ӵ�ѡ��
   *		0������
   *		1���̶�
   */
    DISRCC0 = 0;
    /*DMA 0��ʼĿ��Ĵ��� [30:0]*/
    DIDST0  = ((int)p_dst & 0x7FFFFFFF);
   /*DMA 0��ʼĿ����ƼĴ���
   	*	[2]���������Զ��ټ���ʱ�����жϵ�ʱ��ѡ��
   	*		0����TC����0ʱ�����ж�
   	*		1����ִ�����Զ��ټ��غ����ж�
    *	[1]������Ŀ��λ�õ�ѡ��
    *		0��Ŀ����ϵͳ���ߣ�AHB����
    *		1��Ŀ�����������ߣ�APB����
    *	[0]�����ڵ�ַ���ӵ�ѡ��
    *		0������
    *		1���̶�
    */
    DIDSTC0 = 0;
   	/*DMA 0���ƼĴ���
   	*	[31]��ѯģʽ������ģʽ֮��ѡ������֮һ:
   	*		0��ѡ���ѯģʽ ;
   	*		1��ѡ������ģʽ
   	*	[30]DREQ/DACK��ͬ����ѡ��:
   	*		0��DREQ��DACKͬ����PCLK��APBʱ�ӣ�
   	*		1��DREQ��DACKͬ����HCLK��AHBʱ�ӣ�
   	*	[29]CURR_TC (�յ����)���ж�ʹ��/��ֹ����:
   	*		0����ֹCURR_TC�жϡ��û�����۲�״̬�Ĵ����Ĵ������������ʱ��ѯ��
   	*		1�������д�����ɲ����ж����󣨼�CURR_TC��Ϊ0��
   	*	[27]������ģʽ��ȫ����ģʽ֮��ķ���ģʽѡ��:
   	*		0��ѡ��ÿ��ԭ�Ӵ��䣨���λ�ͻ��4����DMAֹͣ�͵ȴ�����DMA����ĵ�����ģʽ
   	*		1��ѡ��������ﵽ0ǰ�ظ�����õ�ԭ�Ӵ����ȫ����ģʽ����ģʽ����Ҫ��������
   	*	[22]�����ټ��ؿ�/��ѡ��
   	*		0������������ĵ�ǰֵ��Ϊ0ʱ����ִ������������Ĵ��䣩ִ���Զ��ټ���
   	*		1������������ĵ�ǰֵ��Ϊ0ʱDMAͨ����DMA  REQ���رա�����ͨ����/��λ��DMASKTRIGn[1]��Ϊ0��DREQ�رգ���Ԥ����Ԥ������DMA�����Ľ�һ����ʼ
   	*	[21:20]Ҫ��������ݴ�С
   	*		00 = �ֽ�  01 = ����  10 = ��  11 = ����
   	*	[19:0]��ʼ���������������ģ�
   	*		ע����ʵ�ʴ�����ֽ��������µ�ʽ����õ���SZ��TSZ��TC���˴�
	*		DSZ��TSZ��1��4����TC�ֱ�������ݴ�СDCONn[21:20]�������
	*		СDCONn[28]�ͳ�ʼ������������ҽ���CURR_TCΪ0����DMA
	*		ACKΪ1ʱ��ֵ�������ص�CURR_TC��
   	*/
    DCON0 = (len & 0xFFFFF) + (2 << 20) + (1 << 22) + (1 << 27) + (1 << 29) + (1 << 30) + (1U << 31);
    /*DMA 0�������μĴ���
    *	[2]ֹͣDMA������
    *		1��һ�������˵�ǰ��ԭ�Ӵ����ֹͣDMA�������ǰ�����е�ԭ�Ӵ��䣬����ֹͣDMA��
	*			CURR_TC��CURR_SRC��CURR_DST����Ϊ0��
    *	[1]DMAͨ����/��λ
    *		0���ر�DMAͨ���������Դ�ͨ��DMA����
    *		1����DMAͨ�����Ҵ����ͨ��DMA����
    *	[0]�������ģʽ�д���DMAͨ��
    *		1��Ϊ�˿���������DMA����
    */
    DMASKTRIG0 = 0x03;
    /*�ж����Σ�INTMSK���Ĵ������������ĸ��ж�Դ�������ε��ж�Դ���������
    *	0 = �жϷ������  1 = �����жϷ���
    */
    INTMSK &= ~(1 << 17);//enable the dma0 interrupt
}


/**************************************************************
*              DMA��ʼ��
*p_dst:DMA transfer destination address
*p_src:DMA transfer source address
*len  :size
*mode :transfer mode (ie:Uart IIS...)
*m_flag:transfer mode flag (ie:Uart IIS...source)
**************************************************************/
void Dma_init(void* p_dst,const void* p_src,unsigned int len,unsigned char mode,unsigned char m_flag)
{
  DISRC0    =(unsigned long)p_src & 0x7FFFFFFF;					//����source��ַ
  DISRCC0   |=((0<<1)|(0<<0));				                    //[1]ϵͳ����,[0]��ַ�����ݵ��κ�ͻ��ģʽ��ÿ�δ���������ݴ�С������
  DIDST0    =(unsigned long)p_dst & 0x7FFFFFFF;					//����Ŀ���ַ
  DIDSTC0   |=((0<<2)|(1<<1)|(1<<0));		                    //[2]��TC����0ʱ�����ж�, [1] APB(����������)  , [0]������ַ���߲���

  DCON0     |=  (1U<<31)|(0<<30)|(1<<29)|(0<<28)|(0<<27)|(mode << 24)|(m_flag << 23)|(1<<22)|(0<<20)|(len & 0xFFFFF);
  //[31] ����ģʽ; [30]ͬ��PCLK=50M(APBʱ��< ���� >); [29]�����д�����ɲ����ж����󣨼�CURR_TC��Ϊ0��;[28]ִ��һ����Ԫ����;
  //[27]ѡ��ÿ��ԭ�Ӵ��䣨���λ�ͻ��4����DMAֹͣ�͵ȴ�����DMA����ĵ�����ģʽ; [24]UART0; [23]ѡ��DMAԴ����DMA����;
  //[22]����������ĵ�ǰֵ��Ϊ0ʱDMAͨ����DMA  REQ���ر�; [20]�ֽ�(ÿ��һ���ֽڴ���);[19:0]��ʼ�������15��15=SendBuffer�ַ����鳤��;

  DMASKTRIG0=(0<<2)|(1<<1)|(0<<0);  //[1]��DMAͨ�����Ҵ����ͨ��DMA����
}


/**************************************************************
|						  DMA�Ӻ���
| setting DMA0 UART0
**************************************************************/
void DMA_UART(void* p_dst,const void* p_src,unsigned int len,unsigned char mode,unsigned char m_flag)
{
  	Dma_init(p_dst,p_src,len,mode,m_flag);//DMA��ʼ��
  	/*enable the dma interruput*/
	INTMSK &= ~(1 << 17);
}

/*just test for dma_uart*/
void dma_uart_test(void)
{
	int ret;
	/*register dma0 interrupt*/
	ret = register_interrupt(17,Dma0_Handle);
	if(ret)
	{
		printf("register dma int_handle error.\n\r");
		return ;
	}
    char temp[255] = "Hello world!This is a dma send buffer.\nHello world!This is a dma send buffer.\n";
    /*use uart0*/
    DMA_UART((unsigned long*)0x50000020,temp,96,1,1);
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

  MMU_Init();//�ж�ӳ���ַ��ʼ��
  Beep(2000, 100);

  DMA_UART();				  //DMA��ʽʵ��Uart�����ڣ�ͨ��

}
DMA_uart.c


/**************************************************************
              DMA�ж�
**************************************************************/
void __irq Dma0_isr(){
  rSRCPND|=0x1<<17;   //����жϹ���״̬
  rINTPND|=0x1<<17;
  Uart_Printf("\n	   ***DMA TO Uart finished***\n");
  Beep(2000,100);
  rGPBCON=0x015400;
  rGPBDAT=0x6<<5;
}

/**************************************************************
|						  DMA�Ӻ���
|  ������2440lib.c��   rUCON0  |=((1<<0) | (1<<3) | (2<<10) );
**************************************************************/
void DMA_UART(){
  Uart_Printf("\n	   ***HELLO DMA TO Uart ***\n");
  Delay(1000);

  Dma_init();//DMA��ʼ��
  rINTMSK &=~(1<<17);  //�� dma0 �ж�
  pISR_DMA0=(U32)Dma0_isr;


  while(1){
  }
}
#endif

