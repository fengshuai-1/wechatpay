#include "bsp.h"

MD5_CTX md5;
   
void Make_xml(void)	   //组合xml,向支付接口发送xml
{	

	int i,j;	
	u8 appid[]="wx549de0764ba1a4d8";//服务商的APPID
	u8 body[]="test";								//商品描述
	u8 mch_id[]="1603583093";			//商户号
	u8 notify_url[]="http://wxpay.wxutil.com/pub_v2/pay/notify.v2.php";//通知地址
	u8 out_trade_no[]= "1415659990";    //商户订单号
	u8 spbill_create_ip[]="203.195.211.185";//终端IP
	u8 total_fee[]="1";								//总金额
	u8 trade_type[]="NATIVE";						//交易类型
	u8 key[]="qazwsxedcrfvtgbyhnujmikolp142536";//key为商户平台设置的密钥key
	u8 decrypt[16];                   //加密后存放数组
	u8 sign[32];              //签名字符串存放位置
	u8 nonce_str[]="1add1a30ac87aa2db72f57a2375d8fec";      //随机字符串
	u8 encrypt[350];         //加密前字符串存放位置
	u8 xml[500];              //向微信支付接口发送的xml

	sprintf((char*) encrypt,"appid=%s&body=%s&mch_id=%s&nonce_str=%s&\
notify_url=%s&out_trade_no=%s&spbill_create_ip=%s&total_fee=%s&\
trade_type=%s&key=%s",appid,body,mch_id,nonce_str,notify_url,
out_trade_no,spbill_create_ip,total_fee,trade_type,key);
	
//测试！!!	
//	unsigned char encrypt[] ="appid=wx549de0764ba1a4d8\
//&body=test&mch_id=1603583093&nonce_str=1add1a30ac87aa2db72f57a2375d8fec\
//&notify_url=http://wxpay.wxutil.com/pub_v2/pay/notify.v2.php\
//&out_trade_no=1415659990&spbill_create_ip=203.195.211.185&total_fee=1\
//&trade_type=NATIVE&key=qazwsxedcrfvtgbyhnujmikolp142536"; //74831D5BC8A9C935DFA85E1CF5443E2D
	
	MD5Init(&md5);         		
	MD5Update(&md5,encrypt,strlen((char *)encrypt));
	MD5Final(&md5,decrypt);           
	printf("加密前:%s\n加密后:",encrypt);      
	POINT_COLOR=BLUE;                //设置字体为蓝色	
	for(i=0,j=0;i<16;i++,j+=2)
	{
		printf("%02x",decrypt[i]);
		sprintf((char*)sign+j,"%02X",decrypt[i]);   //把16位字符串以16进制存到32位的sign数组中
	}

	LCD_ShowString(0,220,16,sign,0,BLUE);     //测试打印签名字符串     

	sprintf((char*) xml,"<xml>\
	<appid>%s<appid>\
	<body>%s</body>\
	<mch_id>%s</mch_id>\
	<nonce_str>%s</nonce_str>\
	<notify_url>%s</notify_url>\
	<out_trade_no>%s</out_trade_no>\
	<spbill_create_ip>%s</spbill_create_ip>\
	<total_fee>%s</total_fee>\
	<trade_type>%s</trade_type>\
	<sign>%s</sign>\
	</xml>",\
	appid,body,mch_id,nonce_str,notify_url,out_trade_no,\
	spbill_create_ip,total_fee,trade_type,sign);//拼接xml存在xml数组里


}



