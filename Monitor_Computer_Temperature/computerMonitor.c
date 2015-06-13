// Computer Temperature Monitor
// Author: Christopher Jordan - Denny
// Date: 11/15/14
// Version 1.0
// Most Code used from www.bisonacademy.com
// Author: Jacob Glower
//
// This program reads in the Temperature on RA1
// and sends these to the serial port at 9600 baud
// 2 samples / second

// Global Variables

const unsigned char MSG0[21] = "Computer Monitor   "; 
const unsigned char MSG1[21] = "                   ";     
unsigned int DELAY;

// Subroutine Declarations
#include <pic18.h>

// Subroutines
#include        "lcd_portd.c"

// High-priority service
void interrupt IntServe(void)
{
	if (TMR2IF) {
		if (DELAY) {
			DELAY -= 1;
		}
	}
	TMR2IF = 0;
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

unsigned int A2D_Read(unsigned char c)
{
	unsigned int result;
	unsigned char i;
	c = c & 0x0F;
	ADCON0 = (c << 2) + 0x01; // set Channel Select
	for (i=0; i<20; i++); // wait 2.4us (approx)
	GODONE = 1; // start the A/D conversion
	while(GODONE); // wait until complete (approx 8us)
	return(ADRES);
}   


void SCI_Out(unsigned int DATA, unsigned char N)
{
	unsigned char A[5], i;
	for (i=0; i<5; i++) {
		A[i] = DATA % 10;
		DATA = DATA / 10;
	}
	for (i=3; i>0; i--) {
		//if (i == N) {
			//while(!TRMT);  TXREG = '.';
		//}   
		while(!TRMT);  TXREG = A[i-1] + 48; //makes sure they are number 0=48
	}
	while(!TRMT);  TXREG = ' ';
}


// Main Routine

void main(void)
{
	unsigned int A1;   // raw A/D reading
	unsigned int V0;   // corresponding voltage
	unsigned int TIME;
	int TEMP; //Temperature

	unsigned char i, j;

	TRISA = 0;
	TRISB = 0;
	TRISC = 0x0F;
	TRISD = 0;
	TRISE = 0;
	ADCON1 = 0x0F;

	DELAY = 0;
	TIME = 0;

	LCD_Init();                  // initialize the LCD

	LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG0[i]);
	LCD_Move(1,0);  for (i=0; i<20; i++) LCD_Write(MSG1[i]);

	Wait_ms(2000);
	LCD_Inst(1);   // clear the display

	// Initialize the A/D port
	TRISA = 0xFF;
	TRISE = 0x0F;
	ADCON2 = 0x85;
	ADCON1 = 0x07;
	ADCON0 = 0x01;


	// set up Timer2 for 1ms
	T2CON = 0x4D;
	PR2 = 249;
	TMR2ON = 1;
	TMR2IE = 1;
	TMR2IP = 1;
	PEIE = 1;

	// Initialize Serial Port to 9600 baud
	TRISC = TRISC | 0xC0;
	TXIE = 0;
	RCIE = 0;
	BRGH = 0;
	BRG16 = 1;
	SYNC = 0;
	SPBRG = 255;
	TXSTA = 0x22;
	RCSTA = 0x90;

	// turn on all interrupts
	GIE = 1;

	while(1) {
		while(DELAY);
		DELAY = 500; //read data every 1/2 second
		A1 = A2D_Read(1);
		V0 = A1 * 0.488;
		
		//calibration function Linear
		TEMP = -0.05*A1+103;
		
		LCD_Move(0,0);  LCD_Out(A1, 0);
		LCD_Move(1,0);  LCD_Out(V0, 2);

		TIME = TIME + 5; //for 500 ms, 10 for 1s

		SCI_Out(TIME, 1);
		SCI_Out(TEMP, 2);

		while(!TRMT); TXREG = 13;   // carriage return
		while(!TRMT); TXREG = 10;   // line feed

	}      


}