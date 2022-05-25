
#ifndef __RTC3231_H_
#define __RTC3231_H_
/*        Defining Register Addresses for RTC_3231     */

#define SLAVE_ADDRESS                   0x68
#define SLAVE_ADDRESS_WRITE             (SLAVE_ADDRESS << 1)
#define SLAVE_ADDRESS_READ              (SLAVE_ADDRESS_WRITE + 1)
#define RTC3231_CONTROL_REGISTER        0x0E
#define STATUS_REGISTER                 0x0F
#define RTC_REG_SECOND                  0x00
#define RTC_REG_MINUTE                  0x01
#define RTC_REG_HOUR                    0x02
#define RTC_REG_DAY                     0x03
#define RTC_REG_DATE                    0x04
#define RTC_REG_MONTH                   0x05
#define RTC_REG_YEAR                    0x06
#define _24_hour_format                 0
#define _12_hour_format                 1
#define am                              0
#define pm                              1

/*     Defining structure to set variables for Reading Time       */

typedef struct rtc_time_info
{
  uint8_t sec;
  uint8_t mins;
  uint8_t hours;
  uint8_t am_pm;
  uint8_t hour_twelve_twentyfour;
}rtc_time_info_t;

typedef struct rtc_date_info
{
  uint8_t day;
  uint8_t date;
  uint8_t month;
  uint8_t year;
} rtc_date_info_t;

/*      Functions Definitions        */
void rtc_init(void);
uint16_t    bcd_to_dec(uint8_t value);                            /* Return value from RTC (conversion from BCD to DECIMAL) value */
uint16_t    dec_to_bcd(uint8_t value);                            /* RTC accepts value in bcd (coversion of DECIMAL to BCD) */
void        write_to_rtc(uint8_t which_address, uint8_t value);  /* Accepts the adress where to write and the value to write there */
uint8_t     read_from_rtc(uint8_t Addr);                         /* Accepts Adress from where to Read */
void        set_time(rtc_time_info_t rtc_time);      
void        set_date(rtc_date_info_t rtc_date);
rtc_time_info_t  get_time(void);
rtc_date_info_t  get_date(void);
#endif

