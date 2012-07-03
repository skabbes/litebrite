#include "Arduino.h"
#include "LiteBrite.h"

const int debugPin = 5;


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
    if( i < 50/3){
      strand[i].color.red = 0xF;
    } else if(i > 50 * 2 / 3){
      // white
      strand[i].color.red = 0xC;
      strand[i].color.green = 0xC;  
      strand[i].color.blue = 0xC;
    } else {
      // blue
      strand[i].color.blue = 0xF;
    }
  }
}

void loop() {
  static volatile uint8_t ready = 0;
  static uint32_t tick = 0;

  // start sending previous strand
  bulb_t * strand = lite_brite_send_strand(&ready);

  // Do the work in here to compute the next strand
  
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
  
  // wait for entire buffered strand to finish before looping again
  while(!ready){}
}
