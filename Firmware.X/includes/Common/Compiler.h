/*********************************************************************
 *
 *                  Compiler and hardware specific definitions
 *
 ********************************************************************/
#define __COMPILER_H

// Include proper device header file
#define COMPILER_MPLAB_C18

#include <p18cxxx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Base RAM and ROM pointer types for given architecture
#define PTR_BASE		unsigned short
#define ROM_PTR_BASE	unsigned short long

//// Definitions that apply to all 8-bit products
//// (PIC10, PIC12, PIC16, PIC18)
#define	__attribute__(a)
#define FAR                         far
// Microchip C18 specific defines
#define ROM                 	rom