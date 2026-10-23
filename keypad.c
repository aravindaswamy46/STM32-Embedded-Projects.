#include "stm32f401rbt6.h"
#include "keypad.h"
#include "km_lcd.h"
#include "systemticktimer.h"
#include "i2c.h"

#define R1_pin 0x01<<4
#define C1_pin 0x01<<2

char keypad_press=0;

unsigned char keypad_buff[10];

void keypad_init_config(void)
{
		RCC_AHB1ENR |=0X01<<0|0X01<<2; //enable clock for port a and port b
		GPIOA_MODER &= ~(0xFF<<8);    
		GPIOA_MODER |= (0x55<<8);     
		
		GPIOC_MODER &= ~(0xFF<<4);  //set input mode  
		GPIOC_PUPDR &= ~(0xFF<<4);   //clear pupdr bits
		GPIOC_PUPDR |= (0xAA<<4);     //make pull down
}

unsigned char keys_scan(void)
{
	int row,col;
	char key[4][4]={{'1','2','3','A'},
									{'4','5','6','B'},
									{'7','8','9','C'},
									{'*','0','#','D'}};
	
	for(row=0;row<4;row++)
		{
			
			for(col=0;col<4;col++)
			{
				GPIOA_ODR |= 0x01<<4<<row;  

				if(GPIOC_IDR & C1_pin<<col)	//col start from pC2
				{
						GPIOA_ODR &=~(R1_pin<<row);
					return key[row][col];
				}
					GPIOA_ODR &=~(R1_pin<<row);

			}
			GPIOA_ODR &=~(R1_pin<<row);

		} 
		return 0;
}
void keypad_write(void)
{
	  unsigned char buff[10];
   	int i=0;
		while(i<10)
		{
			keypad_press=keys_scan();
			
			if(keypad_press!=0)
			{
				
				KM_LCD_Write_Data(keypad_press);
				//KM_LCD_Write_Data(buff)
				keypad_buff[i]=keypad_press;
				KM_Delay_ms(200);
			}
			
			
				i++;

		}
		keypad_buff[i]='\0';
		KM_I2C_Mem_Write(0x50,0x13,0x01,keypad_buff,10);
		KM_I2C_Mem_Read(0x50, 0x13, 0x01,buff,10);
//		KM_LCD_Write_Cmd(0x01);   // Clear the display (0x01)
//	  KM_LCD_Write_Str(buff);
		
}