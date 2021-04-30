#ifndef __BSP_PROTOCOL_H
#define __BSP_PROTOCOL_H

#include "bsp.h"

#define MAX_LEN             440
#define RBUF_MAX_LEN        32
//#define CBUF_MAX_LEN        50
#define C_MAX_LEN           100

extern unsigned char isEmpty(unsigned char ch);
extern unsigned char buffSize(unsigned char ch);
extern unsigned char isFull(unsigned char ch);
extern void pushBuff (uint16_t data,unsigned char ch);    //定义一个  接收数据的缓冲区
extern uint8_t popBuff(unsigned char ch);
extern void clearBuff(unsigned char ch);
extern uint16_t Rx_Sta;

typedef struct 
{      
    unsigned short rbuf[RBUF_MAX_LEN];
    unsigned short len;
}DataBuf;

typedef struct 
{      
    unsigned short cbuf;
    unsigned char ch;
    unsigned char temp;
}DataCur;

typedef struct uartReceive                                                     
{			 				                                
    short readIndex;                                                
    short writeIndex;					 	       
}uartRec;

extern DataBuf ad_Buf[MAX_LEN];
extern DataBuf adTemp;
extern uartRec adReceive;

extern DataCur cTemp;
extern uartRec cReceive;
extern DataCur C_Buf[C_MAX_LEN];
extern uint16_t c_Sta;
#endif


