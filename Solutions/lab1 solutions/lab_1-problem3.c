/* problem 3:
Configure SW1 and SW2 such that:
Every time SW1 is pressed toggle delay of LED should cycle through approximately
0.5s, 1s, 2s (Of any one color).
Every time SW2 is pressed color of LED should cycle through Red, Green and Blue
*/

/* Include the header files */
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
    uint32_t count2=0;              /* variable used for selecting time delay */
    uint32_t verify2=0;				/* variable used as a flag for sw2 */
    uint32_t t1=6700000;			/* delay variable */
    uint8_t color=2;				/* Selecting color of RGB */
    uint8_t verify1=0;				/* variable used as flag for sw1 */
    uint8_t pinVal;                 /* reading value of sw2 */
    uint8_t var1;		            /* Reading value of sw1 */

    /* Set up the clock */
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    /* Enable the GPIO port */

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    /* Unlock pin PF0 */
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;

    /* Configure port pins as input and output */
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);                                     // make F0 an input
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);  //config pin f0
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);                                     // make F4 an input
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);  //config pin f4
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);              //set f1,f2,f3 as a output

    while(1)
    {
          /* Read the status of sw1 and sw2 */
          pinVal = GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);                               //reading value of sw2
          var1=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4);					                  //reading value of sw1

          /* Turn ON R,G or B LED with delay of 0.5 Seconds */
          GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,color);
          SysCtlDelay(t1);

          /* Turn OFF R,G or B with delay of 0.5 Seconds */
          GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0);
          SysCtlDelay(t1);

          if((GPIO_PIN_0 & pinVal)==0)                                               /* If switch SW2 is pressed */
            {
                  if(verify1==0)
                  {                                                                      //flag condition for sw1
                      color=color*2;						                             //change the color of RGB
                      if(color>8)
                      {
                          color=2;
                      }

                      switch(color)                                                      //switch case for selecting color
                      {
                      case 2:
                          verify1=1;
                          GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,color);
                          SysCtlDelay(4000000);
                          break;

                      case 4:
                          verify1=1;
                          GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,color);
                          SysCtlDelay(4000000);
                          break;

                      case 8:
                         verify1=1;
                         GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,color);
                         SysCtlDelay(4000000);
                         break;
                      }
                  }
               }
               else
                {
                    verify1=0;              // reset flag on release of switch
                }

          if((GPIO_PIN_4 & var1)==0)        //condition for sw1
            {
              if(verify2==0)
              {
                  count2++;              //increament count for changing delay
                 if(count2>2)
                 {
                     count2=0;
                 }
              }
              switch(count2)            //switch for selecting time
              {
              case 0:
                  t1=6700000;
                  break;

              case 1:
                  t1=2*6700000;
                  break;

              case 2:
                  t1=4*6700000;
                  break;
              }
          }
          else
          {
              verify2=0;					//reset flag on release of switch
          }
    }
}




