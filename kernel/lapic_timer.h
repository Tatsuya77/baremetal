/*
 * lapit_timer.h
 */

#ifndef LAPIT_TIMER_H
#define LAPIT_TIMER_H

unsigned int measure_lapic_freq_khz(void);
void lapic_periodic_exec(unsigned int msec, void *callback);
void lapic_set_eoi();

#endif /* !LAPIT_TIMER_H */
