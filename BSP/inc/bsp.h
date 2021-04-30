#ifndef __BSP_H__
#define __BSP_H__

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdarg.h> 
#include <stdlib.h>

//#include "arm_math.h"

#include "gd32e10x.h"
#include "gd32e10x_eval.h"

#include "gd32e10x_rcu.h"
#include "gd32e10x_adc.h"
#include "gd32e10x_can.h"
#include "gd32e10x_crc.h"
#include "gd32e10x_ctc.h"
#include "gd32e10x_dac.h"
#include "gd32e10x_dbg.h"
#include "gd32e10x_dma.h"
#include "gd32e10x_exti.h"
#include "gd32e10x_fmc.h"
#include "gd32e10x_fwdgt.h"
#include "gd32e10x_gpio.h"
#include "gd32e10x_i2c.h"
#include "gd32e10x_pmu.h"
#include "gd32e10x_bkp.h"
#include "gd32e10x_rtc.h"
#include "gd32e10x_spi.h"
#include "gd32e10x_timer.h"
#include "gd32e10x_usart.h"
#include "gd32e10x_wwdgt.h"
#include "gd32e10x_misc.h"
#include "gd32e10x_exmc.h"

////////#include "define_all.h"   //芯片全局变量定义
//////////#include "user_init.h"    //芯片初始化
//////////#include "mf_config.h"

////////////#include "user_define.h"  //用户宏定义_自行添加文件
////////////#include "Lcdzhen.h"      //LCD每个段码宏定义

#include "bsp_usb.h"
#include "bsp_gpio.h"
////////////#include "bsp_flash.h"
////////////#include "bsp_sleep.h"
#include "bsp_timer.h"
#include "bsp_uart.h"
////////////#include "bsp_lpuart.h"
#include "bsp_lcd.h"
#include "bsp_adc.h"
#include "bsp_rtc.h"
//////////////#include "bsp_trng.h"
#include "bsp_debug.h"
#include "bsp_user_lib.h"
////////////#include "bsp_beep.h"
#include "bsp_protocol.h"
#include "bsp_lock.h"
#include "bsp_n58.h"
#include "bsp_spi.h"
////////////#include "bsp_w25qxx.h"

//#include "pic.h"
#include "ziku.h"
#include "Lcd_GUI.h"


#include "cdc_acm_core.h"
#include "usbd_int.h"
#include "QR_Encode.h"
#include "md5.h"
#include "N58.h"
#include "key.h"

#include "menu.h"
#include "bsp_gps.h" 


/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */


#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#define u8      uint8_t 
#define u16     uint16_t 
#define u32     uint32_t 

extern double c_uAh;

void bsp_Init(void);
void PrintfLogo(void);
void bsp_RunPer10ms(void);
void bsp_RunPer1ms(void);
void bsp_Idle(void);

#endif
