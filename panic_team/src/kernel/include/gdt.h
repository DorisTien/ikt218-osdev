#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#define GDT_ENTRIES 5

// Define the GDT entry structure
struct gdt_entry_t
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

// Define the GDT pointer structure
struct gdt_ptr_t
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Initialize the GDT
void init_gdt();

// Load the GDT
void gdt_load();

// Set a GDT entry
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

static gdt_entry_t gdt[GDT_ENTRIES];
static gdt_ptr_t gdt_ptr;

#endif // GDT_H
