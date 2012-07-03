
#include "Arduino.h"
//#include "strand.h"
//#include "GEColorEffects.h"
#include "LiteBrite.h"

int debugPin = 5;
//int pin = 11;
//int lightCount = 50;
//GEColorEffects lights(pin, lightCount);

bulb_t bulb = {value: 0};

void setup(){
  Serial.begin(9600);
  bulb.color.brightness = 0xCC;
  bulb.color.start_bit = 3;

  bulb_t test = {value: 0xCFFFFFF0};
  Serial.println(test.color.start_bit);
  Serial.println(test.color.addr);
  Serial.println(test.color.brightness);
  Serial.println(test.color.blue);
  Serial.println(test.color.green);
  Serial.println(test.color.red);
  Serial.println(test.color._padding);

  pinMode(debugPin, OUTPUT);
  delay(1000);
  digitalWrite(debugPin, HIGH);
  delay(3000);
  lite_brite_init();
  delay(1000);
  digitalWrite(debugPin, LOW);
}

uint32_t rgb;
uint8_t r = 0xF, g = 0x1, b = 0;

uint8_t pos = 0;

void loop() {
  bulb.color.addr = random(0, 50);
  
  bulb.color.green = random(0, 0xF);
  bulb.color.blue = random(0, 0xF);
  bulb.color.red = random(0, 0xF);

  lite_brite_send(bulb.color);
  delay(1000);
}
