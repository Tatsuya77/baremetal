/*
 * pm_timer.c
 */

#include "pm_timer.h"

const unsigned int freq_hz = 3579545;
unsigned short pm_timer_blk;
char pm_timer_is_32;

void
init_acpi_pm_timer(struct RSDP *rsdp)
{
    struct XSDT *xsdt = (struct XSDT *) rsdp->xsdt_address;

    unsigned int length = xsdt->sdth.length;
    unsigned int table_num = (length - 36)/sizeof(unsigned long long);

    for (unsigned int i = 0; i < table_num; i++)
    {
        unsigned long long *table_addr = (unsigned long long *) ((unsigned long long) xsdt + 36 + i*sizeof(unsigned long long));

        if (strncmp(4, (char *) "FACP", (char *) *table_addr) == 0)
        {
            pm_timer_blk = *((unsigned short *) (*table_addr + 76));
            // pm_timer_is_32 = *((char *) ((unsigned long) ((struct FADT *) table_addr)->flags) + 8);
            pm_timer_is_32 = !!(((struct FADT *) table_addr) -> flags && (1 << 8));

            break;
        }

        puts("FADT Not Found\n");
    }

    return;
}

unsigned long load_tmr(unsigned short addr);

void
pm_timer_wait_millisec(unsigned int msec)
{
    unsigned s = load_tmr(pm_timer_blk);
    unsigned carry = 0;

    unsigned long long now;
    unsigned long long end = s + freq_hz * (unsigned long long)msec / 1000;

    unsigned t_pre = s;

    while (1)
    {
        unsigned t = load_tmr(pm_timer_blk);

        if (t < t_pre) carry++;

        now = t + ((unsigned long long)carry << (pm_timer_is_32 ? 32 : 24));

        if (now >= end) return;
        
        t_pre = t;
    }
}
