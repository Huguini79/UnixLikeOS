BITS 32

extern page_fault
extern double_fault

global page_fault_handler_asm
global double_fault_handler_asm

page_fault_handler_asm:
    pushad
    call page_fault
    popad
    iretd

double_fault_handler_asm:
    pushad
    call double_fault
    popad
    iretd