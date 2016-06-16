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
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"

volatile uint8_t char1;       //  variable containing data to be converted into 8 bit.
volatile uint32_t x;          //  variable which holds the digital value after analog to digital conversion of X co-ordinate of joystick.
volatile uint32_t y;          //  variable which holds the digital value after analog to digital conversion of Y co-ordinate of joystick.
volatile uint8_t convert;     //  variable which contains 8 bit data to be transmitted using uart.
uint8_t temp1=3;              //  variable to store the initial X co-ordinate of Joystick.
uint8_t temp2=3;			  //  variable to store the initial Y co-ordinate of Joystick.
uint32_t xdata[4];
uint32_t ydata[4];

/* char convertt is a function which converts the data to be transmitted in 8 bit form. The argument to the function is X or Y co-ordinate
 * of the Joystick location. It returns 8 bit value of the data to be transmitted.
 */
char convertt(uint32_t x)
{
    switch(x)
    {
		case 0:convert='0';
		break;
		case 1:convert='1';
			break;
		case 2:convert='2';
			break;
		case 3:convert='3';
			break;
		case 4:convert='4';
			break;
		case 5:convert='5';
			break;
		case 6:convert='6';
			break;
		case 7:convert='7';
			break;
		case 8:convert='8';
			break;
		case 9:convert='9';
			break;
    }
    return convert;
}


int main(void)
{

	uint32_t ui32ADC0Value[4];
	uint32_t ui32ADC1Value[4];

	/* Set up the System Clock */
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

	/* Enable ADC Peripheral. We use ADC0 and ADC1 for simultaneous conversion of analog values(X znd Y co-ordinates).
	 * They correspond to pins PE2 and PE3.
	 */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

	/* Enable UART */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	/*Configure the UART Pins */
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	/*Set up the Baude Rate. We have used 9600 baudrate */
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(),9600,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));


	ADCSequenceConfigure(ADC1_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC1_BASE, 1, 0, ADC_CTL_CH0);
	ADCSequenceStepConfigure(ADC1_BASE, 1, 1, ADC_CTL_CH0);
	ADCSequenceStepConfigure(ADC1_BASE, 1, 2, ADC_CTL_CH0);
	ADCSequenceStepConfigure(ADC1_BASE,1,3,ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC1_BASE, 1);

	ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH1);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH1);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH1);
	ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 1);



	while(1)
	{
		ADCIntClear(ADC1_BASE, 1);
		ADCProcessorTrigger(ADC1_BASE, 1);
		while(!ADCIntStatus(ADC1_BASE, 1, false))
		{
		}

		ADCIntClear(ADC0_BASE, 1);
		ADCProcessorTrigger(ADC0_BASE, 1);
		while(!ADCIntStatus(ADC0_BASE, 1, false))
		{
		}

		ADCSequenceDataGet(ADC1_BASE, 1, ui32ADC0Value);
		y =  (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;
		y=y/640;
		ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC1Value);
		x =  (ui32ADC1Value[0] + ui32ADC1Value[1] + ui32ADC1Value[2] + ui32ADC1Value[3] + 2)/4;
		x=x/640;

		if(temp1!=x)
		{
			char1=convertt(x);
			if(char1<280)
			{
				UARTCharPut(UART0_BASE,char1);
				UARTCharPut(UART0_BASE,',');
			}

			char1=convertt(y);
			if(char1<280)
			{
				UARTCharPut(UART0_BASE,char1);
				UARTCharPut(UART0_BASE,'\n');
			}
			SysCtlDelay(670000);
		}
		temp1=x;

		if(temp2!=y)
		{

			char1=convertt(x);
			if(char1<280)
			{
				UARTCharPut(UART0_BASE,char1);
				UARTCharPut(UART0_BASE,',');
			}

			char1=convertt(y);
			if(char1<280)
			{
				UARTCharPut(UART0_BASE,char1);
				UARTCharPut(UART0_BASE,'\n');
			}
			SysCtlDelay(670000);
		}
		temp2=y;
	}
}
