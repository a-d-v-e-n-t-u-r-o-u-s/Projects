#include "rtc.h"

static bool is_initialized;

int8_t RTC_configure(void)
{
    if(TRUE != is_initialized)
    {
        is_initialized = TRUE;
        return 0;
    }
    else
    {
        return -1;
    }
}

int8_t RTC_set_time(const RTC_time_t *const time)
{
    if(TRUE == is_initialized)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int8_t RTC_get_time(RTC_time_t *time)
{
    if(TRUE == is_initialized)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int8_t RTC_set_date(const RTC_date_t *const date)
{
    if(TRUE == is_initialized)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int8_t RTC_get_date(RTC_date_t *date)
{
    if(TRUE == is_initialized)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int8_t RTC_get_weekday(RTC_weekday_t *weekday)
{
    if(TRUE == is_initialized)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int8_t RTC_begin_calibration(void)
{
    if(TRUE == is_initialized)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int8_t RTC_slow_down_clock(void)
{
    if(TRUE == is_initialized)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int8_t RTC_speed_up_clock(void)
{
    if(TRUE == is_initialized)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int8_t RTC_save_calibration(void)
{
    if(TRUE == is_initialized)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int8_t RTC_end_calibration(void)
{
    if(TRUE == is_initialized)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
