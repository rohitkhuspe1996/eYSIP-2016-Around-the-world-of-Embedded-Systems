#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "stdio.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"


 uint32_t b;

unsigned char command1;
unsigned char command2;
unsigned char bus_8;
unsigned char  str;
unsigned char sent[50];
uint32_t sw1=1;   // Variable to read the status of switch 1
uint8_t status=0;  //Variable to select the color to be switched on of RGB
uint32_t a;      //Variable to monitor the switch press
uint8_t flag=0;  //To detect key Press
uint8_t result;  // To store the value of flag
uint8_t internal=1;
//uint8_t external=0;
float y=0;//stores adc1 data
//uint32_t
uint8_t char1; //used to store converted integer
float x=0;//stores adc0 data
uint32_t xdata[4];//storing data array of character and printing on lcd
uint32_t temp1;//temporary storing value
float conv1;//used to store digital data
float conv2;

void lcd_set_4bit()
{
    SysCtlDelay(62500);

    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x00);                //RS=0 --- Command Input
                                          //RW=0 --- Writing to LCD
    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7,0x30);                //Sending 3
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x80);                //Set Enable Pin
    SysCtlDelay(67000);                    //Delay
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x00);                //Clear Enable Pin

    SysCtlDelay(67000/4);

    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x00);                //RS=0 --- Command Input
                                                                            //RW=0 --- Writing to LCD
    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7,0x30);                //Sending 3
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x80);                //Set Enable Pin
    SysCtlDelay(67000);                    //Delay
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x00);                //Clear Enable Pin

    SysCtlDelay(67000/4);

    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x00);                //RS=0 --- Command Input
                                                                                //RW=0 --- Writing to LCD
        GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7,0x30);                //Sending 3
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x80);                //Set Enable Pin
        SysCtlDelay(67000);                    //Delay
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x00);                //Clear Enable Pin

    SysCtlDelay(1);

    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x00);                //RS=0 --- Command Input
                                                                                    //RW=0 --- Writing to LCD
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7,0x30);                //Sending 3
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x80);                //Set Enable Pin
            SysCtlDelay(67000);                    //Delay
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x00);                //Clear Enable Pin


}

//lcdinit()-function has initialize command for lcd

void lcdinit()
{
    lcd_set_4bit();
    SysCtlDelay(67000);
    lcdcmd(0x33);   //initialize command
    lcdcmd(0x32);
    lcdcmd(0x28);            //LCD 4-bit mode and 2 lines.
    lcdcmd(0x01);			//clear screen
    SysCtlDelay(6700000);	//delay
    lcdcmd(0x06);			//shift cursor right
//    lcdcmd(0x0E);
    lcdcmd(0x80);			//force cursorto first line

}



/*lcdwrite():this function is used for writting the complete sting to the lcd

*/
void lcdwrite(unsigned char *value)
{
    unsigned char i=0;
    while(value[i]!='\0')      //condition for end of string
    {
        lcddata(value[i]);		//calling lcddata() function to print a character
        i++;
    }
}

//lcdcmd() is used to send command to lcd

void lcdcmd(unsigned char cmd)
{
    unsigned char temp;							//variable used for masking and storing value
    unsigned char lcd_port;
        temp = cmd;
        temp = temp & 0xF0;
        lcd_port &= 0x0F;
        lcd_port |= temp;
        GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7,lcd_port);//sending higher 4 bit

        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x00);                //RS=0 --- Command Input

        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x80);                //enable=0 --- Command Input
        SysCtlDelay(67000);
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x00);


        cmd = cmd & 0x0F;
        cmd = cmd<<4;
        lcd_port &= 0x0F;
        lcd_port |= cmd;
        GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7,lcd_port);//sending lower byte

                GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x00);                //RS=0 --- Command Input

                GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x80);                //enable=0 --- Command Input
                SysCtlDelay(67000);
                GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x00);

}

