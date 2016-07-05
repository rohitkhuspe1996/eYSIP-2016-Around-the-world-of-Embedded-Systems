
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"



//lcdinit()-function has initialize command for lcd

void lcdinit()
{

    lcdcmd(0x38);
    SysCtlDelay(20);//LCD 4-bit mode and 2 lines.
    lcdcmd(0x01);
    //clear screen
    SysCtlDelay(67000);	//delay
    lcdcmd(0x06);			//shift cursor right
    SysCtlDelay(67000);
    lcdcmd(0x0E);
    lcdcmd(0x80);			//force cursorto first line

}



/*lcdwrite():this function is used for writting the complete sting to the lcd
*/
void lcdwrite(unsigned char *value)
{

while(*value)
{
	lcddata(*value++);
}
}

//lcdcmd() is used to send command to lcd

void lcdcmd(unsigned char cmd)
{

        GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_0 | GPIO_PIN_1| GPIO_PIN_2 | GPIO_PIN_3,cmd);//sending higher 4 bits
        GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7,cmd);//sending higher 4 bits
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x00);                //RS=0 --- Command Input
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x01);                //enable high to low
        SysCtlDelay(6700);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x00);



}

//lcddata(): this function is used to send data to the lcd
void lcddata(unsigned char cmd)
{
	 	 	GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_0 | GPIO_PIN_1| GPIO_PIN_2 | GPIO_PIN_3,cmd);//sending higher 4 bits
	        GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7,cmd);//sending higher 4 bits
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6,0x40);                //RS=1, enable high to low.
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x01);                //RS=1, enable high to low.
            SysCtlDelay(2000);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x00);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6,0x00);
}


int main(void)
{


    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;	// unlocking sw2 switch
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIODirModeSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_DIR_MODE_OUT);

                                      // make F0 an input

   GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,GPIO_PIN_6);

    SysCtlDelay(62500);//delay for power on the lcd

    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x00);//rs=0;rw=0
//    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x01);                //RS=1, enable high to low.

    lcdinit();  //initializing lcd command for lcd

    SysCtlDelay(8333);  //delay
//   while(1)
    {
    lcdwrite("Hello");//writing lcd string
    SysCtlDelay(67000);
    }
}

/*when switch is pressed the temprature shifts from external to internal and viceversa  */
/*when the interrupt occurs the following function is called*/


