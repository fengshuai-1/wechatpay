#include "bsp.h"

DataBuf ad_Buf[MAX_LEN];  //ad9220原始数据检测
DataBuf adTemp;
uartRec adReceive = {0,0};

DataCur C_Buf[C_MAX_LEN];
DataCur cTemp;
uartRec cReceive = {0,0};
uint16_t c_Sta = 0;

uint16_t Rx_Sta = 0;


/**************************************************************************************************
 * 缓冲区当前所有数据长度
 *************************************************************************************************/
unsigned char buffSize(unsigned char ch)
{
    if(ch == 1)
    {
        short  wi = adReceive.writeIndex;
        short  ri = adReceive.readIndex;
        return (wi >= ri) ? (wi - ri) : (MAX_LEN - ri + wi);
    }
    if(ch == 2)
    {
        short  wi = cReceive.writeIndex;
        short  ri = cReceive.readIndex;
        return (wi >= ri) ? (wi - ri) : (C_MAX_LEN - ri + wi);
    }
    else
        return 0;
}

/**************************************************************************************************
 * 缓冲区是否空了 
返回值 :1 空  0: 非空
 *************************************************************************************************/
unsigned char isEmpty(unsigned char ch)
{	
    if(ch == 1)
    {
        short  wi = adReceive.writeIndex;
        short  ri = adReceive.readIndex;
        return (wi == ri);
    }
    if(ch == 2)
    {
        short  wi = cReceive.writeIndex;
        short  ri = cReceive.readIndex;
        return (wi == ri);
    }
    else
        return 0;
}

/**************************************************************************************************
 * 缓冲区是否满了
返回值  1: 满 0: 非满
 *************************************************************************************************/
unsigned char isFull(unsigned char ch)
{
    if(ch == 1)
    {
        short  wi = adReceive.writeIndex;
        short  ri = adReceive.readIndex;
        return ((wi + 1) % MAX_LEN == ri);
    }
    if(ch == 2)
    {
        short  wi = cReceive.writeIndex;
        short  ri = cReceive.readIndex;
        return ((wi + 1) % C_MAX_LEN == ri);
    }
    else
        return 0;
}

/**************************************************************************************************
 * 将串口接收到的 数据 从顶部压入堆栈
 *************************************************************************************************/

void pushBuff (uint16_t data,unsigned char ch)
{       
    if(ch == 1)//单次采样值
    {
        if(isFull(1) == 1)//数组满了
        {
            Debug_Printf("\r\n**********Data Over!!*********\r\n");
        }
        else 
        {
            ad_Buf[adReceive.writeIndex].rbuf[Rx_Sta]  = data;
            ad_Buf[adReceive.writeIndex].len   = Rx_Sta + 1;	
        }
    }

    if(ch == 2)//平均ADC
    {
        if(isFull(2) == 1)//数组满了
        {
            Debug_Printf("\r\n**********C  Data Over!!*********\r\n");
        }
        else 
        {
            C_Buf[cReceive.writeIndex].cbuf  = data;
            C_Buf[cReceive.writeIndex].ch  = CHX;
            C_Buf[cReceive.writeIndex].temp = CH_temp;
						cReceive.writeIndex = (cReceive.writeIndex + 1) % C_MAX_LEN; //数据操作50个，为1帧
//            c_Sta += 1;	
        }
    }

}  

/**************************************************************************************************
 * 将堆栈里的数据从底部弹出
 *************************************************************************************************/

uint8_t popBuff(unsigned char ch)
{
    if(ch == 1)
    {
			if(isEmpty(1) == 1)
			{
				return 0;
			}
            memcpy(adTemp.rbuf,ad_Buf[adReceive.readIndex].rbuf,64); //2个字节
//	  adTemp.len = ad_Buf[adReceive.readIndex].len;
			adReceive.readIndex = (adReceive.readIndex + 1) % MAX_LEN;
			return 1;
    }
    if(ch == 2)
    {
			if(isEmpty(2) == 1)
			{
				return 0;
			}
				cTemp.cbuf = C_Buf[cReceive.readIndex].cbuf;
				cTemp.ch = C_Buf[cReceive.readIndex].ch;
				cTemp.temp = C_Buf[cReceive.readIndex].temp;

				cReceive.readIndex = (cReceive.readIndex + 1) % C_MAX_LEN;
				return 1;
    }
    else
        return 0;
}

void clearBuff (unsigned char ch)
{
    if(ch == 1)
    {
        adReceive.readIndex = adReceive.writeIndex;	
    }
    if(ch == 2)
    {
        cReceive.readIndex = cReceive.writeIndex;	
    }
}
