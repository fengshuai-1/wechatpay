#include "bsp.h"
MD5_CTX md5;
u8 xml[500];            //��΢��֧���ӿڷ��͵�xml 
u8 check_xml[500];        //��΢�Ų�ѯ�ӿڷ��͵�xml 

int i,j;	
u32 out_trade_no = 0;   //= "202001041731";    //�̻�������
u32 total_fee = 1;		 	//�ܽ��
static u8 appid[]="wx549de0764ba1a4d8";//�����̵�APPID
static u8 body[]="smart-pet";								//��Ʒ����
static u8 mch_id[]="1603583093";			//�̻���
static u8 notify_url[]="http://wxpay.wxutil.com/pub_v2/pay/notify.v2.php";//֪ͨ��ַ
static u8 spbill_create_ip[]="203.195.211.185";//�ն�IP				
static u8 trade_type[]="NATIVE";						//��������
static u8 key[]="qazwsxedcrfvtgbyhnujmikolp142536";//keyΪ�̻�ƽ̨���õ���Կkey
static u8 nonce_str[]="1add1a30ac87aa2db72f57a2375d8fec";      //����ַ���
u8 decrypt[16]={0};                   //���ܺ�������
u8 decrypt1[16]={0};                   //��ѯ�������ܺ�������

u8 sign[32]={0};              //ǩ���ַ������λ��
u8 sign1[32]={0};              //ǩ���ַ������λ��


u8 encrypt[350]={0};         //֧������ǰ�ַ������λ��
u8 encrypt1[350]={0};         //��ѯ����ǰ�ַ������λ��


void Make_xml(void)	   //���xml,��֧���ӿڷ���xml
{	
		out_trade_no =	rtc_counter_get();  //��ȡ������
//		Debug_Printf("%d",out_trade_no);
	sprintf((char*) encrypt,"appid=%s&body=%s&mch_id=%s&nonce_str=%s&\
notify_url=%s&out_trade_no=%d&spbill_create_ip=%s&total_fee=%d&\
trade_type=%s&key=%s",appid,body,mch_id,nonce_str,notify_url,
out_trade_no,spbill_create_ip,total_fee,trade_type,key);
	
	
	MD5Init(&md5);         		//MD5��ʼ��
	MD5Update(&md5,encrypt,strlen((char *)encrypt)); //��encrypt����MD5����
	MD5Final(&md5,decrypt); //�����ܺõķ���decrypt[16]������
	
	for(i=0,j=0;i<16;i++,j+=2)
	{
//		printf("%02x",decrypt[i]);
		sprintf((char*)sign+j,"%02X",decrypt[i]);   //��16λ�ַ�����16���ƴ浽32λ��sign������
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
spbill_create_ip,total_fee,trade_type,sign);//ƴ��xml����xml������
}


void Make_xml1(void)	   //���xml,���ѯ�ӿڷ���xml
{	

	sprintf((char*) encrypt1,"appid=%s&mch_id=%s&nonce_str=%s&\
out_trade_no=%d&key=%s",appid,mch_id,nonce_str,out_trade_no,key);  //��ѯ����ǰ�ַ���	
	MD5Init(&md5); 
	MD5Update(&md5,encrypt1,strlen((char *)encrypt1));
	MD5Final(&md5,decrypt1); 
	
		for(i=0,j=0;i<16;i++,j+=2)
	{	
//		printf("%02x",decrypt[i]);
		sprintf((char*)sign1+j,"%02X",decrypt1[i]);   //��16λ�ַ�����16���ƴ浽32λ��sign1������
	}
sprintf((char*) check_xml,"<xml>\
<appid>%s</appid>\
<mch_id>%s</mch_id>\
<nonce_str>%s</nonce_str>\
<out_trade_no>%d</out_trade_no>\
<sign>%s</sign>\
</xml>",\
appid,mch_id,nonce_str,out_trade_no,sign1);//ƴ�Ӵ���check_xml������

}

/*****************************************************

 ��΢��֧���ӿڷ���xml

******************************************************/

void Send_HTTPS(void)    // ��΢��֧���ӿڷ���xml
{
	char *str;
	char txbuffer1[40];	
	
	Clear_Buffer();
	sprintf((char *)txbuffer1,"AT+HTTPSACTION=2,%d,3\r\n",strlen(xml));//��΢��֧���ӿڽ�������	
	tx_size = strlen(txbuffer1);
	memcpy(txbuffer,txbuffer1,tx_size);
    txcount =0;
	usart_interrupt_enable(USART0, USART_INT_TBE);
//	Clear_Buffer();
	bsp_DelayMS(400);
//	str=strstr((const char*)rxbuffer,(const char*)">");//�жϽ����OK��������һ����
//	Debug_SendString(txbuffer1);
//	Debug_SendString(xml); //��ӡxml
	
	tx_size = strlen(xml);
	memcpy(txbuffer,xml,tx_size);
	Debug_SendString(txbuffer);
    txcount =0;
	usart_interrupt_enable(USART0, USART_INT_TBE);
	bsp_DelayMS(500);
	str=strstr((const char*)rxbuffer,(const char*)"OK");//�жϽ����OK��������һ����


}

