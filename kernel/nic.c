/*
 * nic.c
 */

#include "nic.h"
#include "pci.h"

#define TX_DESCRIPTORS_NUM 3

struct __attribute__((packed)) TxDescriptor {
    unsigned long long buf_addr;
    unsigned long long legacy;
};

static unsigned int nic_base_address;

static struct TxDescriptor tx_descriptors[TX_DESCRIPTORS_NUM]__attribute__((aligned(16)));

static unsigned int tx_current_idx;

static void set_nic_register(unsigned short offset, unsigned int value) {
    *(unsigned int *)(nic_base_address + offset) = value;
}

static void init_tx() {

}

void init_nic(unsigned int nic_address) {

}

unsigned char send_frame(void *buffer, unsigned short len) {

}
