#include "bsp.h"

MD5_CTX md5;
   
void Make_xml(void)	   //���xml,��֧���ӿڷ���xml
{	

	int i,j;	
	u8 appid[]="wx549de0764ba1a4d8";//�����̵�APPID
	u8 body[]="test";								//��Ʒ����
	u8 mch_id[]="1603583093";			//�̻���
	u8 notify_url[]="http://wxpay.wxutil.com/pub_v2/pay/notify.v2.php";//֪ͨ��ַ
	u8 out_trade_no[]= "1415659990";    //�̻�������
	u8 spbill_create_ip[]="203.195.211.185";//�ն�IP
	u8 total_fee[]="1";								//�ܽ��
	u8 trade_type[]="NATIVE";						//��������
	u8 key[]="qazwsxedcrfvtgbyhnujmikolp142536";//keyΪ�̻�ƽ̨���õ���Կkey
	u8 decrypt[16];                   //���ܺ�������
	u8 sign[32];              //ǩ���ַ������λ��
	u8 nonce_str[]="1add1a30ac87aa2db72f57a2375d8fec";      //����ַ���
	u8 encrypt[350];         //����ǰ�ַ������λ��
	u8 xml[500];              //��΢��֧���ӿڷ��͵�xml

	sprintf((char*) encrypt,"appid=%s&body=%s&mch_id=%s&nonce_str=%s&\
notify_url=%s&out_trade_no=%s&spbill_create_ip=%s&total_fee=%s&\
trade_type=%s&key=%s",appid,body,mch_id,nonce_str,notify_url,
out_trade_no,spbill_create_ip,total_fee,trade_type,key);
	
//���ԣ�!!	
//	unsigned char encrypt[] ="appid=wx549de0764ba1a4d8\
//&body=test&mch_id=1603583093&nonce_str=1add1a30ac87aa2db72f57a2375d8fec\
//&notify_url=http://wxpay.wxutil.com/pub_v2/pay/notify.v2.php\
//&out_trade_no=1415659990&spbill_create_ip=203.195.211.185&total_fee=1\
//&trade_type=NATIVE&key=qazwsxedcrfvtgbyhnujmikolp142536"; //74831D5BC8A9C935DFA85E1CF5443E2D
	
	MD5Init(&md5);         		
	MD5Update(&md5,encrypt,strlen((char *)encrypt));
	MD5Final(&md5,decrypt);           
	printf("����ǰ:%s\n���ܺ�:",encrypt);      
	POINT_COLOR=BLUE;                //��������Ϊ��ɫ	
	for(i=0,j=0;i<16;i++,j+=2)
	{
		printf("%02x",decrypt[i]);
		sprintf((char*)sign+j,"%02X",decrypt[i]);   //��16λ�ַ�����16���ƴ浽32λ��sign������
	}

	LCD_ShowString(0,220,16,sign,0,BLUE);     //���Դ�ӡǩ���ַ���     

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
	spbill_create_ip,total_fee,trade_type,sign);//ƴ��xml����xml������


}



