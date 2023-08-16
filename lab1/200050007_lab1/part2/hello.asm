loop:				; Define a label 'loop', that will allow us
                                ; to jumb back to here to create a loop
                            
mov ah, 0x0e ; set tele-type mode (output to screen)
mov al, 'A' ;one ascii character hex code in register AL
int 0x10   ;send content of register to screen via an interrupt
mov ah, 0x0e ; set tele-type mode (output to screen)
mov al, 'N' ;one ascii character hex code in register AL
int 0x10   ;send content of register to screen via an interrupt
mov ah, 0x0e ; set tele-type mode (output to screen)
mov al, 'A' ;one ascii character hex code in register AL
int 0x10   ;send content of register to screen via an interrupt
mov ah, 0x0e ; set tele-type mode (output to screen)
mov al, 'N' ;one ascii character hex code in register AL
int 0x10   ;send content of register to screen via an interrupt
mov ah, 0x0e ; set tele-type mode (output to screen)
mov al, 'D' ;one ascii character hex code in register AL
int 0x10   ;send content of register to screen via an interrupt
mov ah, 0x0e ; set tele-type mode (output to screen)
mov al, ' ' ;one ascii character hex code in register AL
int 0x10   ;send content of register to screen via an interrupt
mov ah, 0x0e ; set tele-type mode (output to screen)
mov al, 'K' ;one ascii character hex code in register AL
int 0x10   ;send content of register to screen via an interrupt
mov ah, 0x0e ; set tele-type mode (output to screen)
mov al, 'U' ;one ascii character hex code in register AL
int 0x10   ;send content of register to screen via an interrupt
mov ah, 0x0e ; set tele-type mode (output to screen)
mov al, 'M' ;one ascii character hex code in register AL
int 0x10   ;send content of register to screen via an interrupt
mov ah, 0x0e ; set tele-type mode (output to screen)
mov al, 'A' ;one ascii character hex code in register AL
int 0x10   ;send content of register to screen via an interrupt
mov ah, 0x0e ; set tele-type mode (output to screen)
mov al, 'R' ;one ascii character hex code in register AL
int 0x10   ;send content of register to screen via an interrupt


		; This instruction jumps to a new memory
				; address refernced by 'loop'. ie. it will
				; run in a loop

times 510 -( $ - $$ ) db 0	; When compiled, our program must fit 
			        ; into 512 bytes, with the last two bytes 
				; being the magic number, so here, tell our 
				; assembly compiler to pad out our program 
				; with enough zero bytes (db 0) to bring us 
				; to the 510th byte

dw 0xaa55			; Last two bytes (one word) form the magic 
				; number, so BIOS knows we are a boot sector

