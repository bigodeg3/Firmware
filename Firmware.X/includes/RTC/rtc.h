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

void hardware_init (void);

void Initialize_DS1307(void);

unsigned char rtc_get_time (unsigned char);

#endif	/* RTC_H */

