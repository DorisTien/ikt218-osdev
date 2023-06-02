#include "gdt.h"
#include "idt.h"
#include "interrupts.h"
#include "common.h"
#include <cstdlib>
#include <stdio.h>
#include "system.h"
#include "memory.h"
#include "common.h"
#include <cstdlib>
#include "../screen/screen.h"
#include <monitor.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "keyboard.h"
#include "isr.h"
#define SERIAL_PORT 0x3F8
extern uint32_t end; // This is defined in linker.ld

// Overload the new operator for single object allocation
void *operator new(std::size_t size)
{
    return malloc(size); // Call the C standard library function malloc() to allocate memory of the given size and return a pointer to it
}

// Overload the delete operator for single object deallocation
void operator delete(void *ptr) noexcept
{
    free(ptr); // Call the C standard library function free() to deallocate the memory pointed to by the given pointer
}

// Overload the new operator for array allocation
void *operator new[](std::size_t size)
{
    return malloc(size); // Call the C standard library function malloc() to allocate memory of the given size and return a pointer to it
}

// Overload the delete operator for array deallocation
void operator delete[](void *ptr) noexcept
{
    free(ptr); // Call the C standard library function free() to deallocate the memory pointed to by the given pointer
}

// Define entry point in asm to prevent C++ mangling
extern "C"
{
#include <system.h>
#include "memory.h"
#include "monitor.h"
#include "keyboard.h"
#include "isr.h"
    void kernel_main();
}
class OperatingSystem
{
    int tick = 0;

public:
    OperatingSystem(vga_color color)
    {
    }

    void init()
    {

        monitor_write(0xA, "Initializing UiA Operating System....");
        move_cursor_next_line();
    }

    void debug_print(char *str)
    {
        monitor_write(0xE, str);
        move_cursor_next_line();
    }

    void interrupt_handler_3(registers_t regs)
    {
        monitor_write(0xE, "Called Interrupt Handler 3!");
        move_cursor_next_line();
    }
    void interrupt_handler_4(registers_t regs)
    {
        monitor_write(0xE, "Called Interrupt Handler 4!");
        move_cursor_next_line();
    }

    void timer()
    {
        tick++;
        if (tick % 100 == 0)
        {
            monitor_write(0xE, "(Every Second) Tick: ");
            print_int(tick);
            move_cursor_next_line();
        }
    }
};

void demonstrate_page_fault()
{

    void (*callback)(registers_t &) = [](registers_t &regs)
    {
        // A page fault has occurred.
        // The faulting address is stored in the CR2 register.
        uint32_t faulting_address;
        asm volatile("mov %%cr2, %0"
                     : "=r"(faulting_address));

        // The error code gives us details of what happened.
        int present = !(regs.err_code & 0x1); // Page not present
        int rw = regs.err_code & 0x2;         // Write operation?
        int us = regs.err_code & 0x4;         // Processor was in user-mode?
        int reserved = regs.err_code & 0x8;   // Overwritten CPU-reserved bits of page entry?
        int id = regs.err_code & 0x10;        // Caused by an instruction fetch?

        // Output an error message.
        printf("Page fault! ( ");
        if (present)
        {
            printf("present ");
        }
        if (rw)
        {
            printf("read-only ");
        }
        if (us)
        {
            printf("user-mode ");
        }
        if (reserved)
        {
            printf("reserved ");
        }
        printf(") at 0x%x", faulting_address);
        printf("\n");
        // panic("Page fault");
    };

    // Create interrupt handler for interrupt 14, which is page fault
    register_interrupt_handler(
        14, [](registers_t *regs, void *context)
        {
        void (*callback)(registers_t*) = reinterpret_cast<void (*)(registers_t*)>(context);
        callback(regs);

        printf("Page Fault!\n"); },
        (void *)callback);

    uint32_t *ptr = (uint32_t *)0xA0000000;
    uint32_t do_page_fault = *ptr;
}

void kernel_main()
{
    // Initialize kernel memory manager with the end of the kernel image
    // init_kernel_memory(&end);

    auto os = OperatingSystem();
    os.init();

    monitor_write(0xF, "Hello, Per Arne!");

    // monitor_initialize();

    // Newline support is left as an exercise.
    const char *text = "Hello, kernel World!\n";
    uint32_t l = 22;
    // monitor_write(text, l);
    // init_kernel_memory(&end);

    // Initialize Global Descriptor Table (GDT)
    init_gdt();

    // Initialize Interrupt Descriptor Table (IDT)
    init_idt();

    // Initialize Interrupt Requests (IRQs)
    init_irq();

    // Initialize Paging
    // init_paging();

    // Print memory layout
    // print_memory_layout();

    // Allocate some memory using kernel memory manager
    void *some_memory = malloc(12345);
    void *memory2 = malloc(54321);
    void *memory3 = malloc(13331);
    char *memory4 = new char[1000]();

    // demonstrate_page_fault();

    // Create interrupt handlers for interrupt 3 and 4
    register_interrupt_handler(
        3, [](registers_t *regs, void *context)
        { printf("Interrupt 3 - OK\n"); },
        NULL);

    register_interrupt_handler(
        4, [](registers_t *regs, void *context)
        { printf("Interrupt 4 - OK\n"); },
        NULL);

    // Trigger interrupts 3 and 4 which should call the respective handlers
    asm volatile("int $0x3");
    asm volatile("int $0x4");

    // Disable interrupts temporarily
    asm volatile("sti");

    // Create an IRQ handler for IRQ1
    register_irq_handler(
        IRQ1, [](registers_t *, void *)
        {
        printf("Yes we are going on!\n");

        // Read the scan code from keyboard
       // unsigned char scan_code = inb(0x60);

        // Disable interrupts temporarily
        asm volatile("cli"); },
        NULL);

    UiAOS::IO::Keyboard::hook_keyboard([](uint8_t scancode, void *context)
                                       { auto *os = (OperatingSystem *)context; },
                                       &os);

    // Print a message and enter an infinite loop to wait for interrupts
    printf("Waiting...\n");
    while (1)
    {
    };
    printf("Done!...\n");
}