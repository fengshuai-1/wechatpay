#include "bsp.h"
int state = 0;
uint8_t key_ret = 0;	
uint32_t storagePasswd = 0;  //储存密码
uint32_t use_time = 0;   //使用时间
uint8_t buf = 0; //密码缓存

void disPlayFunctionSelect(void)
{		
			Show_Str(40,40,BLUE,WHITE,"智慧储物柜",32,0);
			Show_Str(100,100,BLUE,GREEN,"存件",32,0);
			Show_Str(100,140,BLUE,WHITE,"取件",32,0); 
			Show_Str(50,240,BLUE,WHITE,"---Func Select---",16,0);  
			Gui_DrawBat(30,270,gImage_3);//显示微信支付logo
}

void disPlayFunctionSelect1(void)
{		
			Show_Str(40,40,BLUE,WHITE,"智慧储物柜",32,0);
			Show_Str(100,100,BLUE,WHITE,"存件",32,0);
			Show_Str(100,140,BLUE,GREEN,"取件",32,0); 
			Show_Str(50,240,BLUE,WHITE,"---Func Select---",16,0);  
			Gui_DrawBat(30,270,gImage_3);//显示微信支付logo
}

void disPlayFunctionSelect2(void)  //显示存件信息
{
		LCD_Fill(0,100,240,320,WHITE);   //画白色矩阵清屏
		Show_Str(0,120,BLUE,WHITE,"使用时间：",32,0);
		Show_Str(208,120,RED,WHITE,"秒",32,0);
		Show_Str(0,180,BLUE,WHITE,"使用金额：",32,0); 
		Show_Str(208,180,RED,WHITE,"分",32,0);
		Show_Str(66,266,MAGENTA,WHITE,"-->>",32,0); 
		Show_Str(96,260,BLUE,GREEN,"取件",32,0);  
}

/* 设置密码 */
void setPasswd(void)
{
		Show_Str(0,150,BLUE,WHITE,"设置取货码：",32,0);
		storagePasswd =getPassword();//储存密码
    LCD_Fill(0,100,240,320,WHITE);   //画白色矩阵清屏	
		Show_Str(50,150,BLUE,WHITE,"录入成功",32,0); 
    
    bsp_DelayMS(1000);
    Open();
}

/* 读取密码 */
uint32_t getPassword(void)
{
//    uint8_t buf = 0; //密码缓存
    uint8_t cnt = 0;
    uint32_t passwd = 0;
    disEnterPasswd(0);
    for (cnt = 0; cnt < 4;)
    {
        buf = KEY_SCAN();
        if (buf <= 9)		//判断值是否在0~9之间
        {
            passwd = passwd * 10 + buf;
            cnt++;
            disEnterPasswd(cnt);
        }
    }
    disEnterPasswd(4);
    bsp_DelayMS(500);
    return passwd;
}

/* 显示输入的密码长度 */
void disEnterPasswd(uint8_t passWordLength)
{ 

    switch (passWordLength)
    {
    case 0:
			
				Show_Str(185,162,BLUE,WHITE,"----",16,0); 
				
        break;

    case 1:			
				Show_Str(193,162,BLUE,WHITE,"---",16,0); 
				LCD_ShowNum(185,162,buf,1,16,RED,WHITE,0);   //显示第1位密码	
        break;

    case 2:
				Show_Str(201,162,BLUE,WHITE,"--",16,0); 
				LCD_ShowNum(193,162,buf,1,16,RED,WHITE,0);   //显示第2位密码	

        break;

    case 3:
				Show_Str(209,162,BLUE,WHITE,"-",16,0); 
				LCD_ShowNum(201,162,buf,1,16,RED,WHITE,0);   //显示第3位密码	
        break;

    case 4:
//				Show_Str(185,162,BLUE,WHITE,"****",16,0); 
				LCD_ShowNum(209,162,buf,1,16,RED,WHITE,0);   //显示第4位密码	
        break;
		
    default:
        break;
    }
}

