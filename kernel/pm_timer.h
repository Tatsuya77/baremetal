/*
 * pm_timer.h
 */

#include "hardware.h"
#include "util.h"

void init_acpi_pm_timer(struct RSDP *rsdp);

void pm_timer_wait_millisec(unsigned int msec);
