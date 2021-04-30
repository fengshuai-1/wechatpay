#ifndef __BSP_LCD_H
#define __BSP_LCD_H		
 
#include "bsp.h" 
//��������:2018/08/22
//�汾��V1.0
/****************************************************************************************************
//=========================================��Դ����================================================//
//     LCDģ��                STM32��Ƭ��
//      VCC          ��          3.3V         //��Դ
//      GND          ��          GND          //��Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������Ϊ4����SPI����
//     LCDģ��                STM32��Ƭ��    
//       SDA         ��          PB15         //Һ����SPI��������д�ź�
//=======================================Һ���������߽���==========================================//
//     LCDģ�� 					      STM32��Ƭ�� 
//       BLK         ��          PB9          //Һ������������źţ��������Ҫ���ƣ���3.3V
//       SCK         ��          PB13         //Һ����SPI����ʱ���ź�
//       DC          ��          PB10         //Һ��������/��������ź�
//       RES         ��          PB12         //Һ������λ�����ź�
//       CS          ��          PB11         //Һ����Ƭѡ�����ź�
//=========================================������������=========================================//
//���ģ�鲻���������ܻ��ߴ��д������ܣ����ǲ���Ҫ�������ܣ�����Ҫ���д���������
//	   LCDģ��                STM32��Ƭ�� 
//      T_IRQ        ��          PC10         //�����������ж��ź�
//      T_DO         ��          PC2          //������SPI���߶��ź�
//      T_DIN        ��          PC3          //������SPI����д�ź�
//      T_CS         ��          PC13         //������Ƭѡ�����ź�
//      T_CLK        ��          PC0          //������SPI����ʱ���ź�
**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	

//LCD��Ҫ������
typedef struct  
{										    
	uint16_t width;			//LCD ���
	uint16_t height;			//LCD �߶�
	uint16_t id;				  //LCD ID
	uint8_t  dir;			  //���������������ƣ�0��������1��������	
	uint16_t	 wramcmd;		//��ʼдgramָ��
	uint16_t  setxcmd;		//����x����ָ��
	uint16_t  setycmd;		//����y����ָ��	
}_lcd_dev; 	

//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����
/////////////////////////////////////�û�������///////////////////////////////////	 
#define USE_HORIZONTAL  	 0 //����Һ����˳ʱ����ת���� 	0-0����ת��1-90����ת��2-180����ת��3-270����ת

//////////////////////////////////////////////////////////////////////////////////	  
//����LCD�ĳߴ�
#define LCD_W 240
#define LCD_H 320

//TFTLCD������Ҫ���õĺ���		   
extern uint16_t  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern uint16_t  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

////////////////////////////////////////////////////////////////////
//-----------------LCD�˿ڶ���---------------- 
#define GPIO_TYPE  GPIOA  //GPIO������
#define LED      9        //�����������        PB9
#define LCD_CS   4       //Ƭѡ����            PB11
#define LCD_RS   6       //�Ĵ���/����ѡ������ PB10 
#define LCD_RST  0       //��λ����            PB12


//QDtechȫϵ��ģ������������ܿ��Ʊ��������û�Ҳ���Խ�PWM���ڱ�������
//#define	LCD_LED    PBout(LED) //LCD����    		 PB9
//���ʹ�ùٷ��⺯���������еײ㣬�ٶȽ����½���14֡ÿ�룬���������˾�Ƽ�����
//����IO����ֱ�Ӳ����Ĵ���������IO������ˢ�����ʿ��Դﵽ28֡ÿ�룡 

////////GPIO��λ�����ߣ�
//////#define	LCD_CS_SET    GPIO_TYPE->BSRR=1<<LCD_CS    //Ƭѡ�˿�  	
//////#define	LCD_RS_SET	  GPIO_TYPE->BSRR=1<<LCD_RS    //����/����    
//////#define	LCD_RST_SET	  GPIOB->BSRR=1<<LCD_RST       //��λ			 

////////GPIO��λ�����ͣ�							    
//////#define	LCD_CS_CLR    GPIO_TYPE->BRR=1<<LCD_CS     //Ƭѡ�˿�  	
//////#define	LCD_RS_CLR	  GPIO_TYPE->BRR=1<<LCD_RS     //����/����  	 
//////#define	LCD_RST_CLR	  GPIOB->BRR=1<<LCD_RST        //��λ			  

//GPIO��λ�����ߣ�
#define	LCD_CS_SET    gpio_bit_set(GPIOA,GPIO_PIN_4)
#define	LCD_RS_SET    gpio_bit_set(GPIOA,GPIO_PIN_6)
#define	LCD_RST_SET   gpio_bit_set(GPIOC,GPIO_PIN_4)

//GPIO��λ�����ͣ�
#define	LCD_CS_CLR    gpio_bit_reset(GPIOA,GPIO_PIN_4);
#define	LCD_RS_CLR    gpio_bit_reset(GPIOA,GPIO_PIN_6);
#define	LCD_RST_CLR   gpio_bit_reset(GPIOC,GPIO_PIN_4);

//������ɫ
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 		0XFFE0
#define GBLUE		0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 		0XBC40 //��ɫ
#define BRRED 		0XFC07 //�غ�ɫ
#define GRAY  		0X8430 //��ɫ
#define GRAY0       0xEF7D 
#define GRAY1       0x8410 //��ɫ1      00000 000000 00000
#define GRAY2       0x4208 
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	0X841F //ǳ��ɫ
#define LIGHTGRAY     0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 		0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE      	0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE          0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
	    															  
void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(uint16_t Color);	 
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_DrawPoint(uint16_t x,uint16_t y);//����
uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y); //����
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);		   
void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);

uint16_t LCD_RD_DATA(void);//��ȡLCD����									    
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
void LCD_WR_DATA(uint8_t data);
uint16_t LCD_ReadReg(uint8_t LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(uint16_t RGB_Code);
uint16_t LCD_ReadRAM(void);		   
uint16_t LCD_BGR2RGB(uint16_t c);
void LCD_SetParam(void);
void Lcd_WriteData_16Bit(uint16_t Data);
void LCD_direction(uint8_t direction );

void DrawTestPage(uint8_t *str);
void Display_ButtonUp(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void menu_test(void);
void Test_Color(void);
void Test_FillRec(void);
void Test_Circle(void);
void Test_Triangle(void);
void English_Font_test(void);
void Chinese_Font_test(void);
void Pic_test(void);
void Load_Drow_Dialog(void);
void main_test(void);
void Rotate_Test(void);


//�����Ȼ�����ٶȲ����죬����ʹ������ĺ궨��,����ٶ�.
//ע��Ҫȥ��lcd.c��void LCD_WR_DATA(uint16_t data)��������Ŷ
/*
#if LCD_USE8BIT_MODEL==1//ʹ��8λ������������ģʽ
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	DATAOUT(data<<8);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	}
	#else//ʹ��16λ������������ģʽ
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	} 	
*/
				  		  
	 
	 




