; Zane Durkin (durk7832)  CS150 Michael Wilder
; Homework 6
; April 16, 2017
;.device atmega328p
rjmp entry

.org 0x60


; portB = 0x20
; pin 13 = pin 5 on port B.

entry:
    LDI r16, 0xFF
    LDI r17, 0x00
    OUT 0x04, r16 ; enable writing for output 0x05

    rcall seven
    rcall eight
    rcall three
    rcall two   

    rcall delay 
    rcall delay 
    rcall delay 
    rcall delay 
    rcall delay 
    rcall delay 

    RJMP entry     ; start the loop over again

seven:            ; - - . . .
    RCALL dash
    RCALL dash
    RCALL dot
    RCALL dot
    RCALL dot

    RCALL delay
    RCALL delay
    RCALL delay

    RET

eight:            ; - - - . .
    RCALL dash
    RCALL dash
    RCALL dash
    RCALL dot
    RCALL dot

    RCALL delay
    RCALL delay
    RCALL delay

    ret

three:            ; . . . - -
    RCALL dot
    RCALL dot
    RCALL dot
    RCALL dash
    RCALL dash

    RCALL delay
    RCALL delay
    RCALL delay

    ret

two:	          ; . . - - -
    RCALL dot
    RCALL dot
    RCALL dash
    RCALL dash
    RCALL dash

    RCALL delay
    RCALL delay
    RCALL delay

    ret

delay:
    LDI r19, 255
    LDI r20, 255
    LDI r21, 10
    BR:
	ADD r19, r18
	BRBC 1, BR
	ADD r20, r18
	NOP
	BRBC 1, BR
	ADD r21, r18
	NOP
	BRBC 1, BR
	RET

dot:
    RCALL led_on    ; turn on led
    RCALL delay	    ; wait one count
    RCALL led_off   ; turn off led
    RCALL delay	    ; wait one count
    RET		    ; return

dash:
    RCALL led_on    ; turn on led
    RCALL delay	    ; wait two counts
    RCALL delay
    RCALL led_off   ; turn off led
    RCALL delay	    ; wait one count
    RET		    ; return

led_on:
    OUT 0x05, r16 ; set output to FF (on)
    RET

led_off:
    OUT 0x05, r17 ; set output to 00 (off)
    RET
