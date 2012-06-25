#ifndef lite_brite_h
#define lite_brite_h

#include <stdint.h>
#include "Arduino.h"

typedef uint32_t cmd_light_t, *cmd_light_p;

#define PACKET_LENGTH 50

typedef struct cmd_packet {
  cmd_light_t light[PACKET_LENGTH];
} cmd_packet_t, *cmd_packet_p;

#define CMD_ADDR_MASK 0x03F00000
#define CMD_BRITE_MASK 0x000FF000
#define CMD_BLUE_MASK 0x00000F00
#define CMD_GREEN_MASK 0x000000F0
#define CMD_RED_MASK 0x0000000F

#define CMD_ADDR_OFFSET 20
#define CMD_BRITE_OFFSET 12
#define CMD_BLUE_OFFSET 8
#define CMD_GREEN_OFFSET 4
#define CMD_RED_OFFSET 0

#define CMD_NULL 0x3C
#define CMD_ONE 0x28
#define CMD_ZERO 0x14

void set_cmd_addr(cmd_light_p, uint32_t);

void set_cmd_brite(cmd_light_p, uint32_t);

void set_cmd_blue(cmd_light_p, uint32_t);

void set_cmd_green(cmd_light_p, uint32_t);

void set_cmd_red(cmd_light_p, uint32_t);

void set_cmd_rgb(cmd_light_p, uint32_t);

uint32_t get_cmd_rgb(cmd_light_t);

void set_cmd(cmd_light_p, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

void set_cmd_data(cmd_light_p, uint32_t, uint32_t, uint32_t, uint32_t);

void init_addrs(cmd_packet_p);

void init_lite_brite(void);

void init_lite_addrs(cmd_packet_p);

void lite_brite_send_packet(cmd_packet_p);

void lite_brite_send_packet_slow(cmd_packet_p);

#endif // lite_brite
