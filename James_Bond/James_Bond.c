/*
Problem 3, James Bond Theme song
Christopher Jordan - Denny
Justin Signalness
Version: 1.0
10/25/14
*/

//Global Variables

//Notes, not all acutally used
const unsigned int A4 = 236;
const unsigned int B4 = 210;
const unsigned int C5 = 198;
const unsigned int C5S = 188;
const unsigned int D5 = 176;
const unsigned int D5S = 166;
const unsigned int E5 = 157;
const unsigned int F5 = 148;
const unsigned int F5S = 141;
const unsigned int G5 = 133;
const unsigned int A5 = 125;
const unsigned int A5S = 118;
const unsigned int B5 = 105;
const unsigned int D6 = 89;
const unsigned int D6S = 84;

//Subroutines
#include <pic18.h> 

//waits 1 ms
void Wait_ms(unsigned int X)
{
	unsigned int i, j;

	for (i=0; i<X; i++)
	for (j=0; j<300; j++);
}

void interrupt IntServe(void)
{
	if (TMR2IF){
		RC0 = !RC0; //toggle bit
		TMR2IF = 0;
	}
}

void WholeNote(unsigned char NOTE)
{
	PR2 = NOTE;
	TMR2ON = 1;
	Wait_ms(3600);
	TMR2ON = 0;
	Wait_ms(400);
}

void HalfNote(unsigned char NOTE)
{
	PR2 = NOTE;
	TMR2ON = 1;
	Wait_ms(1800);
	TMR2ON = 0;
	Wait_ms(200);
}

void Note3_8th(unsigned char NOTE)
{
	PR2 = NOTE;
	TMR2ON = 1;
	Wait_ms(1350);
	TMR2ON = 0;
	Wait_ms(150);
}

void Note5_8th(unsigned char NOTE)
{
	PR2 = NOTE;
	TMR2ON = 1;
	Wait_ms(2250);
	TMR2ON = 0;
	Wait_ms(250);
}

void Note3_4th(unsigned char NOTE)
{
	PR2 = NOTE;
	TMR2ON = 1;
	Wait_ms(2700);
	TMR2ON = 0;
	Wait_ms(300);
}

void Note1_4th(unsigned char NOTE)
{
	PR2 = NOTE;
	TMR2ON = 1;
	Wait_ms(900);
	TMR2ON = 0;
	Wait_ms(100);
}

void Note1_8th(unsigned char NOTE)
{
	PR2 = NOTE;
	TMR2ON = 1;
	Wait_ms(450);
	TMR2ON = 0;
	Wait_ms(50);
}

void Note1_16th(unsigned char NOTE)
{
	PR2 = NOTE;
	TMR2ON = 1;
	Wait_ms(225);
	TMR2ON = 0;
	Wait_ms(25);
}

void Note3_32nd(unsigned char NOTE)
{
	PR2 = NOTE;
	TMR2ON = 1;
	Wait_ms(112);
	TMR2ON = 0;
	Wait_ms(12);
}

void firstPattern()
{
	HalfNote(B4);
	HalfNote(C5);
	
	HalfNote(C5S);
	HalfNote(C5);

	HalfNote(B4);
	HalfNote(C5);

	HalfNote(C5S);
	HalfNote(C5);
}

void secondPattern()
{
	Note1_8th(E5);
	Note1_16th(F5S);
	Note1_16th(F5S);
	Note1_8th(F5S);
	Note1_4th(F5S);
	Note1_16th(E5);
	Note1_16th(E5);
	Note1_16th(E5);
	
	Note1_8th(E5);
	Note1_16th(G5);
	Note1_16th(G5);
	Note1_8th(G5);
	Note1_4th(G5);
	Note1_16th(F5S);
	Note1_16th(F5S);
	Note1_16th(F5S);
}

void secondPatternVariation()
{
	Note1_8th(E5);
	Note1_16th(F5S);
	Note1_16th(F5S);
	Note1_8th(F5S);
	Note1_4th(F5S);
	Note1_16th(E5);
	Note1_16th(E5);
	Note1_16th(E5);

	Note1_8th(E5);
	Note1_16th(F5S);
	Note1_16th(F5S);
	Note1_8th(G5);
	Note1_4th(G5);
	Note1_16th(F5S);
	Note1_16th(F5S);
	Note1_16th(E5);  //here is the variation
}

void thirdPattern()
{
	Note1_8th(D6S);
	Note5_8th(D6);
	Note1_8th(B5);
	Note1_8th(A5);
	WholeNote(B5);
}

void fourthPattern()
{
	Note1_8th(E5);
	Note1_4th(G5);
	Note1_8th(D6S);
	Note3_8th(D6);
	Note1_8th(G5);
	
	Note1_8th(A5S);
	Note3_4th(B5);

	Note1_4th(G5);
	Note3_32nd(A5);
	Note3_32nd(G5);
	HalfNote(F5S);
	Note1_16th(B4);
	Note1_16th(E5);
	WholeNote(C5S);
}

void fourthPatternVariation()
{
	Note1_8th(E5);
	Note1_4th(G5);
	Note1_8th(D6S);
	Note3_8th(D6);
	Note1_8th(G5);
	
	Note1_8th(A5S);
	Note3_4th(B5);

	Note1_4th(G5);
	Note3_32nd(A5);
	Note3_32nd(G5);
	HalfNote(F5S);
	Note1_16th(B4);
	Note1_16th(D5S);
	WholeNote(E5);
}

//Main Routine

void main(void)
{
	//Set-up I/0
	TRISC = 0; //PORTC is output
	TRISB = 0XFF; //PORTB is input
	ADCON1 = 15; //Everyone is binary

	// set up Timer2
	T2CON = 0x5D;
	PR2 = 49;
	TMR2ON = 1;
	TMR2IE = 1;
	TMR2IP = 1;
	PEIE = 1;


	// turn on all interrupts
	GIE = 1;
	
	while(1){

		if(RB0){

			//First Stanza
			firstPattern();
			
			//Second and Third Stanza
			secondPattern();
			
			//Third Stanza
			secondPattern();
			
			//Fourth Stanza
			secondPatternVariation();

			//6th Stanza, recognizable part

			thirdPattern();

			//6th - 7th Stanza
			firstPattern();

			//7th Stanza last half
			fourthPattern();
			
			//8th Stanza
			fourthPatternVariation();
		}
	}
}