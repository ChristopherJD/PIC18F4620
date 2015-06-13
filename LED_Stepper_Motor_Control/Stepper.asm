; --- Stepper.asm ----
; Version: 1.0
; Author: Christopher Jordan - Denny
; Author: Justin Signalness
; Date: 9/26/14
; When you press buttons RB7 - RB4 and RB1 the motor will spin
; RB7 fast reverse
; RB6 slow reverse
; RB5 Stop
; RB4 Fast forward
; RB1 slow forward
;


#include <p18f4620.inc>

; --- Variables ---
CNT0  EQU 1
CNT1  EQU 2
CNT2  EQU 3
STEP  EQU 4
TEMP  EQU 5
DIR   EQU 6    ; direction.  +1 = fwd, 0 = stop, -1 = rev
SPEED EQU 7		; speed:  ms/step
COUNT  EQU 8


; --- Main Routine ---
	org 0x300
	call Init
	clrf STEP
	clrf PORTD
	clrf SPEED
	movlw 0xC8	;200
	movwf COUNT	;How many steps motor takes

Loop:

	call  Check_Buttons
	call Step

    call  Drive_IO
	movwf PORTC
	
	movf SPEED,W
	call  Wait_ms

	;incf PORTD		; Checks how many steps the motor is
	;dcfsnz COUNT,F
	;call Pause

	goto  Loop

; --- Subroutines ---

Check_Buttons
	movf DIR,W ; default = no change
	btfsc PORTB,7 ; RB7 = reversefast
	call ReverseFast
	btfsc PORTB,6 ; RB6 = reverseslow
	call ReverseSlow
	btfsc PORTB,5 ; RB5 = stop
	call Stop
	btfsc PORTB,4 ; RB5 = forwardfast
	call FowardFast
	btfsc PORTB,1 ; RB1 = fowardslow
	call FowardSlow
	return

ReverseFast:
	movlw -1
	movwf DIR
	movlw 3
	movwf SPEED
	return

Stop:
	movlw 0
	movwf DIR
	movlw 30
	movwf SPEED
	return

FowardFast:
	movlw 1
	movwf DIR
	movlw 3
	movwf SPEED
	return

ReverseSlow:
	movlw -1
	movwf DIR
	movlw 30
	movwf SPEED
	return

FowardSlow:
	movlw 1
	movwf DIR
	movlw 30
	movwf SPEED
	return

Step:
	movf 	DIR,W
	addwf   STEP,W
	andlw   0x03
	movwf	STEP
	return

Drive_IO
	movf    STEP,W
	movwf   TEMP
	decf    TEMP,F
	btfsc   STATUS,Z
	retlw  1
	decf    TEMP,F
	btfsc   STATUS,Z
	retlw  2
	decf    TEMP,F
	btfsc   STATUS,Z
	retlw  4
	retlw  8

Init:
	clrf   TRISA  	;PORTA is output
    movlw  0xFF
	movwf  TRISB 	;PORTB is input
	clrf   TRISC	;PORTC is output
	clrf   TRISD	;PORTD is output
	clrf   TRISE	;PORTE is output
	movlw  15
	movwf  ADCON1 	;everyone is binary
	movlw  1
	movwf  DIR		; Start with DIR=1 (forward)
	movwf  30
	movwf  SPEED	; Start with 30ms/step
	return

Pause:				; waits for 1 second
	movlw 200
	call  Wait_ms
	movlw 200
	call  Wait_ms
	movlw 200
	call  Wait_ms
	movlw 200
	call  Wait_ms
	movlw 200
	call  Wait_ms
	clrf  PORTD
	movlw 0xC8
	movwf COUNT
	return

Wait_ms:				; Wait W x 1ms
	movwf CNT2
Wait1:		 	
	movlw 46
	movwf CNT1
Wait2:	    
    movlw 20
	movwf CNT0
Wait3:
	NOP
	NOP
	decfsz CNT0,F
	goto   Wait3
	decfsz CNT1,F
	goto   Wait2
	decfsz CNT2,F
	goto   Wait1
	return

	end