//////////////////////////////////////////////////////////////////////
////-----------------LCD�˿ڶ���---------------- 
//#define	LCD_CS_SET    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_9)    //Ƭѡ�˿�  	 PC9
//#define	LCD_DC_SET	  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_11)    //����/����    PC8	   
//#define	LCD_SDI_SET	  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_13)    //����			   PC7
//#define	LCD_SCK_SET	  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_12)    //ʱ��    		 PC6
//								    
//#define	LCD_CS_CLR    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9)     //Ƭѡ�˿�  PC9
//#define	LCD_DC_CLR	  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_11)     //����/���� PC8	   
//#define	LCD_SDI_CLR	  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_13)     //����			 PC7
//#define	LCD_SCK_CLR	  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_12)     //ʱ��			 PC6	    																						  

//extern unsigned char BMP1[][16];
//typedef enum
//{
//	FONT_5X8 = 1,
//	FONT_8X16,
//	FONT_16X16,
//	FONT_128X64,
//	SCRIBING,
//    FONT_16X8
//}Typeface;

///* �������Խṹ, ����LCD_DispStr() */
//typedef struct
//{
//	uint16_t FontCode;	/* ������� 0 ��ʾ16���� */
//	uint16_t FrontColor;	/* ������ɫ */
//	uint16_t BackColor;	/* ���ֱ�����ɫ��͸�� */
//	uint16_t Space;		/* ���ּ�࣬��λ = ���� */
//}FONT_T;


///* ������� */
//typedef enum
//{
//	FC_ST_16 = 0,		/* ����15x16���� ����x�ߣ� */
//	FC_ST_12 = 1,		/* ����12x12���� ����x�ߣ� */
//	FC_ST_24 = 2,		/* ����24x24���� ����x�ߣ� -- ��ʱδ֧�� */
//	FC_ST_32 = 3		/* ����32x32���� ����x�ߣ� -- ��ʱδ֧�� */	
//}FONT_CODE_E;

//void LCDInit(void);
//void Lcd_Address(uint16_t page,uint16_t column);
//void ClearScreen(void);
//void DisplayFont(uint8_t page, uint8_t column, uint8_t No, uint8_t typeface);
//void Scribing(void);
//void Display_Graphic_5x8(uint8_t page,uint8_t column,uint8_t *dp);
//void Display_Graphic_8x16(uint8_t page,uint8_t column,uint8_t *dp);
//void Display_Graphic_16x16(uint8_t page,uint8_t column,uint8_t *dp);
//void Display_128x64(uint8_t *dp);
//void send_dat(unsigned char  dat);
//void send_cmd(unsigned char  cmd);
//void LCDShiftWrite(unsigned char  dat) ;
//void Screen_DispALL(void);
//void OLED_PutPixel(uint16_t _usX, uint16_t _usY, uint8_t _ucColor);
//void OLED_BufToPanel(void);
//void OLED_ClrScr(uint8_t _ucMode);
//void OLED_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t *_ptr);
//void OLED_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint8_t _ucColor);
//void OLED_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint8_t _ucColor);
//void OLED_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint8_t _ucColor);
//void OLED_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint8_t _ucColor);
//uint8_t OLED_GetPixel(uint16_t _usX, uint16_t _usY);
//void OLED_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont);
//void Picture(uint8_t i);
//void OLED_DrawBMP1(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char i);
//void Display_Graphic_16x8(uint8_t page,uint8_t column,uint8_t *dp);		
//void Show_vcc(void);
	 
#endif  
	 
