/*
* To interface 16X2 LCD in 8 bit mode.
*/

/* Include all the header files for GPIO functions */
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



/* void lcdinit()
* This function initializes the LCD.
* Always call this function at the beginning of main program after configuring the port pins.
*/
void lcdinit()
{
    /* Select LCD in 8 bit mode */
    lcdcmd(0x38);
    SysCtlDelay(20);

    /* Clear screen */
    lcdcmd(0x01);
    SysCtlDelay(67000);

    /* Increment cursor */
    lcdcmd(0x06);
    SysCtlDelay(67000);
    lcdcmd(0x0E);

    /* Take cursor to first block */
    lcdcmd(0x80);

}



/* void lcdwrite()
* This function is used for writing string to the lcd
*/
void lcdwrite(unsigned char *value)
{

while(*value)
{
	lcddata(*value++);
}
}

/* void lcdcmd()
* This function is used to send command to lcd
* RS is 0 as we use command register
*/
void lcdcmd(unsigned char cmd)
{

        GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_0 | GPIO_PIN_1| GPIO_PIN_2 | GPIO_PIN_3,cmd);//sending higher 4 bits
        GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7,cmd);//sending higher 4 bits
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x00);                //RS=0 --- Command Input
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x01);                //enable high to low
        SysCtlDelay(6700);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x00);
}

/* void lcddata()
* This function is used to send data to lcd
* RS is 1 as we use data register
*/
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
    /* Enable all the peripherals */
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    /* Unlock pin PF0 */
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;

    /* Configure all pins of LCD as output */
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIODirModeSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_DIR_MODE_OUT);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,GPIO_PIN_6);

    SysCtlDelay(62500);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x00);
    lcdinit();

    SysCtlDelay(8333);
    {
    /* Writing string to LCD  */
    lcdwrite("Hello");
    SysCtlDelay(67000);
    }
}



