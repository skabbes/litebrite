#include "Arduino.h"
#include "litebrite.h"

const int debugPin = 5;
color_t strand[50] = {0};
color_t green  = { 0x4, 0xD, 0x0, 0xFF };
color_t yellow = { 0xF, 0xB, 0x0, 0xFF };
color_t black  = { 0x0, 0x0, 0x0, 0xCC };
color_t red    = { 0xF, 0x0, 0x0, 0xFF * .7 };
color_t blue   = { 0x0, 0x0, 0xA, 0xFF };

void dim(color_t * strand);
void rotate(color_t * strand);
void setup(){
  Serial.begin(9600);
  pinMode(debugPin, OUTPUT);
  digitalWrite(debugPin, HIGH);
  delay(3000);
  digitalWrite(debugPin, LOW);

  lite_brite_init();

  for(int i=0;i<50;i++){
    if( i < 50/4){
      strand[i] = green;
    } else if(i < 50*2/4){
      strand[i] = yellow;
    } else if(i < 50 * 3/ 4){
      strand[i] = red;
    } else {
      strand[i] = blue;
    }
  }

  int i = 12;
  // half second in initial state
  while(i--){
    lite_brite_send_strand_blocking(strand);
  }

  i = 16;
  // dim lights for next 3/4 second
  while(i--){
   dim(strand);
   lite_brite_send_strand_blocking(strand);
  }
}

void rotate(color_t * strand){
  // rotate the bulbs by 1
  color_t prev = strand[0];
  color_t temp;
  int i;
  for(i=1;i<50;i++){
    temp = strand[i];
    strand[i] = prev;
    prev = temp;
  }
  strand[0] = prev;
}

void dim(color_t * strand){
  int i;
  for(i=0;i<50;i++){
    strand[i].red -= strand[i].red ? 1 : 0;
    strand[i].green -= strand[i].green ? 1 : 0;
    strand[i].blue -= strand[i].blue ? 1 : 0;
    strand[i].brightness--;
  }
}

void reset_strand(color_t * strand){
  for(int i=0;i<50;i++){
    strand[i] = black;
  }
}

void loop() {
  static volatile uint8_t ready = 0;
  static uint32_t tick = 0;

  // start sending previous strand
  lite_brite_send_strand(strand, &ready);

  if( tick-- == 0 ){
    tick = random(8, 24);
    int num_flashers = random(1, 6);
    reset_strand(strand);
    while(num_flashers--){
      int pos = random(0, 50);
      int color = random(0, 4);

      if( color == 0 ){
        strand[pos] = green;
      } else if( color == 1 ){
        strand[pos] = yellow;
      } else if(color == 2){
        strand[pos] = red;
      } else {
        strand[pos] = blue;
      }
    }
  }

  // Do the work in here to compute the next strand

  // wait for entire buffered strand to finish before looping again
  while(!ready){}
}
