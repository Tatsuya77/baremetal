/*
 * syscall.h
 */

#ifndef SYSCALL_H
#define SYSCALL_H

typedef enum {
    SYSCALL_PUTS,
} SYSCALL;

unsigned long long syscall_puts(char *);

#endif /* !SYSCALL_H */
