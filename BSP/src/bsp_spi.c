#include "bsp.h"

/*****************************************************************************
 * @name       :u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte)
 * @date       :2018-08-09 
 * @function   :Write a byte of data using STM32's hardware SPI
 * @parameters :SPIx: SPI type,x for 1,2,3
                Byte:Data to be written
 * @retvalue   :Data received by the bus
******************************************************************************/
uint8_t SPI0_WriteByte(uint8_t Byte)
{
	while((SPI_STAT(SPI0) & SPI_FLAG_TBE)==RESET);		//等待发送区空	  
            SPI_DATA(SPI0)=Byte;	 														//发送一个byte   
	while((SPI_STAT(SPI0) & SPI_FLAG_RBNE)==RESET);		//等待接收完一个byte  

	return  SPI_DATA(SPI0);          	     			//返回收到的数据			
} 

void spi_gpio_config(void)
{
  /* SPI0 GPIO config:SCK/PA5, MOSI/PA7 */
  gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_7);
//    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

}

void spi_config(void)
{
	rcu_periph_clock_enable(RCU_SPI0);//使能SPI0时钟
	
	spi_gpio_config();
	
    spi_parameter_struct spi_init_struct;
    /* deinitilize SPI and the parameters */
    spi_i2s_deinit(SPI0);
    spi_struct_para_init(&spi_init_struct);

    /* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_8;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_init_struct);

    /* SPI0 parameter config */
    spi_init_struct.device_mode = SPI_MASTER;
    spi_init_struct.nss         = SPI_NSS_SOFT;
    spi_init(SPI0, &spi_init_struct);
		
		/* SPI enable */
    spi_enable(SPI0);
}

