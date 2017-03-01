use16
org 0x0000


start:
	mov ax, 0x2000
	mov ds, ax
	mov es, ax

	mov ax, 0x1f00
	mov ss, ax
	xor sp, sp
	cli
	lgdt [GDT_32_STRUCT]
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	jmp pword 0x08:(0x20000+code_32)


;;32 bit code starts here

code_32:
	use32
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov ss, ax
	lea eax, [0xb8990]
	mov dword [eax], 0xff7Aff44
	mov dword [eax+4], 0xff61ff69
	mov dword [eax+8], 0xff61ff6c
l:
	jmp l

;;GLOBAL DESCRIPTOR TABLE;;

GDT_32_STRUCT:
dw GDT_32_END-GDT_32_BEGIN-1
dd 0x20000+GDT_32_BEGIN

GDT_32_BEGIN:
;null segment
dq 0

;code segment
dd 0xffff
dd 00000000110011111001101000000000b

;data segment
dd 0xffff
dd 00000000110011111001001000000000b
GDT_32_END:

;;END OF 32 BIT GDT;;
