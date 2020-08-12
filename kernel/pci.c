/*
 * pci.c
 */

#include "hardware.h"
#include "util.h"
#include "pci.h"

union PciConfAddress {
    unsigned int raw;
    struct {
        unsigned int address;
    }__attribute((packed));
}__attribute((aligned(4)));

void write_conf_address(unsigned int field);
unsigned int read_conf_data();
void write_conf_data(unsigned int data);

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
    addr.raw = 0x80000000 | bus<<16 | device<<11 | func<<8 | offset;

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
    union PciConfAddress addr;
    bus &= 0xff;
    device &= 0x1f;
    func &= 0x7;
    offset &= 0xff;
    addr.raw = 0x80000000 | bus<<16 | device<<11 | func<<8 | offset;

    write_conf_address(addr.raw);
    write_conf_data(data);
}

static unsigned int get_nic_pci_header_type() {
    return (read_pci_conf(0, 3, 0, 0x0c) >> 16) & 0xff;
}

void init_nic_pci() {
    if (get_nic_pci_header_type() == 0) {
        write_pci_conf(0, 3, 0, 0x04, 0x0407);
    } else {
        puts("header type is not 0\n");
    }
}

unsigned int get_nic_base_address() {
    return ((read_pci_conf(0, 3, 0, 0x10) >> 4) << 4);
}
