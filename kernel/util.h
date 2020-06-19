/*
 * util.h
 */

#ifndef UTIL_H
#define UTIL_H

#define FONT_HEIGHT 8

void init_frame_buffer(struct FrameBuffer *fb);

void puts(char *str);

void puth(unsigned long long value, unsigned char digits_len);

int strcmp(char *s1, char *s2);

int strncmp(int n, char *s1, char *s2);

#endif /* !UTIL_H */
