#include "stm32f401rbt6.h"
#include "i2c.h"
#include <stdio.h>
#include "km_lcd.h"
#include "gps_extract.h"

char buffer[32];
extern unsigned char cur_time_date[10];

void time_date(void)
{
	
    KM_I2C_Mem_Read(0x68, 0x00, 0x01, cur_time_date, 7);

    // Set the cursor to the first line (0x80 is the command for the first line)
    KM_LCD_Write_Cmd(0x80);

    // Format and print the time in HH:MM:SS format
    sprintf(buffer, "%02d:%02d:%02d", 
            (cur_time_date[2] - 6 * (cur_time_date[2] >> 4)),  // Hours
            (cur_time_date[1] - 6 * (cur_time_date[1] >> 4)),  // Minutes
            (cur_time_date[0] - 6 * (cur_time_date[0] >> 4))); // Seconds

    // Write the time string to the LCD
    KM_LCD_Write_Str((unsigned char *)buffer);

    // Set the cursor to the second line (0xC0 is the command for the second line)
    KM_LCD_Write_Cmd(0xC0);

   
    sprintf(buffer, "%02d/%02d/%02d",                                  // Format and print the date in DD-MM-YYYY format
            (cur_time_date[4] - 6 * (cur_time_date[4] >> 4)),  // Day
            (cur_time_date[5] - 6 * (cur_time_date[5] >> 4)),  // Month
            (cur_time_date[6] - 6 * (cur_time_date[6] >> 4)));  // Year (assuming 21st century)

    
            KM_LCD_Write_Str((unsigned char *)buffer); // Write the date string to the LCD
}