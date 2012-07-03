#ifndef lite_brite_h
#define lite_brite_h

#include <stdint.h>
#include "Arduino.h"

#define NUM_BULBS 50

typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t brightness;
} color_t;


void lite_brite_init(void);
void lite_brite_send_strand(color_t * strand, volatile uint8_t * userReady);
void lite_brite_send_strand_blocking(color_t * strand);

#endif // lite_brite_h
