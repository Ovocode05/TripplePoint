#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Cross-compiler assurance guards */
#if !defined(__i386__)
#error "This kernel must be compiled for 32-bit i386."
#endif

/* 8x16 bitmap font tracking array */
unsigned char font_bitmap[256][16] = {
    ['H'] = {0b11000110,0b11000110,0b11000110,0b11000110,0b11111110,0b11111110,0b11000110,0b11000110,0b11000110,0b11000110,0b00000000},
    ['e'] = {0b00000000,0b00000000,0b01111100,0b11000110,0b11111110,0b11000000,0b11000110,0b01111100,0b00000000},
    ['l'] = {0b01100000,0b01100000,0b01100000,0b01100000,0b01100000,0b01100000,0b01100000,0b00111100,0b00000000},
    ['o'] = {0b00000000,0b00000000,0b01111100,0b11000110,0b11000110,0b11000110,0b11000110,0b01111100,0b00000000},
    ['W'] = {0b11000110,0b11000110,0b11000110,0b11000110,0b11011110,0b11110110,0b11100110,0b11000110,0b00000000},
    ['r'] = {0b00000000,0b00000000,0b10110110,0b11101110,0b11000000,0b11000000,0b11000000,0b11000000,0b00000000},
    ['d'] = {0b00000110,0b00000110,0b01111110,0b11000110,0b11000110,0b11000110,0b11000110,0b01111110,0b00000000},
    ['!'] = {0b00011000,0b00011000,0b00011000,0b00011000,0b00011000,0b00000000,0b00011000,0b00011000,0b00000000},
    [' '] = {0b00000000},
    [','] = {0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00110000,0b00110000,0b00010000,0b00100000},
    ['A'] = {0b00011000,0b00111100,0b01100110,0b01100110,0b11111111,0b11111111,0b11000110,0b11000110,0b00000000},
    ['N'] = {0b11000110,0b11100110,0b11110110,0b11110110,0b11011110,0b11001110,0b11000110,0b11000110,0b00000000},
    ['E'] = {0b11111110,0b11000000,0b11000000,0b11111100,0b11111100,0b11000000,0b11000000,0b11111110,0b00000000},
    ['U'] = {0b11000110,0b11000110,0b11000110,0b11000110,0b11000110,0b11000110,0b11000110,0b01111100,0b00000000},
    ['R'] = {0b11111100,0b11000110,0b11000110,0b11111100,0b11111000,0b11001100,0b11000110,0b11000110,0b00000000},
    ['L'] = {0b11000000,0b11000000,0b11000000,0b11000000,0b11000000,0b11000000,0b11000000,0b11111110,0b00000000},
    ['_'] = {0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,0b00000000},
    ['O'] = {0b01111100,0b11000110,0b11000110,0b11000110,0b11000110,0b11000110,0b11000110,0b01111100,0b00000000},
    ['S'] = {0b01111100,0b11000110,0b11000000,0b01111100,0b00000110,0b11000110,0b11000110,0b01111100,0b00000000},
    ['M'] = {0b11000110,0b11101110,0b11111110,0b11010110,0b11000110,0b11000110,0b11000110,0b11000110,0b00000000},
    ['i'] = {0b00011000,0b00000000,0b00111000,0b00011000,0b00011000,0b00011000,0b00011000,0b00111100,0b00000000},
    ['s'] = {0b00000000,0b00000000,0b00111100,0b01100000,0b00111100,0b00000110,0b01100110,0b00111100,0b00000000},
    ['g'] = {0b00000000,0b00000000,0b00111110,0b01100010,0b01100010,0b00111110,0b00000010,0b01111100,0b00000000},
    ['F'] = {0b11111110,0b11000000,0b11000000,0b11111000,0b11000000,0b11000000,0b11000000,0b11000000,0b00000000},
    ['u'] = {0b00000000,0b00000000,0b11000110,0b11000110,0b11000110,0b11000110,0b11000110,0b01111110,0b00000000},
    ['n'] = {0b00000000,0b00000000,0b11011100,0b11100110,0b11000110,0b11000110,0b11000110,0b11000110,0b00000000},
    ['c'] = {0b00000000,0b00000000,0b00111100,0b01100010,0b11000000,0b11000000,0b01100010,0b00111100,0b00000000},
    ['t'] = {0b00010000,0b00010000,0b01111100,0b00010000,0b00010000,0b00010000,0b00010010,0b00001100,0b00000000},
    ['D'] = {0b11111000,0b11001100,0b11000110,0b11000110,0b11000110,0b11001100,0b11111000,0b00000000,0b00000000},
    ['a'] = {0b00000000,0b00000000,0b00111100,0b00000110,0b01111110,0b11000110,0b11000110,0b01111110,0b00000000},
};

