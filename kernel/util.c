/*
 * util.c
 */

#include "hardware.h"
#include "util.h"

extern unsigned char font[128][FONT_HEIGHT];
struct FrameBuffer *FB;
unsigned int fb_y;
unsigned fb_x;

unsigned long long
pow(unsigned int x, unsigned int n)
{
    unsigned long long ret = 1;

    for (; n > 0; n--) ret = ret*x;

    return ret;
}

void
paint_char(struct Pixel *pixel)
{
    pixel->r = 255;
    pixel->g = 255;
    pixel->b = 253;
}

void
paint_null(struct Pixel *pixel)
{
    pixel->r = 0;
    pixel->g = 0;
    pixel->b = 0;
}

void
check_coordinate(void)
{
    if (fb_x >= FB->width) {fb_x = 0; fb_y += FONT_HEIGHT;}

    if (fb_y >= FB->height) init_frame_buffer(FB);
}

void
init_frame_buffer(struct FrameBuffer *fb)
{
    FB = fb;

    for (unsigned int i = 0; i < FB->height; i++) {
      for (unsigned int j = 0; j < FB->width; j++) {
        struct Pixel *pixel = FB->base + FB->width * i + j;
        paint_null(pixel);
      }
    }

    fb_y = 0;
    fb_x = 0;

    return;
}

static void
putc(char c)
{
    if (c == '\r')
    {
        /* Clear Line */
        for (unsigned int i = 0; i < FONT_HEIGHT; i++)
        {
            for (unsigned int j = 0; j < FB->width; j++)
            {
                struct Pixel *pixel;
                pixel = FB->base + FB->width*(fb_y+i) + j;
                paint_null(pixel);
            }
        }
        fb_x = 0;
        check_coordinate();
        return;
    }

    if (c == '\n')
    {
        fb_x = 0;
        fb_y += FONT_HEIGHT;
        check_coordinate();
        return;
    }

    for (unsigned int i = 0; i < FONT_HEIGHT; i++)
    {
        for (unsigned int j = 0; j < 8; j++)
        {
            if (font[(int)c][i] >> (8 - 1 - j) & 1)
            {
                struct Pixel *pixel;
                pixel = FB->base + FB->width*(fb_y+i) + (fb_x+j);
                paint_char(pixel);
            }
        }
    }

    fb_x += FONT_HEIGHT;

    check_coordinate();

    return;
}

void
puts(char *str)
{
    while (*(str) != '\0')
    {
        putc(*str);
        str++;
    }

    return;
}

void
puth(unsigned long long value, unsigned char digits_len)
{
    /* Error Handler */
    if (!((int) digits_len >= 48 || (int) digits_len <= 57)) return;

    unsigned int len = (int) digits_len - 48;

    puts("0x");

    for (; len > 0; len--)
    {
        unsigned char num = value/pow(16, len-1)%16;

        if (num <= 9) putc(num+48);
        else putc(num+55);
    }

    return;
}

int
strcmp(char *s1, char *s2)
{
    for (; *s1 == *s2 && *s1 != '\0' && *s2 != '\0'; s1++,s2++);
    return *s1 - *s2;
}

int
strncmp(int n, char *s1, char *s2)
{
    for (; *s1 == *s2 && *s1 != '\0' && *s2 != '\0' && n>1; s1++,s2++,n--);
    return *s1 - *s2;
}
