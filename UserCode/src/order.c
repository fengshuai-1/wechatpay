#include "bsp.h"

void Creat_order(void)  //��������
{
//	Count_money();        //��rtc����ʱ�������� 
	Conect_HTTPS();       //��΢��֧���ӿڽ�������	
	Make_xml();           //���xml
	Send_HTTPS();         //����xml�����ն�ά������
}

//void Count_money(void)     //������
//{



//}