void Code_url(void)   //��ȡ��ά������
{
		u16 x,y;
		u16 x0=0;
    char *temp_p = NULL;
    char *end_p = NULL;
		char *buff_p = rxbuffer;
		static	char essid[128] = {0};
		temp_p = strstr(buff_p, "<code_url>");  //ָ��temp_p��λ��<code_url>��λ�ÿ�ʼ��λ��
    if (NULL == temp_p) 
    {
        // /break;
    }
    else
    {
        buff_p = temp_p;     //�ı�buff_p��������ݴ�strstr�Ϳ�ʼ�ĵ�<code_url>  </code_url>���ݵĽ���
        temp_p = strstr(buff_p,"A["); //ָ��temp_p��λ����һ����λ��
        end_p =strstr(buff_p,"]]");     //��end_pָ�����һ��λ��
        memcpy(essid, temp_p+2, (end_p-1)-(temp_p+1)); // essid��ľ���������Ҫ��ȡ���ַ���
        Debug_SendString(essid);
				Clear_Buffer();
				EncodeData(essid);
				x0=240/m_nSymbleSize;   //�Ŵ��� x0

				Gui_DrawBat(30,270,gImage_3);//��ʾ΢��֧��logo

				for(x=0;x<32;x++)
					for(y=0;y<32;y++)
				{		
					if(m_byModuleData[x][y]==0x01)	
							LCD_Fill(x0*x + 3,x0*(y+4),x0*x+x0+2,x0*(y+4)+x0-1,0);
				}

		}	



}

void Check_order(void)   //��ѯ����״̬
{
		char *str;
		Clear_Buffer();
		char txbuffer1[40]={0};	
		sprintf((char *)txbuffer1,"AT+HTTPSACTION=2,%d,3\r\n",strlen(check_xml));//��΢�Ų�ѯ�ӿڽ�������	
		tx_size = strlen(txbuffer1);
		memcpy(txbuffer,txbuffer1,tx_size);
		usart_interrupt_enable(USART0, USART_INT_TBE);
		//	Clear_Buffer();
		bsp_DelayMS(500);
		//	str=strstr((const char*)rxbuffer,(const char*)">");//�жϽ����OK��������һ����
	//	Debug_SendString(txbuffer1);
		//	Debug_SendString(xml); //��ӡxml
			Clear_Buffer();
		tx_size = strlen(check_xml);
		memcpy(txbuffer,check_xml,tx_size);
		Debug_SendString(txbuffer);
		usart_interrupt_enable(USART0, USART_INT_TBE); //���Ͳ�ѯxml�ַ���
		bsp_DelayMS(500);
		str=strstr((const char*)rxbuffer,(const char*)"OK");//�жϽ����OK��������һ����
		Clear_txbuffer();
}


void trade_state(void)   //֧��״̬�ж�
{
    char *temp_p = NULL;
    char *end_p = NULL;
		char *buff_p = rxbuffer;
		char essid[128] = {0};
		temp_p = strstr(buff_p, "<trade_state>");  //ָ��temp_p��λ��<code_url>��λ�ÿ�ʼ��λ��
    if (NULL == temp_p) {
        // /break;
    }else{
        buff_p = temp_p;     //�ı�buff_p��������ݴ�strstr�Ϳ�ʼ�ĵ�<code_url>  </code_url>���ݵĽ���
        temp_p = strstr(buff_p,"A["); //ָ��temp_p��λ����һ����λ��
        end_p =strstr(buff_p,"]]");     //��end_pָ�����һ��λ��
        memcpy(essid, temp_p+2, (end_p-1)-(temp_p+1)); // essid��ľ���������Ҫ��ȡ���ַ���
				Debug_SendString(essid);
			
			if(strcmp((const char*)essid,"SUCCESS")==0) 
			{	
				LCD_Fill(0,32,240,270,WHITE);   //����ɫ��������
				Show_Str(50,150,BRED,WHITE,"֧���ɹ�",32,0);//��ʾ֧���ɹ�	

                Open();

			    timer8_config(); //���PWM�����ת��
				bsp_DelayMS(3000);
				timer_disable(TIMER8);	//�رյ��ת��
				LCD_Fill(0,0,240,320,WHITE);   //����ɫ��������
				Clear_txbuffer();
				state=0;
			}
			
//////////				if(strcmp((const char*)essid,"USERPAYING")==0) 
//////////			{	
//////////				LCD_Fill(0,100,240,270,WHITE);   //����ɫ��������
//////////				Show_Str(100,150,BLUE,WHITE, "USERPAYING",16,0);//��ʾ�û�֧����
//////////			}
//				if(strcmp((const char*)essid,"NOTPAY")==0) 
//			{	
//				LCD_Fill(0,16,240,280,WHITE);   //����ɫ��������
//				Show_Str(100,80,BLUE,WHITE, "NOTPAY",16,0);//��ʾδ֧��
//			}
			
			
		}	
		
}
