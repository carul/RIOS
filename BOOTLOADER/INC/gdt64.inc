gdt64_install:
    lgdt[GDT_addr]
    ret


    GDT_addr:
    dw (GDT64_end - GDT64) - 1
    dd GDT64

    GDT64:
    dd 0, 0

    dd 0xffff  ; segment limit
    dd 0xaf9a00

    dd 0xffff  ; segment limit
    dd 0xaf9200

    dd 0, 0
    GDT64_end:
