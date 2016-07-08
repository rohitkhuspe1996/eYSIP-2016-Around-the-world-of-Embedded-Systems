/*
* In this program 8 images are displayed on GLCD sequentially.
*/


/* Include header files for adc and GPIO functions */
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

/* These header files contain the hex values of images which are stored in an unsigned char array.
* The name of the header file corresponds the the array name in that file.
* Eg. In one.h array is unsigned char one[]={}
      In two.h array is unsigned chaar two[]={}
      and so on till eight.h
*/


#include "driverlib/one.h"
#include "driverlib/two.h"
#include "driverlib/three.h"
#include "driverlib/four.h"
#include "driverlib/five.h"
#include "driverlib/six.h"
#include "driverlib/seven.h"
#include "driverlib/eight.h"




/* void glcd_cmd(cmd)
* This function sends commands to the GLCD.
* Value of RS is 0
* Command is written on data lines.
* Enable is made 1 for a short duration.
*/
void glcd_cmd(unsigned char cmd)
{
	/* Clear data lines */
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,0x00);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,0x00);

	/* RS = 0 */
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6,0x00);

	/* Put command on data lines */
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,cmd);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,cmd);

	/* Generate a high to low pulse on enable */
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x01);
	SysCtlDelay(134);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x00);

}



/* void glcd_data(data)
* This function sends data to the GLCD.
* Value of RS is 1
* Data is written on data lines.
* Enable is made 1 for a short duration.
*/
void glcd_data(unsigned char data)
{
	/*Clear the data lines */

	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,0x00);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,0x00);

	/* RS = 1 */
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6,0x40);

	/* Put data on data lines */
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,data);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,data);

	/* Generate a high to low pulse on enable */
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x01);
	SysCtlDelay(134);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x00);
}




/* void glcd_init()
* This function initializes the GLCD.
* Always call this function at the beginning of main program after configuring the port pins.
*/
void glcd_init()
{
	SysCtlDelay(134000);
	/* Clear RST */
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x00);
	SysCtlDelay(13400);

	/* Set RST */
	GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,0x20);

	/* Initialize left side of GLCD */
	/* Set CS1 (CS1=1 and CS2=0) */
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x00);

	/* Select the start line */
	glcd_cmd(0xC0);
	/* Send the page */
	glcd_cmd(0xB8);
	/*Send the column */
	glcd_cmd(0x40);
	/* Send glcd on command */
	glcd_cmd(0x3F);

	/* Initialize the right side of GLCD */
	/* Set CS2 (CS2=1 and CS1=0) */
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x08);

	/* Select the start line */
	glcd_cmd(0xC0);
	/* Send the page */
	glcd_cmd(0xB8);
	/*Send the column */
	glcd_cmd(0x40);
	/* Send glcd on command */
	glcd_cmd(0x3F);
}



/* void glcd_setpage(page)
* This function selects page number on GLCD.
* Depending on the value of column number CS1 or CS2 is made high.
*/
void glcd_setpage(unsigned char page)
{
	/* Set CS1 (CS1=1 and CS2=0) The right side is selected(column>64) */
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x00);

	/*Select the page */
    glcd_cmd(0xB8 | page);
  //  SysCtlDelay(100);

	/* Set CS2 (CS2=1 and CS1=0) The right side is selected(column>64) */
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x08);

	/*Select the page */
    glcd_cmd(0xB8 | page);
//	SysCtlDelay(100);
}



/* void glcd_setcolumn(column)
* This function selects column number on GLCD.
* Depending on the value of column number CS1 or CS2 is made high.
*/
void glcd_setcolumn(unsigned char column)
{
	if(column < 64)
	{
		/* Set CS1 (CS1=1 and CS2=0) The right side is selected(column>64) */
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x00);

		/*Select column on left side */
		glcd_cmd(0x40 | column);
	//	SysCtlDelay(6700);
	}
	else
	{
		/* Set CS2 (CS2=1 and CS1=0) The right side is selected(column>64) */
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x08);

		/*Select column on right side */
		glcd_cmd(0x40 | (column-64));
	//	SysCtlDelay(6700);
	}

}



/* void glcd_cleardisplay()
* This function clears the data on GLCD by writing 0 on all pixels.
*/
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


int main()
{
	int i,j,p;

	/* Enable all the peripherals */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	/* Unlock pin PF0 */
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;

    /* Configure Enable pin as output */
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIODirModeSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_DIR_MODE_OUT);

    /* Configure PE5 (RST), PE0 to PE3 (D0 to D3) and PB4 to PB7(D4 to D7) as output pins */
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7);

    /* Configure RS as output */
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,GPIO_PIN_6);

    /* Configure CS1 or CS2 as output */
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,GPIO_PIN_3);

    glcd_init();
    SysCtlDelay(6700);
    glcd_cleardisplay();
    SysCtlDelay(9);
    while(1)
    {
    j=0;
    p=0;
    while(p<8)
    {
    	/* Set the page */
	   glcd_setpage(p);

	   for(i=0;i<128;i++)
	   {
		   /*Select columns from 0 to 127 */
		   glcd_setcolumn(i);

		   /*Send the hex values to GLCD */
		   glcd_data(one[j]);
		   j++;
	   }

	   /*Increment the page number after previous page is filled */
	   p++;

    }
    SysCtlDelay(400000);
    glcd_cleardisplay();
       // SysCtlDelay(10000);

        j=0;
        p=0;
        while(p<8)
        {
        	/* Set the page */
    	   glcd_setpage(p);

    	   for(i=0;i<128;i++)
    	   {
    		   /*Select columns from 0 to 127 */
    		   glcd_setcolumn(i);

    		   /*Send the hex values to GLCD */
    		   glcd_data(two[j]);
    		   j++;
    	   }

    	   /*Increment the page number after previous page is filled */
    	   p++;

        }

        SysCtlDelay(400000);
        glcd_cleardisplay();
       //   SysCtlDelay(10000);

            j=0;
            p=0;
            while(p<8)
            {
            	/* Set the page */
        	   glcd_setpage(p);

        	   for(i=0;i<128;i++)
        	   {
        		   /*Select columns from 0 to 127 */
        		   glcd_setcolumn(i);

        		   /*Send the hex values to GLCD */
        		   glcd_data(three[j]);
        		   j++;
        	   }

        	   /*Increment the page number after previous page is filled */
        	   p++;

            }
            SysCtlDelay(400000);
                glcd_cleardisplay();
                   // SysCtlDelay(10000);
            j=0;
            p=0;
            while(p<8)
            {
                /* Set the page */
                    glcd_setpage(p);

                    for(i=0;i<128;i++)
                    {
                        /*Select columns from 0 to 127 */
                		   glcd_setcolumn(i);

                        /*Send the hex values to GLCD */
                            glcd_data(four[j]);
                		    j++;
                	   }

                    /*Increment the page number after previous page is filled */
                    p++;

            }
            SysCtlDelay(400000);
            glcd_cleardisplay();
            // SysCtlDelay(10000);

            j=0;
            p=0;
            while(p<8)
            {
                /* Set the page */
                glcd_setpage(p);

                for(i=0;i<128;i++)
                {
                    /*Select columns from 0 to 127 */
                    glcd_setcolumn(i);

                    /*Send the hex values to GLCD */
                    glcd_data(five[j]);
                    j++;
                }

                /*Increment the page number after previous page is filled */
                p++;

            }
            SysCtlDelay(400000);
            glcd_cleardisplay();
            // SysCtlDelay(10000);

            j=0;
            p=0;
            while(p<8)
            {
                /* Set the page */
                glcd_setpage(p);

                for(i=0;i<128;i++)
                {
                    /*Select columns from 0 to 127 */
                    glcd_setcolumn(i);

                    /*Send the hex values to GLCD */
                    glcd_data(six[j]);
                    j++;
                }

                /*Increment the page number after previous page is filled */
                p++;

            }
            SysCtlDelay(400000);
            glcd_cleardisplay();
            // SysCtlDelay(10000);

            j=0;
            p=0;
            while(p<8)
            {
                /* Set the page */
                glcd_setpage(p);

                for(i=0;i<128;i++)
                {
                    /*Select columns from 0 to 127 */
                    glcd_setcolumn(i);

                    /*Send the hex values to GLCD */
                    glcd_data(seven[j]);
                    j++;
                }

                /*Increment the page number after previous page is filled */
                p++;

            }
            SysCtlDelay(400000);
            glcd_cleardisplay();
            // SysCtlDelay(10000);

            j=0;
            p=0;
            while(p<8)
            {
                /* Set the page */
                glcd_setpage(p);

                for(i=0;i<128;i++)
                {
                    /*Select columns from 0 to 127 */
                    glcd_setcolumn(i);

                    /*Send the hex values to GLCD */
                    glcd_data(eight[j]);
                    j++;
            }

            /*Increment the page number after previous page is filled */
            p++;

            }
}
return 0;
}
