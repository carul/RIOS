use16
org 0x7c00

jmp far 0x0000:start

start:
	mov ax, 0x2000
	mov es, ax
	xor bx, bx 

	;preparing to read stage 2 sectors
	mov ah, 2
	mov al, 72 ;read 3 sectors
	mov ch, 0
	mov cl, 2
	mov dh, 0
	;dl set

	
	int 0x13
	
	jmp far 0x2000:0x0000


times 510 - ($ - $$) db 0
dw 0xAA55

