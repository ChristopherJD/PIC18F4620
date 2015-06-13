/*
Title: Guitar Tuner
Version: 1.0
Author: Christopher Jordan - Denny, Justin Signalness
Date: 1:56 PM Thursday, November 6, 2014
ECE 376 Embedded Systems

This project uses the PIC18F4620 for tuning a guitar. It takes a 
microphone and keypad as input and outputs to the LCD Screen as 
well as being able to output to a speaker. 

Uses a 12-tone equal temperament with A440
f(n)=(2^((n-49)/12))*440 gives the frequency f of the nth key of a
piano

Enter the number of the string you wish to tune into the keypad. The
Note will play for 3 seconds for which string you choose. This will
Occur once with PWM and once with a DAC creating a sine wave.

Once notes have finished playing a screen with the frequency will appear
allowing you to tune to the correct note. The note to the right of the 
frequency reading will display if it is flat or sharp. 

Once the note is tuned correctly it will wait 3 seconds and then return to
the screen where you choose which string to tune.

PORT INFORMATION***************************************************

PORTA_2 Is the PWM note
PORTB_0-6 is the keypad input 
PORTC_2 is the guitar input to determine frequency //captures on the rising edge
PORTD_0,1,2 are outputs for the DAC

*******************************************************************
*/

//Global Variables
const unsigned int E2 = 60675; //82.40626 Hz
unsigned int COUNTER;
unsigned long int Start, End, period,Frequency, OverflowTime;
const unsigned char MSG0[21] = "Guitar Tuner       ";
const unsigned char MSG1[2] = "Hz"; 
const unsigned char MSG2[21] = "Frequency:         ";
const unsigned char MSG3[5] = "E(2) ";
const unsigned char MSG4[5] = "E(2)b";
const unsigned char MSG5[5] = "E(2)#"; 
const unsigned char MSG6[21] = "String Number:     ";
const unsigned char MSG7[21] = "Tuned!             ";

//sine wave sample
unsigned int TABLE[32]={2000,2205,2402,2582,2737,2860,2947,2993,2996,2957,2877
,2760,2610,2434,2240,2035,1829,1630,1447,1287,1158,1065,1012,1001,1033,1106,1218
,1363,1535,1727,1930}; 

//Subroutine Declarations
#include <pic18.h>
#include "lcd_portd.c"

void Port_Init(void)
{
    //0 is output 1 is input 0xFF is all input
    TRISA = 0; //outputs sine wave on RA2
    TRISB = 0xF8; //Keypad stuff
    TRISC = 0xFF; //PORTC is input
    TRISD = 0; //D2A conversion stuff
    TRISE = 0;
    ADCON1 = 0x0F;
}

void timer3Init(void)
{
    // set up Timer3 for 1ms
    TMR3CS = 0;
    T3CON = 0x81; //ps = 1;
    TMR3ON = 1;
    TMR3IE = 1;
    TMR3IP = 1;
    PEIE = 1;
}

void TMR1CAP_Init(void)
{
    // Initialize Timer1 Capture
    TMR1ON = 1;
    CCP1CON = 0x05; //0000 01 01 Rising Edge
    CCP1IE = 1;
    PEIE = 1;
    TMR1IE = 1; //Initiate overflow, 32 bits
} 

void Wait_longer(unsigned int X)
{
    unsigned int i, j;

    for (i=0; i<X; i++)
    for (j=0; j<100; j++);
}        

// High-priority service
void interrupt IntServe(void)
{
    //PWM note
    if (TMR3IF) {
        RA2 = !RA2;
        TMR3 = -E2;
        TMR3IF = 0;
    }

    //This is because overflow occurs on some frequencies. Adding 2^16 because
    //That is the max number in the lower register. 
    if (TMR1IF) {
        OverflowTime += 0x10000; //65536 2^16
        TMR1IF = 0;
    }

    if (CCP1IF) {
        
        COUNTER +=1;
        
        //Define start time
        if (COUNTER == 1) {
            Start = OverflowTime + CCPR1;
        }
        if (COUNTER == 2) {
            End = OverflowTime + CCPR1;
            period = End - Start;
            Frequency = 10000000./(period);
            COUNTER = 0;
            OverflowTime = 0;
            CCPR1 = 0;
            Start = 0;
            End = 0;
        }
        CCP1IF = 0;
    }
}
//D2A Conversion*****************************************************************
//0-4095 is 0-5V
void D2A(unsigned int X)
{
    unsigned char i;
    TRISD0 = 0;
    TRISD1 = 0;
    TRISD2 = 0;
    // Add 0011 to the first four bits to set up the D/A
    X = X & 0x0FFF;
    X = X + 0x3000;
    RD0 = 1;
    RD1 = 1;
    // CS goes low to select the D/A chip
    RD0 = 0;
    // Send out 16 bits of data
    for (i=0; i<16; i++) {
        if (X & 0x8000) RD2 = 1; else RD2 = 0; //test to see if bits are high or low
        RD1 = 0;
        X = X << 1;
        RD1 = 1;
    }
    // CS goes high to terminate the communication
    RD0 = 1;
}

