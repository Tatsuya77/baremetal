/*
 * nic.c
 */

#include "nic.h"
#include "pci.h"

#define TX_DESCRIPTORS_NUM 8
#define RX_DESCRIPTORS_NUM 8
#define RX_BUFFER_NUM 1024

struct __attribute__((packed)) TxDescriptor {
    unsigned long long buf_addr;
    unsigned long long legacy;
};

struct __attribute__((packed)) RxDescriptor {
    unsigned long long buf_addr;
    unsigned long long legacy;
};

static unsigned int nic_base_address;

static struct TxDescriptor tx_descriptors[TX_DESCRIPTORS_NUM]__attribute__((aligned(16)));
static struct RxDescriptor rx_descriptors[RX_DESCRIPTORS_NUM]__attribute__((aligned(16)));

static unsigned char rx_frame_buffers[RX_DESCRIPTORS_NUM][RX_BUFFER_NUM];

static unsigned int tx_current_idx;
static unsigned int rx_current_idx;

static void set_nic_register(unsigned short offset, unsigned int value) {
    *(unsigned int *)(unsigned long long)(nic_base_address + offset) = value;
}

static void init_tx() {
    for (unsigned int i=0; i<TX_DESCRIPTORS_NUM; i++) {
        tx_descriptors[i].legacy = (unsigned long long) 0b00001001 << 24;
    }
    tx_current_idx = 0;
    /* TDBAL */
    set_nic_register(0x3800, (unsigned int) (unsigned long long)tx_descriptors);
    /* TDBAH */
    set_nic_register(0x3804, (unsigned int) ((unsigned long long)tx_descriptors >> 32));
    /* TDLEN */
    set_nic_register(0x3808, (unsigned int) 128);
    /* TDH */
    set_nic_register(0x3810, (unsigned int) 0);
    /* TDT */
    set_nic_register(0x3818, (unsigned int) 0);
    /* TCTL */
    set_nic_register(0x400, (unsigned int) 0b1010 << 22 | 0x40 << 12 | 0x0f << 4 | 0b1010);
}

static void init_rx() {
    for (unsigned int i=0; i<RX_DESCRIPTORS_NUM; i++) {
        rx_descriptors[i].buf_addr = (unsigned long long)rx_frame_buffers[i];
        rx_descriptors[i].legacy = (unsigned long long) 0;
    }
    rx_current_idx = 0;
    /* RDBAL */
    set_nic_register(0x2800, (unsigned int) (unsigned long long)rx_descriptors);
    /* RDBAH */
    set_nic_register(0x2804, (unsigned int) ((unsigned long long)rx_descriptors >> 32));
    /* RDLEN */
    set_nic_register(0x2808, (unsigned int) 128);
    /* RDH */
    set_nic_register(0x2810, (unsigned int) 0);
    /* RDT */
    set_nic_register(0x2818, (unsigned int) RX_DESCRIPTORS_NUM-1);
    /* RCTL */
    set_nic_register(0x100, (unsigned int) 0b011000000000011110);
}

void init_nic(unsigned int nic_address) {
    nic_base_address = nic_address;

    /* IMC */
    set_nic_register(0x00d8, 0x0000ffff);

    init_tx();
    init_rx();
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

unsigned short receive_frame(void *buffer) {
    if (((rx_descriptors[rx_current_idx].legacy >> 32) & 0b1) == 1) {
        unsigned short ret = rx_descriptors[rx_current_idx].legacy & 0xffff;
        unsigned char *buf_access;
        for (unsigned int i=0; i<ret; i++) {
            buf_access = (unsigned char *) buffer + i;
            *buf_access = rx_frame_buffers[rx_current_idx][i];
        }
        /* RDT */
        set_nic_register(0x2818, (unsigned int) (rx_current_idx+1)%RX_DESCRIPTORS_NUM);
        rx_current_idx = (rx_current_idx + 1) % RX_DESCRIPTORS_NUM;
        return ret;
    } else {
        return 0;
    }
}
