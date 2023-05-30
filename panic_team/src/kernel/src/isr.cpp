#include <iostream>
#include <stdint.h>
#include "interrupts.h"

typedef struct {
    isr_t handler;
    void* data;
} int_handler_t;

int_handler_t int_handlers[256];

void ISR1(registers_t* regs, void* context)
{
    std::cout << "ISR1: Interrupt 1 triggered." << std::endl;
    // ISR1 implementation here
}

void ISR2(registers_t* regs, void* context)
{
    std::cout << "ISR2: Interrupt 2 triggered." << std::endl;
    // ISR2 implementation here
}

void ISR3(registers_t* regs, void* context)
{
    std::cout << "ISR3: Interrupt 3 triggered." << std::endl;
    // ISR3 implementation here
}

void register_interrupt_handler(uint8_t n, isr_t handler, void* context)
{
    int_handlers[n].handler = handler;
    int_handlers[n].data = context;
}

void isr_handler(registers_t* regs)
{
    uint8_t int_no = regs->int_no & 0xFF;
    int_handler_t intrpt = int_handlers[int_no];
    if (intrpt.handler != nullptr)
    {
        intrpt.handler(regs);
    }
    else
    {
        std::cout << "Unhandled interrupt: " << int_no << std::endl;
        // Handle unhandled interrupts here
    }
}
