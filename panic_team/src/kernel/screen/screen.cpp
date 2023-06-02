#include "screen.h"
#include "libc.h"
#include <math.h>

static uint32_t next_line_index = 1;
uint8_t g_fore_color = WHITE, g_back_color = BLUE;

void full_screen()
{
    char *video = (char *)0xB8000;

    for (int i = 0; i < 4000; i += 2)
    {
        video[i] = video[i + 160];
        video[i + 1] = video[i + 161];
    }
}

void monitor_write(int colour, const char *string)
{
    char *video = (char *)0xB8000;
    int after_grub = 0;

    for (int i = 0; i < 4000; i += 2)
    {
        if (video[i] != ' ')
        {
            after_grub = i;
        }
    }
    int indent = after_grub % 160;
    after_grub += 160 - indent;
    int counter = 0;

    if (after_grub > 3840)
    {
        full_screen();
    }
    while (string[counter] != 0)
    {
        video[after_grub] = string[counter];
        video[after_grub + 1] = colour;
        counter++;
        after_grub += 2;
    }
}
void print_char(char ch)
{
    vga_buffer[vga_index] = vga_entry(ch, g_fore_color, g_back_color);
    vga_index++;
}

void print_string(char *str)
{
    uint32_t index = 0;
    while (str[index])
    {
        print_char(str[index]);
        index++;
    }
}

void print_int(int num)
{
    int n = num;
    int count = 0;
    // iterate at least once, then until n becomes 0
    // remove last digit from n in each iteration
    // increase count by 1 in each iteration
    do
    {
        n /= 10;
        ++count;
    } while (n != 0);

    char buf[count];
    itoa(num, buf);
    print_string(buf);
}
uint16_t vga_entry(unsigned char ch, uint8_t fore_color, uint8_t back_color)
{
    uint16_t ax = 0;
    uint8_t ah = 0, al = 0;

    ah = back_color;
    ah <<= 4;
    ah |= fore_color;
    ax = ah;
    ax <<= 8;
    al = ch;
    ax |= al;

    return ax;
}

void clear_screen(uint8_t fore_color, uint8_t back_color)
{
    clear_vga_buffer(&vga_buffer, fore_color, back_color);
}

void clear_vga_buffer(uint16_t **buffer, uint8_t fore_color, uint8_t back_color)
{
    uint32_t i;
    for (i = 0; i < BUFSIZE; i++)
    {
        (*buffer)[i] = vga_entry(0, fore_color, back_color);
    }
    next_line_index = 1;
    vga_index = 0;
}

void move_cursor_next_line()
{
    if (next_line_index >= 55)
    {
        next_line_index = 0;
        clear_vga_buffer(&vga_buffer, g_fore_color, g_back_color);
    }
    vga_index = 80 * next_line_index;
    next_line_index++;
}