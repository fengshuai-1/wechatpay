#include "bsp.h"

u8  Version_H = 1;
u8  Version_L = udp + telecom;

char *pl;  // �ַ���ָ��

u32 Flash_Address_Temp = 0;

unsigned char nbSetState = 0; 

unsigned char nbIsReconnect = 0;

char str[100];
char pbuf[4];
							 
u8 nbTimerStart = 0;
u8 nbTurnOff = 0;
t_nb Module;

extern uint08 FlashBuf[100];
extern uint16 Pressure;
////static void Nb_Send_Baowen(void);
////static void Nb_Receive_Baowen(u8* databuf, u8 len);
////static void Nb_RecieveData_Process(u8* databuf);
////static void Nb_Get_State(u16* state);

void bsp_NbSaveParam(void)
{

//	RTC_TimeDateTypeDef rtc_temp;
//	
//	Module.Flash_Address =  AT24CXX_ReadLenByte(0,4);
//	
//	if(Module.Flash_Address > (8000000 - 14))
//	{
//		AT24CXX_WriteLenByte(0,0,4);
//		Module.Flash_Address =  AT24CXX_ReadLenByte(0,4);
//	}
//	
//	w25qxx_wake_up();
//	
//	RTC_GetRTC(&rtc_temp);
//	
//  memcpy(&Module.year,(u8*)(&rtc_temp),6);
//	
//	w25qxx_write(&Module.year,Module.Flash_Address,6);
//	
//	Module.Flow[0] = 0;
//	Module.Flow[1] = 1;
//	Module.Flow[2] = 2;
//	Module.Flow[3] = 3;
//	
//	w25qxx_write(Module.Flow,Module.Flash_Address+6,4);
//	
//	Module.Pressure[0] = 4;
//	Module.Pressure[1] = 5;
//	Module.Pressure[2] = 6;
//	Module.Pressure[3] = 7;
//	
//	w25qxx_write(Module.Pressure,Module.Flash_Address+10,4);
//	
//	w25qxx_power_down();
//	
//	Module.Flash_Address = Module.Flash_Address + 14;
//	
//	printf("��һ��дFlash��ʼ��ַ: %d\r\n",Module.Flash_Address);
//		
//	AT24CXX_WriteLenByte(0,Module.Flash_Address,4);
	
}

void bsp_NbLoadParam(void)
{
//	u8 read_buf[21];
//	
//	w25qxx_wake_up();
//	
//	w25qxx_read(read_buf,Module.Flash_Address - 14,14);
//	
//	memcpy(&Module.year,read_buf,14);
//	
//	w25qxx_power_down();
	
   FLASH_ReadByte(0x003FE00,FlashBuf,8); //��ָ��λ�ö�ȡ����
}

void NB_SendString(uint08* buf)
{
	while(*buf)                  //����ַ���������
	{
		UARTx_TXREG_Write(UART3, *buf++);   //���͵�ǰ�ַ�
		
		while(UARTx_TXBUFSTA_TXFF_Chk(UART3));
	}
}

