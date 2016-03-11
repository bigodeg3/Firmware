/* You can use this library for interfacing the DS18S20
 * NOTE: This library works for 48MHz Frequency i.e. TCY = 0.08333us
 */

#ifndef __OW_H
#define __OW_H

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

static bool bDoneFlag;
static unsigned char nLastDiscrepancy_u8;
static unsigned char nRomAddr_au8[DS1820_ADDR_LEN];

void output_low(void)
{
    OW_TRIS = OUTPUT;
    OW_PIN = LOW;
}

void output_high(void)
{
    OW_TRIS = OUTPUT;
    OW_PIN = HIGH;
}

bool input(void)
{
    OW_TRIS = INPUT;
    return (OW_PIN);
}

bool ow_reset(void)
{
    bool device_found;

    INTCONbits.GIEH = 0;    // Interrupting disabled.

    output_low();           // reset pulse
    Delay100TCYx(58);       // 100 x 0.08333us x 58 = 480us (see datasheet: it must wait between 480us and 960us)
    input();                // release the bus
    Delay10TCYx(84);        // 10 x 0.08333us x 84 = 70us (see datasheet: it must wait 60us minimum)
    device_found = !input(); // presence pulse

    Delay100TCYx(50);       // 100 x 0.08333us x 50 = 410us (to complete the 960us cycle)

    INTCONbits.GIEH = 1;    // Interrupting enabled.
    
    return device_found;
}

void ow_write_bit(bool bBit)
{
    INTCONbits.GIEH = 0;    // Interrupting disabled.

    if (bBit)
    {
        //writing a bit '1'
        output_low();                  // Drive the bus low
        Delay10TCYx(7);                // delay 6 microsecond (us)
        input();                       // Release the bus
        Delay10TCYx(100);              // delay 84 microsecond (us)
        Delay10TCYx(1);                //recovery time slot
    }
    else
    {
        //writing a bit '0'
        output_low();                 // Drive the bus low
        Delay10TCYx(108);             // delay 90 us (between 60 us and 120 us)
        input();                      // Release the bus
        Delay10TCYx(2);               // delay 2 us for recovery
    }

    INTCONbits.GIEH = 1;    // Interrupting enabled.
}

void ow_write_byte(unsigned char val_u8)
{
   unsigned char i;
   unsigned char temp;

   for (i=0; i < 8; i++)      /* writes byte, one bit at a time */
   {
      temp = val_u8 >> i;     /* shifts val right 'i' spaces */
      temp &= 0x01;           /* copy that bit to temp */
      ow_write_bit(temp);     /* write bit in temp into */
   }
}

bool ow_read_bit(void)
{
   bool bBit;

   INTCONbits.GIEH = 0;    // Interrupting disabled.

   output_low();
   Delay10TCYx(7);
   input();
   Delay10TCYx(11);

   bBit = input();

   Delay10TCYx(66);
   Delay10TCYx(1);

   INTCONbits.GIEH = 1;    // Interrupting enabled.

   return (bBit);
}

unsigned char ow_read_byte(void)
{
   unsigned char i;
   unsigned char value = 0;

   for (i=0 ; i < 8; i++)
   {
      if (ow_read_bit())
      {
         value |= (1 << i);
      }
   }
   return (value);
}

void ow_addr_device(unsigned char nAddrMethod)
{
   unsigned char i;

   if (nAddrMethod == DS1820_CMD_MATCHROM)
   {
      ow_write_byte(DS1820_CMD_MATCHROM);           /* address single devices on bus */
      for (i = 0; i < DS1820_ADDR_LEN; i ++)
      {
         ow_write_byte(nRomAddr_au8[i]);
      }
   }
   else
   {
      ow_write_byte(DS1820_CMD_SKIPROM);     /* address all devices on bus */
   }
}

signed int ds_get_temp(void)
{
    unsigned char i;
    unsigned int temp_u16;
    unsigned int highres_u16;
    unsigned char scrpad[DS1820_SCRPADMEM_LEN];

    /* --- start temperature conversion -------------------------------------- */
    ow_reset();
    ow_addr_device(DS1820_CMD_MATCHROM);         /* address the device */
    output_high();
    ow_write_byte(DS1820_CMD_CONVERTTEMP);      /* start conversion */
    Delay10KTCYx(250);
    Delay10KTCYx(250);                          /* wait for temperature conversion */
    Delay10KTCYx(250);
    Delay10KTCYx(150);

    /* --- read sratchpad ---------------------------------------------------- */
    ow_reset();
    ow_addr_device(DS1820_CMD_MATCHROM);   /* address the device */
    ow_write_byte(DS1820_CMD_READSCRPAD);  /* read scratch pad */

    /* read scratch pad data */
    for (i=0; i < DS1820_SCRPADMEM_LEN; i++)
    {
        scrpad[i] = ow_read_byte();
    }

    temp_u16 = 0;
    temp_u16 = (unsigned int)((unsigned int)scrpad[DS1820_REG_TEMPMSB] << 8);
    temp_u16 |= (unsigned int)(scrpad[DS1820_REG_TEMPLSB]);

    if (nRomAddr_au8[0] == DS1820_FAMILY_CODE_DS18B20)
    {
        highres_u16 = temp_u16;
        highres_u16 <<= 4;
    }

    return (highres_u16);
}

