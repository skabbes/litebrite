#include "Arduino.h"
#include "litebrite.h"

typedef struct {
  uint8_t _padding:4;
  uint8_t red:4;
  uint8_t green:4;
  uint8_t blue:4;
  uint8_t brightness:8;
  uint8_t addr:6;
  uint8_t start_bit:2;
} raw_packet_t;

typedef union {
  uint32_t raw;
  raw_packet_t values;
} packet_t;

// The mask for the packet initially has all data bits as 0 because
// each bulb send over the line needs a start bit
// we also shift right one so that we can perform the bitshift
// before the operation (@see nextCmd)
const uint32_t START_MASK = 0x40000000 << 1;

// low for 10uS, then high for 20u
const uint8_t CMD_ZERO = 0x14;

// low for 20uS, then high for 10uS
const uint8_t CMD_ONE  = 0x28;

// low for entire period (30uS)
const uint8_t CMD_NULL = 0x3C;

uint8_t volatile * volatile ready;

packet_t packets[NUM_BULBS] = {0};

ISR(TIMER2_OVF_vect) {
  static uint32_t mask = START_MASK;
  static uint8_t bulb_num = 0;

  // the default packet is never used, but we specified it to a reasonable start value anyway
  static uint32_t packet = 0xC0CC0000;

  // move along to the next bit of the packet in question
  mask = mask >> 1;

  // our mask shifting has reached the padding bits
  if(mask == 0x8){
    // reset the mask to use for the next packet
    mask = START_MASK;
    // we need to idle the line for at least 30uS before sending the next packet
    OCR2B = CMD_NULL;
    bulb_num++;
    if(bulb_num == NUM_BULBS){
      *ready = 1;
      bulb_num = 0;
      // we have finished a strand, disable interrupts until next strand needs to be sent
      TIMSK2 = 0x00;
    }
  }
  else {
    packet = packets[bulb_num].raw;
    // since CMD_ONE is twice of CMD_ZERO, we can write a branch-less version for setting OCR2B.
    // This works because CMD_ONE == (CMD_ZERO << 1)
    OCR2B = CMD_ZERO << ((mask & packet) != 0);
  }
}

void lite_brite_init(void) {
  pinMode(9, OUTPUT);
  TCCR2A = 0x33;
  TCCR2B = 0x0A;
  OCR2A = CMD_NULL;
  OCR2B = CMD_NULL;

  color_t default_color;
  default_color.red = default_color.green = default_color.blue = 0;
  default_color.brightness = 0xCC;
  color_t initial_strand[NUM_BULBS] = {default_color};

  // initialize the values of the packets that will never change
  // we we don't need to worry about setting them later
  for(int i=0;i<NUM_BULBS;i++){
    packets[i].values.start_bit = 3;
    packets[i].values.addr = i;
    packets[i].values._padding = 0;
  }

  volatile uint8_t init_done = 0;
  lite_brite_send_strand(initial_strand, &init_done);
  // wait until the initial bulb addressing is completely done
  while( !init_done ){}
}

void lite_brite_send_strand(color_t * next_strand, volatile uint8_t * userReady){

  for(int i=0;i<NUM_BULBS;i++){
    packets[i].values.red   = next_strand[i].red;
    packets[i].values.green = next_strand[i].green;
    packets[i].values.blue  = next_strand[i].blue;
    packets[i].values.brightness = next_strand[i].brightness;
  }

  // set the ready pointer to the user specified pointer
  ready = userReady;
  *ready = 0;

  // re-enable interrupts to start sending the bits
  TIMSK2 = 0x01;
}

void lite_brite_send_strand_blocking(color_t * strand){
  volatile uint8_t isReady = 0;
  lite_brite_send_strand(strand, &isReady);
  while(!isReady){}
}
