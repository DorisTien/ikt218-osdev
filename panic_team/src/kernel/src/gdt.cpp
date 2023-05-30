//
// Created by per on 1/1/23.
//
#include "gdt.h"
//#include "isr.h"
#include "idt.h"

namespace UiAOS {
namespace CPU {
namespace GDT {

    gdt_entry_t gdt_entries[6];
    gdt_ptr_t gdt_ptr;

    extern "C" {
        // Lets us access our ASM functions from our C code.
        void gdt_flush(uint32_t);
        static void gdt_set_gate(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);
    }

    void init_gdt()
    {
        gdt_ptr.limit = (sizeof(gdt_entry_t) * 6) - 1;
        gdt_ptr.base = reinterpret_cast<uint32_t>(&gdt_entries);

        gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
        gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
        gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
        gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
        gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

        gdt_flush(reinterpret_cast<uint32_t>(&gdt_ptr));
    }

} // namespace GDT
} // namespace CPU
} // namespace UiAOS

// Define gdt_set_gate function
void UiAOS::CPU::GDT::gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    // Set the descriptor base address
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    // Set the descriptor limits
    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    // Set the access flags
    gdt_entries[num].access = access;

    // Set the granularity and flags
    gdt_entries[num].granularity |= (gran & 0xF0);
}
