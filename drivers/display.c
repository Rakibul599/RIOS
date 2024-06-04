#include "display.h"
#include "ports.h"
#include <stdint.h>
#include "../kernel/mem.h"
#include "../kernel/util.h"

void set_cursor(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset & 0xff));
}

int get_cursor() {
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

int get_offset(int col, int row) {
    return 2 * (row * MAX_COLS + col);
}

int get_row_from_offset(int offset) {
    return offset / (2 * MAX_COLS);
}

int move_offset_to_new_line(int offset) {
    return get_offset(0, get_row_from_offset(offset) + 1);
}

void set_char_at_video_memory(char character, int offset) {
    uint8_t *vidmem = (uint8_t *) VIDEO_ADDRESS;
    vidmem[offset] = character;
    vidmem[offset + 1] = WHITE_ON_BLACK;
}

int scroll_ln(int offset) {
    memory_copy(
            (uint8_t * )(get_offset(0, 1) + VIDEO_ADDRESS),
            (uint8_t * )(get_offset(0, 0) + VIDEO_ADDRESS),
            MAX_COLS * (MAX_ROWS - 1) * 2
    );

    for (int col = 0; col < MAX_COLS; col++) {
        set_char_at_video_memory(' ', get_offset(col, MAX_ROWS - 1));
    }

    return offset - 2 * MAX_COLS;
}

/*
 * TODO:
 * - handle illegal offset (print error message somewhere)
 */
void print_string(char *string) {
    int offset = get_cursor();
    int i = 0;
    while (string[i] != 0) {
        if (offset >= MAX_ROWS * MAX_COLS * 2) {
            offset = scroll_ln(offset);
        }
        if (string[i] == '\n') {
            offset = move_offset_to_new_line(offset);
        } else {
            set_char_at_video_memory(string[i], offset);
            offset += 2;
        }
        i++;
    }
    set_cursor(offset);
}

void print_nl() {
    int newOffset = move_offset_to_new_line(get_cursor());
    if (newOffset >= MAX_ROWS * MAX_COLS * 2) {
        newOffset = scroll_ln(newOffset);
    }
    set_cursor(newOffset);
}

void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    for (int i = 0; i < screen_size; ++i) {
        set_char_at_video_memory(' ', i * 2);
    }
    set_cursor(get_offset(0, 0));
}

void print_backspace() {
    int newCursor = get_cursor() - 2;
    set_char_at_video_memory(' ', newCursor);
    set_cursor(newCursor);
}
void intToString(int num, char *buffer) {
    // Handle negative numbers
    int isNegative = 0;
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    // Convert the number to string in reverse order
    int i = 0;
    do {
        buffer[i++] = num % 10 + '0';
        num /= 10;
    } while (num != 0);

    // If the number was negative, add a '-' sign
    if (isNegative)
        buffer[i++] = '-';

    // Terminate the string
    buffer[i] = '\0';

    // Reverse the string
    int len = i;
    for (i = 0; i < len / 2; ++i) {
        char temp = buffer[i];
        buffer[i] = buffer[len - i - 1];
        buffer[len - i - 1] = temp;
    }
}
void print_integer(int num) {
    // Buffer for storing the converted number as string
    char buffer[12]; // Assuming a maximum of 12 digits for an int (including sign)
    
    // Convert the integer to a string
    intToString(num, buffer);
    
    // Print the string
    print_string(buffer);
}
void floatToString(float num, char *buffer) {
    // Convert the integer part of the float
    int intPart = (int)num;
    intToString(intPart, buffer);

    // Find the position of the decimal point
    int i = 0;
    while (buffer[i] != '\0' && buffer[i] != '-') {
        i++;
    }

    // Add the decimal point
    buffer[i] = '.';
    i++;

    // Convert the fractional part of the float
    float fracPart = num - intPart;
    for (int j = 0; j < 2; j++) { // Print up to 2 decimal places
        fracPart *= 10;
        int digit = (int)fracPart;
        buffer[i++] = digit + '0';
        fracPart -= digit;
    }

    // Terminate the string
    buffer[i] = '\0';
}

void print_float(float num) {
    // Buffer for storing the converted number as string
    char buffer[20]; // Adjust the size as needed
    
    // Convert the float to a string
    floatToString(num, buffer);
    
    // Print the string
    print_string(buffer);
}
