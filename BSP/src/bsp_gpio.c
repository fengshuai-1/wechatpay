#include "bsp.h"

void EXTI0_IRQHandler(void)
{

}      
            
void EXTI1_IRQHandler(void) 
{

}    
             
void EXTI2_IRQHandler(void) 
{

}     
           
void EXTI3_IRQHandler(void)
{

}    
              
void EXTI4_IRQHandler(void) 
{

} 

/*!
    \brief      this function handles external lines 10 to 15 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI5_9_IRQHandler(void)
{
    if(RESET != exti_interrupt_flag_get(EXTI_5))  
    {
        if(gpio_input_bit_get(GPIOC,GPIO_PIN_5) == SET)
        {
            //�����Ѿ���
  //          Show_Str(25,260,BLUE,CYAN, "�����Ѿ���",16,0); 
        }
        else
        {
            //�����Ѿ��ر�
  //          Show_Str(25,260,BLUE,CYAN, "�����Ѿ��ر�",16,0); 
        }
        exti_interrupt_flag_clear(EXTI_5);
    }
} 

void EXTI10_15_IRQHandler(void)
{
    if(RESET != exti_interrupt_flag_get(EXTI_14))
    {
        exti_interrupt_flag_clear(EXTI_14);

        /* configure the TIMER4 */
    }
}

void bsp_GPIO(void)
{    
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
	  rcu_periph_clock_enable(RCU_AF);

    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_8); //4Gģ�鿪��
	  gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_13); //LED-->>pc13��ʼ��
	
		gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_2); //PWMA-->>PA2��ʼ��
		gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_3); //AIN1-->>PC3��ʼ��
		gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_1); //AIN2-->>PA1��ʼ��	
		gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_2); //STBY-->>PC2��ʼ��

	
	  gpio_bit_reset(GPIOC,GPIO_PIN_3);    //AIN1�͵�ƽ
		gpio_bit_set(GPIOA,GPIO_PIN_1);     //AIN2�ߵ�ƽ
		gpio_bit_set(GPIOC,GPIO_PIN_2);     //STBY�ߵ�ƽ



    gpio_bit_reset(GPIOB,GPIO_PIN_8);    //����
    bsp_DelayMS(1500);
		gpio_bit_set(GPIOB,GPIO_PIN_8);     //����

}

