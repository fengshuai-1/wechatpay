#include "bsp.h"
MD5_CTX md5;
u8 xml[500];            //向微信支付接口发送的xml 
u8 check_xml[500];        //向微信查询接口发送的xml 

int i,j;	
u32 out_trade_no = 0;   //= "202001041731";    //商户订单号
u32 total_fee = 1;		 	//总金额
static u8 appid[]="wx549de0764ba1a4d8";//服务商的APPID
static u8 body[]="smart-pet";								//商品描述
static u8 mch_id[]="1603583093";			//商户号
static u8 notify_url[]="http://wxpay.wxutil.com/pub_v2/pay/notify.v2.php";//通知地址
static u8 spbill_create_ip[]="203.195.211.185";//终端IP				
static u8 trade_type[]="NATIVE";						//交易类型
static u8 key[]="qazwsxedcrfvtgbyhnujmikolp142536";//key为商户平台设置的密钥key
static u8 nonce_str[]="1add1a30ac87aa2db72f57a2375d8fec";      //随机字符串
u8 decrypt[16]={0};                   //加密后存放数组
u8 decrypt1[16]={0};                   //查询订单加密后存放数组

u8 sign[32]={0};              //签名字符串存放位置
u8 sign1[32]={0};              //签名字符串存放位置


u8 encrypt[350]={0};         //支付加密前字符串存放位置
u8 encrypt1[350]={0};         //查询加密前字符串存放位置


void Make_xml(void)	   //组合xml,向支付接口发送xml
{	
		out_trade_no =	rtc_counter_get();  //获取订单号
//		Debug_Printf("%d",out_trade_no);
	sprintf((char*) encrypt,"appid=%s&body=%s&mch_id=%s&nonce_str=%s&\
notify_url=%s&out_trade_no=%d&spbill_create_ip=%s&total_fee=%d&\
trade_type=%s&key=%s",appid,body,mch_id,nonce_str,notify_url,
out_trade_no,spbill_create_ip,total_fee,trade_type,key);
	
	
	MD5Init(&md5);         		//MD5初始化
	MD5Update(&md5,encrypt,strlen((char *)encrypt)); //将encrypt进行MD5加密
	MD5Final(&md5,decrypt); //将加密好的放入decrypt[16]数组中
	
	for(i=0,j=0;i<16;i++,j+=2)
	{
//		printf("%02x",decrypt[i]);
		sprintf((char*)sign+j,"%02X",decrypt[i]);   //把16位字符串以16进制存到32位的sign数组中
	}
	
sprintf((char*) xml,"<xml>\
<appid>%s</appid>\
<body>%s</body>\
<mch_id>%s</mch_id>\
<nonce_str>%s</nonce_str>\
<notify_url>%s</notify_url>\
<out_trade_no>%d</out_trade_no>\
<spbill_create_ip>%s</spbill_create_ip>\
<total_fee>%d</total_fee>\
<trade_type>%s</trade_type>\
<sign>%s</sign>\
</xml>",\
appid,body,mch_id,nonce_str,notify_url,out_trade_no,\
spbill_create_ip,total_fee,trade_type,sign);//拼接xml存在xml数组里
}


void Make_xml1(void)	   //组合xml,向查询接口发送xml
{	

	sprintf((char*) encrypt1,"appid=%s&mch_id=%s&nonce_str=%s&\
out_trade_no=%d&key=%s",appid,mch_id,nonce_str,out_trade_no,key);  //查询订单前字符串	
	MD5Init(&md5); 
	MD5Update(&md5,encrypt1,strlen((char *)encrypt1));
	MD5Final(&md5,decrypt1); 
	
		for(i=0,j=0;i<16;i++,j+=2)
	{	
//		printf("%02x",decrypt[i]);
		sprintf((char*)sign1+j,"%02X",decrypt1[i]);   //把16位字符串以16进制存到32位的sign1数组中
	}
sprintf((char*) check_xml,"<xml>\
<appid>%s</appid>\
<mch_id>%s</mch_id>\
<nonce_str>%s</nonce_str>\
<out_trade_no>%d</out_trade_no>\
<sign>%s</sign>\
</xml>",\
appid,mch_id,nonce_str,out_trade_no,sign1);//拼接存在check_xml数组里

}

/*****************************************************

 向微信支付接口发送xml

******************************************************/

void Send_HTTPS(void)    // 向微信支付接口发送xml
{
	char *str;
	char txbuffer1[40];	
	
	Clear_Buffer();
	sprintf((char *)txbuffer1,"AT+HTTPSACTION=2,%d,3\r\n",strlen(xml));//与微信支付接口建立连接	
	tx_size = strlen(txbuffer1);
	memcpy(txbuffer,txbuffer1,tx_size);
    txcount =0;
	usart_interrupt_enable(USART0, USART_INT_TBE);
//	Clear_Buffer();
	bsp_DelayMS(400);
//	str=strstr((const char*)rxbuffer,(const char*)">");//判断结果有OK才能做下一步。
//	Debug_SendString(txbuffer1);
//	Debug_SendString(xml); //打印xml
	
	tx_size = strlen(xml);
	memcpy(txbuffer,xml,tx_size);
	Debug_SendString(txbuffer);
    txcount =0;
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(500);
	str=strstr((const char*)rxbuffer,(const char*)"OK");//判断结果有OK才能做下一步。


}

