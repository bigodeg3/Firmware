#include "rtc.h"

void configDS1307 (void)
{
    //CloseI2C();               // close i2c if was operating earlier
    OpenI2C(MASTER,SLEW_OFF);	// I2C MODULE MASTER MODE / 100KHz
    SSPADD= 49;             	// clock i2c
    TRISBbits.TRISB2 = 0;       // vcc rtc
    LATBbits.LATB2 = 1;         // vcc rtc
    Delay10KTCYx(50);
}

void rtcInit(void)
{
    StartI2C();
    WriteI2C(0xD0);
    WriteI2C(0x00);
    WriteI2C(0x80);    //CH = 1 Stop oscillator
    WriteI2C(0x48);    //Minute
    WriteI2C(0x16);    //Hour
    WriteI2C(0x03);    //Tuesday
    WriteI2C(0x14);    //14
    WriteI2C(0x03);    //March
    WriteI2C(0x17);    //2017
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

void get_time(unsigned char time[][6], int sensor_count)
{
    time[sensor_count][0] = rtc_get_time(0x00);     //get second
    time[sensor_count][1] = rtc_get_time(0x01);     //get minute
    time[sensor_count][2] = rtc_get_time(0x02);     //get hour
    time[sensor_count][3] = rtc_get_time(0x04);     //get day
    time[sensor_count][4] = rtc_get_time(0x05);     //get month
    time[sensor_count][5] = rtc_get_time(0x06);     //get year
}