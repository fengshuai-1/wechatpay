#include "bsp.h"

usb_core_handle_struct usbfs_core_dev =
{
    .dev = 
    {
        .dev_desc = (uint8_t *)&device_descriptor,
        .config_desc = (uint8_t *)&configuration_descriptor,
        .strings = usbd_strings,
        .class_init = cdc_acm_init,
        .class_deinit = cdc_acm_deinit,
        .class_req_handler = cdc_acm_req_handler,
        .class_data_handler = cdc_acm_data_handler
    },

    .udelay = bsp_DelayUS,
    .mdelay = bsp_DelayMS
};


void usb_clock_config(void);
void usb_interrupt_config(void);

uint8_t timer_prescaler = 0;
uint32_t usbfs_prescaler = 0;

/*!
    \brief      main routine will construct a USB mass storage device
    \param[in]  none
    \param[out] none
    \retval     none
*/
void bsp_usb_init(void)
{
    /* configure USB clock */
    usb_clock_config();

    /* USB device stack configure */
    usbd_init(&usbfs_core_dev, USB_FS_CORE_ID);

    /* USB interrupt configure */
    usb_interrupt_config();
}

/*!
    \brief      configure USB clock
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usb_clock_config(void)
{
    uint32_t system_clock = rcu_clock_freq_get(CK_SYS);
  
    if (system_clock == 48000000) {
        usbfs_prescaler = RCU_CKUSB_CKPLL_DIV1;
        timer_prescaler = 3;
    } else if (system_clock == 72000000) {
        usbfs_prescaler = RCU_CKUSB_CKPLL_DIV1_5;
        timer_prescaler = 5;
    } else if (system_clock == 120000000) {
        usbfs_prescaler = RCU_CKUSB_CKPLL_DIV2_5;
        timer_prescaler = 9;
    } else {
        /*  reserved  */
    }

    rcu_usb_clock_config(usbfs_prescaler);
    rcu_periph_clock_enable(RCU_USBFS);
}

/*!
    \brief      configure USB interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usb_interrupt_config(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    nvic_irq_enable((uint8_t)USBFS_IRQn, 2U, 0U);

    /* enable the power module clock */
    rcu_periph_clock_enable(RCU_PMU);

    /* USB wakeup EXTI line configuration */
    exti_interrupt_flag_clear(EXTI_18);
    exti_init(EXTI_18, EXTI_INTERRUPT, EXTI_TRIG_RISING);
    exti_interrupt_enable(EXTI_18);

    nvic_irq_enable((uint8_t)USBFS_WKUP_IRQn, 1U, 0U);
}

void usb_Tx(uint8_t *buf , uint8_t tx_len)
{
    if (USB_STATUS_CONFIGURED == usbfs_core_dev.dev.status) 
    {
          if (packet_sent == 1) 
          {
            memcpy(usb_data_buffer , buf ,tx_len);

            packet_sent = 0;
            /* send receive datas */
            cdc_acm_data_send(&usbfs_core_dev, tx_len);
          }
    }
}

void usb_Tx_onedata(uint8_t buf )
{
    if (USB_STATUS_CONFIGURED == usbfs_core_dev.dev.status) 
    {
          if (packet_sent == 1) 
          {
            usb_data_buffer[0] = buf ;

            packet_sent = 0;
            /* send receive datas */
            cdc_acm_data_send(&usbfs_core_dev, 1);
          }
    }
}

void usb_Tx_halfdata(uint16_t buf )
{
    if (USB_STATUS_CONFIGURED == usbfs_core_dev.dev.status) 
    {
//          if (packet_sent == 1) 
//          {
            usb_data_buffer[0] = buf / 256;
						usb_data_buffer[1] = buf % 256;
						
//            packet_sent = 0;
            /* send receive datas */
            cdc_acm_data_send(&usbfs_core_dev, 2);
//          }
    }
}

void usb_Tx_worddata(uint32_t buf )
{
    if (USB_STATUS_CONFIGURED == usbfs_core_dev.dev.status) 
    {
          if (packet_sent == 1) 
          {
            usb_data_buffer[0] = 0XFF;
            usb_data_buffer[1] = buf >>24 ;
            usb_data_buffer[2] = buf >>16 ;
            usb_data_buffer[3] = buf >>8 ;
            usb_data_buffer[4] = buf  ;
			usb_data_buffer[5] = 0XEE;			
            packet_sent = 0;

            /* send receive datas */
            cdc_acm_data_send(&usbfs_core_dev, 6);
          }
    }
}

uint32_t usb_Rx(void)
{
    if (USB_STATUS_CONFIGURED == usbfs_core_dev.dev.status) 
    {
        if (1 == packet_receive) //数据接收完成
        {
            /* receive datas from the host when the last packet datas have sent to the host */
            cdc_acm_data_receive(&usbfs_core_dev);
            packet_receive = 0;
            printf("usb receive data len %d  \r\n",receive_length);
            printf("usb receive data:");
            for(uint16_t i= 0;i<receive_length;i++)
            {
                printf("%d ",usb_data_Rxbuffer[i]);
            }
            printf("\r\n");
            return receive_length;
        }
    }
    return 0;
}
