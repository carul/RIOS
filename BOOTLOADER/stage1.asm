use16
org 0x7c00
STACK_START equ 0x7a00
SHIFT equ 0x200
jmp far 0x0000:start
start:
	mov sp, STACK_START
	xor ax ,ax
	mov ds, ax 
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov [0x2000], dl

	mov si, 0x7c00;copy this code 512 bytes "before" (SHIFT)
	mov di, 0x7a00
	mov ecx, SHIFT
	cld
	rep movsb


include 'INC/exread.inc'

	jmp $

times 510 - ($ - $$) db 0
dw 0xAA55
