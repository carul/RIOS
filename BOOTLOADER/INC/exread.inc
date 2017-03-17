SECTOREAD equ 1
jmp far 0x0000:(0x7c00-SHIFT+dap_read-start+4+1)

dap_read: 
	mov word [dap_struct.cnt-SHIFT], SECTOREAD ;read just one sector
	mov dword [dap_struct.dst-SHIFT], 0x7c00
	mov dword [dap_struct.srcl-SHIFT], 0x01 ;start from sector 1
	mov dword [dap_struct.srch-SHIFT], 0x00
	mov ah, 0x80
	mov si, dap_struct-SHIFT
	int 0x13
	jc ers
	push 0x7c00
	ret

dap_struct:
	.siz:	db 0x10
			db 0x00 ; unused
	.cnt:	dw 0x0000
	.dst:	dd 0x00000000
	.srcl:	dd 0x00000000
	.srch:	dd 0x00000000


ers: ;assume floppy
	mov ax, 0x0000
    mov es, ax
    mov bx, 0x7c00
    mov ah, 2  
    mov al, SECTOREAD
    mov ch, 0
    mov cl, 2 
    mov dh, 0
    int 13h
    jmp far 0x0000:0x7c00