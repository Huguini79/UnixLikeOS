BITS 32

section .multiboot

align 4
    dd 0x1BADB002
    dd 0x00
    dd - (0x1BADB002 + 0x00)

section .text

global start
extern kernel_init

start:
    cli
    call kernel_init
    hlt

section .bss
stack_space: resb 65536