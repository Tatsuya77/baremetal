#include "hardware.h"
#include "util.h"
#include "lapic_timer.h"
#include "syscall.h"

unsigned long long syscall_puts(char *str) {
    puts(str);
    return 0;
}

unsigned long long
syscall_handler_internal(SYSCALL syscall_id, unsigned long long arg1, unsigned long long arg2, unsigned long long arg3) {
    unsigned long long ret;
    if (syscall_id == SYSCALL_PUTS) {
        ret = syscall_puts((char *)arg1);
    }
    lapic_set_eoi();
    return ret;
}
