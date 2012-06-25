
#include "Arduino.h"
//#include "strand.h"
//#include "GEColorEffects.h"
#include "LiteBrite.h"


//int pin = 11;
//int lightCount = 50;
//GEColorEffects lights(pin, lightCount);

cmd_packet_t data;

void setup(){
  //lights.enumerate();
  //pinMode(pin, OUTPUT);
  init_lite_brite();
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  int i;
  for(i = 0; i < 50; i++) {
    //set_cmd(&data.light[i], i, 0xCC, 0x1, 0x1, 0x1);
    //set_cmd_rgb(&data.light[i], i);
    data.light[i] = i << CMD_ADDR_OFFSET | 0x000CC000 | i;
  }
  lite_brite_send_packet_slow(&data);
  delay(5000);
}

uint32_t rgb;
uint8_t r = 0xF, g = 0x1, b = 0, i = 0;
void loop() {
//  digitalWrite(pin, LOW);  
//  delay(1000);
//  digitalWrite(pin, HIGH);  
//  delay(2000);
  //”Red White and Blue” – simple flash between red and white, then blue and white
  
  //lights.fill_color(0, lightCount, DEFAULT_INTENSITY, COLOR_RED);
  
  lite_brite_send_packet(&data);
  if (r == 0xF && g > 0) {
    g++;
    if(g == 0xF) {
      r--;
    }
  } else if (g == 0xF && r > 0) {
    r--;
    if(r == 0x0) {
      b = 1;
    }
  } else if (g == 0xF && b > 0) {
    b++;
    if(b == 0xF) {
      g--;
    }
  } else if (b == 0xF && g > 0) {
    g--;
    if(g == 0x0) {
      r++;
    }
  } else if (b == 0xF && r > 0) {
    r++;
    if(r == 0xF) {
      b--;
    }
  } else if (r == 0xF && b > 0) {
    b--;
    if(b == 0x0) {
      g++;
    }
  }
  
  for(i = 0; i < PACKET_LENGTH; i++) {
    set_cmd_data(&data.light[47], 0xCC, r, g, b);
    if(i == PACKET_LENGTH - 1) {
      //set_cmd_data(&data.light[i], 0xCC, r, g, b);
    } else {
      rgb = get_cmd_rgb(data.light[i+1]);
      //set_cmd_rgb(&data.light[i], rgb);
    }
  }
  //lite_brite_test(data.light[0]);
  delay(100);
  
}
