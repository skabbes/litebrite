#include "Arduino.h"
#include "litebrite.h"

const int debugPin = 5;
color_t strand[50] = {0};
const color_t green  = { 0x4, 0xD, 0x0, 0xFF };
const color_t yellow = { 0xF, 0xB, 0x0, 0xFF };
const color_t black  = { 0x0, 0x0, 0x0, 0xCC };
const color_t red    = { 0xF, 0x0, 0x0, 0xFF * .7 };
const color_t blue   = { 0x0, 0x0, 0xA, 0xFF };
const uint8_t CMD_SET = 1;
const uint8_t CMD_FINISH = 2;

void dim(color_t * strand);
void rotate(color_t * strand);
void setup(){
  Serial.begin(115200);
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

uint8_t readCommand(uint8_t * args){
  // wait until we have 5 bytes to read
  while( Serial.available() < 5 ){}
  Serial.readBytes((char*)args, 5);
  return args[0];
}

void loop() {
  static volatile uint8_t ready = 0;
  static uint32_t tick = 0;

  // start sending previous strand
  lite_brite_send_strand(strand, &ready);

  Serial.println("Ready for commands");
  // Do the work in here to compute the next strand
  uint8_t command;
  static uint8_t args[5] = {0};
  do {
    command = readCommand(args);
    //Serial.println("got a command");
    if(command == CMD_SET){
      uint8_t bulb = args[1];
      strand[bulb].red = args[2];
      strand[bulb].green = ((args[3] & 0xF0) >> 4) & 0xF;
      //Serial.println("green");
      //Serial.println(strand[bulb].green);
      strand[bulb].blue = args[3] & 0xF;
      strand[bulb].brightness = args[4];
    }
  } while( command != CMD_FINISH );
  Serial.println("finished receiving commands");


  // wait for entire buffered strand to finish before looping again
  while(!ready){}
}
