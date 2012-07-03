#include "LiteBrite.h"

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

volatile uint32_t packet;
volatile uint8_t ready = 0;

volatile char sent[28];
volatile int i = 0;

ISR(TIMER2_OVF_vect) {
  static uint32_t mask = START_MASK;
  
  // move along to the next bit of the packet in question
  mask = mask >> 1;

  // our mask shifting has reached the padding bits
  if(mask == 0x8){
    // reset the mask to use for the next packet
    mask = START_MASK;
    // we need to idle the line for at least 30uS before sending the next packet
    OCR2B = CMD_NULL;
    sent[i++] = '2';
    sent[i++] = '\0';
    i = 0;
    ready = 1;
    return;
  }

  // since CMD_ONE is twice of CMD_ZERO, we can write a branch-less version for setting OCR2B.
  // This works because CMD_ONE == (CMD_ZERO << 1)
  uint32_t val = CMD_ZERO << ((mask & packet) != 0);

  sent[i++] = '0' + ((mask & packet) !=0);
  OCR2B = val;
}

void lite_brite_init(void) {
  pinMode(9, OUTPUT);

  TCCR2A = 0x33;
  TCCR2B = 0x0A;
  OCR2A = CMD_NULL;
  OCR2B = CMD_NULL;

  // enable interrupts
  ready = 0;
  TIMSK2 = 0x01;

  int i = 0;
  for(i=0;i<50;i++){
    ready = 0;
    bulb_t temp = {value: 0};
    temp.color.addr = i;
    temp.color.brightness = 0xCC;
    temp.color.start_bit = 3;
    packet = temp.value;

    TIMSK2 = 0x01;
    while( !ready ){}
    TIMSK2 = 0x00;
    delay(20);
  }
}

void lite_brite_send(color_t color){
 
  int i = 0;
  for(i=0;i<50;i++){
    ready = 0;
    bulb_t temp = {value: 0};
    temp.color.addr = i;
    temp.color.brightness = 0xCC;
    temp.color.start_bit = 3;
    
    if( i == color.addr ){
      temp.color = color;
      packet = temp.value;
    }
    packet = temp.value;

    TIMSK2 = 0x01;
    while( !ready ){}
    TIMSK2 = 0x00;
  }
  //Serial.println(packet);
  
  // 11 001000 1100 1100 0000 0000 1100 0000
  // 11 000111 1100 1100 0000 0000 1100 0000

  TIMSK2 = 0x01;
  ready = 0;
  while( !ready ){}
  TIMSK2 = 0x00;
  //Serial.println((const char*)sent);
}
