#include <stdbool.h>
#include <stdint.h>
#include "hw_ints.h"
#include "hw_memmap.h"
#include "gpio.h"
#include "interrupt.h"
#include "ioc.h"
#include "hw_ioc.h"
#include "sys_ctrl.h"
#include "hw_i2cm.h"
#include "hw_i2cs.h"
#include "i2c.h"
#include "uartstdio.h"
#include "rtc3231.h"
#include "i2c_init.h"
#include "uart_init.h"


void rtc_init()
{
  write_to_rtc(0x0E, 0x00);  //control reg 
  write_to_rtc(0x0F, 0x08); 
}

/*                       Conversion Functions                                 */
uint16_t  bcd_to_dec(uint8_t value)
{
  return (value >> 4) * 10 + (value & 0x0F);
}

uint16_t dec_to_bcd(uint8_t value)
{
  return ((value / 10) << 4) + (value % 10);
}

/*                       Write Function to i2c                                */

void write_to_rtc(uint8_t which_addr, uint8_t val)
{
  i2c_slave_set(SLAVE_ADDRESS, false); 
  i2c_send_data(which_addr);
  i2c_data_bytes(I2C_MASTER_CMD_BURST_SEND_START);
  while(I2CMasterBusy()){}
    
  i2c_send_data (val);
  i2c_data_bytes(I2C_MASTER_CMD_BURST_SEND_FINISH);
  while(I2CMasterBusy()){}
}

/*                       Read Values from i2c                                 */

uint8_t read_from_rtc(uint8_t Addr) 
{   
   unsigned char val = 0 ;
   i2c_slave_set(SLAVE_ADDRESS, false); 
   i2c_send_data(Addr);
   i2c_data_bytes(I2C_MASTER_CMD_SINGLE_SEND);
   while(I2CMasterBusy()) {}
         
         
   i2c_slave_set (SLAVE_ADDRESS, true);
   i2c_data_bytes(I2C_MASTER_CMD_SINGLE_RECEIVE);
   while(I2CMasterBusy()){}
   val = i2c_receive_data ();
   return val;
}

/*                        Setting time of RTC                                 */

void set_time(rtc_time_info_t rtc_time)
{
   uint8_t temp = 0;
   write_to_rtc(RTC_REG_SECOND, dec_to_bcd(rtc_time.sec));
   write_to_rtc(RTC_REG_MINUTE, dec_to_bcd(rtc_time.mins));
   switch(rtc_time.hour_twelve_twentyfour) 
    { 
      case 1: 
       {        
         switch(rtc_time.am_pm) 
          { 
            case 1: 
             {            
               temp = 0x60; 
               break; 
             } 
             default: 
              {    
                temp = 0x40; 
                break; 
              } 
           }
         
          write_to_rtc(RTC_REG_HOUR, ((temp | (dec_to_bcd(rtc_time.hours)))));                    
          break; 
        }      
       default: 
        { 
          write_to_rtc(RTC_REG_HOUR,(dec_to_bcd(rtc_time.hours))); 
          break; 
        }  
    }   
}

/*                        Setting Date of RTC                                 */

void set_date(rtc_date_info_t rtc_date)
{
   write_to_rtc(RTC_REG_DAY,  (dec_to_bcd(rtc_date.day)));
   write_to_rtc(RTC_REG_DATE, (dec_to_bcd(rtc_date.date)));
   write_to_rtc(RTC_REG_MONTH,(dec_to_bcd(rtc_date.month)));
   write_to_rtc(RTC_REG_YEAR, (dec_to_bcd(rtc_date.year)));
}

/*                        Get Time from RTC                                   */

rtc_time_info_t get_time()
{
   rtc_time_info_t rtc_info;
   rtc_info.mins = bcd_to_dec(read_from_rtc(RTC_REG_MINUTE));
   rtc_info.sec = bcd_to_dec(read_from_rtc(RTC_REG_SECOND));
   
   switch(rtc_info.hour_twelve_twentyfour)
    {
      case 1:
       {
         rtc_info.hours = bcd_to_dec(0x1F & (read_from_rtc(RTC_REG_HOUR)));
         break;
       }
      default:
       {
         rtc_info.hours = bcd_to_dec(0x3F & (read_from_rtc(RTC_REG_HOUR)));
         break;
       }
    }
   //UARTprintf(" %u : %u : %u \n",rtc_info.hours,rtc_info.mins,rtc_info.sec);
   return rtc_info;
}

/*                         Get Date                                           */

rtc_date_info_t get_date()
{
   rtc_date_info_t rtc_info;
   rtc_info.day=bcd_to_dec(0x07&(read_from_rtc(RTC_REG_DAY)));
   rtc_info.date=bcd_to_dec(0x3F&(read_from_rtc(RTC_REG_DATE)));
   rtc_info.month=bcd_to_dec(0x1F&(read_from_rtc(RTC_REG_MONTH)));
   rtc_info.year=bcd_to_dec(read_from_rtc(RTC_REG_YEAR));
   //UARTprintf(" %u / %u / %u / %u \n",rtc_info.day,rtc_info.date,rtc_info.month,rtc_info.year);
   return rtc_info;
}