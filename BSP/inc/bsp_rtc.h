#ifndef __BSP_RTC_H__
#define __BSP_RTC_H__

#include "bsp.h"

#define DIGIT_TO_CHAR_OFFSET    (48)
#define MIN_YEAR                (1951)
#define YEAR_SPAN               (100)
#define MAX_YEAR                (MIN_YEAR + YEAR_SPAN - 1)
#define IS_LEAP_YEAR(year)      ((year % 4 == 0) && ( year % 100 != 0) || (year % 400 == 0))

extern uint8_t Is1S;

void rtc_configuration(void);
uint32_t calc_days_num_by_date(uint32_t year, uint32_t month, uint32_t day);
uint32_t usart_scanf(uint32_t max_value);
void print_calendar(uint32_t rtc_counter);
void time_adjust(void);
uint32_t time_regulate(void); //时间校准函数
void rtc_process(void);

#endif
