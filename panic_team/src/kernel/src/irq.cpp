#include "interrupts.h"
#include "common.h"


// IRQ handler structure
typedef struct {
    isr_t handler;   // IRQ handler function
    void* data;      // Context data for the handler
    int num;         // IRQ number
} irq_handler_t;

// Array to hold the IRQ handlers
irq_handler_t irq_handlers[IRQ_COUNT];

extern "C" void irq_handler(registers_t regs) asm("irq_handler");

// Initialize IRQ handlers
void init_irq() {
    for (int i = 0; i < IRQ_COUNT; i++) {
        irq_handlers[i].data = NULL;
        irq_handlers[i].handler = NULL;
        irq_handlers[i].num = i;
    }
}

// Register an IRQ handler
void register_irq_handler(int irq, isr_t handler, void* ctx) {
    irq_handlers[irq].handler = handler;
    irq_handlers[irq].data = ctx;
}

// The main IRQ handler
// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs) {
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (regs.int_no >= 40) {
        // Send reset signal to slave.
        outb(0xA0, 0x20);
    }
    // Send reset signal to master (as well as slave, if necessary).
    outb(0x20, 0x20);

    irq_handler_t intrpt = irq_handlers[regs.int_no];
    if (intrpt.handler != NULL) {
        intrpt.handler(&regs);
    }
}
