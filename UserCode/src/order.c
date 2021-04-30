#include "bsp.h"

void Creat_order(void)  //创建订单
{
//	Count_money();        //从rtc计算时间算出金额 
	Conect_HTTPS();       //与微信支付接口建立连接	
	Make_xml();           //组合xml
	Send_HTTPS();         //发送xml并接收二维码链接
}

//void Count_money(void)     //计算金额
//{



//}



