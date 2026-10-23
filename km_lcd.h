/*****************************************************
* File:KM_LCD.H
* Description:This file contain function declration related to KM_LCD
*******************************************************/


void write_high_nibble( unsigned char data );
/*Write Lower Nibble data in to LCD.
Return Value: No return Value; Argument: unsigned char data */

void write_low_nibble( unsigned char data );
/*LCD Initialization.
Return Value: No return Value; Argument: No Argument */

void KM_LCD_Init(void);
/*Write a LCD command.
Return Value: No return Value; Argument: unsigned char */

void KM_LCD_Write_Cmd( unsigned char );
/*Write a LCD Data (single character).
Return Value: No return Value; Argument: unsigned char */

void KM_LCD_Write_Data( unsigned char );
/*Write a LCD Data (Multiple characters).
Return Value: No return Value; Argument: character pointer */

void KM_LCD_Write_Str(unsigned char []);

void gpio_init(void);

