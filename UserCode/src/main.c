#include "bsp.h"
#include "ziku.h"
uint32_t temp = 0;
uint32_t max_counter = 0, reset_value = 0, year = 0;	

int main(void)
{	   
		bsp_Init(); //��ʼ��
		Show_Str(50,150,BLUE,WHITE,"������...",32,0); 
		N58_init();	//4Gģ��N58��ʼ��
		bsp_DelayMS(10);
		LCD_Fill(0,20,240,320,WHITE);   //����ɫ��������	
//		gps_init();		//GPS��ʼ��
		while(1)
		{
			functionSelect(); //�˵�ѡ��	
		
			if(t2_flag == 1 && state == 4)  //�����ʱ
			{
				t2_flag = 0;			
				gd_eval_led_toggle(LED2);  //��תLED����ʾ			
			}			
							
			if(use_time % 30 == 0) 
			{
				total_fee = use_time/30 + 1;  //������ 10��һ��Ǯ
				if(total_fee > 1000)  total_fee=1000;
			}
			
			if(Is1S == 1)  //1s�ж�  ����ʱ��ˢ����ʾ
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
 
					usb_Rx();  //USB���ݶ�ʱ���ռ��
				}

		}

}






