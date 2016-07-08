#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/flash.h"
#include "driverlib/eeprom.h"
uint32_t pui32Data[3];    //variable for sending data to eeprom
uint32_t pui32Read[3];	  //variable for reading data from eeprom
uint8_t led1;			//to display led
int main(void)
{

SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);				//enable portf
GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);//configure it has output
GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
SysCtlDelay(20000000);

GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x02);
SysCtlDelay(20000000);
SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);              //reading data from eeprom
EEPROMInit();											    //eeprom initialization
GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,pui32Read[0]);
				pui32Data[0] =4;
				pui32Data[1] =4;
				pui32Data[2]=4;


EEPROMProgram(pui32Data, 0x100, sizeof(pui32Data));    //writing data to eeprom
while(1)
{
}


}
