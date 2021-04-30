#include "bsp.h"

//电磁锁驱动函数

void lock_init(void)
{    /* enable the key clock */
    rcu_periph_clock_enable(RCU_AF);

    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_0); //继电器控制脚
    gpio_bit_reset(GPIOB,GPIO_PIN_0); 
    
    gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ,GPIO_PIN_5); //关门检测  低电平：柜门关闭  高电平：柜门打开
    //上升沿触发：柜门打开了
    //下降沿触发：柜门关闭了
    nvic_irq_enable(EXTI5_9_IRQn, 2U, 0U);

    /* connect key EXTI line to key GPIO pin */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOC, GPIO_PIN_SOURCE_5);

    /* configure key EXTI line */
    exti_init(EXTI_5, EXTI_INTERRUPT, EXTI_TRIG_BOTH);//边沿触发
    exti_interrupt_flag_clear(EXTI_5);

    if(gpio_input_bit_get(GPIOC,GPIO_PIN_5) == SET)
    {
        //柜门已经打开
        //Show_Str(60,260,BLUE,CYAN, "柜门打开",16,0); 
    }
    else
    {
        //柜门已经关闭
        //Show_Str(60,260,BLUE,CYAN, "柜门关闭",16,0); 
    } 


}

void Open(void)
{
    gpio_bit_set(GPIOB,GPIO_PIN_0); 
    bsp_DelayMS(50);
    gpio_bit_reset(GPIOB,GPIO_PIN_0); 
}
