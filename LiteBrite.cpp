#include "LiteBrite.h"

cmd_packet_t active_cmd;

uint8_t send_packet_light;
uint16_t send_packet_bit;

void set_cmd_addr(cmd_light_p cmd, uint32_t addr) {
  *cmd &= ~CMD_ADDR_MASK;
  *cmd |= (addr & (CMD_ADDR_MASK >> CMD_ADDR_OFFSET)) << CMD_ADDR_OFFSET;
}

void set_cmd_brite(cmd_light_p cmd, uint32_t brite) {
  *cmd &= ~CMD_BRITE_MASK;
  *cmd |= (brite & (CMD_BRITE_MASK >> CMD_BRITE_OFFSET)) << CMD_BRITE_OFFSET;
}

void set_cmd_blue(cmd_light_p cmd, uint32_t blue) {
  *cmd &= ~CMD_BLUE_MASK;
  *cmd |= (blue & (CMD_BLUE_MASK >> CMD_BLUE_OFFSET)) << CMD_BLUE_OFFSET;
}

void set_cmd_green(cmd_light_p cmd, uint32_t green) {
  *cmd &= ~CMD_GREEN_MASK;
  *cmd |= (green & (CMD_GREEN_MASK >> CMD_GREEN_OFFSET)) << CMD_GREEN_OFFSET;
}

void set_cmd_red(cmd_light_p cmd, uint32_t red) {
  *cmd &= ~CMD_RED_MASK;
  *cmd |= (red & (CMD_RED_MASK >> CMD_RED_OFFSET)) << CMD_RED_OFFSET;
}

void set_cmd_rgb(cmd_light_p cmd, uint32_t rgb) {
  *cmd &= ~(CMD_RED_MASK | CMD_BLUE_MASK | CMD_GREEN_MASK);
  *cmd |= rgb & (0x00000FFF);
}

uint32_t get_cmd_rgb(cmd_light_t cmd) {
  return cmd & 0x00000FFF;
}

void set_cmd(cmd_light_p cmd, uint32_t addr, uint32_t brite, uint32_t red, uint32_t green, uint32_t blue) {
  *cmd &= ~CMD_ADDR_MASK;
  *cmd |= (addr & (CMD_ADDR_MASK >> CMD_ADDR_OFFSET)) << CMD_ADDR_OFFSET;
  *cmd &= ~CMD_BRITE_MASK;
  *cmd |= (brite & (CMD_BRITE_MASK >> CMD_BRITE_OFFSET)) << CMD_BRITE_OFFSET;
  *cmd &= ~CMD_BLUE_MASK;
  *cmd |= (blue & (CMD_BLUE_MASK >> CMD_BLUE_OFFSET)) << CMD_BLUE_OFFSET;
  *cmd &= ~CMD_GREEN_MASK;
  *cmd |= (green & (CMD_GREEN_MASK >> CMD_GREEN_OFFSET)) << CMD_GREEN_OFFSET;
  *cmd &= ~CMD_RED_MASK;
  *cmd |= (blue & (CMD_RED_MASK >> CMD_RED_OFFSET)) << CMD_RED_OFFSET;
}

void set_cmd_data(cmd_light_p cmd, uint32_t brite, uint32_t red, uint32_t green, uint32_t blue) {
  *cmd &= ~CMD_BRITE_MASK;
  *cmd |= (brite & (CMD_BRITE_MASK >> CMD_BRITE_OFFSET)) << CMD_BRITE_OFFSET;
  *cmd &= ~CMD_BLUE_MASK;
  *cmd |= (blue & (CMD_BLUE_MASK >> CMD_BLUE_OFFSET)) << CMD_BLUE_OFFSET;
  *cmd &= ~CMD_GREEN_MASK;
  *cmd |= (green & (CMD_GREEN_MASK >> CMD_GREEN_OFFSET)) << CMD_GREEN_OFFSET;
  *cmd &= ~CMD_RED_MASK;
  *cmd |= (red & (CMD_RED_MASK >> CMD_RED_OFFSET)) << CMD_RED_OFFSET;
}

uint8_t slow_data;

ISR(TIMER2_OVF_vect) {
  digitalWrite(8, HIGH);
  send_packet_bit -= 1;
  if(send_packet_bit >= 27) {
    // Inter-frame space
    OCR2B = CMD_NULL;
  } else if(send_packet_bit == 26) {
    // This is the start signal (10uS high)
    OCR2B = CMD_ONE;
  } else {
    if(active_cmd.light[send_packet_light] & (0x1 << send_packet_bit)) {
      OCR2B = CMD_ONE;
    } else {
      OCR2B = CMD_ZERO;
    }
  }
  if(send_packet_bit == 0) {
    send_packet_light += 1;
    send_packet_bit = 28;
    if(slow_data) {
      send_packet_bit = 2000;
    }
  }
  if(send_packet_light >= 50) {
    OCR2B = CMD_NULL;
    TIMSK2 = 0x00;
    digitalWrite(8, LOW);
  }
}



void init_lite_brite(void) {
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  // CMD_NULL is also the max value for the counter
  // So this is what we want to set OCR0A to
  OCR2A = CMD_NULL;
  OCR2B = CMD_NULL;
  TIMSK2 = 0x01;
  TCCR2A = 0x33;
  TCCR2B = 0x0A;
}

void init_lite_addrs(cmd_packet_p packet) {
  int i;
  for(i = 0; i < PACKET_LENGTH; i++) {
    (*packet).light[i] = (cmd_light_t)0x00000000;
    set_cmd_addr(&(*packet).light[0] + i, i);
  }
}

void lite_brite_send_packet(cmd_packet_p packet) {
  // Disable interrupt
  TIMSK2 = 0x00;
  slow_data = 0;
  memcpy(&active_cmd, packet, 200);
  send_packet_light = 0;
  send_packet_bit = 28;
  // Enable interrupt
  TIMSK2 = 0x01;
}

void lite_brite_send_packet_slow(cmd_packet_p packet) {
  // Disable interrupt
  TIMSK2 = 0x00;
  slow_data = 1;
  memcpy(&active_cmd, packet, 200);
  send_packet_light = 0;
  send_packet_bit = 28;
  // Enable interrupt
  TIMSK2 = 0x01;
}

/*int main(int argc, char** argv) {
  cmd_packet_t packet;
  init_addrs(&packet);
  set_cmd_data(&packet.light[10], 0xCC, 0xF, 0xF, 0xF);
  int i;
  for(i = 0; i < PACKET_LENGTH; i++) {		
  }
  return 0;
}*/
