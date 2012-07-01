#include "LiteBrite.h"

cmd_packet_t active_cmd;

volatile uint8_t send_packet_light;
volatile uint16_t send_packet_bit;

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

void  packetForI(uint8_t packet_num, uint8_t *pac){
  int i = 0;
  for(i=0;i<6;i++){
    if( packet_num | (1 << i) ){
      pac[6-i] = 1;
    } else {
     pac[6-i]=0;
    }
  }
}

uint8_t black_packet[] = {
  1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2
};

uint8_t red_packet[] = {
  1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,2,2
};

uint8_t blue_packet[] = {
  1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,2,2
};

uint8_t green_packet[] = {
  1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,2,2
};

uint8_t *packet = black_packet;

volatile uint8_t lite_num = 0;
volatile uint8_t pos = 0;
volatile uint8_t ready = 0;
volatile uint8_t initial = 1;


ISR(TIMER2_OVF_vect) {

  uint8_t current = packet[pos];
  if(current == 0){
    OCR2B = CMD_ZERO;
  } else if(current == 1){
    OCR2B = CMD_ONE;
  } else {
    OCR2B = CMD_NULL;
  }

  pos++;
  if(pos >= sizeof(black_packet)){
    packetForI(pos, packet);
    ready = 1;
    pos = 0;
  }
}

void send_all(){
  int i = 0;
  for(i=0;i<50;i++){    
    ready = 0;  
    while( !ready ){}
  } 
}

void init_lite_brite(void) {
  pinMode(9, OUTPUT);
  //digitalWrite(9, LOW);
  // CMD_NULL is also the max value for the counter
  // So this is what we want to set OCR0A to
  
  TCCR2A = 0x33;
  TCCR2B = 0x0A;
  OCR2A = CMD_NULL;
  OCR2B = CMD_NULL;

  // enable interrupts
  ready = 0;
  TIMSK2 = 0x01;
  // wait until the packet has been send completely
  int i = 0;
  for(i=0;i<50;i++){
    ready = 0;
    send_all();
  }
  // do nothing just wait
  // disable interrupts
  TIMSK2 = 0x00;
  initial = 0;
}



void all_blue(void) {
  ready = 0;
  packet = blue_packet;
  //packetForI(lite, packet);
  TIMSK2 = 0x01;
  // wait until the packet has been send completely
  send_all();
  // disable interrupts
  TIMSK2 = 0x00;
}

void all_red(void) {
  ready = 0;
  packet = red_packet;
  TIMSK2 = 0x01;
  // wait until the packet has been send completely
  send_all();
  // disable interrupts
  TIMSK2 = 0x00;
}

void all_green(void) {
  ready = 0;
  packet = green_packet;
  TIMSK2 = 0x01;
  // wait until the packet has been send completely
  send_all();
  // disable interrupts
  TIMSK2 = 0x00;
}

void all_black(void) {
  ready = 0;
  packet = black_packet;
  TIMSK2 = 0x01;
  // wait until the packet has been send completely
  send_all();
  // disable interrupts
  TIMSK2 = 0x00;
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
