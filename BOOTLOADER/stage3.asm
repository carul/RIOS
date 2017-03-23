;;this file will be used to setup 64-bit mode, paging ETC
jmp stage3

    include 'INC/gdt64.inc'

stage3:
    use32
    mov ax, 0x10 
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov eax, [0x500+8];framebuffer address
    mov ecx, 1280*720
    .l:
    mov dword [eax+4*ecx], 0x00ff00
    dec ecx
    cmp ecx, 0
    jg .l
    ;if screen gets green - we are in real mode!
    cli
    cld
    mov ecx, 0xC0000080
    rdmsr
    or eax, 100000000b
    wrmsr
    call gdt64_install
    ;;TODO: paging
    ;;not working yet
    jmp $
    ;;-------------
    jmp dword 0x8:longmode

longmode:
    jmp $

    use64
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp $

