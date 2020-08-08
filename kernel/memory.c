/*
 * memory.c
 */

#include "memory.h"

#define TASK_NUM 3

struct Entry {
    unsigned long long entry;
} __attribute__((packed));

extern unsigned long long task_cr3s[TASK_NUM];
extern unsigned long long kernel_cr3;

struct Entry PML4s[TASK_NUM][512]__attribute__((aligned(4096)));
struct Entry PDPs[TASK_NUM][512]__attribute__((aligned(4096)));
struct Entry PDs[TASK_NUM][512]__attribute__((aligned(4096)));
struct Entry PTs[TASK_NUM][8][512]__attribute__((aligned(4096)));

struct Entry kernel_PD[512]__attribute__((aligned(4096)));
struct Entry kernel_PTs[8][512]__attribute__((aligned(4096)));

struct Entry io_PD[512]__attribute__((aligned(4096)));
struct Entry fb_PT[512]__attribute__((aligned(4096)));
struct Entry lapic_PT[512]__attribute__((aligned(4096)));

void init_entry(struct Entry *entry, unsigned long long phys_addr) {
    phys_addr = phys_addr & 0xffffffffff;
    entry->entry = (phys_addr << 12) | 0b1111;
}

void init_virtual_memory() {
    asm volatile ("mov %%cr3, %0" : "=r"(kernel_cr3));

    for(unsigned int i=0; i<512; i++) {
        if (i == (unsigned int) (((unsigned long long)fb_PT >> 12) & 0x1ff))
            init_entry(&io_PD[i], ((unsigned long long)fb_PT >> 12));
        if (i == (unsigned int) (((unsigned long long)lapic_PT >> 12) & 0x1ff))
            init_entry(&io_PD[i], ((unsigned long long)lapic_PT >> 12));
    }
    for(unsigned int i=0; i<512; i++) {
        init_entry(&fb_PT[i], 0x0c0000 + i);
    }
    init_entry(&lapic_PT[0], 0x0fee00);

    for(unsigned int i=0; i<TASK_NUM; i++) {
        task_cr3s[i] = (unsigned long long)PML4s[i];

        for(unsigned int j=0; j<512; j++) {
            if (j == (unsigned int) ((0x040000000 >> 39) & 0x1ff))
                init_entry(&PML4s[i][j], (unsigned long long)PDPs[i]);

            if (j == (unsigned int) ((0x040000000 >> 30) & 0x1ff))
                init_entry(&PDPs[i][j], (unsigned long long)PDs[i]);
            if (j == (unsigned int) ((0x0c0000000 >> 30) & 0x1ff))
                init_entry(&PDPs[i][j], (unsigned long long)io_PD);
            if (j == (unsigned int) ((0x100000000 >> 30) & 0x1ff))
                init_entry(&PDPs[i][j], (unsigned long long)kernel_PD);

            for(unsigned int k=0; k<8; k++) {
                if (j == (unsigned int) (((0x040000000+k*(1<<30)) >> 30) & 0x1ff))
                    init_entry(&PDs[i][j], (unsigned long long)PTs[i][k]);
                if (j == (unsigned int) (((0x100000000+k*(1<<30)) >> 30) & 0x1ff))
                    init_entry(&kernel_PD[j], (unsigned long long)kernel_PTs[k]);

                init_entry(&PTs[i][k][j], (0x104000000 + i*0x1000000 + k*0x2000000 +j*0x10000) >> 12);
                init_entry(&kernel_PTs[k][j], (0x100000000 + k*0x2000000 +j*0x10000) >> 12);
            }
            if (j == (unsigned int) ((0x0c0000000 >> 21) & 0x1ff))
                init_entry(&io_PD[j], (unsigned long long)fb_PT);
            if (j == (unsigned int) ((0x0fee00000 >> 21) & 0x1ff))
                init_entry(&io_PD[j], (unsigned long long)lapic_PT);
            init_entry(&fb_PT[j], (0x0c0000000 +j*0x10000) >> 12);
            init_entry(&lapic_PT[j], (0x0fee00000 +j*0x10000) >> 12);
        }
    }
}
