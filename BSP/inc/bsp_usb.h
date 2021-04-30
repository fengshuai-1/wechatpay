#ifndef __BSP_USB_H
#define __BSP_USB_H		
 
#include "bsp.h" 
 
//extern usb_core_handle_struct usbfs_core_dev;
extern uint32_t usbfs_prescaler;

void bsp_usb_init(void);
void usb_clock_config(void);
void usb_interrupt_config(void);
void usb_Tx(uint8_t *buf , uint8_t tx_len);
uint32_t usb_Rx(void);
void usb_Tx_onedata(uint8_t buf );
void usb_Tx_worddata(uint32_t buf );
void usb_Tx_halfdata(uint16_t buf );

#endif 