/* Generic Multiboot 2 Tag Structures */
struct mb2_tag {
    uint32_t type;
    uint32_t size;
};

struct mb2_tag_framebuffer {
    uint32_t type;
    uint32_t size;
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t  framebuffer_bpp;
    uint8_t  framebuffer_type;
    uint16_t reserved;
    uint8_t  red_field_position;
    uint8_t  red_mask_size;
    uint8_t  green_field_position;
    uint8_t  green_mask_size;
    uint8_t  blue_field_position;
    uint8_t  blue_mask_size;
};

struct mb2_tag_module {
    uint32_t type;
    uint32_t size;
    uint32_t mod_start;
    uint32_t mod_end;
    char     cmdline[1];
};

/* Global graphics references */
uint32_t* fb_buffer = NULL;
uint32_t fb_pitch_pixels = 0;
uint32_t fb_width = 0;
uint32_t fb_height = 0;

/* Terminal Layout properties */
size_t terminal_row = 0;
size_t terminal_column = 0;
uint32_t text_color = 0xFFFFFFFF;       
uint32_t background_color = 0x001A0033; /* Dark Indigo */

void putpixel(uint32_t x, uint32_t y, uint32_t color) {
    if (fb_buffer == NULL || x >= fb_width || y >= fb_height) return;
    fb_buffer[y * fb_pitch_pixels + x] = color;
}

void clear_screen(uint32_t color) {
    for (uint32_t y = 0; y < fb_height; y++) {
        for (uint32_t x = 0; x < fb_width; x++) {
            putpixel(x, y, color);
        }
    }
}

void scroll_screen() {
    for (uint32_t y = 16; y < fb_height; y++) {
        for (uint32_t x = 0; x < fb_width; x++) {
            fb_buffer[(y - 16) * fb_pitch_pixels + x] = fb_buffer[y * fb_pitch_pixels + x];
        }
    }
    for (uint32_t y = fb_height - 16; y < fb_height; y++) {
        for (uint32_t x = 0; x < fb_width; x++) {
            putpixel(x, y, background_color);
        }
    }
    terminal_row--;
}

void draw_char(char c, uint32_t start_x, uint32_t start_y, uint32_t fg, uint32_t bg) {
    for (int row = 0; row < 16; row++) {
        uint8_t font_row = font_bitmap[(unsigned char)c][row];
        for (int col = 0; col < 8; col++) {
            if (font_row & (0b10000000 >> col)) {
                putpixel(start_x + col, start_y + row, fg);
            } else {
                putpixel(start_x + col, start_y + row, bg);
            }
        }
    }
}

void terminal_putchar(char c) {
    // Handle CR LF safely from text files
    if (c == '\r') return;
    if (fb_buffer == NULL) return;
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row * 16 >= fb_height) scroll_screen();
        return;
    }

    draw_char(c, terminal_column * 8, terminal_row * 16, text_color, background_color);

    if (++terminal_column * 8 >= fb_width) {
        terminal_column = 0;
        if (++terminal_row * 16 >= fb_height) scroll_screen();
    }
}

