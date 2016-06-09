/***LAB 3 PART 1 ***/
/*
In this lab you have to design RGB LED controller using SW1 and SW2 present in
Launchpad board.RGB LED controller has two modes of operation. Auto mode and Manual
mode At initial, when program is loaded controller will be in Auto mode. Combination of
SW1 an SW2 has to be pressed to go to Manual mode.When Reset button is pressed,
controller will go to Auto mode.
1. Auto mode
• In Auto mode color of the RGB LED follows a pattern in a cycle.
• The pattern must follow the color circle as shown in Figure 1.
• In Auto mode SW1 will increase the speed of color transition and SW2 will
decrease the speed.
2. Manual mode
• In Manual mode, user must be able to select any one of the color from the color
circle. For this intensity of any of the 3 LEDs must be controlled independently.
• Mode 1 (Red LED control) - When SW2 is pressed continuously(long press) and
SW1 is pressed once controller goes to Manual Mode 1. In this mode, intensity
of Red LED can be controlled using SW1 and SW2.
• Mode 2 (Blue LED control) - When SW2 is pressed continuously(long press) and
SW1 is pressed twice controller goes to Manual Mode 2. In this mode, intensity
of Blue LED can be controlled using SW1 and SW2.
• Mode 3 (Green LED control) - When SW1 and SW2 are pressed continuously
controller goes to Manual Mode 3. In this mode, intensity of Green LED can be
controlled using SW1 and SW2.
*/

#include "driverlib/pin_map.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c123gh6pm.h"

uint8_t flag1=0;
uint8_t flag2=0;
uint32_t a;
uint32_t b;
uint8_t mode=0;
uint8_t flag=0;
uint32_t pwm2;
uint32_t cnt=1;
uint32_t x=8;
uint32_t check1;
uint32_t check2;
uint32_t ui32Period;
uint8_t sw1=0;
uint8_t sw2=0;

/*intensity():this function is used for controllling the intensity of the led when it is in manual mode*/


void intensity(uint8_t color){
if(color==7){							//when color green is selected
	PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
		PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_6_BIT, false);  //off the pwm pin 5,6
		   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,0);					  //on the pwm pin 7

		   // Turn on the Output pins
		//  PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);


		   cnt=20;     //initial pwm width
		   while(1)
		   {
		   	SysCtlDelay( (SysCtlClockGet()/(1000*3))*20 ) ;    //delay
		       mode=2;											  //change the mode to change the intensity of the light
		       PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,cnt);		  //set pwm width
		   }
}

else if(color==5){						//select color red

	PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);			//on th pwm pin for red color
	PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT | PWM_OUT_7_BIT, false);  //off the pwm pin for green,blue
	   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,0);

	   // Turn on the Output pins
	//  PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);


	   cnt=2;				//initialize  the pulse width
	   while(1)
	   {
	       SysCtlDelay( (SysCtlClockGet()/(1000*3))*20 ) ;  //delay
	       mode=2;											//change mode
	       PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,cnt);		//set pulse width
	   }

}
	   else{
	   	PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);		//on the pwm pin for blue
	   	PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_7_BIT, false);  //on the pwm pin for green,red
	   		   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,0);

	   		   // Turn on the Output pins
	   		//  PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);


	   		   cnt=2;			//set the initial pulse width
	   		   while(1)
	   		   {

	   		   	SysCtlDelay( (SysCtlClockGet()/(1000*3))*20 ) ;
	   		   	mode=2;			//change mode
	   		   	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,cnt);  //change pulse width
	   }
}


}


void select(){
	if((GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0) & GPIO_PIN_0)!=0){  //condition for green color
		intensity(7);   //calling function for intensity
	}

	else if(sw1==1){         //condition for red color
		intensity(5);
	}
	else{
		intensity(6);     //condition for green color
	}
}




