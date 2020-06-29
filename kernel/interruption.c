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

    /* Debug */
        // puts("data[1:0] : ");
        // puth((unsigned long long) *(data+1), 2);
        // puth((unsigned long long) *(data), 2);
        // puts("\n");
        // puts("data[9:2] : ");
        // for (int i=9; i>1; i--) puth((unsigned long long) *(data+i), 2);
        // puts("\n");
        // puts("IDT Addre : ");
        // puth((unsigned long long) IDT, 16);
        // puts("\n");

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

    unsigned char idx = 32;
    unsigned long long handler;
    asm volatile ("lea lapic_intr_handler(%%rip), %[handler]" : [handler]"=r"(handler));
    unsigned short reg16;
    asm volatile ("mov %%cs, %0" : "=r"(reg16));
    unsigned short attr = 0b1000111000000000;

    register_intr_handler(idx, handler, reg16, attr);

    /* Debug */
        // puts("handler address : ");
        // puth((unsigned long long) handler, 16);
        // puts("\noffset : ");
        // puth(IDT[idx].offset_hi, 8);
        // puth(IDT[idx].offset_mid, 4);
        // puth(IDT[idx].offset_lo, 4);
        // puts("\nreg16 : ");
        // puth(reg16, 4);
        // puts("\nsegment : ");
        // puth(IDT[idx].segment, 4);
        // puts("\nattr : ");
        // puth(attr, 4);
        // puts("\nattribute : ");
        // puth(IDT[idx].attribute, 4);
        // puts("\n");

    asm volatile ("sti");

    return;
}
