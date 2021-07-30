#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "spi.h"
#include <cstdint>

#ifdef USE_WIRINGPI
#include <wiringPi.h>
#include <wiringPiSPI.h>
#endif

float fps = 60;
int frames = -10;

#ifdef USE_WIRINGPI
void send_color(int stripes, int leds, uint8_t* pixbuf) {
  unsigned char buffer[(2+192*12)*4];
  int i = 0;
  // start frame -> 32bit all 0
  buffer[i] = 0; i++;
  buffer[i] = 0; i++;
  buffer[i] = 0; i++;
  buffer[i] = 0; i++;

  int s,l;
  for (s=0; s < stripes; s++){
    for (l=0; l < leds; l++){
      // first led -> 32bit all 1
      buffer[i] = 0b11100001; i++;
      buffer[i] = pixbuf[2 + (s * leds + l) * 4]; i++;   // blue
      buffer[i] = 0; i++;   // green
      buffer[i] = 0; i++;   // red
    }
  }

  // end frame -> all 1
  buffer[i] = 0xFF; i++;
  buffer[i] = 0xFF; i++;
  buffer[i] = 0xFF; i++;
  buffer[i] = 0xFF; i++;

  int status = wiringPiSPIDataRW(0, buffer, i);
  if (status == -1) {
    perror("SPI write failed");
    abort();
  }
}

void init_leds()
{
  wiringPiSetup();

  int fd = wiringPiSPISetup(0, 5000000);
  if (fd == -1) {
    perror("SPI Channel setup failed");
    abort();
  }
}
#else
void init_leds() {
    printf("init leds\n");
}

void send_color(int stripes, int leds, uint8_t* pixbuf) {
    // printf("send colors\n");
}
#endif
