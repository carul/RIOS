a20_activation:
        mov ax, 0x2403
        int 0x15
        jb .other
        cmp ah, 0
        jnz .other
        ;;;;
        mov ax, 0x2402
        int 0x15
        jb .other
        cmp ah, 0
        jb .other
    
        cmp al, 1
        jz .activated

        mov ax, 0x2401
        int 0x15
        jb .other
        cmp ah, 0
        jnz .other
        jmp .activated

    .other:
        in al, 0x92
        test al, 2
        jnz .activated
        or al, 2
        and al, 0xFE
        out 0x92, al

    .activated:
    ret