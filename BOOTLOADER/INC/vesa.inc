fb_addr_to_eax:
    mov eax, [FinalStruct.FrameBuffer]
    ret

get_pixel_position:
    ;ax <- X position
    ;bx <- Y position
    ;ecx -> result
    push ebp
    mov ebp, esp
    push eax
    mov eax, ebx
    xor edx, edx
    mov dx, [FinalStruct.Width]
    mul edx
    mov edx, 4
    mul edx
    mov ebx, eax
    pop eax
    xor edx, edx
    mov byte dl, [FinalStruct.ColorDepth]
    mul edx
    mov ecx, 8
    div ecx
    mov dword ecx, [FinalStruct.FrameBuffer]
    add ecx, eax
    add ecx, ebx
    mov esp, ebp
    pop ebp
    ret 

run_in_vesa:
    mov ax, 0x4F00
    mov di, VBEInfoBlock
    push es
    int 0x10
    pop es
    cmp ax, 0x004F
    jne .error
    cld
    mov si, [VBEInfoBlock.VideoModes]
    .count_start:
        lodsw
        cmp ax, 0xFFFF ; if this, end of supported video modes
        je .error
            mov bx, ax
            mov cx, ax
            mov ax, 0x4F01
            mov di, VBEModeInfoStructure
            push si
            int 0x10
            pop si
            cmp ax, 0x004F
            jne .error
            ;we want linear frame buffer
            xor eax, eax
            mov al, [VBEModeInfoStructure.Attributes]
            shr eax, 7
            cmp al, 1
            jne .not_wanted
            ;we want 1280 pixels screen width
            mov ax, [VBEModeInfoStructure.Width]
            cmp ax, 1280
            jne .not_wanted
            ;720 height
            mov ax, [VBEModeInfoStructure.Height]
            cmp ax, 720
            jne .not_wanted
            ;and 32 bit color depth
            mov al, [VBEModeInfoStructure.BPP]
            cmp al, 32
            jne .not_wanted
            ;set bit 14 for linear framebuffer
            mov edx,  [VBEModeInfoStructure.FrameBuffer]
            mov dword [FinalStruct.FrameBuffer], edx
            mov dx, [VBEModeInfoStructure.Pitch]
            mov [FinalStruct.Pitch], dx
            mov dx, [VBEModeInfoStructure.Width]
            mov [FinalStruct.Width], dx
            mov dx, [VBEModeInfoStructure.Height]
            mov [FinalStruct.Height], dx
            mov dl, [VBEModeInfoStructure.BPP]
            mov [FinalStruct.ColorDepth], dl
            or bx, 0x4000
            jmp .count_finish
    .not_wanted:
        jmp .count_start
    .count_finish:
        mov ax, 0x4F02
        push es
        int 0x10
        pop es
        cmp ax, 0x004F
        jne .error
        ;now we will save our videomode information to 0x500
        ;HIS for short (Hardware Info Structure)
        mov di, 0x500
        mov ecx, 0

        .ls:
            mov al, [HIS_BGIN+ecx]
            cmp al, 0x00
            je .ous
            stosb
            inc cx
            jmp .ls
        .ous:
        
        mov eax, [FinalStruct.FrameBuffer]
        stosd
        mov ax, [FinalStruct.Pitch]
        stosw
        mov al, [FinalStruct.ColorDepth]
        stosb
        mov ax, [FinalStruct.Width]
        stosw
        mov ax, [FinalStruct.Height]
        stosw
        mov ecx, 0
        .ls2:
            mov al, [MEM_DATA+ecx]
            cmp al, 0x00
            je .ous2
            stosb
            inc cx
            jmp .ls2
        .ous2:
        ret


    .error:
        mov bx, 0xb800
        mov es, bx	
        xor di, di
        mov al, ' '
        mov ah, 0xf0
        mov ecx, 80*25
        rep stosw
        cli
        hlt
        jmp $


HIS_BGIN: db "HIS_BGIN", 0x00
MEM_DATA: db "MEM_DATA", 0x00

;this structure will be filled by INT call
;initialising every label so later they can be easily accesed
VBEInfoBlock:
    .Signature: db "VBE2"; four bytes(chars in C), - to be changed to "VESA"" by INT
    .Version: dw 0
    .OEM: dd 0
    .Capabilities: dd 0
    .VideoModes: dd 0
    .VideoMemory: dw 0
    .SoftwareRev: dw 0
    .Vendor: dd 0
    .ProductName: dd 0
    .ProductRev: dd 0
    .Reserved: db 222 dup(0)
    .OemData: db 256 dup(0)

;okay its really easier to declare it by hand than
;to calculate the address 
VBEModeInfoStructure:
    .Attributes: dw 0 ; bit 7 indicates linear frame buffer support
    .WindowA: db 0
    .WindowB: db 0
    .Granularity: dw 0
    .WindowSize: dw 0
    .SegmentA: dw 0
    .SegmentB: dw 0
    .WinFuncPtr: dd 0
    .Pitch: dw 0 ; bytes for each horizontal line
    .Width: dw 0 ; width in pixels
    .Height: dw 0 ; height in pixels
    .WChar: db 0
    .Ychar: db 0
    .Planes: db 0
    .BPP: db 0   ; bits per pixel
    .Banks: db 0
    .MemoryModel: db 0
    .BankSize: db 0
    .ImagePages: db 0
    .Reserved0: db 0

    .RedMask: db 0
    .RedPosition: db 0
    .GreenMask: db 0
    .GreenPosition: db 0
    .BlueMask: db 0
    .BluePosition: db 0
    .ReservedMask: db 0
    .ReservedPosition: db 0
    .DirectColorAttributes: db 0

    .FrameBuffer: dd 0 ;address of where to write
    .OffScreenMemOff: dd 0
    .OffScreenMemSize: dw 0
    .Reserved1: db 206 dup(0)

FinalStruct:
    .FrameBuffer: dd 0
    .Pitch: dw 0
    .ColorDepth: db 0
    .Width: dw 0
    .Height: dw 0