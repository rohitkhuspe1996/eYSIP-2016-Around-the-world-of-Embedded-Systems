/* LAB 2
 * Problem Statement 2
 * Use sw2 to increment a global variable once for each button press. Check if the variable
always increments by one (adjust the time interval of 10 ms if you wish to)
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
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"


uint32_t sw2=1;   // Variable to read the status of switch 2
uint32_t sw2status=0 ; //Variable to monitor each button press
uint32_t a=0;      //Variable to monitor the switch press
uint8_t flag=0;  //To detect key Press
uint8_t result;  // To store the value of flag
unsigned char detectkeyPress()
{
    /* Check the status of sw2. If switch is pressed increment a by 1. Check the condition of switch press twice after interval of 10ms.
     * if switch is pressed both the times then set the flag else flag is cleared     */
    if((sw2 & GPIO_PIN_0)==0 )
    {
        a++;

        if(a==2)
            {
                flag=1;
                return flag;

            }
        else
        {
            flag=0;
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
SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);    // set up the clock

// Unlock F0 for switch 2
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    // enable port F
      HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;
      HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
      HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;
      GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);    // make F0 an input
      GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);


GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);  //initializing port as a output
GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //config. pin 4 of port F

//Timer Configuration
SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);


//Calculate the delay and load the value in TimerLoadSet.
//To calculate delay divide "SysCtlClockGet()" by the required frequency. Divide this answer by 2 as required duty cycle is 50%.
ui32Period = (SysCtlClockGet() / 100) / 2;
TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);


//Enable the interrupt in timer module and NVIC
IntEnable(INT_TIMER0A);
TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
IntMasterEnable();


//Enable the timer
TimerEnable(TIMER0_BASE, TIMER_A);


while(1)
    {
    }
}



void Timer0IntHandler(void)
{

/* Clear the timer interrupt */
TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
sw2=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);        //reading the data from input port sw1
/* If switch is pressed then incerment the value of sw2status by 1 */
result = detectkeyPress();
    if(result==1)
    {
        sw2status++;
    }
    else
    {
    }
}