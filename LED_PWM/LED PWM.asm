; --- LED_PWM.asm ----
; Version: 1.0
; Author: Christopher Jordan - Denny
; Author: Justin Signalness
; Date: 9/26/14
; Turns LED's on at a certain intensity
; PORTC_0 = 100%
; PORTC_1 = 50%
; PORTC_2 = 10%
; PORTC_3 = 1%

#include <p18f4620.inc> 

CNT0 EQU 1
RED EQU 2
GREEN	EQU 3
BLUE EQU 4
WHITE EQU 5

org 0x300
clrf TRISA ;PORTA is output
movlw 0xFF
movwf TRISB ;PORTB is input
clrf TRISC ;PORTC is output
clrf TRISD ;PORTD is output
clrf TRISE ;PORTE is output
movlw 15
movwf ADCON1 ;everyone is binary
clrf PORTC ; start from zero
movlw 0x64
movwf RED
movlw 0x32
movwf GREEN
movlw 0x0A
movwf BLUE
movlw 0x01
movwf WHITE
Loop:	movlw 0x64
movwf CNT0
clrf PORTC

Loop1:	
movf CNT0,W
cpfslt RED
bsf	PORTC,0
movf CNT0,W
cpfslt GREEN
bsf	PORTC,1
movf CNT0,W
cpfslt BLUE
bsf	PORTC,2
movf CNT0,W
cpfslt WHITE
bsf	PORTC,3
decfsz CNT0,F
goto Loop1
goto Loop

return
end