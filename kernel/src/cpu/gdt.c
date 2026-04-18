#include <cpu/gdt.h>
#include <stdint.h>

extern void gdt_flush(uint64_t gdtr);

constexpr int GDT_MAX_ENTRIES = 3;

struct __attribute__((packed)) gdt_entry {
        uint16_t        limit_low;
        uint16_t        base_low;
        uint8_t         base_mid;
        uint8_t         access;
        uint8_t         gran;
        uint8_t         base_high;
};

struct __attribute__((packed)) gdtr {
        uint16_t limit;
        uint64_t base;
};

static struct gdt_entry gdt[GDT_MAX_ENTRIES];

static void gdt_set_entry(int n, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
        gdt[n].limit_low        = limit & 0xffff;
        gdt[n].base_low         = base & 0xffff;
        gdt[n].base_mid         = (base >> 16) & 0xff;
        gdt[n].access           = access;
        gdt[n].gran             = ((limit >> 16) & 0x0f) | (flags & 0xf0);
        gdt[n].base_high        = (base >> 24) & 0xff; 
}

void gdt_init()
{
        struct gdtr gdt_ptr = {
                .limit = sizeof(gdt) - 1,
                .base = (uint64_t)&gdt
        };

        gdt_set_entry(0, 0, 0, 0, 0);
        gdt_set_entry(1, 0, 0, 0x9a, 0xa0);
        gdt_set_entry(2, 0, 0, 0x92, 0x80);

        gdt_flush((uint64_t)&gdt_ptr);
}