void Code_url(void)   //截取二维码链接
{
		u16 x,y;
		u16 x0=0;
    char *temp_p = NULL;
    char *end_p = NULL;
		char *buff_p = rxbuffer;
		static	char essid[128] = {0};
		temp_p = strstr(buff_p, "<code_url>");  //指针temp_p定位到<code_url>的位置开始的位置
    if (NULL == temp_p) 
    {
        // /break;
    }
    else
    {
        buff_p = temp_p;     //改变buff_p里面的内容从strstr就开始的到<code_url>  </code_url>内容的结束
        temp_p = strstr(buff_p,"A["); //指针temp_p定位到第一个的位置
        end_p =strstr(buff_p,"]]");     //把end_p指向最后一个位置
        memcpy(essid, temp_p+2, (end_p-1)-(temp_p+1)); // essid存的就是我们想要截取的字符串
        Debug_SendString(essid);
				Clear_Buffer();
				EncodeData(essid);
				x0=240/m_nSymbleSize;   //放大倍数 x0

				Gui_DrawBat(30,270,gImage_3);//显示微信支付logo

				for(x=0;x<32;x++)
					for(y=0;y<32;y++)
				{		
					if(m_byModuleData[x][y]==0x01)	
							LCD_Fill(x0*x + 3,x0*(y+4),x0*x+x0+2,x0*(y+4)+x0-1,0);
				}

		}	



}

void Check_order(void)   //查询订单状态
{
		char *str;
		Clear_Buffer();
		char txbuffer1[40]={0};	
		sprintf((char *)txbuffer1,"AT+HTTPSACTION=2,%d,3\r\n",strlen(check_xml));//与微信查询接口建立连接	
		tx_size = strlen(txbuffer1);
		memcpy(txbuffer,txbuffer1,tx_size);
		usart_interrupt_enable(USART0, USART_INT_TBE);
		//	Clear_Buffer();
		bsp_DelayMS(500);
		//	str=strstr((const char*)rxbuffer,(const char*)">");//判断结果有OK才能做下一步。
	//	Debug_SendString(txbuffer1);
		//	Debug_SendString(xml); //打印xml
			Clear_Buffer();
		tx_size = strlen(check_xml);
		memcpy(txbuffer,check_xml,tx_size);
		Debug_SendString(txbuffer);
		usart_interrupt_enable(USART0, USART_INT_TBE); //发送查询xml字符串
		bsp_DelayMS(500);
		str=strstr((const char*)rxbuffer,(const char*)"OK");//判断结果有OK才能做下一步。
		Clear_txbuffer();
}


void trade_state(void)   //支付状态判断
{
    char *temp_p = NULL;
    char *end_p = NULL;
		char *buff_p = rxbuffer;
		char essid[128] = {0};
		temp_p = strstr(buff_p, "<trade_state>");  //指针temp_p定位到<code_url>的位置开始的位置
    if (NULL == temp_p) {
        // /break;
    }else{
        buff_p = temp_p;     //改变buff_p里面的内容从strstr就开始的到<code_url>  </code_url>内容的结束
        temp_p = strstr(buff_p,"A["); //指针temp_p定位到第一个的位置
        end_p =strstr(buff_p,"]]");     //把end_p指向最后一个位置
        memcpy(essid, temp_p+2, (end_p-1)-(temp_p+1)); // essid存的就是我们想要截取的字符串
				Debug_SendString(essid);
			
			if(strcmp((const char*)essid,"SUCCESS")==0) 
			{	
				LCD_Fill(0,32,240,270,WHITE);   //画白色矩阵清屏
				Show_Str(50,150,BRED,WHITE,"支付成功",32,0);//显示支付成功	

                Open();

			    timer8_config(); //输出PWM波电机转动
				bsp_DelayMS(3000);
				timer_disable(TIMER8);	//关闭电机转动
				LCD_Fill(0,0,240,320,WHITE);   //画白色矩阵清屏
				Clear_txbuffer();
				state=0;
			}
			
//////////				if(strcmp((const char*)essid,"USERPAYING")==0) 
//////////			{	
//////////				LCD_Fill(0,100,240,270,WHITE);   //画白色矩阵清屏
//////////				Show_Str(100,150,BLUE,WHITE, "USERPAYING",16,0);//显示用户支付中
//////////			}
//				if(strcmp((const char*)essid,"NOTPAY")==0) 
//			{	
//				LCD_Fill(0,16,240,280,WHITE);   //画白色矩阵清屏
//				Show_Str(100,80,BLUE,WHITE, "NOTPAY",16,0);//显示未支付
//			}
			
			
		}	
		
}
