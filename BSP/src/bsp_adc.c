#include "bsp.h"

#define     BOARD_ADC_CHANNEL ADC_CHANNEL_1
#define     ADC_GPIO_PORT GPIOA
#define     ADC_GPIO_PIN  GPIO_PIN_1

volatile  uint16_t  adc_value[20][2];
uint16_t VCC_BAT = 0;

/*!
    \brief      configure the DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(void)
{
    /* enable DMA0 clock */
    rcu_periph_clock_enable(RCU_DMA0); 
    /* ADC_DMA_channel configuration */
    dma_parameter_struct dma_data_parameter;
    
    /* ADC DMA_channel configuration */
    dma_deinit(DMA0, DMA_CH0);
    
    /* initialize DMA single data mode */
    dma_data_parameter.periph_addr = (uint32_t)(&ADC_RDATA(ADC0));
    dma_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_data_parameter.memory_addr = (uint32_t)(&adc_value);
    dma_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;  
    dma_data_parameter.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_data_parameter.number = 40;
    dma_data_parameter.priority = DMA_PRIORITY_HIGH;
    dma_init(DMA0, DMA_CH0, &dma_data_parameter);

    dma_circulation_enable(DMA0, DMA_CH0);
  
    /* enable DMA channel */
    dma_channel_enable(DMA0, DMA_CH0);


}
/*!
    \brief      configure the ADC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_config(void)
{
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC0);
 
    /* config ADC clock */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV4);
    /* reset ADC */
    adc_deinit(ADC0);
    /* ADC mode config */
    adc_mode_config(ADC_MODE_FREE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);
    /* ADC channel length config */
//    adc_channel_length_config(ADC0,ADC_REGULAR_CHANNEL,1);
    adc_channel_length_config(ADC0,ADC_REGULAR_CHANNEL,2);    
    adc_resolution_config(ADC0,ADC_RESOLUTION_12B);
    /* ADC scan mode disable */
    adc_special_function_config(ADC0,ADC_SCAN_MODE,ENABLE);
    /* ADC contineous function disable */
    adc_special_function_config(ADC0,ADC_CONTINUOUS_MODE, ENABLE);
    /* ADC trigger config */
    adc_external_trigger_source_config(ADC0,ADC_REGULAR_CHANNEL,ADC0_1_EXTTRIG_REGULAR_NONE);
    adc_external_trigger_config(ADC0,ADC_REGULAR_CHANNEL,ENABLE);
    /* ADC discontinuous config */
//    adc_discontinuous_mode_config(ADC1,ADC_REGULAR_CHANNEL,1);
    /* ADC regular channel config */
    adc_regular_channel_config(ADC0,0,ADC_CHANNEL_1,ADC_SAMPLETIME_71POINT5);
    adc_regular_channel_config(ADC0,1,ADC_CHANNEL_9,ADC_SAMPLETIME_71POINT5);
    /* enable ADC interface */
    adc_enable(ADC0);

    bsp_DelayMS(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable (ADC0);

    adc_dma_mode_enable(ADC0);
    adc_software_trigger_enable(ADC0,ADC_REGULAR_CHANNEL);
}


uint16_t  GET_ADC(void)
{
   adc_software_trigger_enable(ADC1,ADC_REGULAR_CHANNEL);
         while( !adc_flag_get(ADC1,ADC_FLAG_EOC));

         return (adc_regular_data_read(ADC1) *3300)/4096;
}

uint16_t Get_BAT(void)  //获取锂电池电压
{
    uint32_t X1;

    gpio_bit_set(GPIOB,GPIO_PIN_10);
    bsp_DelayMS(20);  //等待数据稳定

		/* 从排序过的数组里中间抽取连续的5组数据，进行取平均值,获得较高的精度 */
		X1 = 0;
		for(uint16_t count = 7; count < 16; count=count+2)
			{
			X1 = X1 + adc_value[count][1];
		}	

		X1 = X1 / 5;	

    gpio_bit_reset(GPIOB,GPIO_PIN_10);    

    return (X1 *3300)/2048;
}
	
void show_bat(void)//显示电池电压   高电压绿色  中等蓝色 低电压红色
{
	uint8_t v_buf = 0;
	
	VCC_BAT = Get_BAT();
	if(VCC_BAT > 4149)
		v_buf = 13;
	else if(VCC_BAT < 3551)
		v_buf = 0;
	else
		v_buf = (VCC_BAT -3550) / 50;
	
	if(v_buf > 8)
		LCD_ShowChar(227,4,GREEN, WHITE,33 + 94 + v_buf,16,0);
	else if(v_buf < 4)
		LCD_ShowChar(227,4,RED, WHITE,33 + 94 + v_buf,16,0);
	else
		LCD_ShowChar(227,4,BLUE, WHITE,33 + 94 + v_buf,16,0);
}


uint16_t Get_LOAD_VCC(void) //计算高边负载电压
{
    uint32_t X1;

	/* 从排序过的数组里中间抽取连续的5组数据，进行取平均值,获得较高的精度 */
	X1 = 0;
	for(uint16_t count = 7; count < 15; count++)
    {
		X1 = X1 + adc_value[count][0];
	}	

	X1 = X1 / 8;	 
  
    if(X1 < 80)  //小于1V电压 默认0v
    {
        X1 = 0;
    }
    return (X1 *47289)/4096;
}

