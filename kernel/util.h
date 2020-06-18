/*
 * util.h
 */

#ifndef UTIL_H
#define UTIL_H

#define FONT_HEIGHT 8

void init_frame_buffer(struct FrameBuffer *fb);

void puts(char *str);

void puth(unsigned long long value, unsigned char digits_len);

#endif /* !UTIL_H */
