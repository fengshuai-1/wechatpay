#ifndef __BSP_LCD_H
#define __BSP_LCD_H		
 
#include "bsp.h" 
//创建日期:2018/08/22
//版本：V1.0
/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接          3.3V         //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为4线制SPI总线
//     LCD模块                STM32单片机    
//       SDA         接          PB15         //液晶屏SPI总线数据写信号
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 					      STM32单片机 
//       BLK         接          PB9          //液晶屏背光控制信号，如果不需要控制，接3.3V
//       SCK         接          PB13         //液晶屏SPI总线时钟信号
//       DC          接          PB10         //液晶屏数据/命令控制信号
//       RES         接          PB12         //液晶屏复位控制信号
//       CS          接          PB11         //液晶屏片选控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机 
//      T_IRQ        接          PC10         //触摸屏触摸中断信号
//      T_DO         接          PC2          //触摸屏SPI总线读信号
//      T_DIN        接          PC3          //触摸屏SPI总线写信号
//      T_CS         接          PC13         //触摸屏片选控制信号
//      T_CLK        接          PC0          //触摸屏SPI总线时钟信号
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

//LCD重要参数集
typedef struct  
{										    
	uint16_t width;			//LCD 宽度
	uint16_t height;			//LCD 高度
	uint16_t id;				  //LCD ID
	uint8_t  dir;			  //横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint16_t	 wramcmd;		//开始写gram指令
	uint16_t  setxcmd;		//设置x坐标指令
	uint16_t  setycmd;		//设置y坐标指令	
}_lcd_dev; 	

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
/////////////////////////////////////用户配置区///////////////////////////////////	 
#define USE_HORIZONTAL  	 0 //定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转

//////////////////////////////////////////////////////////////////////////////////	  
//定义LCD的尺寸
#define LCD_W 240
#define LCD_H 320

//TFTLCD部分外要调用的函数		   
extern uint16_t  POINT_COLOR;//默认红色    
extern uint16_t  BACK_COLOR; //背景颜色.默认为白色

////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义---------------- 
#define GPIO_TYPE  GPIOA  //GPIO组类型
#define LED      9        //背光控制引脚        PB9
#define LCD_CS   4       //片选引脚            PB11
#define LCD_RS   6       //寄存器/数据选择引脚 PB10 
#define LCD_RST  0       //复位引脚            PB12


//QDtech全系列模块采用了三极管控制背光亮灭，用户也可以接PWM调节背光亮度
//#define	LCD_LED    PBout(LED) //LCD背光    		 PB9
//如果使用官方库函数定义下列底层，速度将会下降到14帧每秒，建议采用我司推荐方法
//以下IO定义直接操作寄存器，快速IO操作，刷屏速率可以达到28帧每秒！ 

////////GPIO置位（拉高）
//////#define	LCD_CS_SET    GPIO_TYPE->BSRR=1<<LCD_CS    //片选端口  	
//////#define	LCD_RS_SET	  GPIO_TYPE->BSRR=1<<LCD_RS    //数据/命令    
//////#define	LCD_RST_SET	  GPIOB->BSRR=1<<LCD_RST       //复位			 

////////GPIO复位（拉低）							    
//////#define	LCD_CS_CLR    GPIO_TYPE->BRR=1<<LCD_CS     //片选端口  	
//////#define	LCD_RS_CLR	  GPIO_TYPE->BRR=1<<LCD_RS     //数据/命令  	 
//////#define	LCD_RST_CLR	  GPIOB->BRR=1<<LCD_RST        //复位			  

//GPIO置位（拉高）
#define	LCD_CS_SET    gpio_bit_set(GPIOA,GPIO_PIN_4)
#define	LCD_RS_SET    gpio_bit_set(GPIOA,GPIO_PIN_6)
#define	LCD_RST_SET   gpio_bit_set(GPIOC,GPIO_PIN_4)

//GPIO复位（拉低）
#define	LCD_CS_CLR    gpio_bit_reset(GPIOA,GPIO_PIN_4);
#define	LCD_RS_CLR    gpio_bit_reset(GPIOA,GPIO_PIN_6);
#define	LCD_RST_CLR   gpio_bit_reset(GPIOC,GPIO_PIN_4);

//画笔颜色
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
#define BROWN 		0XBC40 //棕色
#define BRRED 		0XFC07 //棕红色
#define GRAY  		0X8430 //灰色
#define GRAY0       0xEF7D 
#define GRAY1       0x8410 //灰色1      00000 000000 00000
#define GRAY2       0x4208 
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	0X841F //浅绿色
#define LIGHTGRAY     0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 		0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE      	0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE          0X2B12 //浅棕蓝色(选择条目的反色)
	    															  
void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(uint16_t Color);	 
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_DrawPoint(uint16_t x,uint16_t y);//画点
uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y); //读点
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);		   
void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);

uint16_t LCD_RD_DATA(void);//读取LCD数据									    
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


//如果仍然觉得速度不够快，可以使用下面的宏定义,提高速度.
//注意要去掉lcd.c中void LCD_WR_DATA(uint16_t data)函数定义哦
/*
#if LCD_USE8BIT_MODEL==1//使用8位并行数据总线模式
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
	#else//使用16位并行数据总线模式
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
////-----------------LCD端口定义---------------- 
//#define	LCD_CS_SET    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_9)    //片选端口  	 PC9
//#define	LCD_DC_SET	  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_11)    //数据/命令    PC8	   
//#define	LCD_SDI_SET	  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_13)    //数据			   PC7
//#define	LCD_SCK_SET	  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_12)    //时钟    		 PC6
//								    
//#define	LCD_CS_CLR    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9)     //片选端口  PC9
//#define	LCD_DC_CLR	  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_11)     //数据/命令 PC8	   
//#define	LCD_SDI_CLR	  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_13)     //数据			 PC7
//#define	LCD_SCK_CLR	  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_12)     //时钟			 PC6	    																						  

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

///* 字体属性结构, 用于LCD_DispStr() */
//typedef struct
//{
//	uint16_t FontCode;	/* 字体代码 0 表示16点阵 */
//	uint16_t FrontColor;	/* 字体颜色 */
//	uint16_t BackColor;	/* 文字背景颜色，透明 */
//	uint16_t Space;		/* 文字间距，单位 = 像素 */
//}FONT_T;


///* 字体代码 */
//typedef enum
//{
//	FC_ST_16 = 0,		/* 宋体15x16点阵 （宽x高） */
//	FC_ST_12 = 1,		/* 宋体12x12点阵 （宽x高） */
//	FC_ST_24 = 2,		/* 宋体24x24点阵 （宽x高） -- 暂时未支持 */
//	FC_ST_32 = 3		/* 宋体32x32点阵 （宽x高） -- 暂时未支持 */	
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
	 
