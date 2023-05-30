section .text
    extern isr0
    extern isr1
    extern isr2

    global idt_init

idt_init:
    ; Set up IDT entries
    mov eax, idt_entry  ; Base address of IDT
    mov edx, 256       ; Number of entries in IDT

    xor ecx, ecx       ; Clear the counter

idt_entry_loop:
    mov edi, eax       ; Destination address for the entry
    mov [edi], ecx     ; Set the low word of the entry
    add edi, 4         ; Move to the high word of the entry
    mov [edi], ecx     ; Set the high word of the entry

    add eax, 8         ; Move to the next entry
    inc ecx            ; Increment the counter

    cmp ecx, edx       ; Check if all entries have been filled
    jl idt_entry_loop  ; If not, continue the loop

    ; Load IDT using lidt instruction
    mov eax, edx
    shl eax, 3         ; IDT limit << 3
    mov word [idt_ptr + 2], ax
    mov dword [idt_ptr], eax

    lidt [idt_ptr]
    ret

section .data
    idt_entry: times 256 dd 0 ; Define 256 double words (entries)
    idt_ptr:   dw 256*8-1, 0   ; IDT limit and base pointer
