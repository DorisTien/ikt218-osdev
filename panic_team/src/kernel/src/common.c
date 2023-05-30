#include "common.h"

#define SERIAL_PORT 0x3F8

void kernel_putchar(char c)
{
    outb(SERIAL_PORT, c);
}

void kernel_printf(const char* format, ...)
{
    // Implement your printf function here
    // You can use the provided kernel_putchar function to output characters
    // and handle the format string and variable arguments accordingly
}

void outb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

uint16_t inw(uint16_t port)
{
    uint16_t ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}
