; ######  FLASHLIGHT.ASM ######
; When you press button RB0...RB4, you turn on lights
; RB0 => all lights off
; RB1 => one led on
; RB5 => two led's on
; RB6 => Three led's on
; RB4 => Four led's on

#include <p18f4620.inc>

; ###### Variables #########

; ##### Main Routine  #######

    org 0x300
    call  Init
    
Loop:
	clrf PORTC
	clrf PORTD
	btfsc PORTB,0x0	;If RB0 = 1
	call SetRB0		;call subroutine
	btfsc PORTB,0x1	;If RB1 = 1
	call SetRB1		;call subroutine
	btfsc PORTB,0x5	;If RB5 = 1
	call SetRB5		;call subroutine
	btfsc PORTB,0x6	;If RB6 = 1
	call SetRB6		;call subroutine
	btfsc PORTB,0x4	;If RB4 = 1
	call SetRB4		;call subroutine
	goto Loop
    
; ###### Initialization ###########
; Init: Initializes I/O, Time = 0:00:00

Init:
	clrf TRISA	;PORTA is output
	movlw 0xFF
	movwf TRISB	;PORTB is input
	clrf TRISC	;PORTC is output
	clrf TRISD	;PORTD is output
	movlw 15
	movwf ADCON1	;everyone is binary
	return

; ####### Subroutines ############

;Set RB0 to 0
SetRB0:
	movlw 0x0		;Move 0 to W
	movwf PORTA	;Move 0 to PORTA
	return

;Set RB1 = 1;
SetRB1: 
	movlw 0x1		;Move 1 to W
	movwf PORTA	;Move 1 to PORTA
	return

;Set RB5 = 3;
SetRB5:
	movlw 0x3		;Move 3 to W
	movwf PORTA	;Move 3 to PORTA
	return

;Set RB6 = 7;
SetRB6:
	movlw 0x7		;Move 7 to W
	movwf PORTA	;Move 7 to PORTA
	return

;Set RB4 = 15;
SetRB4:
	movlw 0xf		;Move 15 to W
	movwf PORTA	;Move 15 to PORTA
	return

end	