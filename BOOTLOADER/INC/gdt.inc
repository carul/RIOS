;;always zero base



create_gdt: ;(num of gdt parts, )
;    call null_segment_byte_options
;    mov [GDT_32_BEGIN+2], eax
;    call code_segment_byte_options
;    mov [GDT_32_BEGIN+6], eax
;    call data_segment_byte_options
;    mov [GDT_32_BEGIN+10], eax
    lgdt [GDT_32_STRUCT] ;; gdt is now statically allocated
    ret
;
;setbit: ;gets from eax, returns in eax
;    ; bytes to move in bx
;    ; position to move in cx
;    shl ebx, cl ; position
;    xor eax, ebx
;    xor ebx, ebx
;    ret
;
;null_segment_byte_options:
;    xor eax, eax
;    ret
;
;code_segment_byte_options:
;    xor eax ,eax
;
;    mov cx, 0
;    mov bx, 0x0
;    call setbit
;
;    mov cx, 8 ; at position 8
;    mov bx, 10011010b ;; acces byte
;    call setbit
;    
;    mov cx, 16 ;pos 16
;    mov bx, 0xf
;    call setbit ;; limit higher
;
;    mov cx, 20
;    mov bx, 1100b ;;flags
;    call setbit
;    ret
;
;
;data_segment_byte_options:
;    xor eax ,eax
;
;    mov cx, 0
;    mov bx, 0x0
;    call setbit
;
;    mov cx, 8 ; at position 8
;    mov bx, 10010010b ;; acces byte
;    call setbit
;    
;    mov cx, 16 ;pos 16
;    mov bx, 0xf
;    call setbit ;; limit higher
;
;    mov cx, 20
;    mov bx, 1100b ;;flags
;    call setbit
;    ret


GDT_32_STRUCT:
    dw GDT_32_END-GDT_32_BEGIN-1
    dd GDT_32_BEGIN

GDT_32_BEGIN:
    dq 0

    dd 0xffff
    dd 0xcf9a00

    dd 0xffff
    dd 0xcf9200
GDT_32_END: