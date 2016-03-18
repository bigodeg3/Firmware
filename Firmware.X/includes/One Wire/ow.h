/* You can use this library for interfacing the DS18S20
 * NOTE: This library works for 48MHz Frequency i.e. TCY = 0.08333us
 */

#ifndef __OW_H
#define __OW_H

#include <p18f4550.h>
#include <delays.h>

#define OUTPUT  0
#define INPUT   1
#define HIGH    1
#define LOW     0

#define OW_LAT  LATBbits.LATB3
#define OW_PIN  PORTBbits.RB3
#define OW_TRIS TRISBbits.TRISB3

typedef unsigned char bool;
#define true    1
#define false   0

#define DS1820_REG_TEMPLSB    0
#define DS1820_REG_TEMPMSB    1
#define DS1820_REG_CNTREMAIN  6
#define DS1820_REG_CNTPERSEC  7
#define DS1820_SCRPADMEM_LEN  9     /* length of scratchpad memory */

#define DS1820_ADDR_LEN       8

#define DS1820_CMD_SEARCHROM     0xF0
#define DS1820_CMD_READROM       0x33
#define DS1820_CMD_MATCHROM      0x55
#define DS1820_CMD_SKIPROM       0xCC
#define DS1820_CMD_ALARMSEARCH   0xEC
#define DS1820_CMD_CONVERTTEMP   0x44
#define DS1820_CMD_WRITESCRPAD   0x4E
#define DS1820_CMD_READSCRPAD    0xBE
#define DS1820_CMD_COPYSCRPAD    0x48
#define DS1820_CMD_RECALLEE      0xB8

#define DS1820_FAMILY_CODE_DS18B20      0x28

extern bool bDoneFlag;
extern unsigned char nLastDiscrepancy_u8;
extern unsigned char nRomAddr_au8[DS1820_ADDR_LEN];

void output_low(void);
void output_high(void);
bool input(void);
bool ow_reset(void);
void ow_write_bit(bool);
void ow_write_byte(unsigned char);
bool ow_read_bit(void);
unsigned char ow_read_byte(void);
void ow_addr_device(unsigned char);
signed int ds_get_temp(void);
bool find_next_device(void);
bool find_first_device(void);
void Timer0_Init(void);

#endif