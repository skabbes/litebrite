/*#include "strand.h"

#define BULB_COUNT 50
#define CHANNEL_MAX        (0xF) //Each color channel is 4-bit
#define DEFAULT_INTENSITY  (0xCC) //Brightness is 8-bit
#define HUE_MAX            ((CHANNEL_MAX+1)*6-1)
#define COLOR(r,g,b)       ((r)+((g)<<4)+((b)<<8))  //Color is 12-bit (4-bit each R, G, B)
#define COLOR_WHITE      COLOR(CHANNEL_MAX,CHANNEL_MAX,CHANNEL_MAX
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


volatile uint8_t protocolHasFinished = 1;

// Buffered space for writing out our strands
Bulb strand_a [BULB_COUNT];
Bulb strand_b [BULB_COUNT];
Bulb * currentStrand = strand_a;
Bulb * bufferedStrand = strand_b;

// initialize the initial strand for addressing purposes
uint8_t i;
for(i=0; i<50; i+=1){
  currentStrand[i].address = i;
  currentStrand[i].brightness = DEFAULT_INTENSITY;
  currentStrand[i].color = COLOR_BLACK;
}

void startSendingStrandData( void (*fn)(Bulb*, Bulb*), uint8_t pin ){

  Bulb * temp = bufferStrand;

  // continually loop here to allow the strand
  while(1){
    protocolHasFinished = 0;
    writeAStrand(currentStrand, pin, &protocolHasFinished);

    // start generating the next strand in parallel
    fn(currentStrand, bufferedStrand);

    // wait for the protocol to complete its writing
    while( !protocolHasFinished ){}

    // swap the 2 buffers
    Bulb * temp = bufferStrand;
    bufferStrand = currentStrand;
    currentStrand = temp;
  }
}*/
