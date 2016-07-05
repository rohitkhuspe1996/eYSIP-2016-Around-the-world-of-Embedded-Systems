/* ***LAB 4 *** */
/* PROBLEM STATEMENT :
 *  1. Use the inbuilt ADC to interface a joystick with Tiva Board. The analog values read from the joystick has to be converted to digital and displayed on the watch window of the CCS IDE.
 *  2. Now, use the inbuilt UART to communicate the digital values (i.e. both the X axis and the Y axis) to the computer. Use a terminal emulation software like Serial Terminal or Real Term
 *      to view the data being sent by the Tiva Board to your computer.
 *     The data sent should have the following syntax : ”X: Digital equivalent of the read value” ”Y: Digital equivalent of the read value”
 *  3. Once you complete the above mentioned problem statements, you have to depict the values received in a graphical representation. In short, create a GUI which tracks the real time movements
 *     of the joystick.
 *     The circle marker should move to the left if the joystick button
 *     is tilted towards left and right if tilted towards right. The marker should remain at the centre when joystick is stationary.
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

      //  variable containing data to be converted into 8 bit.
volatile uint8_t x;          //  variable which holds the digital value after analog to digital conversion of X co-ordinate of joystick.
volatile uint8_t y;          //  variable which holds the digital value after analog to digital conversion of Y co-ordinate of joystick.
    //  variable which contains 8 bit data to be transmitted using uart.


/* char convertt is a function which converts the data to be transmitted in 8 bit form. The argument to the function is X or Y co-ordinate
 * of the Joystick location. It returns 8 bit value of the data to be transmitted.
 */



int main(void)
{

	uint32_t ui32ADC0Value;
	uint32_t ui32ADC1Value;

	/* Set up the System Clock */
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);


	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0);
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_1);



	ADCSequenceConfigure(ADC1_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC1_BASE,0,0,ADC_CTL_CH7|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC1_BASE, 0);

	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE,0,0,ADC_CTL_CH6|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 0);


	while(1)
	{
		ADCIntClear(ADC1_BASE, 0);
		ADCProcessorTrigger(ADC1_BASE, 0);
		while(!ADCIntStatus(ADC1_BASE, 0, false))
		{
		}
		ADCSequenceDataGet(ADC1_BASE, 0,&ui32ADC0Value);
		y =  ui32ADC0Value/32;
		ADCIntClear(ADC0_BASE, 0);
		ADCProcessorTrigger(ADC0_BASE, 0);
		while(!ADCIntStatus(ADC0_BASE, 0, false))
		{
		}
		ADCSequenceDataGet(ADC0_BASE, 0,&ui32ADC1Value);
		x =  ui32ADC1Value/512;

		SysCtlDelay(67000);
	}
}
