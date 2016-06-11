/*** LAB 3 PART 2 ***/

/*Interface a servo motor and control it using a switch.
1. When switch 1 is pressed the motor should rotate by ten degrees clockwise.
2. When switch 2 is pressed the motor should rotate by ten degrees anti-clockwise.
3. While doing the above two actions check for limits of the servo motor. It should no
move beyond the operating range.
*/

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
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c123gh6pm.h"

uint8_t flag1=0; //Check the status of SW1
uint8_t flag2=0; //Check the status of SW2
uint8_t sw1;   //Read the value of SW1
uint8_t sw2;   //Read the value of SW2
uint32_t degree=400;
uint32_t step;
uint32_t ui32Period;

/* This function is used to rotate the motor by 10 degrees in clockwise or anti-clockwise direction. */
void stepvalue(uint32_t step)
{
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0,step);
    SysCtlDelay(100000);
}


int main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);  //Set the oscillator frequency
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);   //Set the PWM clock.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);  //Enable the PWM1 module.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);  //Enable PORTD
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  //Enable PORTF

	//Configure the pin PD0 as PWM output
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
	GPIOPinConfigure(GPIO_PD0_M1PWM0);
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;


	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);    // make F0 an input
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4);                            //initializing port as a input
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //configure pin 4 of port F


	/*Set the mode of operation of PWM Generator. It is configured in count down mode*/
	PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);

	/*Set the Period of PWM Generator. It is obtained by dividing SysCtlClock value by 64. Divide this value by 50Hz. */
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0,12500);

	/*The maximum value of Servo motor (180 Degrees) is obtained at 2ms. Thus divide the value of period by 10 ms */
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0,1250);

	PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true); //Enable the PWM Output state.
	PWMGenEnable(PWM1_BASE, PWM_GEN_0);  //Enable the PWM Generator

	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0,degree);  //Set the duty cycle of PWM.
	SysCtlDelay(100000);

	//Timer Configuration
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	  TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

	  /*
	   * Calculate the delay and load the value in TimerLoadSet
	   * Delay is calculated using "SysCtlClockGet()". Divide this function by the required frequency. divide the answer by 2 as the duty cycle is %
	   */
	  ui32Period = (SysCtlClockGet() / 100) / 2;
	  TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);


	  //Enable the interrupt in Timer and NVIC
	  IntEnable(INT_TIMER0A);
	  TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	  IntMasterEnable();

	//Enable the Timer
	TimerEnable(TIMER0_BASE, TIMER_A);

	while(1)
	{
	}
}


void Timer0IntHandler(void)
	{
	/* Clear the Timer */
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	sw2=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);
	sw1=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4);

	/*Check whether switch is pressed. If SW1 is pressed then move by 10 degrees anti-clockwise, if SW2 is pressed then move the motor by 10 degrees clockwise. */
	if((sw2 & GPIO_PIN_0)==0){
		flag1++;
		if(flag1==2){
			degree=degree+55;
			if(degree>1400){
				degree=1400;
			}
			stepvalue(degree);
		}
	}

	else{
		flag1=0;
	}

	if((sw1 & GPIO_PIN_4)==0){
		flag2++;
		if(flag2==2){
			degree=degree-55;
			if(degree<400){
				degree=400;
			}
		stepvalue(degree);
		}

	}

	else{
		flag2=0;
	}

}

