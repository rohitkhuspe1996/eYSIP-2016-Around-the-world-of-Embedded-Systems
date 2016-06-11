/* LAB 2
 * Problem Statement - Use sw1 to change the color of the led (R? G? B? R. . . .) where you should press
the switch just once instead of long press in Lab 1. Use switch debouncing mentioned
below in the procedure to differentiate between switch bounce and actual key press.
function detectkeyPress checks whether switch is pressed after an interval of 10ms twice. If the condition is satisfied both the times, flag is set.
The ISR Timer0IntHandler is used to create an interval of 10ms.
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"


uint32_t sw1=1;   // Variable to read the status of switch 1
uint8_t count=2;  //Variable to select the color to be switched on of RGB
uint32_t a;      //Variable to monitor the switch press
uint8_t flag=0;  //To detect key Press
uint8_t result;  // To store the value of flag
unsigned char detectkeyPress()
{
    if((sw1 & GPIO_PIN_4)==0 )
        /* Check the status of sw1. If switch is pressed change the color. Check the condition of switch press twice after interval of 10ms.
             * if switch is pressed both the times then set the flag else flag is cleared     */
    {
        a++;

        if(a==2)
            {
                flag=1;
                return flag;

            }

    }
    else
    {
        a=0;
        flag=0;
        return flag;
    }

}

int main(void)
{
uint32_t ui32Period;

SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);  //initializing port as a output
GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4);                            //initializing port as a input
GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //config. pin 4 of port F


//Timer Configuration
SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

/*
 * Calculate the delay and load the value in TimerLoadSet
 * Delay is calculated using "SysCtlClockGet()". Divide this function by the required frequency. divide the answer by 2 as the duty cycle is %
 */
ui32Period = (SysCtlClockGet() / 100) / 2;
TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);


//Enable the interrupt in Timer and NVIC
IntEnable(INT_TIMER0A);
TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
IntMasterEnable();


//Enable the Timer
TimerEnable(TIMER0_BASE, TIMER_A);
while(1)
    {
    }
}



void Timer0IntHandler(void)
{

TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
sw1=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4);        //reading the data from input port sw1
result = detectkeyPress();
/* If switch is pressed then incerment the color of rgb changes */
    if(result==1)
    {
        count=count*2;
                if(count>8)
                {
                    count=2;
                }
                switch(count)
                {
                case 2:GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,2);
                break;
                case 4:GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,8);
                break;
                case 8:GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,4);
                break;
                }
    }
    else
    {
    }
}