/*
* To use 4 push button switches to turn ON 4 LEDs
*/

/* Include the header files for GPIO */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"

/* Variables to check the status of switch press.
* For variables correspond to 4 switches
* led1 - PF4
* led2 - PD7
* led3 - PB2
* led4 - PF3
*/

uint8_t led1;
uint8_t led2;
uint8_t led3;
uint8_t led4;

int main(void)

{
    /* Enable the System clock */
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	/* Enable the GPIO peripherlas */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    /* Configure the pins connected to LED as output */
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_3);
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2);

    /* Unlock PD7 */
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK)= 7;

     /* Configure the pins connected to push buttons as inout */
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,GPIO_PIN_6|GPIO_PIN_7);
	GPIOPadConfigSet(GPIO_PORTD_BASE,GPIO_PIN_6,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	GPIOPadConfigSet(GPIO_PORTD_BASE,GPIO_PIN_7,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE,GPIO_PIN_7);
	GPIOPadConfigSet(GPIO_PORTC_BASE,GPIO_PIN_7,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4);
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);


    /* Turn OFF all the LEDs */
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,1);
	GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5,64);
	GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,4);
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,8);
	SysCtlDelay(670000);

	while(1)
	{
	    /* Check the status of switch 1 */
		led1=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4);

        /* If switch is pressed turn ON the corresponding LED */
		if(led1 ==0 )
		{
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0);
		}
		else
		{
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,2);
		}

		/* Check the status of switch 2 */
		led2=GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_7);

		/* If switch is pressed turn ON the corresponding LED */
		if(led2 ==0 )
		{
			GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5,0);
		}
		else
		{
			GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5,32);
		}

		/* Check the status of switch 3 */
		led3=GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_6);

		/* If switch is pressed turn ON the corresponding LED */
		if(led3==0)
		{
			GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,0);
		}
		else
		{
			GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,4);
		}

		/* Check the status of switch 4 */
		led4=GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_7);

		/* If switch is pressed turn ON the corresponding LED */
		if(led4==0 )
		{
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,0);
		}
		else
		{
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,8);
		}
		SysCtlDelay(67000);

	}
}
