#ifndef lite_brite_h
#define lite_brite_h

#include <stdint.h>
#include "Arduino.h"

typedef struct {
  uint8_t _padding:4;
  uint8_t red:4;
  uint8_t green:4;
  uint8_t blue:4;
  uint8_t brightness:8;
  uint8_t addr:6;
  uint8_t start_bit:2;
} color_t;

typedef union {
  uint32_t value;
  color_t color;
} bulb_t;

bulb_t * lite_brite_init(void);
void lite_brite_send(color_t bulb);
bulb_t * lite_brite_send_strand(volatile uint8_t * userReady);

#endif // lite_brite_h
