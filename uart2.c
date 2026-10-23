#include "stm32f401rbt6.h"
#include "uart2.h"
#include "km_lcd.h"

int timeout_flag=0;
void USART2_Init(void) 
{   
	  RCC_AHB1ENR |= (0x01 << 0);   // Enable GPIOA clock

    GPIOA_MODER &= 0xFFFFFFCF;    // Clear bits 4-5 (PA2)
    GPIOA_MODER |= 0x00000020;    // Set PA2 as alternate function 
		
    GPIOA_MODER &= 0xFFFFFF3F;    // Clear bits 6-7 (PA3)
    GPIOA_MODER |= 0x00000080;    // Set PA3 as alternate function
		
    GPIOA_AFRL  &= 0xFFFFF0FF;    // Clear bits 8-11 (PA2)
    GPIOA_AFRL  |= 0x00000700;    // Set PA2 to AF7 (USART2_TX)
		
    GPIOA_AFRL  &= 0xFFFF0FFF;    // Clear bits 12-15 (PA3)
    GPIOA_AFRL  |= 0x00007000;    // Set PA3 to AF7 (USART2_RX)
		
    RCC_APB1ENR |= (0x01 << 17);  // Enable USART2 clock
}

void USART2_Config(void) 
{
    USART2_BRR = 0x683;            // Set baud rate to 9600 (for 16 MHz clock)
    USART2_CR1 |= (0x01 << 13);   // Enable USART2
    USART2_CR1 |= (0x01 << 3);    // Enable transmitter
    USART2_CR1 |= (0x01 << 2);    // Enable receiver
}

void USART2_OutChar(unsigned char ch) 
{
    while (!(USART2_SR & (0x01 << 7)));        // Wait for TX buffer to be empty
    USART2_DR = ch;                          // Send the character
}

void USART2_OutString(char str[])
{
	   int i=0;
     while (str[i] != '\0') 
	   {                                               // Loop until null terminator
        USART2_OutChar(str[i]);                       // Send each character
        i++;
     }
 }

/****************UART1************/

void USART1_Init(void)
{
	RCC_AHB1ENR |=(0x1<<0);           //Enable Port A Clock
	
	GPIOA_MODER &=0xffc3ffff;       //Enable PA9 and PA10 as alternate functions (load ‘10’ in GPIO_PORTA_MODE Register)//clear 21-18 bit
	GPIOA_MODER |=0x00280000;       //set 1 to 21st and 19th bit
                                  //Select UART1_Tx and UART1_RX functionality using Mux register (Load ‘7’ in GPIO_PORTA_AFRH)
	GPIOA_AFRH &=0xfffff00f;        //clear 4-7 bit field
	GPIOA_AFRH |=0x00000770;        //load 0111 to 7-4 bit fields
                                  //Enable UART1 clock
	
//	GPIOA_AFRH &=(0xffffff0f);//clear 4-7 bit field
//  GPIOA_AFRH |=(0x00000770);//load 0111 to 7-4 bit fields
	RCC_APB2ENR |=(0x1<<4);           //set 4th bit
}

void USART1_Config(void)
{
//Set 9600 baud rate in BRR (0x683)
	USART1_BRR = 0x683;
//Set 13th, 2nd and 3rd bits in CR1 register to enable USART 1 enable
	USART1_CR1 |=(0x1<<13);
	USART1_CR1 |=(0x1<<2);
	USART1_CR1 |=(0x1<<3);
	USART1_CR3 |= (1 << 6); // Enable DMA for RX
}

char USART1_RX_INCHAR_TIMEOUT(int timeout)
{
    while(timeout>0)
		{			
    if(USART1_SR & 0x1<<5)
    {
        return (USART1_DR &0xff);
    }
		timeout--;
	  }
		timeout_flag=1;
		return 1;
}



///***************STR_STR***********//
int mystr_str(char str[], char sub[]) 
{
    int i=0,j=0;
    
    for (i = 0; str[i] != '\0'; i++) 
			{
        for(j=0;sub[j]!='\0'&& str[i+j]==sub[j];j++);
					if(sub[j]=='\0')
					{
						return i;
					}
					j=0;
				
			}
				return 1;
			
}