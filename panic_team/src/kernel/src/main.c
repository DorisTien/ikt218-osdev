#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "gdt.h"
#include "pit.h"
#include "monitor.h"
#include <stdio.h>
#include "system.h"
#include "idt.h"
#include "malloc.h"
#define SERIAL_PORT 0x3F8



extern uint32_t end;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
// Overload the new operator for single object allocation
void* operator new(size_t size) {
    return malloc(size);
}

// Overload the delete operator for single object deallocation
void operator delete(void* ptr) noexcept {
    free(ptr);
}

// Overload the new operator for array allocation
void* operator new[](size_t size) {
    return malloc(size);
}

// Overload the delete operator for array deallocation
void operator delete[](void* ptr) noexcept {
    free(ptr);
}
#endif

// PIT frequency
#define PIT_FREQUENCY 1193180

// Function to sleep for a specified number of milliseconds using busy waiting
void sleep(uint32_t milliseconds) {
    // Calculate the number of PIT ticks needed
    uint32_t pitTicks = (PIT_FREQUENCY * milliseconds) / 1000;

    // Read the current PIT counter value
    uint16_t initialCounter = pit_get_counter();

    // Calculate the target PIT counter value
    uint16_t targetCounter = initialCounter + pitTicks;

    // Busy wait until the target counter value is reached
    while (pit_get_counter() < targetCounter) {
        // Perform idle operations to prevent excessive CPU usage
        asm volatile("hlt");
    }
}

void kernel_main() {
    // Initialize terminal interface 
    monitor_initialize();

    // Newline support is left as an exercise. 
    const char* text = "Hello, kernel World!\n";
    uint32_t l = 22;
    monitor_write(text, l);
    init_kernel_memory(&end);


    // Initialize Global Descriptor Table (GDT)
    init_gdt();

    // Initialize Interrupt Descriptor Table (IDT)
    init_idt();

    // Initialize Interrupt Requests (IRQs)
    init_irq();


      // Initialize Paging
    init_paging(); // <------ THIS IS PART OF THE ASSIGNMENT

    // Print memory layout
    print_memory_layout();

    // Setup PIT
    init_pit();

    // Allocate some memory using kernel memory manager
    void* some_memory = malloc(12345);
    void* memory2 = malloc(54321);
    void* memory3 = malloc(13331);
    char* memory4 = (char*)malloc(1000);

    //TEST OF TASK 2 ASSIGNMENT4
    int counter = 0;
 while(true){
        printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
        sleep_busy(1000);
        printf("[%d]: Slept using busy-waiting.\n", counter++);

        printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
        sleep_interrupt(1000);
        printf("[%d]: Slept using interrupts.\n", counter++);
 };
}



//printf
#include <stdarg.h>
#include "common.h"



void print(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    while (*format != '\0')
    {
        if (*format == '%')
        {
            format++; // Move past '%'

            // Handle the format specifier
            switch (*format)
            {
                case 'c':
                {
                    // Print a character
                    char c = va_arg(args, int);
                    putchar(c);
                    break;
                }
                case 's':
                {
                    // Print a string
                    char* str = va_arg(args, char*);
                    while (*str != '\0')
                    {
                        putchar(*str);
                        str++;
                    }
                    break;
                }
                // Add support for other format specifiers as needed

                default:
                    // If an unsupported specifier is encountered, simply print '%'
                    putchar('%');
                    putchar(*format);
                    break;
            }
        }
        else
        {
            // Regular character, just print it
            putchar(*format);
        }

        format++;
    }

    va_end(args);
}




