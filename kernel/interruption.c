/*
 * interruption.c
 */

// #include "hardware.h"
// #include "util.h"
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

unsigned char data[10];

static void load_ldt_to_ldtr(void)
{
    // for (int i=0; i<16; i++)
    // {
    //     if ((sizeof(struct InterruptDescriptor)*256 - 1) >> i & 1) data[i] = 1;
    // }
    *((unsigned short *) data) = (unsigned short) sizeof(struct InterruptDescriptor)*256 - 1;
    // for (int i=16; i<80; i++)
    // {
    //     if (((unsigned long long) IDT) >> i & 1) data[i] = 1;
    // }
    *((unsigned long long *) data+2) = (unsigned long long) IDT;
    asm volatile ("lidt %0" :: "m"(data));
    return;
}

static void register_intr_handler(unsigned char index, unsigned long long offset, unsigned short segment, unsigned short attribute)
{
    struct InterruptDescriptor *id = &(IDT[index]);

    id->offset_lo = (unsigned short) offset;
    id->offset_mid = (unsigned short) (offset >> 16);
    id->offset_hi = (unsigned int) (offset >> 32);
    id->segment = segment;
    id->attribute = attribute;

    // puts("offset: ");
    // puth(offset, 64);
    // puts("\n");
    // puts("offset_lo: ");
    // puth(id->offset_lo, 64);
    // puts("\n");
    // puts("offset_mid: ");
    // puth(id->offset_mid, 64);
    // puts("\n");
    // puts("offset_hi: ");
    // puth(id->offset_hi, 64);
    // puts("\n");

    return;
}

void init_intr(void)
{
    // load_ldt_to_ldtr();

    unsigned char idx = 32;

    unsigned long long handler;
    asm volatile ("lea lapic_intr_handler(%%rip), %[handler]" : [handler]"=r"(handler));

    unsigned short reg16;
    asm volatile ("mov %%cs, %0" : "=r"(reg16));

    unsigned short attr = 0b1110111000000000;

    register_intr_handler(idx, handler, reg16, attr);

    asm volatile ("sti");

    return;
}
