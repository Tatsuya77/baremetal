/*
 * interruption.c
 */

#include "hardware.h"
#include "util.h"
#include "interruption.h"

struct InterruptDescriptor
{
    unsigned short offset_lo;
    unsigned short segment;
    unsigned short attribute;
    unsigned short offset_mid;
    unsigned int offset_hi;
    unsigned int reserved;
}__attribute__((packed));

/* zero cleared */
struct InterruptDescriptor IDT[256];

static void load_ldt_to_ldtr(void)
{
    unsigned char data[10];

    /* 0xFFF bytes */
    data[0] = 0b11111111;
    data[1] = 0b00001111;

    /* IDT Address */
    for (int i=2; i<10; i++)
    {
        unsigned char x = 0;

        for (int j=0; j<8; j++)
        {
            if (((unsigned long long) IDT) >> ((i-2)*8 + j) & 1) x += (1 << j);
        }

        data[i] = x;
    }

    asm volatile ("lidt %0" :: "m"(data));
    return;
}

static void
register_intr_handler(unsigned char index, unsigned long long offset, unsigned short segment, unsigned short attribute)
{
    struct InterruptDescriptor *id = &(IDT[index]);

    id->offset_lo = (unsigned short) offset;
    id->offset_mid = (unsigned short) (offset >> 16);
    id->offset_hi = (unsigned int) (offset >> 32);
    id->segment = segment;
    id->attribute = attribute;

    return;
}

void init_intr(void)
{
    load_ldt_to_ldtr();

    /* lapic_intr_handler */
    unsigned char idx = 32;
    unsigned long long handler;
    asm volatile ("lea lapic_intr_handler(%%rip), %[handler]" : [handler]"=r"(handler));
    unsigned short reg16;
    asm volatile ("mov %%cs, %0" : "=r"(reg16));
    unsigned short attr = 0b1000111000000000;
    register_intr_handler(idx, handler, reg16, attr);

    /* syscall_handler */
    idx = 0x80;
    asm volatile ("lea syscall_handler(%%rip), %[handler]" : [handler]"=r"(handler));
    // asm volatile ("mov %%cs, %0" : "=r"(reg16));
    register_intr_handler(idx, handler, reg16, attr);

    asm volatile ("sti");

    return;
}
