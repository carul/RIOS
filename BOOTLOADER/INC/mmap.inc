SMAP equ 0x0534D4150

memory_map:
    pushad
    push ebp
    mov ebp, esp
    sub esp, 0x4
    mov dword [ebp-4], 0
    xor ebx, ebx    
        .e820first: 
            mov edx, SMAP
            mov eax, 0xe820
            xor ebx ,ebx
            mov [es:di+20], dword 1
            mov ecx, 24
            int 0x15
            jc .lerr
            mov edx, SMAP
            cmp eax, edx
            jne .lerr
            test ebx, ebx
            je .lerr
            jmp .testres
        .e820:
            mov eax, 0xe820
            mov [es:di + 20], dword 1 ; get acpi v3 entry if possible
            mov ecx, 24
            int 0x15
            jc .fin
            mov edx, SMAP
        .testres:
            jcxz .wrong
            cmp cl, 20
            jbe .good
            test byte [es:di + 20], 1
            je .wrong
        .good:
            mov ecx, [es:di + 8]
            or ecx, [es:di + 12]
            jz .wrong
            inc dword [ebp-4]
            add di, 24
        .wrong:
            test ebx, ebx
            jne .e820
    .fin:
    mov dword eax, [ebp-4]
    mov esp, ebp
    pop ebp
    ; valid entry count in eax
    ; di set to address of last 
    mov edx, eax
    xor ecx, ecx
        .ls:
            mov al, [ENTRY_NUM+ecx]
            cmp al, 0x00
            je .ous
            stosb
            inc cx
            jmp .ls
        .ous:
    
    mov eax, edx
    stosd
    
    xor ecx, ecx
        .ls2:
            mov al, [SYS_EOIS+ecx]
            cmp al, 0x00
            je .ous2
            stosb
            inc cx
            jmp .ls2
        .ous2:

    popad
    ret

    .lerr:
       nop
       nop
       jmp .lerr


ENTRY_NUM: db "ENTRY_NUM", 0x00
SYS_EOIS: db "SYS_EOIS", 0x00