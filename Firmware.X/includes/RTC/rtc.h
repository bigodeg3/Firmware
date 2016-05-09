/* 
 * File:   rtc.h
 * Author: Danilo
 *
 * Created on 14 de Setembro de 2015, 19:10
 */

#ifndef RTC_H
#define	RTC_H

#define __PACKED

#include <i2c.h>
#include <delays.h>

void configDS1307 (void);
void rtcInit(void);
unsigned char rtc_get_time (unsigned char);
void get_time(unsigned char time[][7], int);

#endif	/* RTC_H */

