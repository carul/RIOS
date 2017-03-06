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
	mov eax, (PAGING_LAYER4 - $$) + 0x20000
	mov cr3, eax

	mov eax, cr4
	or eax, 100000b
	mov cr4, eax

	mov ecx, 0xC0000080
	rdmsr
	or eax, 100000000b
	wrmsr

	mov eax, cr0
	or eax, 10000000000000000000000000000000b
	mov cr0, eax 
	
	lgdt [GDT_64_STRUCT+0x20000]
	jmp pword 0x08:(0x20000+code_64)

code_64:
	use64
	mov rsi, [0x20000 + kernel_entry + 0x20]
	add rsi, 0x20000 + kernel_entry
	movzx ecx, word [0x20000+kernel_entry+0x38]

	cld

	xor r14, r14

	.loadloop:
	mov eax, [rsi+0]
	cmp eax, 1
	jne .next
	mov r8, [rsi + 8]
	mov r9, [rsi + 0x10]
	mov r10, [rsi + 0x20]
	;;;=======
	test r14, r14
	jnz .skip
	mov r14, r9
	.skip:

	;;;-------
	mov rbp, rsi
	mov r15, rcx

	lea rsi, [0x20000 + kernel_entry + r8d]
	mov rdi, r9
	mov rcx, r10
	rep movsb

	mov rcx, r15
	mov rsi, rbp

	.next:
	add rsi, 0x20
	loop .loadloop

	mov rsp, 0x30f000

	mov rdi, r14
	mov rax, [0x20000 + kernel_entry + 0x18]
	call rax
	

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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

GDT_64_STRUCT:
dw GDT_64_END-GDT_64_BEGIN-1
dd 0x20000+GDT_64_BEGIN

GDT_64_BEGIN:
;null segment
dq 0

;code segment
dd 0xffff
dd 00000000101011111001101000000000b

;data segment
dd 0xffff
dd 00000000101011111001001000000000b
GDT_64_END:

times (4096 - ($-$$) mod 4096) db 0

PAGING_LAYER4:
dq (PAGING_LAYER3-$$ + 0x20000) or 1 or 10b
dq 511 dup(0)

PAGING_LAYER3:
dq 1 or 10b or 10000000b
dq 511 dup(0)


times (512- ($-$$) mod 512) db 0

kernel_entry: