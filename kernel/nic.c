/*
 * nic.c
 */

#include "nic.h"
#include "pci.h"

#define TX_DESCRIPTORS_NUM 8

struct __attribute__((packed)) TxDescriptor {
    unsigned long long buf_addr;
    unsigned long long legacy;
};

static unsigned int nic_base_address;

static struct TxDescriptor tx_descriptors[TX_DESCRIPTORS_NUM]__attribute__((aligned(16)));

// static unsigned int tx_current_idx;

static void set_nic_register(unsigned short offset, unsigned int value) {
    *(unsigned int *)(unsigned long long)(nic_base_address + offset) = value;
}

static void init_tx() {
    for (unsigned int i=0; i<TX_DESCRIPTORS_NUM; i++) {
        tx_descriptors[i].legacy = (unsigned long long) 0b00001001 << 24;
    }
    /* TDBAL */
    set_nic_register(0x3800, (unsigned int) (unsigned long long)tx_descriptors);
    /* TDBAH */
    set_nic_register(0x3804, (unsigned int) ((unsigned long long)tx_descriptors >> 32));
    /* TDLEN */
    // set_nic_register(0x3808, (unsigned int) TX_DESCRIPTORS_NUM);
    set_nic_register(0x3808, (unsigned int) 128);
    /* TDH */
    set_nic_register(0x3810, (unsigned int) 0);
    /* TDT */
    set_nic_register(0x3818, (unsigned int) 0);
    /* TCTL */
    set_nic_register(0x400, (unsigned int) 0b1010 << 22 | 0x40 << 12 | 0x0f << 4 | 0b1010);
}

void init_nic(unsigned int nic_address) {
    nic_base_address = nic_address;

    /* IMC */
    set_nic_register(0x00d8, 0x0000ffff);

    init_tx();
}

unsigned char send_frame(void *buffer, unsigned short len) {
    for (unsigned int i=0; i<1; i++) {
        tx_descriptors[(0+i)%TX_DESCRIPTORS_NUM].buf_addr = (unsigned long long) buffer + i;
        /* length */
        tx_descriptors[(0+i)%TX_DESCRIPTORS_NUM].legacy = tx_descriptors[(1+i)%TX_DESCRIPTORS_NUM].legacy | len;
        /* TDT */
        set_nic_register(0x3818, (unsigned int) (1+i)%TX_DESCRIPTORS_NUM);

        while (((tx_descriptors[(0+i)%TX_DESCRIPTORS_NUM].legacy >> 32) & 0b1) == 0);
    }

    return 0;
}
