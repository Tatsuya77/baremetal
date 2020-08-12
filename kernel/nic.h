/*
 * nic.h
 */

#ifndef NIC_H
#define NIC_H

void init_nic(unsigned int nic_address);
unsigned char send_frame(void *buffer, unsigned short len);
unsigned short receive_frame(void *buffer);

#endif /* !NIC_H */