bool find_next_device(void)
{
    unsigned char state_u8;
    unsigned char byteidx_u8;
    unsigned char mask_u8 = 1;
    unsigned char bitpos_u8 = 1;
    unsigned char nDiscrepancyMarker_u8 = 0;
    bool bit_b;
    bool bStatus;
    bool next_b = false;

    /* init ROM address */
    for (byteidx_u8=0; byteidx_u8 < 8; byteidx_u8 ++)
    {
        nRomAddr_au8[byteidx_u8] = 0x00;
    }

    bStatus = ow_reset();        /* reset the 1-wire */

    if (!bStatus || bDoneFlag)        /* no device found */
    {
        nLastDiscrepancy_u8 = 0;     /* reset the search */
        return false;
    }

    /* send search rom command */
    ow_write_byte(DS1820_CMD_SEARCHROM);

    byteidx_u8 = 0;
    do
    {
        state_u8 = 0;

        /* read bit */
        if (ow_read_bit() != 0)
        {
            state_u8 = 2;
        }

        /* read bit complement */
        if (ow_read_bit() != 0)
        {
            state_u8 |= 1;
        }

        /* description for values of state_u8: */
        /* 00    There are devices connected to the bus which have conflicting */
        /*       bits in the current ROM code bit position. */
        /* 01    All devices connected to the bus have a 0 in this bit position. */
        /* 10    All devices connected to the bus have a 1 in this bit position. */
        /* 11    There are no devices connected to the 1-wire bus. */

        /* if there are no devices on the bus */
        if (state_u8 == 3)
        {
            break;
        }
        else
        {
            /* devices have the same logical value at this position */
            if (state_u8 > 0)
            {
                /* get bit value */
                bit_b = (bool)(state_u8 >> 1);
            }
            /* devices have confilcting bits in the current ROM code */
            else
            {
                /* if there was a conflict on the last iteration */
                if (bitpos_u8 < nLastDiscrepancy_u8)
                {
                    /* take same bit as in last iteration */
                    bit_b = ((nRomAddr_au8[byteidx_u8] & mask_u8) > 0);
                }
                else
                {
                    bit_b = (bitpos_u8 == nLastDiscrepancy_u8);
                }

                if (bit_b == 0)
                {
                    nDiscrepancyMarker_u8 = bitpos_u8;
                }
            }

            /* store bit in ROM address */
           if (bit_b != 0)
           {
               nRomAddr_au8[byteidx_u8] |= mask_u8;
           }
           else
           {
               nRomAddr_au8[byteidx_u8] &= ~mask_u8;
           }

           ow_write_bit(bit_b);

           /* increment bit position */
           bitpos_u8 ++;

           /* calculate next mask value */
           mask_u8 = mask_u8 << 1;

           /* check if this byte has finished */
           if (mask_u8 == 0)
           {
               byteidx_u8 ++;  /* advance to next byte of ROM mask */
               mask_u8 = 1;    /* update mask */
           }
        }
    } while (byteidx_u8 < DS1820_ADDR_LEN);


    /* if search was unsuccessful then */
    if (bitpos_u8 < 65)
    {
        /* reset the last discrepancy to 0 */
        nLastDiscrepancy_u8 = 0;
    }
    else
    {
        /* search was successful */
        nLastDiscrepancy_u8 = nDiscrepancyMarker_u8;
        bDoneFlag = (nLastDiscrepancy_u8 == 0);

        /* indicates search is not complete yet, more parts remain */
        next_b = true;
    }

    return next_b;
}

bool find_first_device(void)
{
    nLastDiscrepancy_u8 = 0;
    bDoneFlag = false;

    return (find_next_device());
}

void Timer0_Init(void)
{
    // Set up Interrupts for timer
    INTCONbits.TMR0IF = 0;          // clear roll-over interrupt flag
    INTCON2bits.TMR0IP = 0;         // Timer0 is low priority interrupt
    INTCONbits.TMR0IE = 1;          // enable the Timer0 interrupt.
    // Set up timer itself
    T0CON = 0b00000111;             // prescale 1:256 - about 1.4 second maximum delay.
    TMR0H = 0;                      // clear timer - always write upper byte first
    TMR0L = 0;
    T0CONbits.TMR0ON = 1;           // start timer
}

#endif