void terminal_writestring(const char* data) {
    while (*data) {
        terminal_putchar(*data++);
    }
}

/* Helper to render exact size-bounded buffer bytes out of modules */
void terminal_write_buffer(const char* buffer, size_t length) {
    for (size_t i = 0; i < length; i++) {
        terminal_putchar(buffer[i]);
    }
}

/* Multiboot2 passes its boot magic and information pointer in EAX and EBX. */
void kernel_main(uint32_t magic, uint32_t addr) {
    if (magic != 0x36d76289) {
        return;
    }

    if (addr == 0 || addr > UINT32_MAX - 16) return;

    uint32_t info_size = *(uint32_t*)(uintptr_t)addr;
    if (info_size < 16 || info_size > UINT32_MAX - addr) return;

    uint8_t* cursor = (uint8_t*)(uintptr_t)(addr + 8);
    uint8_t* info_end = (uint8_t*)(uintptr_t)(addr + info_size);
    char* module_data_start = NULL;
    size_t module_data_size = 0;

    while (cursor <= info_end - 8) {
        struct mb2_tag* tag = (struct mb2_tag*)cursor;
        if (tag->size < sizeof(struct mb2_tag) || tag->size > (uint32_t)(info_end - cursor)) break;
        if (tag->type == 0) break;

        if (tag->type == 8 && tag->size >= sizeof(struct mb2_tag_framebuffer)) {
            struct mb2_tag_framebuffer* fb = (struct mb2_tag_framebuffer*)tag;
            uint64_t framebuffer_bytes = (uint64_t)fb->framebuffer_pitch * fb->framebuffer_height;
            uint64_t framebuffer_end = fb->framebuffer_addr + framebuffer_bytes;
            bool supported_rgb =
                fb->framebuffer_type == 1 && fb->framebuffer_bpp == 32 &&
                fb->red_field_position == 16 && fb->red_mask_size == 8 &&
                fb->green_field_position == 8 && fb->green_mask_size == 8 &&
                fb->blue_field_position == 0 && fb->blue_mask_size == 8;

            if (supported_rgb && fb->framebuffer_width >= 8 && fb->framebuffer_height >= 16 &&
                fb->framebuffer_width <= UINT32_MAX / 4 &&
                fb->framebuffer_pitch >= fb->framebuffer_width * 4 &&
                fb->framebuffer_pitch % 4 == 0 && fb->framebuffer_addr % 4 == 0 &&
                fb->framebuffer_addr <= UINT32_MAX && framebuffer_end >= fb->framebuffer_addr &&
                framebuffer_end <= 0x100000000ULL) {
                fb_buffer = (uint32_t*)(uintptr_t)fb->framebuffer_addr;
                fb_width = fb->framebuffer_width;
                fb_height = fb->framebuffer_height;
                fb_pitch_pixels = fb->framebuffer_pitch / 4;
            }
        }

        if (tag->type == 3 && tag->size >= 16) {
            struct mb2_tag_module* mod = (struct mb2_tag_module*)tag;
            if (mod->mod_end >= mod->mod_start) {
                module_data_start = (char*)(uintptr_t)mod->mod_start;
                module_data_size = mod->mod_end - mod->mod_start;
            }
        }

        uint32_t next_offset = (tag->size + 7) & ~7U;
        if (next_offset < tag->size || next_offset > (uint32_t)(info_end - cursor)) break;
        cursor += next_offset;
    }

    clear_screen(background_color);

    terminal_writestring("NEURAL_OS Boot Completed.\n");
    terminal_writestring("Parsing Multiboot 2 Module Assets...\n\n");

    if (module_data_start != NULL && module_data_size > 0) {
        terminal_writestring("--- Dynamic File Content Start ---\n");
        terminal_write_buffer(module_data_start, module_data_size);
        terminal_writestring("\n--- Dynamic File Content End ---\n");
    } else {
        terminal_writestring("Error: No external text modules loaded by bootloader.\n");
    }
}
