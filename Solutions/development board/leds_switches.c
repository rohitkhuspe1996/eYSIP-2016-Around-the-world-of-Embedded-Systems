#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"

uint8_t led1=0;
uint8_t led2;
uint8_t led3;
uint8_t led4;

int main(void)

{


	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_3);  //initializing port as a output
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);



	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2);

	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);    // make F0 an input
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK)= 7;

	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,GPIO_PIN_6|GPIO_PIN_7);
	GPIOPadConfigSet(GPIO_PORTD_BASE,GPIO_PIN_6,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //config. pin 4 of port F
	GPIOPadConfigSet(GPIO_PORTD_BASE,GPIO_PIN_7,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //config. pin 4 of port F
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE,GPIO_PIN_7);
	GPIOPadConfigSet(GPIO_PORTC_BASE,GPIO_PIN_7,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //config. pin 4 of port F
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4);							//initializing port as a input
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //config. pin 4 of port F



	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,1);
	GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5,64);
	GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,4);
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,8);
	SysCtlDelay(670000);

	while(1)

	{
		led1=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4);		//reading the data from input port sw1



		if(led1 ==0 )
		{						//varifying the condition for input port
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0);
		}
		else
		{
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,2);
		}
		led2=GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_7);		//reading the data from input port sw1
		if(led2 ==0 )
		{						//varifying the condition for input port
			GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5,0);
		}
		else
		{
			GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5,32);
		}
		led3=GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_6);		//reading the data from input port sw1
		if(led3==0)
		{
			GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,0);
		}
		else
		{
			GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,4);
		}
		led4=GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_7);		//reading the data from input port sw1
		if(led4==0 )
		{						//varifying the condition for input port
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,0);
		}
		else
		{
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,8);
		}
		SysCtlDelay(67000);

	}
}
