# uint32_t load_tmr(unsigned short addr);
.global load_tmr
load_tmr:
  mov %di, %dx 
  in %dx, %eax
  ret

# void write_conf_address(unsigned int field);
.global write_conf_address
write_conf_address:
  mov %di, %dx 
  mov 0xcf8, %ax
  out %ax, %dx
  ret

# uint32_t read_conf_data(void);
.global read_conf_data
read_conf_data:
  mov 0xcfc, %dx
  in %dx, %ax
  ret
