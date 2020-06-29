#include "hardware.h"
#include "segmentation.h"
#include "util.h"
#include "pm_timer.h"
#include "lapic_timer.h"
#include "interruption.h"

void start(void *SystemTable __attribute__ ((unused)), struct HardwareInfo *_hardware_info) {
  // From here - Put this part at the top of start() function
  // Do not use _hardware_info directry since this data is located in UEFI-app space
  hardware_info = *_hardware_info;
  init_segmentation();
  // To here - Put this part at the top of start() function

  init_frame_buffer(&(hardware_info.fb));
  init_acpi_pm_timer(hardware_info.rsdp);
  init_intr();
  puts("init\n");

  puts("khz: ");
  puth(measure_lapic_freq_khz(), 9);
  puts("\n");

  lapic_periodic_exec(4000, &init_frame_buffer);
  puts("exec\n");

  // Do not delete it!
  while (1);
}
