
#include "Arduino.h"
//#include "strand.h"
//#include "GEColorEffects.h"
#include "LiteBrite.h"

int debugPin = 5;
//int pin = 11;
//int lightCount = 50;
//GEColorEffects lights(pin, lightCount);

cmd_packet_t data;

void setup(){
  Serial.begin(9600);
  pinMode(debugPin, OUTPUT);
  delay(1000);
  digitalWrite(debugPin, HIGH);
  delay(3000);

  //lights.enumerate();
  //pinMode(pin, OUTPUT);
  delay(10000);

  init_lite_brite();
  delay(1000);
  digitalWrite(debugPin, LOW);


  //init_lite_addrs(&data);
}

uint32_t rgb;
uint8_t r = 0xF, g = 0x1, b = 0, i = 0;

void loop() {
  digitalWrite(debugPin, HIGH);
  all_red();
  delay(1000);
  all_green();
  delay(1000);
  all_blue();
  delay(1000);
  all_black();
  digitalWrite(debugPin, LOW);
  delay(1000);
}
