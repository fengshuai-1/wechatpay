#ifndef __KEY_H
#define __KEY_H
#include <string.h>

#define KEY_row0_Pin GPIO_PIN_9		//B5
#define KEY_row1_Pin GPIO_PIN_8	//B4
#define KEY_row2_Pin GPIO_PIN_15  //B3
#define KEY_row3_Pin GPIO_PIN_14  //A15

#define KEY_CLO0_OUT_LOW   gpio_bit_reset(GPIOB,GPIO_PIN_13)  
#define KEY_CLO1_OUT_LOW   gpio_bit_reset(GPIOB,GPIO_PIN_12)
#define KEY_CLO2_OUT_LOW   gpio_bit_reset(GPIOB,GPIO_PIN_11)
#define KEY_CLO3_OUT_LOW   gpio_bit_reset(GPIOB,GPIO_PIN_10)
#define KEY_CLO4_OUT_LOW   gpio_bit_reset(GPIOB,GPIO_PIN_1)

#define KEY_CLO0_OUT_HIGH  gpio_bit_set(GPIOB,GPIO_PIN_13) 
#define KEY_CLO1_OUT_HIGH  gpio_bit_set(GPIOB,GPIO_PIN_12)
#define KEY_CLO2_OUT_HIGH  gpio_bit_set(GPIOB,GPIO_PIN_11)
#define KEY_CLO3_OUT_HIGH  gpio_bit_set(GPIOB,GPIO_PIN_10)
#define KEY_CLO4_OUT_HIGH  gpio_bit_set(GPIOB,GPIO_PIN_1)

#define KEY_UP     16
#define KEY_DOWN   12
#define KEY_ENTWE  10




int key_init( void );
int key_scan( void );
char KEY_SCAN(void);
char KEY_ROW_SCAN(void);
void HW_KEY_FUNCTION(void);

#endif