int main(void)
{
    //Set the clock
   SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC |   SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

   //Configure PWM Clock to match system
   SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

   // Enable the peripherals used by this program.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);  //The Tiva Launchpad has two modules (0 and 1). Module 1 covers the LED pins

    //unlocking the sw2 switch
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);                                     // make F0 an input
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);  //config pin f0
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);                                     // make F4 an input
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);  //config pin f4

    //Configure PF1,PF2,PF3 Pins as PWM
    GPIOPinConfigure(GPIO_PF1_M1PWM5);
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    //Configure PWM Options
    //PWM_GEN_2 Covers M1PWM4 and M1PWM5
    //PWM_GEN_3 Covers M1PWM6 and M1PWM7 See page 207 4/11/13 DriverLib doc
    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    //Set the Period (expressed in clock ticks)
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, 120);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 120);

    //Set PWM duty-50% (Period /2)
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,60);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,60);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,60);

    // Enable the PWM generator
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);

    // Turn on the Output pins
   PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);



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

    cnt=1;
    while(1)
    {
    	if(mode==1){    //mode=1 - it is in manaul made
    		SysCtlDelay(6700000) ;  //delay for selecting the color
    		select();			//calling the select function for selecting the color
    	}

    	else{                //auto mode

        SysCtlDelay( (SysCtlClockGet()/(266*x))*20 ) ;   //delay

        /*From the color wheel for the movement in degrees the state of RGB will be
         * In 0 to 60 degrees
         * Red-On , Blue-Off and Green turns on till 60 degrees
         *
         * In 60 to 120 degrees
         * Green-On, Blue-Off and Red starts glowing from 60 and turns on as it reachs 120 degrees
         *
         * In 120 to 180 degrees
         * Green-On, Red-off and blue turns on from 120 to 180 degrees.
         *
         * In 180 to 240 degrees
         * blue-on, red-off and greeb turns on from 180 to 240 degrees.
         *
         * In 240 to 300 degrees
         * blue-n,green-off and red turns on from 240 to 300 degrees.
         *
         * In 300 to 360 degrees
         * red-on,green-off and blue turns on from 300 to 360 degrees.
         */

    if(cnt<=60)
    {
         PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_7_BIT, true);
         PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, false);
         PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,60);
         PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,cnt);
         cnt++;
    }
    else if(cnt >60 && cnt<120)
    {
        PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_7_BIT, true);
        PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, false);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,(120-cnt));
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,60);
        cnt++;
    }
    else if(cnt>=120 && cnt<180)
    {
        PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);
        PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, false);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,(cnt-119));
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,60);
        cnt++;
    }
    else if(cnt>=180 && cnt<240)
    {
        PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);
        PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, false);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,60);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,(240-cnt));
        cnt++;
    }
    else if(cnt>=240 && cnt<300)
    {
        PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_6_BIT, true);
        PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, false);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,(cnt-239));
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,60);
        cnt++;
    }
    else if(cnt>=300 && cnt<=359)
    {
        PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_6_BIT, true);
        PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, false);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,60);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,(360-cnt));
        cnt++;
    }
    else
    {
        cnt=1;
    }
    }
    }


return 0;
}


void Timer0IntHandler(void)
{
TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);     //clear the timer
if((GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0) & GPIO_PIN_0)==0){   //condition for sw2
	check1++;
	if(mode==1){	//when in manual mode monitor the sw2 switch
	a++;
	if(a==2){
		sw2++;
	}
	}



	if(check1==2){  //it reduces the speed
		x=x-4;
	}

	if((GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4) & GPIO_PIN_4)==0){ //condition for sw1 switch
		mode=1;        //change to manual mode
	}
	if(mode==2){         //in mode2 sw2 is pressed then increase the intensity of the light
		if(flag1==0){
		flag1=1;
		cnt=cnt+5;
		if(cnt>60){
			cnt=60;
		}
	}
	}

}
else{
	check1=0;
	a=0;
	flag1=0;
}

if((GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4) & GPIO_PIN_4)==0){ //condition for sw1
	check2++;
	if(check2==2){   //increase the speed in the auto mode
		x=x+4;
	}

	if(mode==1){   //monitoring the sw1 in mode 1
		b++;
		if(b==2){
			sw1++;
		}
	}

	if(mode==2){    // decrease the intensity when in mode
		if(flag2==0){
		flag2=1;
		cnt=cnt-5;   //decrease the pulse width of the pwm
		if(cnt<5){
		cnt=5;
		}

	}
	}
}
else{
	flag2=0;  //reset the variable on release of switch
	check2=0;
	b=0;
}

}

