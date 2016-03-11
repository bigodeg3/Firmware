/* 
 * File:   rtc.h
 * Author: Danilo
 *
 * Created on 14 de Setembro de 2015, 19:10
 */

#ifndef RTC_H
#define	RTC_H

#define __PACKED

//typedef struct __PACKED
//{
//    unsigned char sec, min, hour, day, date, month, year;
//}Time;

void hardware_init (void)
{
    //CloseI2C();    		// close i2c if was operating earlier
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
/*
int get_msg (char* buffer, float temp, unsigned char sec, unsigned char min, unsigned char hour, unsigned char day, unsigned char date, unsigned char month, unsigned char year)
{
    int size, intpart, decpart;
    float decPartAux;

    intpart = temp;                             // Get the integer part
    decPartAux = temp - intpart;                // Get fractional part
    decpart = (int)(decPartAux*10000);          // Turn into integer
    
    switch(day)
        {
//            case 1: size = sprintf (buffer,"Horario: Domingo, %02u/%02u/20%02u, %02u:%02u:%02u\r\nTemperatura: %d.%04d °C\r\n", time.date, time.month, time.year, time.hour, time.min, time.sec, intpart, decpart);
//                    break;
//            case 2: size = sprintf (buffer,"Horario: Segunda, %02u/%02u/20%02u, %02u:%02u:%02u\r\nTemperatura: %d.%04d °C\r\n", time.date, time.month, time.year, time.hour, time.min, time.sec, intpart, decpart);
//                    break;
//            case 3: size = sprintf (buffer,"Horario: Terca, %02u/%02u/20%02u, %02u:%02u:%02u\r\nTemperatura: %d.%04d °C\r\n", time.date, time.month, time.year, time.hour, time.min, time.sec, intpart, decpart);
//                    break;
//            case 4: size = sprintf (buffer,"Horario: Quarta, %02u/%02u/20%02u, %02u:%02u:%02u\r\nTemperatura: %d.%04d °C\r\n", time.date, time.month, time.year, time.hour, time.min, time.sec, intpart, decpart);
//                    break;
//            case 5: size = sprintf (buffer,"Horario: Quinta, %02u/%02u/20%02u, %02u:%02u:%02u\r\nTemperatura: %d.%04d °C\r\n", time.date, time.month, time.year, time.hour, time.min, time.sec, intpart, decpart);
//                    break;
//            case 6: size = sprintf (buffer,"Horario: Sexta, %02u/%02u/20%02u, %02u:%02u:%02u\r\nTemperatura: %d.%04d °C\r\n", time.date, time.month, time.year, time.hour, time.min, time.sec, intpart, decpart);
//                    break;
//            case 7: size = sprintf (buffer,"Horario: Sabado, %02u/%02u/20%02u, %02u:%02u:%02u\r\nTemperatura: %d.%04d °C\r\n", time.date, time.month, time.year, time.hour, time.min, time.sec, intpart, decpart);
//                    break;
        }
    return size;
}
*/
#endif	/* RTC_H */

