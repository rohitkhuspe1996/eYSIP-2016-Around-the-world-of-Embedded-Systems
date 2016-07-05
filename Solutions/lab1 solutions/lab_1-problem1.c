
/*
* Problem 1:
* Use switch SW1 to Turn on Red LED on first switch press, Green LED on second
* switch press and Blue LED on third switch press. Repeat the same cycle next switch
* press onwards. Note that LED should remain on for the duration switch is kept pressed
* i.e. LED should turn off when switch is released.
*/
/* SW1 is connected to pin PF4.
The 3 pins of RGB are
R - PF1
G - PF3
B - PF2
/*
Include the header files.
*/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
int main(void)

{	uint8_t sw1status=0;       /* variable used for keeping the LED on till switch is pressed */
	uint32_t count=0;	/* for selection of RGB led */
	uint8_t led1;		/* for reading the value of sw1 */


	/* Set the crystal oscillator */
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	/* Enable the GPIO Peripheral */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    /* Configure the GPIO pins as input or output */
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);  //initializing port as a output
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4);							//initializing port as a input
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //config. pin 4 of port F

	while(1)

	{
	    led1=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4);		/* reading the data from input port sw1 */
		if((led1 & GPIO_PIN_4)==0 )                         /* verifying the condition for input port */
        {
			if(sw1status==0)                                      /* When switch is pressed */
			{
                switch (count)
                {

                    /* selecting rgb led using switch case */
                    case 0:                                                             /* Select Red LED */
                    sw1status=1;
                    count=1;
                    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,2);
                    SysCtlDelay(4000000);
                    break;

                    case 1:                                                             /* Selects Blue LED */
                    sw1status=1;
                    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,4);
                    count=2;
                    SysCtlDelay(4000000);
                    break;

                    case 2:                                                             /* Selects Green LED */
                    sw1status=1;
                    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,8);
                    count=0;
                    SysCtlDelay(4000000);
                }
			}
		}
			else
            {
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0);   /* Turn off the LED when switch is released */
                sw1status=0;
            }



	}
}
