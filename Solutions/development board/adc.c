/*
Use the on chip ADC and observe the result in watch window of debug perspective
*/


/* Include all the header files. Include "adc.h"  which contains definations for adc driver and "uart.h" for using UART */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"


volatile uint8_t x;          //  variable which holds the digital value after analog to digital conversion of X co-ordinate of joystick.
volatile uint8_t y;          //  variable which holds the digital value after analog to digital conversion of Y co-ordinate of joystick.



int main(void)
{

	uint32_t ui32ADC0Value;
	uint32_t ui32ADC1Value;

	/* Set up the System Clock and enable the peripherals */
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    /* Enable the ADC */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0);
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_1);


    /* Configure both the ADCs . One ADC is used to convert X value of Joystick and another ADC converts Y value of Joystick */
	ADCSequenceConfigure(ADC1_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC1_BASE,0,0,ADC_CTL_CH7|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC1_BASE, 0);

	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE,0,0,ADC_CTL_CH6|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 0);


	while(1)
	{
	     /* Clear the ADC interrupt flag and start the conversion process */
		ADCIntClear(ADC1_BASE, 0);
		ADCProcessorTrigger(ADC1_BASE, 0);

		/* Wait till conversion is complete */
		while(!ADCIntStatus(ADC1_BASE, 0, false))
		{
		}

		/* Get the conversion result */
		ADCSequenceDataGet(ADC1_BASE, 0,&ui32ADC0Value);

		/*Divide the result of ADC to get column number */
		y =  ui32ADC0Value/32;

		/* Clear the ADC interrupt flag and start the conversion process */
		ADCIntClear(ADC0_BASE, 0);
		ADCProcessorTrigger(ADC0_BASE, 0);

		/* Wait till conversion is complete */
		while(!ADCIntStatus(ADC0_BASE, 0, false))
		{
		}

		/* Get the conversion result */
		ADCSequenceDataGet(ADC0_BASE, 0,&ui32ADC1Value);

		/*Divide the result of ADC to get page number */
		x =  ui32ADC1Value/512;

		SysCtlDelay(67000);
	}
}
