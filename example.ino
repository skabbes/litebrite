#include "Arduino.h"
#include "litebrite.h"

const int debugPin = 5;
void dim(bulb_t * strand);


void setup(){
  Serial.begin(9600);

  bulb_t test = {value: 0xCFFFFFF0};
  Serial.println(test.color.start_bit);
  Serial.println(test.color.addr);
  Serial.println(test.color.brightness);
  Serial.println(test.color.blue);
  Serial.println(test.color.green);
  Serial.println(test.color.red);
  Serial.println(test.color._padding);

  pinMode(debugPin, OUTPUT);
  digitalWrite(debugPin, HIGH);
  delay(3000);
  digitalWrite(debugPin, LOW);
  bulb_t * strand = lite_brite_init();
  int i;
  
  for(i=0;i<50;i++){
    if( i < 50/4){
      // green
      strand[i].color.red   = 0x4;
      strand[i].color.green = 0xD;
      strand[i].color.blue  = 0x0;
    } else if(i < 50*2/4){
      // yellow
      strand[i].color.red   = 0xF;
      strand[i].color.green = 0xB;
      strand[i].color.blue  = 0x0;
    } else if(i < 50 * 3/ 4){
      // red
      strand[i].color.red   = 0xF;
      strand[i].color.green = 0x0;
      strand[i].color.blue  = 0x0;
      strand[i].color.brightness  = 0xFF * .7;
    } else {
      // blue
      strand[i].color.red   = 0x0;
      strand[i].color.green = 0x0;
      strand[i].color.blue  = 0xA;
    }
  }
  
  i = 12;
  // half second in initial state
  while(i--){
    lite_brite_send_strand_blocking();
  }
  
  i = 16;
  // dim lights for next 3/4 second
  while(i--){
   bulb_t * strand = lite_brite_send_strand_blocking();
   dim(strand);
  }
}

void rotate(bulb_t * strand){
  // rotate the bulbs by 1
  bulb_t prev = strand[0];
  bulb_t temp;
  int i;
  for(i=1;i<50;i++){
    temp = strand[i];
    strand[i] = prev;
    prev = temp;
  }
  strand[0] = prev;
}

void dim(bulb_t * strand){
  int i;
  for(i=0;i<50;i++){
    strand[i].color.red -= strand[i].color.red ? 1 : 0;
    strand[i].color.green -= strand[i].color.green ? 1 : 0;
    strand[i].color.blue -= strand[i].color.blue ? 1 : 0;
    strand[i].color.brightness--;
  }
}

void reset_strand(bulb_t * strand){
  int i = 0;
  for(i=0;i<50;i++){
    strand[i].color.red = strand[i].color.green = strand[i].color.blue = 0x0;
    strand[i].color.brightness = 0xCC;
  }
}

void loop() {
  static volatile uint8_t ready = 0;
  static uint32_t tick = 0;

  // start sending previous strand
  bulb_t * strand = lite_brite_send_strand(&ready);
  
  //dim(strand);
  if( tick-- == 0 ){
    tick = random(8, 24);
    int num_flashers = random(1, 6);
    reset_strand(strand);
    while(num_flashers--){
      int pos = random(0, 50);
      int color = random(0, 4);
      if( color == 0 ){
        // green
        strand[pos].color.red   = 0x4;
        strand[pos].color.green = 0xD;
        strand[pos].color.blue  = 0x0;
      } else if( color == 1 ){
        // yellow
        strand[pos].color.red   = 0xF;
        strand[pos].color.green = 0xB;
        strand[pos].color.blue  = 0x0;
      } else if(color == 2){
        // red
        strand[pos].color.red   = 0xF;
        strand[pos].color.green = 0x0;
        strand[pos].color.blue  = 0x0;
        strand[pos].color.brightness  = 0xFF * .7;
      } else {
        // blue
        strand[pos].color.red   = 0x0;
        strand[pos].color.green = 0x0;
        strand[pos].color.blue  = 0xA;
      }
    }
  }

  // Do the work in here to compute the next strand

  // wait for entire buffered strand to finish before looping again
  while(!ready){}
}
