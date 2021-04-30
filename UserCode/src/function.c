#include "bsp.h"
//#include "font.h"
#include "ziku.h"

//uint32_t temp = 0;
//uint32_t max_counter = 0, reset_value = 0, year = 0;
uint32_t storagePasswd = 0;  //储存密码


/* 显示功能选择界面 */
void disPlayFunctionSelect(void)
{		
//		Show_Str(60,260,BLUE,CYAN, "平均功率",24,0);

//		Show_Str(60,100,BLUE,WHITE,"->",16,0); 
//		Show_Str(50,40,BLUE,WHITE,"字体测试",32,0); 
//		Show_Str(80,100,BLUE,WHITE,"Save parts",16,0);
//		Show_Str(80,120,BLUE,WHITE,"Take things",16,0); 
//		Show_Str(50,250,BLUE,WHITE,"---Func Select---",16,0);    
}

void disPlayFunctionSelect1(void)
{
		LCD_Fill(0,20,240,320,WHITE);   //画白色矩阵清屏

		Show_Str(20,40,BLUE,WHITE,"--smart locker--",16,0); 
		Show_Str(80,100,BLUE,WHITE,"Used already",16,0);
		Show_Str(80,120,BLUE,WHITE,"money",16,0); 
		Show_Str(80,140,BLUE,WHITE,"->",16,0); 
		Show_Str(100,140,BLUE,WHITE,"Take things",16,0);  



}


/* 显示输入的密码长度 */
void disEnterPasswd(uint8_t passWordLength)
{
		Show_Str(0,100,BLUE,WHITE,"Set the pickup code:",16,0); 

    switch (passWordLength)
    {
    case 0:
			
				Show_Str(160,100,BLUE,WHITE,"----",16,0); 

        break;

    case 1:			
				Show_Str(160,100,BLUE,WHITE,"*---",16,0); 

        break;

    case 2:
				Show_Str(160,100,BLUE,WHITE,"**--",16,0); 
        break;

    case 3:
				Show_Str(160,100,BLUE,WHITE,"***-",16,0); 
        break;

    case 4:
				Show_Str(160,100,BLUE,WHITE,"****",16,0); 
        break;
		
    default:
        break;
    }
}

/* 读取密码 */
uint32_t getPassword(void)
{
    uint8_t buf = 0;
    uint8_t cnt = 0;
    uint32_t passwd = 0;
    LCD_Fill(0,20,240,320,WHITE);   //画白色矩阵清屏
    disEnterPasswd(0);
    for (cnt = 0; cnt < 4;)
    {
        buf = KEY_SCAN();
        if (buf != 0xff)
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

/* 设置密码 */
void setPasswd(void)
{
		storagePasswd =getPassword();//储存密码
    LCD_Fill(0,20,240,320,WHITE);   //画白色矩阵清屏
		Show_Str(0,150,BLUE,WHITE,"success!",16,0); 
    bsp_DelayMS(1000);
}

/* 通过密码确认 */
void unlockByPasswd(void)
{
    uint32_t inputPasswd = 0;
    inputPasswd = getPassword();
    LCD_Fill(0,20,240,320,WHITE);   //画白色矩阵清屏
    if (inputPasswd == storagePasswd)
    {
					Show_Str(0,150,BLUE,WHITE,"Identfy succ!",16,0); 
					bsp_DelayMS(500);	
					LCD_Fill(0,20,240,320,WHITE);   //画白色矩阵清屏
					Make_xml(); //组合xml字符串
					Conect_HTTPS();// 与微信接口建立连接
					Send_HTTPS();  // 向微信支付接口发送xml
					Code_url();    //截取二维码链接
					
    }
    else
    {
					Show_Str(0,150,BLUE,WHITE,"Identfy Failed",16,0); 
					
    }
    bsp_DelayMS(1000);
}
/* 1.功能选择 */
int Ordering(void)
{
    uint8_t key_ret = 0;
    disPlayFunctionSelect1();
    while (1)
    {
        key_ret = KEY_SCAN();
        if (key_ret == KEY_ENTWE) 
        {
					unlockByPasswd();		
        }
     
    }
}

/*功能选择*/
void functionSelect10(void)
{
//    uint8_t key_ret = 0;
//    int fun_num = 0;
		
//    disPlayFunctionSelect();
			Show_Str(20,40,BLUE,WHITE,"智慧",32,0);
			bsp_DelayMS(1000);
//    while (1);
//    {
//        key_ret = KEY_SCAN();//获取键值
//				switch (key_ret)
//        {
//        case KEY_DOWN:
//            fun_num++;
//            if (fun_num == 2)
//                fun_num = 0;
//            break;

//        case KEY_UP:
//            fun_num--;
//            if (fun_num == -1)
//                fun_num = 1;
//            break;
//        default:
//            break;
//        }
//				if (fun_num == 0) /* 向上选择*/
//				{		
//					Show_Str(60,100,BLUE,WHITE,"->",16,0); 
//					Show_Str(60,120,BLUE,WHITE,"  ",16,0); 
//					disPlayFunctionSelect();
//					
//						if(key_ret == KEY_ENTWE)
//						{
//							LCD_Fill(0,20,240,320,WHITE);   //画白色矩阵清屏
//							setPasswd(); //设置密码	
//							Ordering();		
//						}	
//					
//            
//        }

//        if (fun_num == 1) /* 向下选择 */
//        {
//						Show_Str(60,100,BLUE,WHITE,"  ",16,0); 
//						Show_Str(60,120,BLUE,WHITE,"->",16,0); 
//						disPlayFunctionSelect();
//        }

//        if (key_ret == 0xff) /* 设置 */
//        {
////						if(Is1S == 1)  //1s中断  ：做时间刷新显示
////						{
////								gd_eval_led_toggle(LED2);  //翻转LED灯提示

////								Is1S =0;
////								temp = rtc_counter_get();
////								print_calendar(temp);

////								year = (temp >> 25) + MIN_YEAR;
////								max_counter = IS_LEAP_YEAR(year) ? (366 * 24 * 3600) : (365 * 24 * 3600);

////								/* reset RTC counter when time is 31st Dec. 23:59:59 */
////								if((temp & 0x01FFFFFF) == (max_counter - 1))
////								{
////                year++;
////                if(year > MAX_YEAR){
////                    year = MIN_YEAR;
////									}
////                reset_value = ((year - MIN_YEAR) << 25);

////                /* set new year's value to RTC counter register */
////                rtc_lwoff_wait();
////                rtc_counter_set(reset_value);
////                rtc_lwoff_wait();
////								}
////       
////								usb_Rx();  //USB数据定时接收检测
////						}
//					}//rtc延时
//        
//    }
}
