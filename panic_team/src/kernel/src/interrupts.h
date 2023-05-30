#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <iostream>
#include <stdint.h>
#include "gdt.h"

#define ISR_COUNT 32
#define IRQ_COUNT 16

extern "C" {
    // ISR declarations
    extern void isr0();
    extern void isr1();
    extern void isr2();
    extern void isr3();
    // ...

    // IRQ declarations
    extern void irq0();
    extern void irq1();
    extern void irq2();
    extern void irq3();
    /*
    extern void irq4();
    extern void irq5();
    extern void irq6();
    extern void irq7();
    extern void irq8();
    extern void irq9();
    extern void irq10();
    extern void irq11();
    extern void irq12();
    extern void irq13();
    extern void irq14();
    extern void irq15();
    */
   
    // ...
}

// Interrupt Handler Structure
typedef struct registers {
    uint32_t ds;                  // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

// Interrupt Service Routine (ISR) handler type
typedef void (*isr_t)(registers_t*);

// Interrupt Request (IRQ) handler type
typedef void (*irq_t)(registers_t*);

// Function to initialize IRQs
void init_irq();

// Function to initialize interrupts
void init_interrupts();

// Function to register an ISR handler
void register_isr_handler(uint8_t isr, isr_t handler);

// Function to register an IRQ handler
void register_irq_handler(uint8_t irq, irq_t handler);

#endif
