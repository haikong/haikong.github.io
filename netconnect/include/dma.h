#ifndef DMA_H_
#define DMA_H_

#ifdef	__cplusplus
extern "C"{
#endif

#define DMA_SIZE     0x1000				//transfer the data size bytes
#define DMA_SRC     (0x30008000)		//source address
#define DMA_DST     (0x30009000)		//destination address

typedef struct{					//DISRC0:0x4B000000
    unsigned long DISRC;	    //0x0
    unsigned long DISRCC;    	//0x4
    unsigned long DIDST;	    //0x8
    unsigned long DIDSTC;    	//0xc
    unsigned long DCON;	    	//0x10
    unsigned long DSTAT;	    //0x14
    unsigned long DCSRC;	    //0x18
    unsigned long DCDST;	    //0x1c
    unsigned long DMASKTRIG; 	//0x20
}t_DMAReg,*pt_DMAReg;

void DMA_memcpy(void* p_dst,const void* p_src,int len);
void DMA_UART(void* p_dst,const void* p_src,unsigned int len,unsigned char mode,unsigned char m_flag);
/*just test for dma_uart*/
void dma_uart_test(void);
void Dma0_Handle(unsigned int vector);

#ifdef	__cplusplus
}
#endif

#endif
