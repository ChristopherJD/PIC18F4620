// Play Chord
// Author: Christopher Jordan - Denny
// Date: 11/19/14
// Version 1.0
//
// This program plays a chord
// RC0 = 391.9954 (G4)
// RC1 = 329.6276 (E4)
// RC2 = 261.6256 (C4)
// through a speaker

// Global Variables
const unsigned int G4 = 12755; //392.0031 Hz
const unsigned int E4 = 15169; //329.619619 Hz
const unsigned int C4 = 19111; //261.6294.281 Hz

unsigned int Timer0;
const unsigned char MSG0[21] = "Chord.C             ";

//subroutine Declarations
#include <pic18.h>

//Subroutines
#include "LCD_PortD.c"

// High priority service interrupt
void interrupt IntServe(void)
{
    if (TMR0IF)
    {
        Timer0 += 1;
        RB0 = !RB0;
        TMR0 = -G4;
        TMR0IF = 0;
    }
    
    if (TMR1IF) {
        RB1 = !RB1;
        TMR1 = -E4;
        TMR1IF = 0;
    }
    
    if (TMR3IF) {
        RB2 = !RB2;
        TMR3 = -C4;
        TMR3IF = 0;
    }

}

void LCD_Out(unsigned int DATA, unsigned char N)
{
    unsigned char A[5], i;

    for (i=0; i<5; i++) {
        A[i] = DATA % 10;
        DATA = DATA / 10;
    }
    for (i=5; i>0; i--) {
        if (i == N) LCD_Write('.');
        LCD_Write(A[i-1] + '0');
    }
}


//main routine

void main(void)
{
    //variables
    unsigned char i;

    //inputs and outputs
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    ADCON1 = 0x0F;
    
    LCD_Init();

    LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG0[i]);
    Wait_ms(3000);
    LCD_Inst(1);

    // set up Timer0 for PS = 1
    T0CS = 0;
    T0CON = 0x88;
    TMR0ON = 1;
    TMR0IE = 1;
    TMR0IP = 1;
    PEIE = 1;
    
    // set up Timer1 for 1ms
    TMR1CS = 0;
    T1CON = 0x81;
    TMR1ON = 1;
    TMR1IE = 1;
    TMR1IP = 1;
    PEIE = 1;

    // set up Timer3 for 1ms
    TMR3CS = 0;
    T3CON = 0x81;
    TMR3ON = 1;
    TMR3IE = 1;
    TMR3IP = 1;
    PEIE = 1;

    //Turn on all interrupts
    GIE = 1;

    while(1){
        LCD_Move(1,0); LCD_Out(Timer0, 3);
    }
}