void sineWave(void)
{
    unsigned int play = 200;
    unsigned int i;
    while(play > 10)
    {
        for(i=0; i<31; i++){
            D2A(TABLE[i]);
            Wait_longer(2);
        }
        play --;
    }
}

//KEYPAD CODE******************************************
char GetKey(void)
{
    int i;
    TRISB = 0xF8;
    PORTB = 4;
    for (i=0; i<1000; i++);
    if (RB6) return(1);
    if (RB5) return(4);
    if (RB4) return(7);
    if (RB3) return(10);
    PORTB = 2;
    for (i=0; i<1000; i++);
    if (RB6) return(2);
    if (RB5) return(5);
    if (RB4) return(8);
    if (RB3) return(0);
    PORTB = 1;
    for (i=0; i<1000; i++);
    if (RB6) return(3);
    if (RB5) return(6);
    if (RB4) return(9);
    if (RB3) return(11);

    return(13);
}

char ReadKey(void)
{
    char X, Y;
    do {
        X = GetKey();
    }   while(X > 20);
    do {
        Y= GetKey();
    }   while(Y < 20);
    Wait_ms(100);  // debounce
    return(X);
}

void LCD_Out(unsigned int DATA, unsigned char N)
{
    unsigned char A[5], i;

    for (i=0; i<5; i++) {
        A[i] = DATA % 10;
        DATA = DATA / 10;
    }
    
    //N is where to place the decimal if desired
    for (i=5; i>0; i--) {
        if (i == N) LCD_Write('.');
        LCD_Write(A[i-1] + '0');
    }
}

//Main Routine

void main(void)
{
    //local variables
    unsigned int i, j;
    int X, Y, Z, T, TEMP;
    unsigned char FLAG;

    //Initialize Stuff
    RA2 = 0; //makes sure its clear
    COUNTER = 0;
    X = 0;
    Y = 0;
    Z = 0;
    T = 0;
    TEMP = 0;
    FLAG = 0;  // Flag=1 indicates the last key was an operation
    Port_Init();
    TMR1CAP_Init(); //initialize A2D PORTA
    LCD_Init(); //initialize LCD Screen
    timer3Init(); //initialize timer 3 for 1ms
    TMR3ON = 0; //make sure note doesn't play until I say muhahahaha
    
    //**********************************************************
    GIE = 1; //turn on all interrupts
    //**********************************************************
    
    //Display the program
    LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG0[i]);
    Wait_ms(3000);
    LCD_Inst(1);   // clear the display
    
    //continuously run the program
    while(1) {
        
        //Keypad input
        while(TEMP != 11){
            TEMP = GetKey();
            LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG6[i]);
            if (TEMP != 11){ 
                if (TEMP != 13) X = TEMP;
            }
            LCD_Move(1,0); LCD_Out(X,0);
        }
        
        if (X == 6)
        {
            TMR3ON = 1; //make the note play for a bit
            Wait_ms(3000);
            TMR3ON = 0; //turn off timer
            
            sineWave();
            LCD_Init();
        }
        
        while (X == 6){
            
            LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG2[i]);
            LCD_Move(1,0); LCD_Out(Frequency,0);
            LCD_Move(1,6); for (i=0; i<2; i++) LCD_Write(MSG1[i]);

            //Determine if the note is low E(2) on the guitar
            if (Frequency == 82){
                LCD_Move(1,9); for (i=0; i<5; i++) LCD_Write(MSG3[i]);
                Wait_ms(3000);
                X = 0; //break the loop
            }
            if (Frequency < 82) {
                LCD_Move(1,9); for (i=0; i<5; i++) LCD_Write(MSG4[i]);
            }
            if (Frequency > 82){
                LCD_Move(1,9); for (i=0; i<5; i++) LCD_Write(MSG5[i]);  
            }
        }   

        if (X != 6){
            LCD_Move(0,0); for (i=0; i<20; i++) LCD_Write(MSG7[i]);
            Wait_ms(3000);
            LCD_Inst(1);
            TEMP = 0;
        }
        
    } 
}

