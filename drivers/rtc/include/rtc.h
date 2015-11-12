#ifndef RTC_H
#define RTC_H

#include "stm8l15x.h"

typedef struct
{
    uint8_t hour;
    uint8_t minute;
} RTC_time_t;

typedef struct
{
    uint8_t day;
    uint8_t month;
    uint16_t year;
} RTC_date_t;

typedef enum
{
    RTC_MONDAY,
    RTC_TUESDAY,
    RTC_WEDNESDAY,
    RTC_THURSDAY,
    RTC_FRIDAY,
    RTC_SATURDAY,
    RTC_SUNDAY
} RTC_weekday_t;

int8_t RTC_configure(void);
int8_t RTC_set_time(const RTC_time_t *time);
int8_t RTC_get_time(RTC_time_t *time);
int8_t RTC_set_date(const RTC_date_t *date);
int8_t RTC_get_date(RTC_date_t *date);
int8_t RTC_begin_calibration(void);
int8_t RTC_slow_down_clock(void);
int8_t RTC_speed_up_clock(void);
int8_t RTC_save_calibration(void);
int8_t RTC_end_calibration(void);

#endif // PRESSURE_H
