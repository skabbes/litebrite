#include "Arduino.h"

#define BULB_COUNT         50
#define CHANNEL_MAX        (0xF) //Each color channel is 4-bit
#define DEFAULT_INTENSITY  (0xCC) //Brightness is 8-bit
#define HUE_MAX            ((CHANNEL_MAX+1)*6-1)
#define COLOR(r,g,b)       ((r)+((g)<<4)+((b)<<8))  //Color is 12-bit (4-bit each R, G, B)

#define COLOR_WHITE      COLOR(CHANNEL_MAX,CHANNEL_MAX,CHANNEL_MAX)
#define COLOR_BLACK      COLOR(0,0,0)
#define COLOR_RED        COLOR(CHANNEL_MAX,0,0
#define COLOR_GREEN      COLOR(0,CHANNEL_MAX,0)
#define COLOR_BLUE       COLOR(0,0,CHANNEL_MAX)
#define COLOR_CYAN       COLOR(0,CHANNEL_MAX,CHANNEL_MAX)
#define COLOR_MAGENTA    COLOR(CHANNEL_MAX,0,CHANNEL_MAX)
#define COLOR_YELLOW     COLOR(CHANNEL_MAX,CHANNEL_MAX,0)
#define COLOR_PURPLE     COLOR(0xa,0x3,0xd)
#define COLOR_ORANGE     COLOR(0xf,0x1,0x0)
#define COLOR_WARMWHITE  COLOR(0xf,0x7,0x2)

typedef struct {
  uint8_t address;
  uint8_t brightness;
  uint16_t color;
} Bulb;


// the function that kicks off the entire process
// @note never returns...
void startSendingStrandData( void (*fn)(cmd_packet_p, cmd_packet_p), uint8_t pin );
