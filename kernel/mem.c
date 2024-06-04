#include <stdbool.h>
#include <stdint.h>
#include "mem.h"
#include "../drivers/display.h"
#include "util.h"

// http://www.sunshine2k.de/articles/coding/cmemalloc/cmemory.html#ch33

void memory_copy(uint8_t *source, uint8_t *dest, uint32_t nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

/*
 * The following code is based on code licensed under MIT licence
 * and thus also licensed under MIT license I guess?
 * For further details, see http://www.sunshine2k.de/license.html.
 */

