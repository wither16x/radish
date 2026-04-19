bits 64

; Generate a stub for an ISR which has an error code
%macro isr_err_stub 1
align 16
global __asm_isr%1
__asm_isr%1:
        push %1
        jmp __isr_common
%endmacro

; Generate a stub for an ISR which has no error code
%macro isr_noerr_stub 1
align 16
global __asm_isr%1
__asm_isr%1:
        push 0
        push %1
        jmp __isr_common
%endmacro

section .text
extern isr_handle

isr_noerr_stub  0               ; division by zero
isr_err_stub    13              ; general protection fault
isr_err_stub    14              ; page fault

__isr_common:
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rbp
    push rdi
    push rsi
    push rdx
    push rcx
    push rbx
    push rax
    mov rax, cr3
    push rax
    mov rax, cr2
    push rax
    mov rdi, rsp
    call isr_handle
    pop rax
    pop rax
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop r15
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
    add rsp,16
    iretq