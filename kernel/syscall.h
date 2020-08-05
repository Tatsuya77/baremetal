#ifndef SYSCALL_H
#define SYSCALL_H

typedef enum SYSCALL {
    SYSCALL_PUTS,
} SYSCALL;

unsigned long long syscall_handler_internal(SYSCALL, unsigned long long, unsigned long long, unsigned long long);

#endif /* !SYSCALL_H */
