#include "bsp.h"
#include "ziku.h"
uint32_t temp = 0;
uint32_t max_counter = 0, reset_value = 0, year = 0;	

int main(void)
{	   
		bsp_Init(); //初始化
		Show_Str(50,150,BLUE,WHITE,"加载中...",32,0); 
		N58_init();	//4G模块N58初始化
		bsp_DelayMS(10);
		LCD_Fill(0,20,240,320,WHITE);   //画白色矩阵清屏	
//		gps_init();		//GPS初始化
		while(1)
		{
			functionSelect(); //菜单选择	
		
			if(t2_flag == 1 && state == 4)  //计入计时
			{
				t2_flag = 0;			
				gd_eval_led_toggle(LED2);  //翻转LED灯提示			
			}			
							
			if(use_time % 30 == 0) 
			{
				total_fee = use_time/30 + 1;  //计算金额 10秒一分钱
				if(total_fee > 1000)  total_fee=1000;
			}
			
			if(Is1S == 1)  //1s中断  ：做时间刷新显示
			{

					Is1S =0;
					temp = rtc_counter_get();
					print_calendar(temp);

					year = (temp >> 25) + MIN_YEAR;
					max_counter = IS_LEAP_YEAR(year) ? (366 * 24 * 3600) : (365 * 24 * 3600);

					/* reset RTC counter when time is 31st Dec. 23:59:59 */
					if((temp & 0x01FFFFFF) == (max_counter - 1))
					{
							year++;
							if(year > MAX_YEAR){
									year = MIN_YEAR;
							}
							reset_value = ((year - MIN_YEAR) << 25);

							/* set new year's value to RTC counter register */
							rtc_lwoff_wait();
							rtc_counter_set(reset_value);
							rtc_lwoff_wait();
					}
 
					usb_Rx();  //USB数据定时接收检测
				}

		}

}






