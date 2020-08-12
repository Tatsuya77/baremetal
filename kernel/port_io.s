# uint32_t load_tmr(unsigned short addr);
.global load_tmr
load_tmr:
  mov %di, %dx 
  in %dx, %eax
  ret

# void write_conf_address(unsigned int field);
.global write_conf_address
write_conf_address:
  mov %edi, %eax 
  mov $0xcf8, %dx
  out %eax, %dx
  ret

# uint32_t read_conf_data(void);
.global read_conf_data
read_conf_data:
  mov $0xcfc, %dx
  in %dx, %eax
  ret

# void write_conf_data(unsigned int data);
.global write_conf_data
write_conf_data:
  mov %edi, %eax
  mov $0xcfc, %dx
  out %eax, %dx
  ret
