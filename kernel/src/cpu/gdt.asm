bits 64

section .text
global gdt_flush

gdt_flush:
        lgdt [rdi]                      ; load the GDT
        push 0x08                       ; push value of CS
        lea rax, [rel .reload_cs]
        push rax
        retfq
.reload_cs:
; Reload segments
        mov ax, 0x10
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax
        ret