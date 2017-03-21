SMAP equ 0x0534D4150

memory_map:
    pushad
    xor ebx, ebx
    .e820call:
        mov eax, 0xE820
        mov di, MemStruct
        mov [es:di+20], dword 1
        mov ecx, 24
        mov edx, SMAP
        int 0x15
        jc .fin
        cmp eax, SMAP
        jne .err
        cmp ebx, 0
        jz .fin
        cmp dword [MemStruct.RegionType], 1
        jne .e820call
    .entry:
        jcxz .skipentry
        cmp cl, 20
        jbe .validentry
        test byte [es:di+20], 1 ;test if ACPI 3 byte is set, if so, skip result
        je .skipentry
    .validentry:
        mov eax, dword [MemStruct.RegionLength]
        add eax, dword [MemStruct.RegionLength+4]
        cmp eax, 0 ; check if entry length is 0
        je .e820call
        add dword [MemSize], eax
        mov eax, dword [MemStruct.RegionLength+4];this will be higher memory
        cmp eax, 0
        je .e820call 
        add dword [MemSize+4], eax
        jmp .e820call
    .skipentry:
        cmp ebx, 0 ;list is finalised when ebx is zero
        jne .e820call

    .fin:
    popad
    mov eax, dword [MemSize]
    mov ebx, dword [MemSize+4]
    ret

    .err:
        .lerr:
            nop
            nop
            jmp .lerr

MemStruct:
    .BaseAddr: dq 0
    .RegionLength: dq 0
    .RegionType: dd 0
    .ACPIExt: dd 0

MemSize dq 0