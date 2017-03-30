gdt64_install:
    lgdt[GDT64.Pointer]
    ret

GDT64:               
    .Null: 
    dw 0             
    dw 0             
    db 0             
    db 0             
    db 0             
    db 0             
    .Code:
    dw 0             
    dw 0             
    db 0             
    db 10011010b     
    db 00100000b     
    db 0             
    .Data:
    dw 0             
    dw 0             
    db 0             
    db 10010010b     
    db 00000000b     
    db 0             
    .Pointer:        
    dw $ - GDT64 - 1 
    dq GDT64         