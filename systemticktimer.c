/*****************************************************
* File:systemticktimer
* Description:This file contain of systemtick timer definations and header files
*******************************************************/

#include "stm32f401rbt6.h"
#include "systemticktimer.h"

#define INTMAX_32            2147483647

/*****************************************************
*           SysTick_Handler()
*******************************************************
*Description:This funtion contain of starup code SysTick_Handler[interrupt]
*Arguments:No arguments 
*Return Type:No return types 
*Notes:Conditon of tick and INTMAX_32[FROM STDIO.H]
*******************************************************/

volatile unsigned int tick=0; 

void SysTick_Handler(void)      //FUNCALL FROM STATUP CODE
{
	 if(tick==INTMAX_32)         //INTMAX_32 FROM STDIO.H [WHAT EVER THE BIT[32,64]
	 {
		 tick=0;                   //AGAIN TICK =0
	 }
	 tick++;                     //INR THE TICK
 }
/*****************************************************
*             KM_mdelay()
*******************************************************
*Description:This function ontain of km_m_delay 
*Arguments:No arguments
*Return Type:No return type
*Notes:In this km_delay[M] for error handling condtion for the interrupt
*******************************************************/

void KM_Delay_ms(int Delay)
{
    unsigned int delay_until;

                                   
    if (tick + Delay >= INTMAX_32)     // Check if adding the delay to the current tick will overflow (i.e., exceed the max value of an unsigned int)
    {
        tick = 0;  										// Reset tick if it overflows
    }
    delay_until = tick + Delay;  			// Calculate the target tick value after the delay										
    while (tick < delay_until);      // Wait until the tick reaches the target value
}
/*****************************************************
*             systick_timer()
*******************************************************
*Description:This function contain systick_timer
*Arguments:
*Return Type:No return type
*Notes:In the function defination of init and config of the systemtick timer
*******************************************************/

void systick_timer(void)
{
	
	STK_LOAD = 16000;           // Load the reload value for the SysTick timer[m]
	STK_VAL = 0x123;            // Set the current value of the SysTick timer (G.V)
	STK_CTRL |= 0x04;           // Set the clock source (bit 2 = 1, use processor clock)
	STK_CTRL |= 0x01;           // Enable the SysTick timer (bit 0 = 1)
	STK_CTRL |= 0x01 << 1;      // Enable SysTick interrupt (bit 1 = 1)

}



	
	