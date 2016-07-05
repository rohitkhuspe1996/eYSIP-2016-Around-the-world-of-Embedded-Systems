/* Displaying Joystick movement on GLCd */
/* This code uses PA5-PA7 instead of PB4-PB7 as data lines(D4-D7) of GLCD */

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
unsigned char temp1=3;
unsigned char temp2=64;

/* char convertt is a function which converts the data to be transmitted in 8 bit form. The argument to the function is X or Y co-ordinate
 * of the Joystick location. It returns 8 bit value of the data to be transmitted.
 */

void glcd_cmd(unsigned char cmd)
{
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,0x00);
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,0x00);

	/* RS = 0 */
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6,0x00);

	/* Put command on data lines */
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,cmd);
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,cmd);

	/* Generate a high to low pulse on enable */
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x01);
	SysCtlDelay(1340);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x00);

}

void glcd_data(unsigned char data)
{
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,0x00);
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,0x00);

	/* RS = 1 */
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6,0x40);

	/* Put command on data lines */
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,data);
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,data);

	/* Generate a high to low pulse on enable */
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x01);
	SysCtlDelay(1340);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x00);
}


void glcd_init()
{
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_5); //PE5 RST D0 to D3
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7); //PE5 RST D0 to D3
//	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0x00);	//cbi(GPORTC_2,GLCD_RST);

 /*   GPIODirModeSet(GPIO_PORTB_BASE,GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7,GPIO_DIR_MODE_OUT);
   GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_5,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
  //  GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_6,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_7,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);*/

    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,GPIO_PIN_6);  //RS
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,GPIO_PIN_3);  //CS2 and CS1
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;	// unlocking sw2 switch
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0); //En
    GPIODirModeSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_DIR_MODE_OUT);
	SysCtlDelay(6700000/50);							// creates ~10ms delay - TivaWare fxn
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x00);	//cbi(GPORTC_2,GLCD_RST);
	/* Set RST */
	GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,0x20);

	/* Set CS1 (CS1=1 and CS2=0) The right side is selected(column>64) */
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x00);

	/* Select the start line */
	glcd_cmd(0xC0);
	//SysCtlDelay(6700);

	/* Send the page */
	glcd_cmd(0xB8);
//	SysCtlDelay(6700);

	/*Send the column */
	glcd_cmd(0x40);
	//SysCtlDelay(6700);

	/* Send glcd on command */
	glcd_cmd(0x3F);


	/* Initialize the right side of GLCD */
	/* Set CS2 (CS2=1 and CS1=0) The right side is selected(column>64) */
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x08);

	/* Select the start line */
	glcd_cmd(0xC0);
	SysCtlDelay(6700);

	/* Send the page */
	glcd_cmd(0xB8);
//	SysCtlDelay(6700);

	/*Send the column */
	glcd_cmd(0x40);
//	SysCtlDelay(6700);

	/* Send glcd on command */
	glcd_cmd(0x3F);
//	SysCtlDelay(6700);
}

void glcd_setpage(unsigned char page)
{
	/* Set CS1 (CS1=1 and CS2=0) The right side is selected(column>64) */
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x00);
    glcd_cmd(0xB8 | page);
	SysCtlDelay(100);

	/* Set CS2 (CS2=1 and CS1=0) The right side is selected(column>64) */
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x08);
    glcd_cmd(0xB8 | page);
	SysCtlDelay(100);
}

void glcd_setcolumn(unsigned char column)
{
	if(column < 64)
	{
		/* Set CS1 (CS1=1 and CS2=0) The right side is selected(column>64) */
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x00);
		glcd_cmd(0x40 | column);
		SysCtlDelay(670);
	}
	else
	{
		/* Set CS2 (CS2=1 and CS1=0) The right side is selected(column>64) */
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x08);
		glcd_cmd(0x40 | (column-64));
		SysCtlDelay(670);
	}

}

void glcd_cleardisplay()
{
	unsigned char i,j;
	for(i=0;i<8;i++)
	{
		glcd_setpage(i);
		for(j=0;j<128;j++)
		{
			glcd_setcolumn(j);
			glcd_data(0x00);
		}
	}
}

void glcd_joystickdata(unsigned char page,unsigned char column)
{
	unsigned char d;
	glcd_setpage(page+1);
/*	for(d=column-4;d<column;d++)
	{
		glcd_setcolumn(d);
		glcd_data(0xFF);
	}
	for(d=column;d<(column+4);d++)
	{
		glcd_setcolumn(d);
		glcd_data(0xFF);
	}*/
	SysCtlDelay(670);
	glcd_setpage(page);
	for(d=column-4;d<column;d++)
	{
		glcd_setcolumn(d);
		glcd_data(0xFF);
	}
	for(d=column;d<(column+4);d++)
	{
		glcd_setcolumn(d);
		glcd_data(0xFF);
	}
}

int main(void)
{
    uint8_t i=0;
	uint32_t ui32ADC0Value;
	uint32_t ui32ADC1Value;

	/* Set up the System Clock */
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0);
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_1);


   glcd_init();
    SysCtlDelay(6700);
    glcd_cleardisplay();
    SysCtlDelay(9000);
    glcd_setpage(5);
    for(i=60;i<68;i++)
    {
    	glcd_setcolumn(i);
    	glcd_data(0xFF);
    }
    SysCtlDelay(6700);
    glcd_cleardisplay();


    SysCtlDelay(67000);

	ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC1_BASE,3,0,ADC_CTL_CH7|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC1_BASE, 3);
	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_CH6|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 3);



	while(1)
	{
		ADCIntClear(ADC1_BASE, 3);
		ADCProcessorTrigger(ADC1_BASE, 3);
		while(!ADCIntStatus(ADC1_BASE, 3, false))
		{
		}
		ADCIntClear(ADC1_BASE, 3);
		ADCSequenceDataGet(ADC1_BASE, 3,&ui32ADC0Value);
		y =  ui32ADC0Value/32;
		ADCIntClear(ADC0_BASE, 3);

		ADCProcessorTrigger(ADC0_BASE, 3);
		while(!ADCIntStatus(ADC0_BASE, 3, false))
		{
		}
		ADCIntClear(ADC0_BASE, 3);
		ADCSequenceDataGet(ADC0_BASE, 3,&ui32ADC1Value);
		x =  ui32ADC1Value/510;
		SysCtlDelay(670);

		if(temp1!=x || temp2!=y || ((temp1!=x)&&(temp2!=y)))
		{
			glcd_cleardisplay();
		}
		glcd_joystickdata(x,y);
		temp1=x;
		temp2=y;
		SysCtlDelay(100);

	}
}
