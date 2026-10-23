#include "stm32f401rbt6.h"
#include "systemticktimer.h"
#include "gps_extract.h"
#include "uart2.h"
#include "km_lcd.h"
#include <stdio.h>
#include <stdlib.h>
#include "i2c.h"
#include "dma.h"
#include <string.h>


// gps_extract.h
extern char map[100];

// rtc_eeprom.h
extern unsigned char cur_time_date[10];


#define BUFFER_SIZE 1000
char rx_buffer[BUFFER_SIZE];

char rmc_buff[1000];
char time[20];
char latitude[20],longitude[20];
char date[20];
char status;
unsigned char str[16];
double n1,n2;
float y1,y2;
extern int timeout;
int i;
char lat_buff[10];
char log_buff[10];
extern char map[100];

//char RAW_buffer[]="GVTG,,T,,M,0.132,N,0.244,K,A*3F<CR><LF>$GNGGA,071141.00,1728.39685,N,07823.16395,E,1,05,7.51,574.7,M,-74.0,M,,*62<CR><LF>$GNGSA,A,3,09,07,27,04,,,,,,,,,8.96,7.51,4.89*12<CR><LF>$GLGSV,1,1,02,67,54,327,41,77,,,31*56<CR><LF>$GNGLL,1728.39685,N,07823.16395,E,071141.00,A,A*7E<CR><LF>$GNRMC, 064400. 00,A,1728.42448,N,07823.11452,£,0.583, ,130325,, ,A*6F<CR><LF>";
void rmc_data_exract()
{
	int i=mystr_str(rx_buffer,"$GNRMC");   //live
	int j=0;
	while(rx_buffer[i]!='*')        //live
	{
		rmc_buff[j]=rx_buffer[i];    //live
		i++;
		j++;
	}
	rmc_buff[i]='\0';
		
}



void extract(void)
{
	int i=0,j=0;
	
		while(rmc_buff[i]!='\0'&&rmc_buff[i]!=',')
		{
			i++;
		}
		i++;
			while(rmc_buff[i]!='\0'&&rmc_buff[i]!=',')
			{
				time[j]=rmc_buff[i];
				i++;
				j++;	
			}
			i++;
			time[j]='\0';
			j=0;

			while(rmc_buff[i]!='\0'&&rmc_buff[i]!=',')
			{
				status=rmc_buff[i];
				i++;
			}
			i++;
			j=0;
			while(rmc_buff[i]!='\0'&&rmc_buff[i]!=',')
			{
				latitude[j]=rmc_buff[i];
				i++;
				j++;
			}
			i+=3;
			latitude[j]='\0';

			j=0;
			while(rmc_buff[i]!='\0'&&rmc_buff[i]!=',')
			{
			longitude[j]=rmc_buff[i];
			i++;
			j++;
			}
			longitude[j]='\0';
}	

      
int a_to_i(char str[])
{
	int i=0,num;
	while(str[i]!='\0')
	{
		num = num*10 +(str[i]-'0');
		i++;
	}
	return num;
}

// Function to convert ASCII string to float
float ascii_to_float(char str[], int length) 
{
    float result=0.0;                                    // Initialize the result to 0
    float fraction=0.1;                                  // Used for fractional part calculation
    int decimal_found=0;                                 // Flag to check if decimal point is found
    int i;

    // Loop through each character in the string
    for (i=0;i<length;i++) 
	  {
        if (str[i]=='.') 
				{
            decimal_found=1;                               // Set flag when decimal point is found
            continue;                                        // Skip to the next character
        }
        if(decimal_found==0) 
				{
          result=result*10.0+(str[i]-'0');          // Integer part: Multiply result by 10 and add the current digit
            
        } 
				else 
				{
            result+=(str[i]-'0')*fraction;          // Fractional part: Add the current digit multiplied by fraction
            fraction*=0.1;                             // Update fraction for the next digit
        }
    }
    return result;                                        // Return the final floating-point number
}

void conversion_lat_log(void)
{

	y1=n1-((int)n1/100)*100;
	y2=n2-((int)n2/100)*100;
	
	n1=((int)n1/100)+(y1/60);
	n2=((int)n2/100)+(y2/60);
	
  sprintf(latitude,"%lf",n1);
	sprintf(longitude,"%lf",n2);
	
	 KM_LCD_Write_Cmd(0X80);
	KM_LCD_Write_Str((unsigned char *)latitude);
  KM_LCD_Write_Cmd(0XC0);
	KM_LCD_Write_Str((unsigned char *) longitude);
//	KM_Delay_ms(2000);
//	 KM_LCD_Write_Cmd(0x01);

    KM_I2C_Mem_Write(0x50,0x00,0x01,(unsigned char*)latitude,10);
	  KM_I2C_Mem_Write(0x50,0x0A,0x01,(unsigned char*)longitude,10);
	
  	//KM_LCD_Write_Cmd(0X85);
    KM_I2C_Mem_Read(0x50, 0x00, 0x01,(unsigned char*)lat_buff,10);
	 // KM_LCD_Write_Str((unsigned char *)lat_buff);
		//KM_LCD_Write_Cmd(0XC0);
	  KM_I2C_Mem_Read(0x50, 0x0A, 0x01,(unsigned char*)log_buff,10);
			//KM_LCD_Write_Str((unsigned char *)log_buff);
}

