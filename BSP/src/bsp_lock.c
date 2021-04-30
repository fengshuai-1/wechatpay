#include "bsp.h"

//�������������

void lock_init(void)
{    /* enable the key clock */
    rcu_periph_clock_enable(RCU_AF);

    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_0); //�̵������ƽ�
    gpio_bit_reset(GPIOB,GPIO_PIN_0); 
    
    gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ,GPIO_PIN_5); //���ż��  �͵�ƽ�����Źر�  �ߵ�ƽ�����Ŵ�
    //�����ش��������Ŵ���
    //�½��ش��������Źر���
    nvic_irq_enable(EXTI5_9_IRQn, 2U, 0U);

    /* connect key EXTI line to key GPIO pin */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOC, GPIO_PIN_SOURCE_5);

    /* configure key EXTI line */
    exti_init(EXTI_5, EXTI_INTERRUPT, EXTI_TRIG_BOTH);//���ش���
    exti_interrupt_flag_clear(EXTI_5);

    if(gpio_input_bit_get(GPIOC,GPIO_PIN_5) == SET)
    {
        //�����Ѿ���
        //Show_Str(60,260,BLUE,CYAN, "���Ŵ�",16,0); 
    }
    else
    {
        //�����Ѿ��ر�
        //Show_Str(60,260,BLUE,CYAN, "���Źر�",16,0); 
    } 


}

void Open(void)
{
    gpio_bit_set(GPIOB,GPIO_PIN_0); 
    bsp_DelayMS(50);
    gpio_bit_reset(GPIOB,GPIO_PIN_0); 
}
