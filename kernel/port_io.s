# uint32_t load_tmr(unsigned short addr);
.global load_tmr
load_tmr:
  mov %di, %dx 
  in %dx, %eax
  ret