//lcddata(): this function is used to send data to the lcd
void lcddata(unsigned char cmd)
{
        unsigned char temp;      //variables used for sending higher and lower four bit
        unsigned char lcd_port;
            temp = cmd;
            temp = temp & 0xF0;
            lcd_port &= 0x0F;
            lcd_port |= temp;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7,lcd_port); //sending higher 4 bit

            //GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x00);                //RS=0 --- Command Input

            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0xC0);                //enable=0 --- Command Input
            SysCtlDelay(2);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x40);
//            SysCtlDelay(45);

            cmd = cmd & 0x0F;
            cmd = cmd<<4;
            lcd_port &= 0x0F;
            lcd_port |= cmd;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7,lcd_port);//sending lower 4 bit

                    //GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x00);                //RS=0 --- Command Input

                    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0xC0);                //enable=0 --- Command Input
                    SysCtlDelay(2);
                    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x40);
                    SysCtlDelay(67000);
}


/* convert(): this function is used to convert integer into character */
char convert(uint32_t x){
    char convert;
    switch(x){
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
    return convert; //returning character corresponding to the integer
}

volatile uint32_t conv3;  //storing value of adc digital code
uint32_t ui32ADC0Value[4];  //array to store adc0 value
volatile uint32_t ui32TempAvg;//storing value of adc digital code
volatile float ui32TempValueC; //storing celsius value converted from internal chip
volatile uint32_t ui32TempAvg1;  //storing value of adc1 digital code
int main(void)
{
    uint32_t ui32ADC1Value[4];//array for storing adc1 value
    uint32_t ui32Period;  //variable used in adc

    //------------------- Stellaris Launchpad System Initilization------------------//
    //SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL| SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); //enabaling the paripheral of the board,adc0,adc1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);

    ADCSequenceConfigure(ADC1_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);//configuring adc1
    ADCSequenceStepConfigure(ADC1_BASE, 1, 0, ADC_CTL_CH1);     //ch1:pe2 pin is used for conversion
    ADCSequenceStepConfigure(ADC1_BASE, 1, 1, ADC_CTL_CH1);
    ADCSequenceStepConfigure(ADC1_BASE, 1, 2, ADC_CTL_CH1);
    ADCSequenceStepConfigure(ADC1_BASE,1,3,ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceEnable(ADC1_BASE, 1);     //start adc conversion


    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR,0);//configuring ad0
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS);     //on chip internal temp is used
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 1);

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4);                            //initializing port as a input
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //config. pin 4 of port F
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1| GPIO_PIN_2 | GPIO_PIN_3|GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7);

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

    SysCtlDelay(62500);//delay for power on the lcd

    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0x00);//rs=0;rw=0

    lcdinit();  //initializing lcd command for lcd

    SysCtlDelay(8333);  //delay
    lcdwrite("Voltage=");//writing lcd string

    TimerEnable(TIMER0_BASE, TIMER_A);//timer enable

        while(1)
    {



        switch(internal)
        {
        case 2:    //the following case will be used for internal temprature  conversion
            lcdcmd(0x80);  //force the lcd to first line
            SysCtlDelay(8333);
            lcdwrite("voltage=");  //printing string

            temp1=((conv3*3.3)/4095)*1000;  //converting the digital value into the analog voltage

            	//syntax given below converts 4 digit into seprate character and print them on lcd
                      xdata[0]=temp1/1000;       //seprating 4 digit and storing them in array
                      xdata[1]=((temp1%1000)/100);
                      xdata[2]=((temp1%1000)%100)/10;
                      xdata[3]=temp1%10;
                     char1=convert(xdata[0]);//convert function converts integer into character
                     lcddata(char1);      //printing character on the lcd screen
                     lcddata('.');
                     char1=convert(xdata[1]);
                     lcddata(char1);

                     char1=convert(xdata[2]);
                     lcddata(char1);
                     char1=convert(xdata[3]);
                     lcddata(char1);

                     lcddata('V');

            lcdcmd(0xC0);     //bringing lcd to second line beganing
            SysCtlDelay(8333);
            lcdwrite("int-");   //writing the string
            ADCIntClear(ADC0_BASE, 1);       //clear adc interrupt
                                    ADCProcessorTrigger(ADC0_BASE, 1);//trigger adc conversion process
                                    while(!ADCIntStatus(ADC0_BASE, 1, false))//wait untill interrupt is not produced
                                    {
                                    }
            ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);//getting adc data
            ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;
            conv3=ui32TempAvg;
            y = (1475 - ((2475 * ui32TempAvg)) / 4096)/10;  //converting data into celcius
            /* the following syntax will convert 4 digit integer into seprate character */
            y=y*100;
            temp1=y;
            //the following syntax will convert 4 digit integer into seprate character and printing
             xdata[0]=temp1/1000;
             xdata[1]=((temp1%1000)/100);
             xdata[2]=((temp1%1000)%100)/10;
             xdata[3]=temp1%10;
             char1=convert(xdata[0]);
             lcddata(char1);

             char1=convert(xdata[1]);
             lcddata(char1);
             lcddata('.');
             char1=convert(xdata[2]);
             lcddata(char1);
             char1=convert(xdata[3]);
             lcddata(char1);
             lcddata(0xDF);
             lcdwrite("C");

SysCtlDelay(6700000);


            break;

        case 1:            //following case for external temprature conversion
            lcdcmd(0x80);  //force lcd to first line
            SysCtlDelay(8333); //delay
            lcdwrite("voltage=");//printing string


            conv1=((conv2*3.3)/4095);//conversion of digital value to analog
            //the following syntax will convert 4 digit into seprate character and printing on lcd screen
            conv1=conv1*1000;
            temp1=conv1;
                       //temp1=y;
            xdata[0]=temp1/1000;
            xdata[1]=((temp1%1000)/100);
            xdata[2]=((temp1%1000)%100)/10;
            xdata[3]=temp1%10;
            char1=convert(xdata[0]);
            lcddata(char1);
            lcddata('.');
            char1=convert(xdata[1]);
            lcddata(char1);

            char1=convert(xdata[2]);
            lcddata(char1);
            char1=convert(xdata[3]);
            lcddata(char1);
            lcddata('V');

            lcdcmd(0xC0);//forcing lcd to second line
            SysCtlDelay(8333);  //delay
            lcdwrite("ext-");
            ADCIntClear(ADC1_BASE, 1);//clear adc interrupt
            ADCProcessorTrigger(ADC1_BASE, 1);//trigger adc processor
            while(!ADCIntStatus(ADC1_BASE, 1, false))//wait untill interrupt occurs
            {
            }
            ADCSequenceDataGet(ADC1_BASE, 1, ui32ADC1Value);//get adc data
            x =  (ui32ADC1Value[0] + ui32ADC1Value[1] + ui32ADC1Value[2] + ui32ADC1Value[3] + 2)/4;
            conv2=x;
            x=x/12.41;//converting adc data into temprature
            //	the following syntax will convert 4 digit into seprate character and printing on screen
            x=x*100;
            temp1=x;
            xdata[0]=temp1/1000;
            xdata[1]=((temp1%1000)/100);
            xdata[2]=((temp1%1000)%100)/10;
            xdata[3]=temp1%10;
            //        if(b<1000){
            //        b++;
            char1=convert(xdata[0]);
            lcddata(char1);
            char1=convert(xdata[1]);
            lcddata(char1);
            lcddata('.');
            char1=convert(xdata[2]);
            lcddata(char1);
            char1=convert(xdata[3]);
            lcddata(char1);
            lcddata(0xDF);
            lcdwrite("C");
SysCtlDelay(670000);
                            break;


        }


    }

}

/*when switch is pressed the temprature shifts from external to internal and viceversa  */
/*when the interrupt occurs the following function is called*/

void Timer0IntHandler(void)
{

TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
sw1=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4);        //reading the data from input port sw1
if((sw1 & GPIO_PIN_4)==0 ){               //condition for switch pressed
    a++;
    if(a==2){
        if(internal==2){
            internal=1;
        }
        else{
            internal=2;
        }
    }
}
else{
    a=0;
}

}
