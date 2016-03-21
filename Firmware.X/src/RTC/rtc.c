#include "rtc.h"

void hardware_init (void)
{
    //CloseI2C();               // close i2c if was operating earlier
    OpenI2C(MASTER,SLEW_OFF);	// I2C MODULE MASTER MODE / 100KHz
    SSPADD= 49;             	// clock i2c
    TRISBbits.TRISB2 = 0;       // vcc rtc
    LATBbits.LATB2 = 1;         // vcc rtc
    Delay10KTCYx(50);
}

void Initialize_DS1307(void)
{
    StartI2C();
    WriteI2C(0xD0);
    WriteI2C(0x00);
    WriteI2C(0x80);    //CH = 1 Stop oscillator
    WriteI2C(0x37);    //Minute
    WriteI2C(0x19);    //Hour
    WriteI2C(0x01);    //Sunday
    WriteI2C(0x27);    //27
    WriteI2C(0x09);    //September
    WriteI2C(0x15);    //2015
    StopI2C();         //Stop the I2C Protocol
    Delay10KTCYx(250);
    Delay10KTCYx(250);
    Delay10KTCYx(250);
    Delay10KTCYx(250);
    //Have to start the Clock again
    StartI2C();
    WriteI2C(0xD0);
    WriteI2C(0x00);
    WriteI2C(0x00);    //start Clock and set the second hand to Zero
    StopI2C();
    Delay10KTCYx(250);
    Delay10KTCYx(250);
    Delay10KTCYx(250);
    Delay10KTCYx(250);
}

unsigned char rtc_get_time (unsigned char address)
{
    unsigned char time;
    StartI2C();
    WriteI2C(0xD0);
    WriteI2C(address);
    RestartI2C();
    WriteI2C(0xD1);
    time = ReadI2C();

    return (((time & 0xF0) >> 4) * 10) + (time & 0x0F); // BCD to decimal
}
