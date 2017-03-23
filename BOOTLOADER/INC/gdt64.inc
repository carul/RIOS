gdt64_install:
    lgdt[GDT_64_STRUCT]
    ret

GDT_64_STRUCT:
    dw GDT_64_END-GDT_64_BEGIN-1
    dd GDT_64_BEGIN



GDT_64_BEGIN:

    dq 0

;ring 0 ; code
    dd 0xffff
    db 0;base
    db 10011110b;acces
    db 10101111b;granularity
    db 0;base
        ;data
    dd 0xffff
    db 0;base
    db 10010010b;acces
    db 10101111b;granularity
    db 0;base

;ring 3 ; code
    dd 0xffff
    db 0;base
    db 11110000b;acces
    db 10101111b;granularity
    db 0;base
        ;data
    dd 0xffff
    db 0;base
    db 11110000b;acces
    db 10101111b;granularity
    db 0;base

GDT_64_END:
