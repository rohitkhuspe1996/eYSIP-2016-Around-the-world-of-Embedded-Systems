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
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/eeprom.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c123gh6pm.h"



volatile uint32_t z;	      //variable containing white line data on right side	

volatile uint32_t x;          //  variable which holds the digital value after analog to digital conversion of middle white line sensor.
volatile uint32_t y;          //  variable which holds the digital value after analog to digital conversion of left white line sensor.



void forward(){
	ui8LED=9;
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0| GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,ui8LED);
	
}
void back(){
	ui8LED=6;
    GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0| GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,ui8LED);
   
}

void right(){
	ui8LED=5;
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0| GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,ui8LED);

}
void left(){
	ui8LED=10;
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0| GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,ui8LED);
	
}
void stop(){
	ui8LED=0;
        GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0| GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,ui8LED);
		

}
uint32_t pui32Data[2];
uint32_t pui32Read[2];
uint8_t flag=0;
uint8_t led1;
uint32_t ui32Period;
int main(void)
{
	pui32Data[0]=200;
	pui32Data[1]=200;

	SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);//initialize and enable eeprom
         EEPROMInit();
	
	EEPROMRead(pui32Read, 0x100, sizeof(pui32Read));//read data from eeprom
	
	
	uint32_t ui32ADC0Value[4]; //storing aberage value of the variable
	uint32_t ui32ADC1Value[4];  //storing average value of variable
	uint32_t ui32ADC2Value[4];   //storing average value of variable
	/* Set up the System Clock */
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);  
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);   //enable the portf
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);    //enable the portd
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;	// unlocking sw2 switch
        HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
        HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;

	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //configure pin as a output
	//Configure PWM Clock to match system
	   SysCtlPWMClockSet(SYSCTL_PWMDIV_1);  //pwm pin


	/* Enable ADC Peripheral. We use ADC0 and ADC1 for simultaneous conversion of analog values(X znd Y co-ordinates).
	 * They correspond to pins PE2 and PE3.
	 */

	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4);							//initializing port as a input
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);  //The Tiva Launchpad has two modules (0 and 1). Module 1 covers the LED pins
	GPIOPinConfigure(GPIO_PF3_M1PWM7);
	GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_3);

	PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	// Enable the PWM generator
	    PWMGenEnable(PWM1_BASE, PWM_GEN_2);
	    PWMGenEnable(PWM1_BASE, PWM_GEN_3);
	    // Turn on the Output pins
	       PWMOutputState(PWM1_BASE,PWM_OUT_7_BIT, true);
	       PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,120);


	//configure adc1 chip of board

	ADCSequenceConfigure(ADC1_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC1_BASE, 1, 0, ADC_CTL_CH2);
	ADCSequenceStepConfigure(ADC1_BASE, 1, 1, ADC_CTL_CH2);
	ADCSequenceStepConfigure(ADC1_BASE, 1, 2, ADC_CTL_CH2);
	ADCSequenceStepConfigure(ADC1_BASE,1,3,ADC_CTL_CH2|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC1_BASE, 1);

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

	//GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0| GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4,ui8LED);
	forward();
	SysCtlDelay(67000);
	while(1)
	{






		//configure adc ch0
		ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
			ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH0);
			ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH0);
			ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH0);
			ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);
			ADCSequenceEnable(ADC0_BASE, 1);


		ADCIntClear(ADC0_BASE, 1);
		ADCProcessorTrigger(ADC0_BASE, 1);
		while(!ADCIntStatus(ADC0_BASE, 1, false))  //wait untill conversion is completed
		{
		}
		ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC1Value);
	    x =  (ui32ADC1Value[0] + ui32ADC1Value[1] + ui32ADC1Value[2] + ui32ADC1Value[3] + 2)/4;

		//configure adc ch1
	    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
	    	ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH1);
	    	ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH1);
	    	ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH1);
	    	ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END);
	    	ADCSequenceEnable(ADC0_BASE,1);


	    ADCIntClear(ADC0_BASE, 1);           
		ADCProcessorTrigger(ADC0_BASE, 1);              //wait untill conversion is complete
		while(!ADCIntStatus(ADC0_BASE, 1, false))
		{
		}

		ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);
		y =  (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;

		ADCIntClear(ADC1_BASE, 1);
				ADCProcessorTrigger(ADC1_BASE, 1);        //wait untill conversion is complete
				while(!ADCIntStatus(ADC1_BASE, 1, false))
				{
				}

				ADCSequenceDataGet(ADC1_BASE, 1, ui32ADC2Value);
				z =  (ui32ADC2Value[0] + ui32ADC2Value[1] + ui32ADC2Value[2] + ui32ADC2Value[3] + 2)/4;

				
		/*syntax given below will take left and right turn when it miss the white line else it will follow forward path*/

				if(x>pui32Read[0] && y>pui32Read[0]){
					SysCtlDelay(67000);

					right();
				}
				else if(z >pui32Read[0] && y > pui32Read[0]){
					//stop();
					SysCtlDelay(67000);
					left();
				}
				else{
					forward();
				}


	}
}

void Timer0IntHandler(void)
{
				TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);     //clear the timer

				led1=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4);		/* reading the data from input port sw1 */
				if((led1 & GPIO_PIN_4)==0 ){
								

				stop();
				SysCtlDelay(20*6700000);   //wait for some duration
				SysCtlDelay(67000);
				pui32Data[0]=y;            //storing value of sensor
				pui32Data[0]=pui32Data[0]+40;  //maximum possible value for white line
				right();
			
				SysCtlDelay(6700000);
				pui32Data[1]=x;
				ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
	    			ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH1);
	    			ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH1);
	    			ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH1);
	    			ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END);
	    			ADCSequenceEnable(ADC0_BASE,1);


	    			ADCIntClear(ADC0_BASE, 1);           
				ADCProcessorTrigger(ADC0_BASE, 1);              //wait untill conversion is complete
				while(!ADCIntStatus(ADC0_BASE, 1, false))
				{
				}

				ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);
				y =  (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;
				left();
				SysCtlDelay(6700000);    //maximum possible value for black line

				stop();
				EEPROMProgram(pui32Data, 0x100, sizeof(pui32Data));//store the new value in eeprom
				forward();
				}

}

