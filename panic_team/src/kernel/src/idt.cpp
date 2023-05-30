#include "gdt.h"
#include "idt.h"
#include "common.h"
//https://github.com/perara-lectures/ikt218-osdev/blob/master/group_per-arne/src/kernel/src/cpu/idt.cpp

#define IDT_ENTRIES 256

extern "C" void idt_flush(uint32_t);

//Step 1: Define the Interrupt Descriptor Table and its pointer header files
struct idt_entry_t {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
};

struct idt_ptr_t {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct idt_entry_t idt[IDT_ENTRIES];
struct idt_ptr_t idt_ptr;

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    if (num >= IDT_ENTRIES) {
        // Handle out-of-bounds index error
        return;
    }

    idt[num].offset_low = base & 0xFFFF;
    idt[num].offset_high = (base >> 16) & 0xFFFF;
    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].type_attr = flags;
}

void idt_load();

void init_idt() {
    // Set the IDT limit and base address
    idt_ptr.limit = sizeof(struct idt_entry_t) * IDT_ENTRIES - 1;
    idt_ptr.base = reinterpret_cast<uint32_t>(&idt);

    // Initialize all IDT entries to default values

    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    // Load the IDT
    idt_load();
}

void idt_load() {
    // Load the IDT using the LIDT instruction
    asm volatile("lidt %0" : : "m" (idt_ptr));
}


