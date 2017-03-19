use16
org 0x7c00
jmp 0x0000:stage2_start

    include 'INC/a20.inc'
    include 'INC/gdt.inc'
    include 'INC/vesa.inc'

stage2_start:
    call a20_activation
    call run_in_vesa
    call create_gdt
    ;since other calls might modify GDT (for example VESA)
    ;make sure to call create_gdt as last call before 32-bit mode
    mov eax, 512
    mov ebx, 368
    call get_pixel_position
    mov dword [ecx], 0x00ff00ff
    .lf:
        nop
        nop
        jmp .lf

    FRAMEBUFFER_ADR dd 0