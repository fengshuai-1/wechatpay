#include "bsp.h"

//#define ARRAYNUM(arr_nanme)      (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))
//#define TRANSMIT_SIZE            (ARRAYNUM(txbuffer) - 1)

//uint8_t txbuffer[] = "AT\r\n";
//uint8_t rxbuffer[800];
//uint8_t tx_size = 4;
//uint8_t rx_size = 32;
//__IO uint8_t txcount = 0; 
//__IO uint16_t rxcount = 0; 

void uart0_init(void)
{    
    /* USART interrupt configuration */
    nvic_irq_enable(USART0_IRQn, 0, 0);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);
    rcu_periph_clock_enable(RCU_AF);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
    gpio_pin_remap_config(GPIO_USART0_REMAP,ENABLE);   //重映射到PB6、PB7

    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_parity_config(USART0, USART_PM_NONE);  
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);

    /* enable USART TBE interrupt */  
    usart_interrupt_enable(USART0, USART_INT_TBE);

    /* enable USART RBNE interrupt */
    usart_interrupt_enable(USART0, USART_INT_RBNE);

    /* wait until USART send the transmitter_buffer */
//    while(txcount < tx_size);
//    
//    while(RESET == usart_flag_get(USART0, USART_FLAG_TC));	
}

/*!
    \brief      this function handles USART RBNE interrupt request and TBE interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART0_IRQHandler(void)
{
		static uint8_t data;
    if((RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)) && 
       (RESET != usart_flag_get(USART0, USART_FLAG_RBNE)))
    {
        /* receive data 串口接收数据*/ 
				data = usart_data_receive(USART0);
			  Debug_SendOneData(data);
			  rxbuffer[Rxcouter++] = data;
			
			
        if(Rxcouter > 1999)
        {
					  Rxcouter = 0;
//            usart_interrupt_disable(USART0, USART_INT_RBNE);
        }
    }
    if((RESET != usart_flag_get(USART0, USART_FLAG_TBE)) && 
       (RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_TBE))){
           
				 /* transmit data   串口发送数据*/
        usart_data_transmit(USART0, txbuffer[txcount++]);

				 if(txcount > 1999)  txcount = 0;     
				 
         if(txcount == tx_size)
					{
						
						txcount = 0;
            usart_interrupt_disable(USART0, USART_INT_TBE);
        }
				
    }
}

