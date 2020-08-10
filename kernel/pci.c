/*
 * pci.c
 */

#include "pci.h"

union PciConfAddress {
    unsigned int raw;
    struct {
        unsigned int address;
    }__attribute((packed));
};

void write_conf_address(unsigned int field);
unsigned int read_conf_data();

static unsigned int read_pci_conf(
        unsigned int bus,
        unsigned int device,
        unsigned int func,
        unsigned int offset
        ) {
    union PciConfAddress addr;
    bus &= 0xff;
    device &= 0x1f;
    func &= 0x7;
    offset &= 0xff;
    addr.raw = (1<<31) | (0x7f<<24) | (bus<<16) | (device<<11) | (func<<8) | (offset);

    write_conf_address(addr.raw);
    return read_conf_data();
}

static void write_pci_conf(
        unsigned int bus,
        unsigned int device,
        unsigned int func,
        unsigned int offset,
        unsigned int data
        ) {

}

static get_nic_pci_header_type() {

}

void init_nic_pci() {

}

unsigned int get_nic_base_address() {
    return read_pci_conf(0, 3, 0, 0);
}
