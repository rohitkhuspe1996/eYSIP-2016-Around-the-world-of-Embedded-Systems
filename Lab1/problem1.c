
/*.problem 1: 
Use switch SW1 to Turn on Red LED on first switch press, Green LED on second
switch press and Blue LED on third switch press. Repeat the same cycle next switch
press onwards. Note that LED should remain on for the duration switch is kept pressed
i.e. LED should turn off when switch is released.*/


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
int main(void)

{	uint8_t cor=0;       //variable used for turning on the led untill switch is pressed
	uint32_t count=0;	//for selection of rgb led
	uint8_t led1;		//for reading the value of sw1

	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);  //initializing port as a output
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4);							//initializing port as a input
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //config. pin 4 of port F

	while(1)

	{	led1=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4);		//reading the data from input port sw1
		if((led1 & GPIO_PIN_4)==0 ){						//varifying the condition for input port
			if(cor==0)
			{
			switch (count){									//selecting rgb led using switch case
			case 0:cor=1;
			count=1;
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,2);
			SysCtlDelay(4000000);
			break;
			case 1:cor=1;
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,4);
			count=2;
			SysCtlDelay(4000000);
			break;
			case 2:cor=1;
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,8);
			count=0;
			SysCtlDelay(4000000);
			}
			}
		}
			else{						//on the release of the switch off the led
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0);
			cor=0;
			}



	}
}
