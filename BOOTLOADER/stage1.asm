use16
org 0x7c00
jmp start
	include 'INC/gdt.inc'	


start:
	call create_gdt
	jmp $


times 510 - ($ - $$) db 0
dw 0xAA55