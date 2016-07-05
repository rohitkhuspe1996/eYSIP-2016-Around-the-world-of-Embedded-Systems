/* problem 2:
 Use switch SW2 and sw2Status (a variable). Your program should increment sw2Status
by one, every time switch is pressed. Note how the value of sw2Status changes on each
switch press. Use debugger and add sw2Status to “Watch Expression” window. Does
the value of sw2Status increment by one always? Show the result to TA. Note: Define
sw2Status as a global variable and in debug perspective use continuous refresh option
(You will find Continuous Refresh button on top of the Expression Window). You can
use step debugging or breakpoints to check the variable value.
*/
/*
Pins used:
SW2- PF0
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


uint32_t sw2status=0;		/* variable used to monitor the switch pressed */
uint32_t a=0;			    /* variable to store the status of switch (ON or OFF) */

int main(void)
{

      uint32_t pinVal=0;    /* variable to read the status of switch 2 */

      /* Set up the Clock */
      SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

      /* Enabling the GPIO Peripheral (Port F) */
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

      /* PF0 is NMI by default. Unlock PF0. */
	  HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;
      HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
      HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;

      /* Configuring the GPIO pins as input and output */
      GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
      GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);
      GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
      while(1)
      {

          /* Reading the switch status */
          pinVal=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);
          a=pinVal;

          /* If switch is pressed */
          if(a==0)
          {
              sw2status++;						/* increment value of status varaiable */
          }

      }

}
