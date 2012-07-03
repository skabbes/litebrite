#include "Arduino.h"
#include "litebrite.h"

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

bulb_t buf_a[50] = {0};
bulb_t buf_b[50] = {0};

bulb_t * volatile current_strand = buf_a;
bulb_t * volatile next_strand = buf_b;

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
    if(bulb_num == 50){
      *ready = 1;
      bulb_num = 0;
      // we have finished a strand, disable interrupts until next strand needs to be sent
      TIMSK2 = 0x00;
    }
  }
  else {
    current_strand[bulb_num].color.addr = bulb_num;
    packet = current_strand[bulb_num].value;
    // since CMD_ONE is twice of CMD_ZERO, we can write a branch-less version for setting OCR2B.
    // This works because CMD_ONE == (CMD_ZERO << 1)
    OCR2B = CMD_ZERO << ((mask & packet) != 0);
  }
}

bulb_t * lite_brite_init(void) {
  pinMode(9, OUTPUT);
  TCCR2A = 0x33;
  TCCR2B = 0x0A;
  OCR2A = CMD_NULL;
  OCR2B = CMD_NULL;

  int i = 0;
  for(i=0;i<50;i++){
    ready = 0;
    bulb_t temp = {value: 0};
    temp.color.addr = i;
    temp.color.brightness = 0xCC;
    temp.color.start_bit = 3;

    // initialize both strand buffers with correct address and whatnot
    current_strand[i] = temp;
    next_strand[i] = temp;
  }

  volatile uint8_t init_done = 0;
  lite_brite_send_strand(&init_done);
  // wait until the bulb addressing is completely done
  while( !init_done ){}
  return next_strand;
}

bulb_t * lite_brite_send_strand(volatile uint8_t * userReady){

  // implicity, the user will have written the data to send to next_strand
  bulb_t * temp = current_strand;
  current_strand = next_strand;
  next_strand = temp;
  // make sure that the next_strand encapsulates the "now" state of the strand by copying them over
  memcpy(next_strand, current_strand, 50 * sizeof(bulb_t));

  // set the ready pointer to the user specified pointer
  ready = userReady;
  *ready = 0;

  // re-enable interrupts to start sending the bits
  TIMSK2 = 0x01;
  return next_strand;
}

bulb_t * lite_brite_send_strand_blocking(){
  volatile uint8_t isReady = 0;
  bulb_t * new_strand = lite_brite_send_strand(&isReady);
  while(!isReady){}
  return new_strand;
}