// DMA2 Stream 5 Configuration for USART1 RX
void DMA2_Stream5_UART1_RX_Config() 
{
     RCC_AHB1ENR |= (1 << 22);            // Enable DMA2 clock
     DMA2_S5CR &= ~(1 << 0);             // Disable DMA Stream before configuration
    
     DMA2_S5PAR = (unsigned int)&USART1_DR;  // Set peripheral address (USART1 DR)	
     DMA2_S5M0AR = (unsigned int)rx_buffer;      // Set memory address
	
     DMA2_S5NDTR = BUFFER_SIZE;                  // Number of data to transfer

     DMA2_S5CR |=(4 << 25);                     //Channel 4 for USART1 RX
     DMA2_S5CR |=(1 << 10);                    // Memory increment mode
     DMA2_S5CR |=(1 << 8);                    // circular   
     DMA2_S5CR |=(1 << 0);                   // Enable DMA Stream 5
}

/*void gsm_send(void)
{   
	      
	      USART2_OutString("ATD+919010933040;\r\n");
	      KM_Delay_ms(100);
	      KM_LCD_Write_Cmd(0x89);
			  KM_LCD_Write_Str((unsigned char *)" help ");
				KM_LCD_Write_Cmd(0xC9);
	      KM_LCD_Write_Str((unsigned char *)" VALID ");
				//sprintf(map,"https://maps.google.com/?q=%.5lf,%.5lf",lat_buff,log_buff);
				//sprintf(map," https://www.google.com/maps?q=%.5lf,%.5lf",n1,n2);
					sprintf(map," https://www.google.com/maps?q=%.5lf,%.5lf",n1,n2);


				USART2_OutString("AT+CMGF=1\r\n");
	       KM_Delay_ms(100);
		    USART2_OutString(map);
			  KM_Delay_ms(100);
		    USART2_OutString(" help me ");
				KM_Delay_ms(100);
		    USART2_OutString(map);
		    USART2_OutChar(0x1A);
				KM_Delay_ms(10);
	      GPIOB_ODR |= 0x1<<12 ;//SET 12TH BIT IN GPIOB_ODR(for BUZZER OFF)
	      KM_Delay_ms(1000);
	      GPIOB_ODR &= ~(0x1<<12) ; //CLEAR 12TH BIT IN GPIOB_ODR(for BUZZER ON)
}*/

/*void gsm_send(void)
{
    // 1. SIM Card Check
    USART2_OutString("AT+CPIN?\r\n");     // SIM check
    KM_Delay_ms(1000);

    // 2. Network Registration Check
    USART2_OutString("AT+CREG?\r\n");    // Network registered?
    KM_Delay_ms(1000);

    // 3. Set SMS Text Mode
    USART2_OutString("AT+CMGF=1\r\n");   // Set SMS to text mode
    KM_Delay_ms(500);

    // 4. Dial Emergency Number (for ringing)
    USART2_OutString("ATD+919010933040;\r\n");  // Replace with real number
    KM_Delay_ms(1000); // Let it ring briefly

    // 5. LCD Indication
    KM_LCD_Write_Cmd(0x89);
    KM_LCD_Write_Str((unsigned char *)" HELP ");
    KM_LCD_Write_Cmd(0xC9);
    KM_LCD_Write_Str((unsigned char *)" VALID ");

    // 6. Prepare Google Maps Link
    sprintf(map, "https://www.google.com/maps?q=%.5lf,%.5lf", n1, n2);

    // 7. Send SMS with location
    USART2_OutString("AT+CMGS=\"+919010933040\"\r\n");  // SMS to number
    KM_Delay_ms(500);
    USART2_OutString("HELP ME!\n");
    USART2_OutString(map);  // Send location
    USART2_OutChar(0x1A);   // End of SMS (Ctrl+Z)
    KM_Delay_ms(1000);

    // 8. Buzzer Indication
    GPIOB_ODR |= (1 << 12);     // BUZZER ON
    KM_Delay_ms(1000);
    GPIOB_ODR &= ~(1 << 12);    // BUZZER OFF
}*/


void gsm_send(void)
{   
	      
	      USART2_OutString("ATD+917093473483;\r\n");
	      KM_Delay_ms(100);
	      KM_LCD_Write_Cmd(0x89);
			  KM_LCD_Write_Str((unsigned char *)" help ");
				KM_LCD_Write_Cmd(0xC9);
	      KM_LCD_Write_Str((unsigned char *)" VALID ");
				//sprintf(map,"https://maps.google.com/?q=%.5lf,%.5lf",lat_buff,log_buff);
				//sprintf(map," https://www.google.com/maps?q=%.5lf,%.5lf",n1,n2);
					sprintf(map," https://www.google.com/maps?q=%.5lf,%.5lf",n1,n2);


				USART2_OutString("AT+CMGF=1\r\n");
	       KM_Delay_ms(100);
		    USART2_OutString(map);
			  KM_Delay_ms(100);
		    USART2_OutString(" help me ");
				KM_Delay_ms(100);
		    USART2_OutString(map);
		    USART2_OutChar(0x1A);
				KM_Delay_ms(10);
}

