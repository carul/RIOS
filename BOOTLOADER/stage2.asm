use16
org 0x7c00
jmp 0x0000:stage2_start

    include 'INC/a20.inc'
    include 'INC/gdt.inc'

stage2_start:
    call a20_activation
    call create_gdt
    mov bx, 0xb800
	mov es, bx	
    xor di, di
    mov al, ' '
    mov ah, 0x20
	mov ecx, 80*25
    rep stosw

jmp $