void NB_Init(void) //�ϵ�֮���ʼ��NBģ��
{
	 u8 buf[18]; //���ݴ�����
	 RTC_TimeDateTypeDef temptime; //ϵͳʱ��ṹ�建��
	
	 switch(Module.Init_Suatus)
	 {	 
		 case  0:  //NBӲ����λ
							GPIO_SetBits(GPIOB,GPIO_Pin_2);
							TicksDelayMs( 200, NULL );	 //�����ʱ200ms
							GPIO_ResetBits(GPIOB,GPIO_Pin_2);
							Module.Init_Suatus = 1;
			 break;
			
		 case  1: //NBģ�鿪��
							GPIO_SetBits(GPIOB,GPIO_Pin_0);
							TicksDelayMs( 2000, NULL );	//�����ʱ2.2s
							TicksDelayMs( 200, NULL );	 
		          IWDT_Clr();  		            //��ϵͳ���Ź�	
							GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		 
		          Module.Delay_time  = 200;
		          Module.Init_Suatus = 2;
		   break;
		 
			case  2://�ȴ�ģ������ϵ�
				 if(NbReceive.readIndex!=NbReceive.writeIndex)
	       { 
					 if(!popBuff(UART3))
				 	 {
						 break;
					 }
					 if(strstr((const char*)NbTemp.rbuf,"PIN: READY") != NULL)
					 {
						 Module.At_ErrCnt=0;
						 
//						 Module.At_WaitTime = 0;

//						 Module.send_cnt=1;
						 
//						 Module.ControlFlag = 1;
						 
//						 nbTimerStart = 0;
//						 nbTurnOff = 0;

						 Module.Init_Suatus = 3; // �ϵ����
						 
						 Debug_SendString("NB is READY ");
						 Debug_SendString("\r\n");							 
						 break;
					 }
					 if(strstr((const char*)NbTemp.rbuf,"REBOOTING") != NULL)
					 {
						 Debug_SendString("REBOOTING ");
						 Debug_SendString("\r\n");						 
					 }
			   }
				 else
				 {
						Module.Init_Suatus = 0;  // ģ���쳣
					  Debug_SendString("Waiting NB READY \r\n");
					 
					  Module.At_ErrCnt++;
					  if(Module.At_ErrCnt > 5)     //5�ζ��޷�READY���������
						{
							Module.At_ErrCnt=0;
							Debug_SendString("NB NOT READY \r\n");
							Module.Init_Suatus = 255;  // ��������
							break;
						}
				 }
				break;
			
		  case  3://֮ǰģ���Ѿ��Ϲ��磬��ôֱ�Ӷ�ȡ�����Ϣ����CCID/IMEI��
				if(FlashBuf[0] == 0x01)  
				{
//					Module.CCID
//					Module.DeviceID
//					Module.IMEI
//					Module.version
					  Module.Init_Suatus = 8;
					  Module.At_ErrCnt = 0;
				}
				else
				{
					Module.Init_Suatus = 4;
					Module.At_ErrCnt = 0;
				}
				break;
				
			case  4: //��ȡģ����ͺ�
							if(NbReceive.readIndex!=NbReceive.writeIndex)
							{
								if(!popBuff(UART3))
								{
									break;
								}

								if(strstr((const char*)NbTemp.rbuf,"BC30") != NULL)
								{
									Module.Model = BC30;
									Debug_SendString("Model:  BC30\r\n");
									Module.Init_Suatus = 5;
									break;
								}
								if(strstr((const char*)NbTemp.rbuf,"BC35") != NULL)
								{
									Module.Model = BC35;
									Debug_SendString("Model:  BC35\r\n");
									Module.Init_Suatus = 5;
									break;
								}
								if(strstr((const char*)NbTemp.rbuf,"BC95") != NULL)
								{	
									Module.Model = BC95;
									Debug_SendString("Model:  BC95\r\n");
									Module.Init_Suatus = 5;
									break;
								}
								if(strstr((const char*)NbTemp.rbuf,"TPB21") != NULL)
								{	
									Module.Model = TPB21_5;
									Debug_SendString("Model:  TPB21_5\r\n");
									Module.Init_Suatus = 5;
									break;
								}
								if(strstr((const char*)NbTemp.rbuf,"TPB23") != NULL)
								{						
									Module.Model = TPB23;
									Debug_SendString("Model:  TPB23\r\n");
									Module.Init_Suatus = 5;
									break;
								}
								if(strstr((const char*)NbTemp.rbuf,"M5311") != NULL)
								{						
									Module.Model = M5311;
									Debug_SendString("Model:  M5311\r\n");
									Module.Init_Suatus = 5;
									break;
								}
								if(strstr((const char*)NbTemp.rbuf,"ERROR") != NULL)
								{
									break;
								}								
							}
							else
							{
									Module.At_ErrCnt++;
									if(Module.At_ErrCnt > ERR_MAX)
									{
										Module.At_ErrCnt=0;
										Module.Init_Suatus = 255;  // ��������
										Debug_SendString("Unknown Model \r\n");
										break;
									}
								NB_SendString("AT+CGMM\r\n");
							}
				break;
			
			case  5: //��ȡ�豸IMEI��
							if(NbReceive.readIndex!=NbReceive.writeIndex)
							{
								if(!popBuff(UART3))
								{
									break;
								}
								if(strstr((const char*)NbTemp.rbuf,"ERROR") != NULL)
								{						
									break;
								}
								if(strstr((const char*)NbTemp.rbuf,"86") != NULL)
								{
									Module.At_ErrCnt=0;
									
									pl = strstr((const char*)NbTemp.rbuf,"86");
									memcpy(&Module.IMEI,&pl[0],15);
									 
									Debug_SendString("IMEI: ");
									Debug_SendData(Module.IMEI,15);
									Debug_SendString("\r\n");	
									
                  Module.Init_Suatus = 6;									
									break;
								}
							}
							else
							{
								Module.At_ErrCnt++;
								if(Module.At_ErrCnt > ERR_MAX)
								{
									Module.At_ErrCnt=0;
									nbSetState = 255;  // ��������
									break;
								}
								NB_SendString("AT+GSN\r\n");
				}
				break;
			
			case  6:  //��ȡCCID SIM��Ψһʶ���
				      if(NbReceive.readIndex!=NbReceive.writeIndex)
							{
								if(!popBuff(UART3))
								{
									break;
								}
								if(strstr((const char*)NbTemp.rbuf,"ERROR") != NULL)
								{						
									break;
								}
								if(strstr((const char*)NbTemp.rbuf,"46") != NULL)
								{
									Module.At_ErrCnt=0;
									
									pl = strstr((const char*)NbTemp.rbuf,"46");
									memcpy(&Module.CCID,&pl[0],15);
									 
									Debug_SendString("CCID: ");
									Debug_SendData(Module.CCID,15);
									Debug_SendString("\r\n");	
									
                  Module.Init_Suatus = 7;									
									break;
								}
							}
							else
							{
								Module.At_ErrCnt++;
								if(Module.At_ErrCnt > ERR_MAX)
								{
									Module.At_ErrCnt=0;
									Module.Init_Suatus = 255;  // ��������
									break;
								}
								NB_SendString("AT+CIMI\r\n");
				}
				break;
				
			case 7:
							if(NbReceive.readIndex!=NbReceive.writeIndex)
							{
								if(!popBuff(UART3))
								{
									break;
								}
								if(strstr((const char*)NbTemp.rbuf,"ERROR") != NULL)
								{						
									break;
								}
								if(strstr((const char*)NbTemp.rbuf,"MLVH") != NULL)
								{
									Module.At_ErrCnt=0;
									
									pl = strstr((const char*)NbTemp.rbuf,"MLVH");
									memcpy(&Module.Version,&pl[4],4);
									 
									Debug_SendString("Version: ");
									Debug_SendData(Module.Version,4);
									Debug_SendString("\r\n");	
									
                  Module.Init_Suatus = 8;									
									break;
								}
							}
							else
							{
								Module.At_ErrCnt++;
								if(Module.At_ErrCnt > ERR_MAX)
								{
									Module.At_ErrCnt=0;
									Module.Init_Suatus = 255;  // ��������
									break;
								}
								NB_SendString("AT+CGMR\r\n");
				}
				break;
			
			case 8://��ȡ������Ϣ
						 if(NbReceive.readIndex!=NbReceive.writeIndex)
						 {
							 if(!popBuff(UART3))
							 {
								 break;
							 }						 
							 if(strstr((const char*)NbTemp.rbuf,"CEREG:") != NULL)
							 {
								 pl = strstr((const char*)NbTemp.rbuf,"CEREG");
								 {
									 Module.At_ErrCnt=0;
									 
									 memcpy(&Module.NetStatus,&pl[9],1);

									 Debug_SendString("CEREG: ");
									 Debug_SendData(&Module.NetStatus,1); 
									 Debug_SendString("\r\n");
									 
								   if((Module.NetStatus==0x31)||(Module.NetStatus==0x35))
								   {
									 Module.Init_Suatus = 9;					 
									 Module.At_ErrCnt=0;

									 Debug_SendString("NET IS OK");
									 Debug_SendString("\r\n");
								   }
								 }
							 }
						 }
						 else
						 {
							 Module.At_ErrCnt++;
							 if(Module.At_ErrCnt > 30)  //30sû��ע������������
							 {
								 Module.At_ErrCnt=0;
								 
								 Debug_SendString("NET IS ERROR! ");
								 Debug_SendString("\r\n");
								 
								 Module.Init_Suatus = 255;  // ���¸�������  ���Ż�
							 }
							 NB_SendString("AT+CEREG?\r\n");	
							 Module.Delay_time = 500;
						 }
				break;

			case 9: //��ȡCSQ�ź�
						 if(NbReceive.readIndex!=NbReceive.writeIndex)
						 {
								if(!popBuff(UART3))
								{
									break;
								}
								if(strstr((const char*)NbTemp.rbuf,"CSQ:") != NULL)
								{
								 pl = strstr((const char*)NbTemp.rbuf,"CSQ:");				 
								 Module.At_ErrCnt=0;
								 								 
								 if(pl[7] == 0x2C)
								 {
									 memcpy(&Module.CSQ,&pl[5],2);
								 }
								 else if(pl[6] == 0x2C)
								 {
								   memcpy(&Module.CSQ,&pl[5],1);
								 }
								 else
								 {
									Module.Init_Suatus = 9;
									break;
								 }
								 Debug_SendString("CSQ: ");
								 Debug_SendData(Module.CSQ,2);
								 Debug_SendString("\r\n");
								 
								 if((Module.CSQ[0]=='9')&&(Module.CSQ[1]=='9'))
								 {
									 Module.Init_Suatus = 9; 
								 }
								 else if((Module.CSQ[0]=='0')&&(Module.CSQ[1]=='0'))
								 {
									 Module.Init_Suatus = 9; 
								 }
								 else
								 {
										Module.Init_Suatus = 10;  
								 }
								 
								 if(Module.CSQ[0] == 9) //LCD�ź���ʾ
								 {
										SET_VALUE_L1;RESET_VALUE_L2;RESET_VALUE_L3;RESET_VALUE_L4;
								 }
								 else if(Module.CSQ[0] == 0)
								 {
										SET_VALUE_L1;SET_VALUE_L2;RESET_VALUE_L3;RESET_VALUE_L4;
								 }
								 else if(Module.CSQ[0] == 1)
								 {
										SET_VALUE_L1;SET_VALUE_L2;SET_VALUE_L3;RESET_VALUE_L4;
								 }
								 else if(Module.CSQ[0] > 1)
								 {
										SET_VALUE_L1;SET_VALUE_L2;SET_VALUE_L3;SET_VALUE_L4;
								 }
								}
						 }
						 else
						 {
							 Module.At_ErrCnt++;
							 if(Module.At_ErrCnt > ERR_MAX)
							 {
								 Module.At_ErrCnt=0;
								 
								 Module.Init_Suatus = 255;    // ��������
								 break;
							 }
							 NB_SendString("AT+CSQ\r\n");
							 Module.Delay_time = 100;
						 }
				break;

			case 10: //��ȡʱ��
				     if(NbReceive.readIndex!=NbReceive.writeIndex)
						 {
							 if(!popBuff(UART3))
							 {
								 break;
							 }						 
							 if(strstr((const char*)NbTemp.rbuf,"CCLK:") != NULL)
							 {
								 pl = strstr((const char*)NbTemp.rbuf,"CCLK:");
								 {
									 Module.At_ErrCnt=0;
									 
									 memcpy(buf,&pl[6],18);
									 
									 Module.year = Int_to_Bcd((buf[0]-'0')*10 + (buf[1]-'0'));
									 Module.month = Int_to_Bcd((buf[3]-'0')*10 + (buf[4]-'0'));
									 Module.day = Int_to_Bcd((buf[6]-'0')*10 + (buf[7]-'0'));
									 Module.hour = Int_to_Bcd(((buf[9]-'0')*10 + (buf[10]-'0')+8)%24);
									 Module.min = Int_to_Bcd((buf[12]-'0')*10 + (buf[13]-'0'));
									 Module.sec = Int_to_Bcd((buf[15]-'0')*10 + (buf[16]-'0'));
									 
									 memcpy((u8*)(&temptime), &Module.year, 6);
								
									 RTC_SetRTC(&temptime);//����ʱ��
							
									 RTC_GetRTC(&temptime);
									 
									 memcpy(buf,(u8*)(&temptime),6);
									 
									 Debug_SendString("+CCLK: ");
									 printf("%02d-%02d-%02d, %02d:%02d:%02d",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
									 Debug_SendString("\r\n");
                   LCD_Show_Time();
									 
                   Module.Init_Suatus = 11;									 
								 }
							 }
						 }
						 else
						 {
							 Module.At_ErrCnt++;
							 if(Module.At_ErrCnt > 5)  //
							 {
								 Module.At_ErrCnt=0;
								 
								 Debug_SendString("CLK IS ERROR! ");
								 Debug_SendString("\r\n");
								 
								 Module.Init_Suatus = 255;  // ���¸�������  ���Ż�
							 }
							 NB_SendString("AT+CCLK?\r\n");	
							 Module.Delay_time = 500;
						 }
				break;

			case 11:
				      nbSetState = 11;          //�������ݽ����׶�
//			        nbSetState = 254;          //�������ݽ����׶�
			        Module.Delay_time = 1000;
						  Module.Init_Suatus = 12;
				break;
			
			case 12:
				
				break;
			
			default:
				break;
	 } 
//	 TicksDelayMs( Module.Delay_time, NULL );	//�����ʱ(Module.Delay_time);
}

void Nb_MainTask(void)
{
  char *pl;  // �ַ���ָ��
//  u8 i,j;
//	u8 recieve_len = 0;
	u8 buf[18];
////	RTC_TimeDateTypeDef temptime;
	
	switch(nbSetState)
		{
			case 0:
				NB_Init();  //����NB������Ϣ����
				break;
			
      /*******************************   ����ҵ�񽻻�   ******************************/	 
			case  11:  //�豸ע����
						 if(NbReceive.readIndex!=NbReceive.writeIndex)
						 {
							 if(!popBuff(UART3))
							 { 
								 break;
							 }						 
							 if(strstr((const char*)NbTemp.rbuf,"MIPLCREATE:") != NULL)
							 {
								 pl = strstr((const char*)NbTemp.rbuf,"MIPLCREATE");
								 {
									 Module.At_ErrCnt=0;
									 
									 memcpy(&Module.OneNet_ID,&pl[11],1);

									 Debug_SendString("MIPLCREATE: ");
									 Debug_SendData(&Module.OneNet_ID,1); 
									 Debug_SendString("\r\n");
									 

									 nbSetState= 12;					 
									 Module.At_ErrCnt=0;
									 
                   clearBuff(UART3);
									 
									 Debug_SendString("CREATION IS OK");
									 Debug_SendString("\r\n");
									 
									 Module.Delay_time = 1000;
								 }
							 }
							 else if(strstr((const char*)NbTemp.rbuf,"CIS") != NULL)
               {
									 nbSetState= 12;					 
									 Module.At_ErrCnt=0; 
								 
								   Debug_SendString(NbTemp.rbuf);
									 Debug_SendString("\r\n");
								 
								   clearBuff(UART3);
														  	   
							 }
						 }
						 else
						 {
							 Module.At_ErrCnt++;
							 if(Module.At_ErrCnt > ERR_MAX)
							 {
								 Module.At_ErrCnt=0;
								 
								 Debug_SendString("CREATION IS FAILED! ");
								 Debug_SendString("\r\n");
								 
								 nbSetState= 254;  
							 }
							 NB_SendString("AT+MIPLCREATE =49,130031F10003F2002304001100000000000000123138332E3233302E34302E34303A35363833000131F30008C000000000,0,49,0\r\n");	
							 Module.Delay_time = 7000;
						 }
				     break;	
						 
		 case  12:  //����ѹ��ʵ��
						 if(NbReceive.readIndex!=NbReceive.writeIndex)
						 {
							 if(!popBuff(UART3))
							 {
								 break;
							 }						 
							 if(strstr((const char*)NbTemp.rbuf,"OK") != NULL)
							 {
									 nbSetState= 13;					 
									 Module.At_ErrCnt=0;
                   clearBuff(UART3);
									 Debug_SendString("ADDOBJ pressure IS OK");
									 Debug_SendString("\r\n");							 
							 }
						 }
						 else
						 {
							 Module.At_ErrCnt++;
							 if(Module.At_ErrCnt > ERR_MAX)  
							 {
								 Module.At_ErrCnt=0;
								 
								 Debug_SendString("ADDOBJ pressure IS FAILED! ");
								 Debug_SendString("\r\n");
								 
								 nbSetState= 254; 
							 }
//							 NB_SendString("AT+MIPLADDOBJ=0,3200,1,\"1\",0,1\r\n");	
							 NB_SendString("AT+MIPLADDOBJ=0,3323,1,\"1\",0,1\r\n");	 //ѹ��
						 }		
				     break;	

			case  13: //����ѹ������Ϊstring
						 if(NbReceive.readIndex!=NbReceive.writeIndex)
						 {
							 if(!popBuff(UART3))
							 {
								 break;
							 }						 
							 if(strstr((const char*)NbTemp.rbuf,"OK") != NULL)
							 {
									 nbSetState= 14;					 
									 Module.At_ErrCnt=0;
									 
                   clearBuff(UART3);
							
									 Debug_SendString("DISCOVERRSP string IS OK");
									 Debug_SendString("\r\n");
								 
							 }
							 else if(strstr((const char*)NbTemp.rbuf,"CIS") != NULL)
               {
									 nbSetState= 14;					 
									 Module.At_ErrCnt=0;
								   clearBuff(UART3);
								 
								   Debug_SendString("CIS ERROR:601");
									 Debug_SendString("\r\n");
								   
							 }
						 }
						 else
						 {
							 Module.At_ErrCnt++;
							 if(Module.At_ErrCnt > ERR_MAX)
							 {
								 Module.At_ErrCnt=0;
								 
								 Debug_SendString("DISCOVERRSP string IS FAILED! ");
								 Debug_SendString("\r\n");
								 
								 nbSetState= 254;  
							 }
//							 NB_SendString("AT+MIPLDISCOVERRSP=0,3200,1,4,\"5750\"\r\n");	
							 NB_SendString("AT+MIPLDISCOVERRSP=0,3323,1,4,\"5750\"\r\n");	
							 Module.Delay_time = 1000;
						 }
				     break;

		 case  14:  //�����¶�ʵ��
						 if(NbReceive.readIndex!=NbReceive.writeIndex)
						 {
							 if(!popBuff(UART3))
							 {
								 break;
							 }						 
							 if(strstr((const char*)NbTemp.rbuf,"OK") != NULL)
							 {
									 nbSetState= 15;					 
									 Module.At_ErrCnt=0;
                   clearBuff(UART3);
									 Debug_SendString("ADDOBJ temperature IS OK");
									 Debug_SendString("\r\n");							 
							 }
						 }
						 else
						 {
							 Module.At_ErrCnt++;
							 if(Module.At_ErrCnt > ERR_MAX)  
							 {
								 Module.At_ErrCnt=0;
								 
								 Debug_SendString("ADDOBJ temperature IS FAILED! ");
								 Debug_SendString("\r\n");
								 
								 nbSetState= 254; 
							 }
//							 NB_SendString("AT+MIPLADDOBJ=0,3200,1,\"1\",0,1\r\n");	
							 NB_SendString("AT+MIPLADDOBJ=0,3303,1,\"1\",0,1\r\n");	 //�¶�
						 }		
				     break;	

			case  15: //�����¶�����Ϊstring
						 if(NbReceive.readIndex!=NbReceive.writeIndex)
						 {
							 if(!popBuff(UART3))
							 {
								 break;
							 }						 
							 if(strstr((const char*)NbTemp.rbuf,"OK") != NULL)
							 {
									 nbSetState= 16;					 
									 Module.At_ErrCnt=0;
									 
                   clearBuff(UART3);
							
									 Debug_SendString("DISCOVERRSP float IS OK");
									 Debug_SendString("\r\n");
								 
							 }
							 else if(strstr((const char*)NbTemp.rbuf,"CIS") != NULL)
               {
									 nbSetState= 16;					 
									 Module.At_ErrCnt=0;
								   clearBuff(UART3);
								 
								   Debug_SendString(NbTemp.rbuf);
									 Debug_SendString("\r\n");						   
							 }
						 }
						 else
						 {
							 Module.At_ErrCnt++;
							 if(Module.At_ErrCnt > ERR_MAX)
							 {
								 Module.At_ErrCnt=0;
								 
								 Debug_SendString("DISCOVERRSP float IS FAILED! ");
								 Debug_SendString("\r\n");
								 
								 nbSetState= 254;  
							 }
//							 NB_SendString("AT+MIPLDISCOVERRSP=0,3200,1,4,\"5750\"\r\n");	
							 NB_SendString("AT+MIPLDISCOVERRSP=0,3303,1,4,\"5701\"\r\n");	
							 Module.Delay_time = 1000;
						 }
				     break;
						 					 
			case  16:  //ģ��෢���¼����
						 if(NbReceive.readIndex!=NbReceive.writeIndex)
						 {
							 if(!popBuff(UART3))
							 {
								 break;
							 }						 
							 if(strstr((const char*)NbTemp.rbuf,"MIPLEVENT:") != NULL)
							 {
								 pl = strstr((const char*)NbTemp.rbuf,"MIPLEVENT");
								 {
									 Debug_SendString("MIPLEVENT:");
									 Debug_SendData(&pl[10],3); 
									 Debug_SendString("\r\n");	
                   Module.Delay_time = 200;									 
								 }
							 }
							 else if(strstr((const char*)NbTemp.rbuf,"MIPLOBSERVE:") != NULL)
							 {
								 pl = strstr((const char*)NbTemp.rbuf,"MIPLOBSERVE");
								 {
									 Debug_SendString("MIPLOBSERVE:");
									 Debug_SendData(&pl[12],19); 
									 Debug_SendString("\r\n");								 
								 }
							 }
							 else if(strstr((const char*)NbTemp.rbuf,"MIPLDISCOVER:") != NULL)
							 {
								 pl = strstr((const char*)NbTemp.rbuf,"MIPLDISCOVER");
								 {
									 nbSetState= 17;					 
									 Module.At_ErrCnt=0;
								   clearBuff(UART3);
									 
									 Debug_SendString("MIPLDISCOVER:");
									 Debug_SendData(&pl[13],12); 
									 Debug_SendString("\r\n");								 
								 }
							 }
							 else if(strstr((const char*)NbTemp.rbuf,"CIS") != NULL)
               {
									 nbSetState= 17;					 
									 Module.At_ErrCnt=0;
								   clearBuff(UART3);
								 
								   Debug_SendString("CIS ERROR:602");
									 Debug_SendString("\r\n");								   
							 }
						 }
						 else
						 {
							 Module.At_ErrCnt++;
							 if(Module.At_ErrCnt > ERR_MAX)
							 {
								 Module.At_ErrCnt=0;
								 
								 Debug_SendString("OPEN IS FAILED! ");
								 Debug_SendString("\r\n");
								 
								 nbSetState= 254;  
							 }
							 NB_SendString("AT+MIPLOPEN=0,3000,30\r\n");	
							 Module.Delay_time = 7000;
						 }			
				     break;	

			case  17:  //����ѹ������
						 if(NbReceive.readIndex!=NbReceive.writeIndex)
						 {
							 if(!popBuff(UART3))
							 {
								 break;
							 }						 
							 if(strstr((const char*)NbTemp.rbuf,"OK") != NULL)
							 {
									 nbSetState= 18;					 
									 Module.At_ErrCnt=0;
                   clearBuff(UART3);
									 Debug_SendString("NOTIFY IS OK");
									 Debug_SendString("\r\n");					 
							 }
						 }
						 else
						 {
							 Module.At_ErrCnt++;
							 if(Module.At_ErrCnt > ERR_MAX)  
							 {
								 Module.At_ErrCnt=0;
								 
								 Debug_SendString("NOTIFY IS FAILED! ");
								 Debug_SendString("\r\n");
								 
								 nbSetState= 254; 
							 }
														
//							 Pressure = 1234;
							 uint_to_str(Pressure,pbuf, 4);
							 //Pressure
							 sprintf(str,"AT+MIPLNOTIFY=0,0,3323,0,5750,1,4,%s,0,0\r\n",pbuf);
//							 sprintf(str,"AT+MIPLNOTIFY=0,0,3200,0,5750,1,4,%s,0,0\r\n",pbuf);
							 NB_SendString(str);	
//							 NB_SendString("AT+MIPLNOTIFY=0,0,3200,0,5750,1,4,\"1234567890\",0,0\r\n");	
							 Module.Delay_time = 3000;						 
						 }	
				     break;	
						 
			case  18:  //�����¶�����
						 if(NbReceive.readIndex!=NbReceive.writeIndex)
						 {
							 if(!popBuff(UART3))
							 {
								 break;
							 }						 
							 if(strstr((const char*)NbTemp.rbuf,"OK") != NULL)
							 {
									 nbSetState= 254;					 
									 Module.At_ErrCnt=0;
                   clearBuff(UART3);
									 Debug_SendString("NOTIFY IS OK");
									 Debug_SendString("\r\n");					 
							 }
						 }
						 else
						 {
							 Module.At_ErrCnt++;
							 if(Module.At_ErrCnt > ERR_MAX)  
							 {
								 Module.At_ErrCnt=0;
								 
								 Debug_SendString("NOTIFY IS FAILED! ");
								 Debug_SendString("\r\n");
								 
								 nbSetState= 254; 
							 }
//							 sprintf(str,"AT+MIPLNOTIFY=0,0,3303,0,5701,1,4,%s,0,0\r\n",pbuf);
//							 sprintf(str,"AT+MIPLNOTIFY=0,0,3200,0,5750,1,4,%s,0,0\r\n",pbuf);
//							 NB_SendString(str);	
							 NB_SendString("AT+MIPLNOTIFY=0,0,3303,0,5701,1,4,\"25\",0,0\r\n");	
							 Module.Delay_time = 3000;						 
						 }	
				     break;
						 
			case  254:  //��������ģʽ
				
			     NB_SendString("AT*SLEEP=1\r\n");
			
				   Debug_SendString("ģ���������......\r\n");
			     
			     Module.TimeWake = 0;
           nbSetState = 255;
			     
				     break;							 
			
			case  255:  //��������ģʽ
				
           nbSetState = 255;
			
				     break;	
			
			default :
				     break;
		}
		TicksDelayMs( Module.Delay_time, NULL );	//�����ʱ(Module.Delay_time);
}

//static void Nb_Send_Baowen(void)
//{
//	u8 buf[60];
//	char p_buf[60] = {0};
//	char p_buf1[100] = {0};
//	u16 crc;
//	u8 i=0,j=0;
//	
//	bsp_NbLoadParam();
//	
//	buf[0] = 0x68;
//	
//  AT24CXX_Read(4,&buf[1],7);
//	
//	memcpy(&buf[8],&Module.year,14);
//	
//	buf[22] = Module.send_cnt >> 8;
//	buf[23] = Module.send_cnt;
//	
//	buf[24] = Module.voltage >> 8;
//	buf[25] = Module.voltage;
//	
//	buf[26] = Module.rssi;
//	
//	Nb_Get_State(&Module.state);
//	
//	buf[27] = Module.state >> 8;
//	buf[28] = Module.state;
//	
//	buf[29] = Version_H;
//	buf[30] = Version_L;
//	
//	memset(&buf[31],0x00,16);
//	
//	crc = CRC16_Modbus(buf,47);
//	
//	buf[47] = crc >> 8;
//	buf[48] = crc;
//	buf[49] = 0x16;
//	
//	for(i = 0;i < 50;i ++)
//	{
//			if(buf[i] == 0X00)
//			{
//					sprintf(&p_buf[j],"%s","00");
//			}
//			else if(buf[i] < 0X10)
//			{
//					sprintf(&p_buf[j],"%s","0");
//					sprintf(&p_buf[j + 1],"%x",buf[i]);
//			}
//			else 
//			{							
//					sprintf(&p_buf[j],"%x",buf[i]);
//			}
//			
//			j = j + 2;
//	}
//	
//	
//				
//	sprintf(p_buf1,"AT+NMGS=50,%s\r\n",p_buf);
//	
//	Debug_SendString((u8*)p_buf1);
//	
////	NB_SendString(p_buf1);
//						
//}

//static void Nb_Receive_Baowen(u8* databuf, u8 len)
//{
//	u8 i;
//	u8 temp[36];
//	u16 crc;
//	u8 crc_h,crc_l;
//	
//	static u8 err = 0;
//	
//	for(i=0;i<len;i=i+2)
//	{
//		temp[i] = (databuf[i]-'0')<<8 + (databuf[i+1]-'0');
//	}
//	
//	crc = CRC16_Modbus(temp,len/2-3);
//	crc_h = crc >> 8;
//	crc_l = crc;
//	
//	if((temp[0] == 0x68) && (temp[len-1] == 0x16) && (temp[len-2] == crc_h) && (temp[len-3] == crc_l) )
//	{
//		
//		for(i=0;i<7;i++)
//		{
//			if(Module.DeviceID[i] !=  temp[i])
//			{
//				return;
//			}
//			
//			switch(temp[8])
//			{
//				case 0x70:Nb_RecieveData_Process(temp);
//					break;
//				
//				case 0x29:	
//				  Debug_SendString("ҵ����δ��ɣ����ڼ�������......�\r\n");			
//				  break;
//			
//				case 0x07:
//				  Debug_SendString("ʱ���쳣���\r\n");
//					break;
//				
//				case 0x10:
//				  Debug_SendString("�����������쳣���\r\n");
//					break;
//				
//				case 0x17:
//				  Debug_SendString("CRC��֤ʧ�ܣ��\r\n");
//					break;
//				
//				case 0x24:
//				  Debug_SendString("�豸�����ڣ��\r\n");
//					break;
//				
//				case 0x30:
//				  Debug_SendString("δ֪���󣡡\r\n");
//					break;
//				
//				default:break;
//			}
//		}
//		err = 0;
//	}
//	else
//  {
//		if(!Module.buffered)
//		{
//			if(++err < 5)
//			{
//				nbSetState = 11;
//			}
//			else
//			{
//				err = 0;
//				nbSetState = 255;
//				Module.ControlFlag = 1;
//			}
//	  }
//		else
//    {
//			nbSetState = 12;
//		}
//	}
//	
//}

void Nb_RecieveData_Process(u8* databuf)
{
//	RTC_TimeDateTypeDef temptime;
	static u8 err = 0;
	
	if(Module.send_cnt == (databuf[15] << 8) + databuf[16])
	{
		Module.send_cnt++;
		
//		Module.year = databuf[9];
//		Module.month = databuf[10];
//		Module.day = databuf[11];
//		Module.hour = databuf[12];
//		Module.min = databuf[13];
//		Module.sec = databuf[14];
//		
//		memcpy((u8*)(&temptime), &Module.year, 6);
//		
//		RTC_SetRTC(&temptime);//����ʱ��
		
		switch(databuf[17])
		{
			case 0x00:
				     if(Module.ControlFlag == 1)
						 {
							 nbSetState = 255;
						 }
						 
						 if(Module.ControlFlag == 2)
						 {
							 Module.Flash_Address = Flash_Address_Temp + 14;
							 
							 nbSetState = 11;
							 
							 if(Module.Flash_Address > Flash_Address_Temp)
							 {
								 nbSetState = 255;
								 Module.ControlFlag = 1;
							 }
							 
						 }
			
				break;
			case 0x01:
////				     Module.Flash_Address =  AT24CXX_ReadLenByte(0,4);
			
			       Module.ControlFlag = 1;
		         nbSetState = 11;         //������ʱ�������
				
				break;
			case 0x02:
////				     Flash_Address_Temp =  AT24CXX_ReadLenByte(0,4);
////             Module.Flash_Address = Flash_Address_Temp - 14*9;
			
				     Module.ControlFlag = 2;
		         nbSetState = 11;         //����ʷʱ�������
			
				break;
			case 0x10:
				
//						 Module.year = databuf[18];
//						 Module.month = databuf[19];
//						 Module.day = databuf[20];
//						 Module.hour = databuf[21];
//						 Module.min = databuf[22];
//						 Module.sec = databuf[23];
//						
//						 memcpy((u8*)(&temptime), &Module.year, 6);
//						
//						 RTC_SetRTC(&temptime);//����ʱ��
			
				break;
			case 0x11:
				
//						 Module.OnlineTime[0] = databuf[18];
//						 Module.OnlineTime[1] = databuf[19];
//						 Module.OnlineTime[2] = databuf[20];
//						 Module.OnlineTime[3] = databuf[21];
//						 Module.OnlineTime[4] = databuf[22];
//						 Module.OnlineTime[5] = databuf[23];
//			
//			       Module.OnlineDelay = (databuf[24]<<8) + databuf[25];
//			
//			       AT24CXX_Write(11,Module.OnlineTime,8);
			
				break;
			case 0x12:
				break;
		}
		err = 0;
	}
	else
	{
		
		if(!Module.buffered)
		{
			if(++err < 5)
			{
				nbSetState = 11;
			}
			else
			{
				err = 0;
				nbSetState = 255;
				Module.ControlFlag = 1;
			}
	  }
		else
    {
			nbSetState = 12;
		}
	}
	 
	
}

//static void Nb_Get_State(u16* state)
//{
//	*state = 0;   //����ȡ
//}

void Nb_Check_Time(void)
{
	RTC_TimeDateTypeDef rtc_temp;
	u16 k;
	u16 temp;
	u8 buf[6];
	
	static u8 temp_hour,temp_min;
	
	if(Module.TimeWake != 0)
	{
		return;
	}
	
	RTC_GetRTC(&rtc_temp);
	
  memcpy(&Module.year,(u8*)(&rtc_temp),6);
	
	memcpy(buf, &Module.year, 6);
	printf("Time: %02d-%02d-%02d, %02d:%02d:%02d\r\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
	
	
	
	if((temp_hour == Module.hour) && (temp_min == Module.min))
	{
		return;
	}
	temp_hour = Module.hour;
	temp_min = Module.min;
	
	if(Module.hour > Module.OnlineTime[3])
	{
		temp = (u16)(Module.hour-Module.OnlineTime[3])*60 + (u16)Module.min - (u16)Module.OnlineTime[4];
	}
	else if(Module.hour < Module.OnlineTime[3])
	{
		temp = (u16)((Module.hour+24)-Module.OnlineTime[3])*60 + (u16)Module.min - (u16)Module.OnlineTime[4];
	}
	else
	{
		if(Module.min >= Module.OnlineTime[4])
		{
			temp = Module.min - Module.OnlineTime[4];
		}
		else
		{
			temp = Module.OnlineTime[4] - Module.min;
		}
	}
	
  Module.OnlineCount = 24 * 60 / Module.OnlineDelay;
	
	for(k=0;k<Module.OnlineCount;k++) //ѭ������ÿ����Ҫ�ϴ���ʱ���  
	{
		if(temp == (k * Module.OnlineDelay))
	 {	 
			Maintask_Status = State_NB;;
			Module.TimeWake = 180;
		  nbSetState = 18;
	 }
	}
}
