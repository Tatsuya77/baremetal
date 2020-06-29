/*
 * lapic_timer.c
 */

#include "lapic_timer.h"
#include "pm_timer.h"

volatile unsigned int *lvt_timer = (unsigned int *)0xfee00320;
volatile unsigned int *initial_count = (unsigned int *)0xfee00380;
volatile unsigned int *current_count = (unsigned int *)0xfee00390;
volatile unsigned int *divide_config = (unsigned int *)0xfee003e0;

volatile unsigned int *lapic_eoi = (unsigned int *)0xfee000b0;
void *fun;

unsigned int lapic_timer_freq_khz;

unsigned int measure_lapic_freq_khz(void)
{
    *lvt_timer = 0x00000000;
    *divide_config = 0b1111;

    unsigned int measure_time = 4000;

    *initial_count = 0xFFFFFFFF;

    pm_timer_wait_millisec(measure_time);

    lapic_timer_freq_khz =  (*initial_count - *current_count) / (measure_time / 1000) / 1000;

    return lapic_timer_freq_khz;
}

void lapic_periodic_exec(unsigned int msec, void *callback)
{
    *lvt_timer = 0b0100000000010000000;
    *divide_config = 0b1111;

    fun = callback;

    *initial_count = lapic_timer_freq_khz * msec;

    return;
}

void lapic_intr_handler_internal(void)
{
    // puts("ok\n");

    *lapic_eoi = 0;

    return;
}
