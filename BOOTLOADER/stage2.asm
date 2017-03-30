use16
org 0x7c00
jmp 0x0000:stage2_start

    include 'INC/a20.inc'
    include 'INC/gdt.inc'
    include 'INC/vesa.inc'
    include 'INC/mmap.inc'

stage2_start:
    call a20_activation
    call zero_his
    call run_in_vesa;be carefull and keep track of di now, since we are going to store memory map output into it
    call memory_map
    call create_gdt
    
    ;since other calls might modify GDT (for example VESA)
    ;make sure to call create_gdt as last call before 32-bit mode
    cli
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    lea ebx, [$+5]
    jmp dword 0x8:e_o_s2 ;;size of this instruction is 5 bytes



zero_his: ;we will create a hardware info structure, which will contain:
;1. Video mode settings- screen width, framebuffer addr, pitch, scr. height and color depth
;2. All valid returns from memory map e820 call, unosrted, the list will be finished by 
;SYS_EOIS string, which will be added in the mmap.inc file
;this function will just zero memory arund 0x500, just in case
    cld
    mov di, 0x500
    mov al, 0
    mov cx, 0x1000
    rep stosb
    ret


  ;;  include 'INC/gdt64.inc'

e_o_s2:
    use32
    mov ax, 0x10 
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov eax, [0x500+8];framebuffer address
    mov ecx, 1280*720
    .l:
    mov dword [eax+4*ecx], 0x0000ff00
    dec ecx
    cmp ecx, 0
    jg .l
    ;if screen gets green - we are in real mode!
           

    jmp $
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

 