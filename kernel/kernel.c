#include "hardware.h"
#include "segmentation.h"
#include "util.h"
#include "pm_timer.h"
#include "lapic_timer.h"
#include "interruption.h"
#include "sched.h"
#include "syscall.h"
#include "memory.h"
#include "pci.h"

void start(void *SystemTable __attribute__ ((unused)), struct HardwareInfo *_hardware_info) {
  // From here - Put this part at the top of start() function
  // Do not use _hardware_info directry since this data is located in UEFI-app space
  hardware_info = *_hardware_info;
  init_segmentation();
  // To here - Put this part at the top of start() function

  init_frame_buffer(&(hardware_info.fb));
  init_acpi_pm_timer(hardware_info.rsdp);
  init_intr();
  init_virtual_memory();

  measure_lapic_freq_khz();

  /* 7 */
      // /* kadaiA sample codes */
      // puts("System\nProgramming\nLab\n");
      // puts("This text is not shown\r");
      // puth(12345678, 9);
      // puts("\n");

      // /* kadaiB1 sample codes */
      // puts("wait: start\n");
      // pm_timer_wait_millisec(10000);
      // puts("wait: end\n");

      // /* kadaiB2 sample codes */
      // puth(measure_lapic_freq_khz(), 8);
      // puts("\n");

  /* 8 */
      // puts("start\n");
      // void *handler;
      // asm volatile ("lea schedule(%%rip), %[handler]" : [handler]"=r"(handler));
      // lapic_periodic_exec(1000, handler);
      // init_tasks();
      // puts("end\n");

  /* 9 */
      /* kadaiA sample codes */
      // unsigned long long ret;
      // char *str = "tatsuya\n";
      // asm volatile (
      //         "mov %[id], %%rdi\n"
      //         "mov %[str], %%rsi\n"
      //         "int $0x80\n"
      //         "mov %%rax, %[ret]\n"
      //         : [ret]"=r"(ret)
      //         : [id]"r"((unsigned long long)SYSCALL_PUTS),
      //           [str]"m"((unsigned long long)str)
      //         );

      /* kadaiB */
      // puts("start\n");
      // void *handler;
      // asm volatile ("lea schedule(%%rip), %[handler]" : [handler]"=r"(handler));
      // lapic_periodic_exec(1000, handler);
      // init_tasks();
      // puts("end\n");

      /* kadaiC */
      // puts("start\n");
      // void *handler;
      // asm volatile ("lea schedule(%%rip), %[handler]" : [handler]"=r"(handler));
      // lapic_periodic_exec(1000, handler);
      // init_tasks();
      // puts("end\n");

  /* 10 */
  init_nic_pci();
  puth(get_nic_base_address(), 8);

  // Do not delete it!
  while (1);
}
