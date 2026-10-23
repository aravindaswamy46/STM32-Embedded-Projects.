#include "stm32f401rbt6.h"
#include "km_lcd.h"
#include "systemticktimer.h"


void gpio_init(void)
{
	//intilaztion phase
	RCC_AHB1ENR |=0X01<<1; //Enable clock to GPIOB peripheral
	
	//configuration phase
	GPIOB_MODER &=0xFFFCF000; // Set the GPIOB pin mode for LCD (e.g., output)

	GPIOB_MODER |=0x00010555;
}
	

void write_high_nibble(unsigned char data)
{
	
data = data >> 4; // Shift the data to  higher nibble (bits 4-7)
GPIOB_ODR &=~(0x0f); // Clear the lower 4 bits (bits 0 to 3)
GPIOB_ODR |= data;   // Set the upper 4 bits in GPIOB_ODR to the value of 'data'
GPIOB_ODR |= (0x01 << 8); //enable the pin in lcd 
KM_Delay_ms(30); //short delay
GPIOB_ODR &= ~(0x01 << 8); //clear the enable pin in lcd

}

void write_low_nibble(unsigned char data)
{
	
	   data &= (0x0f);
     GPIOB_ODR &=~(0x0f); // Clear bits 0-3 of GPIOB_ODR
     GPIOB_ODR |= (data);  // Set bits 0-3 of GPIOB_ODR with the data
    
    GPIOB_ODR |= (0x01 << 8); 
    KM_Delay_ms(30);                     
    GPIOB_ODR &= ~(0x01 << 8);   

}
void KM_LCD_Write_Cmd(unsigned char data)
{
	GPIOB_ODR &=~(0X01<<4); //clear the slecet register in lcd
	
	write_high_nibble(data); //fucntion call of the higher
	write_low_nibble(data); //function call of lower
	
}

void KM_LCD_Write_Data(unsigned char data)
{
	
	GPIOB_ODR |=(0X01<<4);//set the slecet rigester in lcd
	
	write_high_nibble(data);
	write_low_nibble(data);	
}

void KM_LCD_Init(void)
{
	KM_Delay_ms(20);  // Delay for initial setup
	KM_LCD_Write_Cmd(0x33); // Send the first initialization command (0x33)
	KM_Delay_ms(1); // Wait for 1 ms to ensure the command is processed

	KM_LCD_Write_Cmd(0x32);  // Send the second initialization command (0x32)
	KM_LCD_Write_Cmd(0x0C);   // Set display control command (e.g., display on, cursor off, no blinking)
	KM_LCD_Write_Cmd(0x01);   // Clear the display (0x01)
	
}
void KM_LCD_Write_Str(unsigned char str[])
{
	
	int i=0;
	while(str[i]!='\0')
	{
		KM_LCD_Write_Data(str[i]);
		if(i>15)
		{
			KM_LCD_Write_Cmd(0xc0);
			break;
		}
			i++;
	}
	for(;str[i]!='\0';i++)
	{
	KM_LCD_Write_Data(str[i]);
	}
}