/* 通过密码确认 */
void unlockByPasswd(void)
{	
    uint32_t inputPasswd = 0;
	  LCD_Fill(0,100,240,320,WHITE);   //画白色矩阵清屏
		Show_Str(0,150,BLUE,WHITE,"输入取货码：",32,0);
    inputPasswd = getPassword();
    if (inputPasswd == storagePasswd)
    {
			
					//开始下单
					LCD_Fill(0,33,240,320,WHITE);   //画白色矩阵清屏	
					Show_Str(50,150,BLUE,WHITE,"取货码正确",32,0); 
					bsp_DelayMS(500);	
					LCD_Fill(0,33,240,320,WHITE);   //画白色矩阵清屏		
					Gui_Drawbmp16(60,100,wechat_logo,120);  //微信支付logo
						
				
					Make_xml(); //组合xml字符串
					HTTPS_para();//连接微信支付端口
					bsp_DelayMS(300);	
					Conect_HTTPS();// 与微信接口建立连接			
					Send_HTTPS();  // 向微信支付接口发送xml
					LCD_Fill(0,33,240,320,WHITE);   //画白色矩阵清屏	
					Code_url();    //截取二维码链接			
					state=5;
    }
    else
    {
					LCD_Fill(0,33,240,320,WHITE);   //画白色矩阵清屏	
					Show_Str(50,150,BRED,WHITE,"取货码错误",32,0); 
					bsp_DelayMS(500);	
					LCD_Fill(0,33,240,320,WHITE);   //画白色矩阵清屏	
					unlockByPasswd();		 //再次取货码对比

    }
    bsp_DelayMS(1000);
}

void functionSelect(void)
{
		key_ret = KEY_SCAN();//获取键值
		switch(state)
		{
			case 0:										
					Show_Str(60,106,MAGENTA,WHITE,"-->>",32,0); 
					disPlayFunctionSelect();
					if(key_ret==KEY_ENTWE) state=3;
						if(key_ret==KEY_DOWN) state=1;
							if(key_ret==KEY_UP) state=2;
					break;
			
			case 1:
					Show_Str(60,106,BLUE,WHITE,"    ",32,0); 
					Show_Str(60,146,MAGENTA,WHITE,"-->>",32,0); 
					disPlayFunctionSelect1();					
						if(key_ret==KEY_UP) state=2;
					break;
			
			case 2:
					Show_Str(60,106,MAGENTA,WHITE,"-->>",32,0); 
					Show_Str(60,146,BLUE,WHITE,"    ",32,0); 
					disPlayFunctionSelect();
					if(key_ret==KEY_DOWN) state=1;
						 if(key_ret==KEY_ENTWE) state=3;
					break;
			
			case 3:
					LCD_Fill(0,100,240,320,WHITE);   //画白色矩阵清屏
					setPasswd(); //设置密码	
					disPlayFunctionSelect2();		//显示存件信息	
					timer_enable(TIMER2);  //定时器2							
					state=4;						
					break;
					
			case 4:		
					LCD_ShowNum(150,130,use_time,5,16,RED,WHITE,0);   //显示使用时间	
					LCD_ShowNum(150,190,total_fee,4,16,RED,WHITE,0);   //显示金额	
					if (key_ret == KEY_ENTWE)	
					{	
						timer_disable(TIMER2);  //关闭定时器2
						
						unlockByPasswd();		 //取货码对比
						
					}
					break;
			
			case 5:
						use_time = 1;  //使用时间清0
						total_fee = 1;  //金额变成初始值
						timer_enable(TIMER1);  //定时器1		
						Make_xml1();   //组合查询xml
						state=6;
			
			case 6:
					if(t1_flag == 1)	
					{	
						t1_flag = 0;     //定时器中断每秒查询付款接口一次
						HTTPS_check();   //连接微信查询端口
						Conect_HTTPS(); // 与微信接口建立连接
						Check_order();  //查询订单状态
						trade_state();//支付状态判断
					}					
				break;			
		
		}

}
