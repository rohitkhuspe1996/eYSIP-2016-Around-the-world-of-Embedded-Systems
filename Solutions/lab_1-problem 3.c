/* problem 3: 
Configure SW1 and SW2 such that:
Every time SW1 is pressed toggle delay of LED should cycle through approximately
0.5s, 1s, 2s (Of any one color).
Every time SW2 is pressed color of LED should cycle through Red, Green and Blue
*/





#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"

int main(void)
{
    uint8_t pinVal;                //reading value of sw2			
    uint8_t cycle;		   
    uint8_t check=0;		  //flag which enables only one time execution
    uint8_t var1;		  //reading value of sw1

      SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);    // set up the clock
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    // enable port F0

      //unlock pin0 of PORT F
      HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;
      HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
      HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;
      
      GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);                                     // make F0 an input
      GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);  //config pin f0
      GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);                                     // make F4 an input
      GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);  //config pin f4
      GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);              //set f1,f2,f3 as a output
      while(1)
      {
      pinVal = GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);                                       //reading value sw2
      var1=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4);					     //reading value sw1
      if((GPIO_PIN_0 & pinVal)==0 )								//check press condition for sw1
      {
          GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,2);			//turn on led in sequence  
         SysCtlDelay(2700000);
         GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,8);
         SysCtlDelay(2700000);
         GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,4);
         SysCtlDelay(2700000);
         GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0);
         SysCtlDelay(270000);

      }

      else if((var1 & GPIO_PIN_4)==0)							//check the condition for sw1
      {
            if(check==0)							//flag which enables only one time execution of switch pressed
            {
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,2);      //turn on the led on and off for three cycle of 0.5,1,2second
                SysCtlDelay(6700000);
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0);
                SysCtlDelay(6700000);
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,2);
                SysCtlDelay(2*6700000);
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0);
                SysCtlDelay(2*6700000);
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,2);
                SysCtlDelay(4*6700000);
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0);

                check=1;



            }

            else
           {
            check=0;			restore flag value when switch is released
           }

      }
      else
      {
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0);     //off the rgb led when no switch is pressed
      }

      }
}