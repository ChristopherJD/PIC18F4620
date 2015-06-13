// Global Variables

const unsigned char MSG0[16] = "R: 100% G: 50%  "; 
const unsigned char MSG1[16] = "B:  10% W:  1%  ";     

// Subroutine Declarations
#include <pic18.h>
#include "lcd_portd.h"
// Subroutines
#include        "lcd_portd.c"

// Main Routine

void main(void)
{
    TRISA = 0;
       TRISB = 0xFF;
       TRISC = 0;
       TRISD = 0;
       TRISE = 0;
       TRISA = 0;
       PORTB = 0;
       PORTC = 0;
       PORTD = 0;
       PORTE = 0;
       ADCON1 = 0x0F;

       LCD_Init();                  // initialize the LCD


       LCD_Move(0,0);  for (int i=0; i<16; i++) LCD_Write(MSG0[i]);
       LCD_Move(1,0);  for (int i=0; i<16; i++) LCD_Write(MSG1[i]);

    int i;

    while(1 == 1)
    {
        i = 100;
        PORTC = 0;
        while (i > 0)
        {
            if ( i <= 100) 
                RC0 = 1;
            if ( i <= 50)
                RC1 = 1;
            if ( i <= 10)
                RC2 = 1;
            if ( i <= 1)
                RC3 = 1;
            i--;
        }
    }
        
}

