/*
  Program:
  File Name:
  Author Name:
  Date:

  Board:RAAYAN MINI
   Version:4.0
   Controller:STM3F401RBT6
 
   Pins used:
     
 *
 */
#include "stm32f401rbt6.h"
#include "gps_extract.h"
#include "systemticktimer.h"
#include "km_lcd.h"
#include "uart2.h"
#include "keypad.h"
#include "switch.h"
#include <stdio.h>
#include "i2c.h"
#include "dma.h"

//extern char buffer[32];
unsigned char today_date[]={0x00,0x26,0x09,0x03,0x26,0x02,0x25}; //ARRAY OF ELEMENTS 
unsigned char cur_time_date[10];
#define BUFFER_SIZE 1000
extern double n1,n2;   
extern int ent_sw_flag;
extern int up_sw_flag;

extern char rx_buffer[BUFFER_SIZE];
extern char status;
char map[100];
extern char latitude[20],longitude[20];

int main()
{
  	systick_timer();
	  gpio_init();
  	KM_LCD_Init();
	
	  KM_LCD_Write_Str((unsigned char *)" SMART TRACKING   SYSTEM V1.0");
	  KM_Delay_ms(1000);
	  KM_LCD_Write_Cmd(0x01);                                            // Clear the display (0x01)
    
	  KM_LCD_Write_Cmd(0X80);
	  KM_LCD_Write_Str((unsigned char *)"ENTER SWTICH");
	  KM_LCD_Write_Cmd(0XC0);
	  KM_LCD_Write_Str((unsigned char *)"UP SWTICH");
	 
	  ent_switch();
	  ent_sys_conf();
		
	  up_switch();
	  up_sys_config();
	
	  
	  USART2_Init();
    USART2_Config();
	
	  keypad_init_config();
		
		KM_I2C1_Init();
	  KM_I2C1_Config();
	  KM_I2C_Mem_Write(0x68,0x00,0x01,today_date,7);
	
	  USART1_Init();
    USART1_Config();
    DMA2_Stream5_UART1_RX_Config();  // Start DMA reception
	
/*phase3: Device Operation:*/

while(1)
{
	    
	    //time_date();
	    if(up_sw_flag==1)
			{
				KM_LCD_Write_Cmd(0x01); 
				up_sw_flag=0;
				
				KM_LCD_Write_Str((unsigned char *)"ENTER MOBILE NUMBER:");
        KM_Delay_ms(1000);
				KM_LCD_Write_Cmd(0x01); 
				keypad_write();
			}
			
	    if(ent_sw_flag==1)
      {
				
				KM_LCD_Write_Cmd(0x01); 
				ent_sw_flag=0;
				
				 rmc_data_exract();
				 extract();
				   n1=ascii_to_float(latitude,10);
						n2=ascii_to_float(longitude,11);  //in place of sscanf [ascii to float]
						
						conversion_lat_log();
				
				
					/*if(status=='A')
					{
						
						KM_LCD_Write_Cmd(0X80);
						KM_LCD_Write_Str((unsigned char *)"HELP");
						KM_LCD_Write_Cmd(0XC9);
						KM_LCD_Write_Str((unsigned char *)" VAILD");
						
						n1=ascii_to_float(latitude,10);
						n2=ascii_to_float(longitude,11);  //in place of sscanf [ascii to float]
						conversion_lat_log();
						
						sprintf(map," https://www.google.com/maps?q=%.5lf,%.5lf",n1,n2);
						KM_Delay_ms(200);
						USART2_OutString(map);
						 //gsm_send();
						*/
				  }
					else
				  {
						KM_LCD_Write_Cmd(0XC9);
						KM_LCD_Write_Str((unsigned char *)"INVALID ");		
				  }
			}
   }






