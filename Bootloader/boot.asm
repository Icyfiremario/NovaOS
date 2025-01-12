[BITS    16]
[ORG 0x7C00]

%define KERNEL 0x7E00

BootMain:
        ;Setup Data
        xor     ax, ax
        mov     ds, ax
        mov     es, ax

        ;Setup Stack
        mov     sp, 0x7C00
        mov     ss, ax

        ;Setup VESA (640x480 8bpp)
        mov     ax, 0x4F02
        mov     bx, 0x4101
        int     0x10

        call    ReadSectors
        call    SetupGDT

        jmp     CODESEG:pmode

;---------------------------------------

;I tried optimizing this part
;However I couldn't get it...
;If you solve it make a pull request!
ReadSectors:
        mov     ah, 0x42
        mov     si, DAP
        int     0x13

        mov     bx, (KERNEL + 127 * 512) >> 4
        mov     word [DAP + 6], bx
        mov     word [DAP + 8], 128

        mov     ah, 0x42
        mov     si, DAP
        int     0x13

        mov     bx, (KERNEL + (127 * 2) * 512) >> 4
        mov     word [DAP + 6], bx
        add     word [DAP + 8], 127

        mov     ah, 0x42
        mov     si, DAP
        int     0x13

        mov     bx, (KERNEL + (127 * 3) * 512) >> 4
        mov     word [DAP + 6], bx
        add     word [DAP + 8], 127

        mov     ah, 0x42
        mov     si, DAP
        int     0x13

        mov     bx, (KERNEL + (127 * 4) * 512) >> 4
        mov     word [DAP + 6], bx
        add     word [DAP + 8], 127

        mov     ah, 0x42
        mov     si, DAP
        int     0x13

        mov     bx, (KERNEL + (127 * 5) * 512) >> 4
        mov     word [DAP + 6], bx
        add     word [DAP + 8], 127

        mov     ah, 0x42
        mov     si, DAP
        int     0x13

        mov     bx, (KERNEL + (127 * 6) * 512) >> 4
        mov     word [DAP + 6], bx
        add     word [DAP + 8], 127

        mov     ah, 0x42
        mov     si, DAP
        int     0x13

        mov     bx, (KERNEL + (127 * 7) * 512) >> 4
        mov     word [DAP + 6], bx
        add     word [DAP + 8], 127

        mov     ah, 0x42
        mov     si, DAP
        int     0x13

        mov     bx, (KERNEL + (127 * 8) * 512) >> 4
        mov     word [DAP + 6], bx
        add     word [DAP + 8], 127

        mov     ah, 0x42
        mov     si, DAP
        int     0x13

        mov     bx, (KERNEL + (127 * 9) * 512) >> 4
        mov     word [DAP + 6], bx
        add     word [DAP + 8], 127

        mov     ah, 0x42
        mov     si, DAP
        int     0x13

        ret

DAP:
        db      0x10        ;DAP     byte [dap+0]
        db      0x00        ;Unused  byte [dap+1]
        dw      0x7F        ;Sectors byte [dap+2]
        dw      0x0000      ;Segment byte [dap+3] byte [dap+4]
        dw      KERNEL >> 4 ;Offset  word [DAP+6]
        dq      1           ;LBA      word [DAP+8]

;---------------------------------------

%include "Bootloader/gdt.asm"
%include "Bootloader/pmode.asm"

times 510 - ($ - $$) db 0x00
dw 0xAA55