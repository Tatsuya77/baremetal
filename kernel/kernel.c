#include "hardware.h"
#include "segmentation.h"
#include "util.h"
#include "pm_timer.h"
#include "lapic_timer.h"

void start(void *SystemTable __attribute__ ((unused)), struct HardwareInfo *_hardware_info) {
  // From here - Put this part at the top of start() function
  // Do not use _hardware_info directry since this data is located in UEFI-app space
  hardware_info = *_hardware_info;
  init_segmentation();
  // To here - Put this part at the top of start() function

  init_frame_buffer(&(hardware_info.fb));
  init_acpi_pm_timer(hardware_info.rsdp);

  /* kadaiA sample codes */
  puts("System\nProgramming\nLab\n");
  puts("This text is not shown\r");
  puth(12345678, 9);
  puts("\n");

  /* kadaiB1 sample codes */
  puts("wait: start\n");
  pm_timer_wait_millisec(10000);
  puts("wait: end\n");

  /* kadaiB2 sample codes */
  puth(measure_lapic_freq_khz(), 8);
  puts("\n");

  // Do not delete it!
  while (1);
}
