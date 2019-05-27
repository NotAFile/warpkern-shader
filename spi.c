#include <stdio.h>
#include <stdlib.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <time.h>
#include <unistd.h>

float fps = 60;
int frames = -10;

void send_color(int stripes, int leds, int r, int g, int b) {
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
      buffer[i] = b; i++;   // blue
      buffer[i] = g; i++;   // green
      buffer[i] = r; i++;   // red
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

int main(void)
{
  wiringPiSetup();

  int fd = wiringPiSPISetup(0, 5000000);
  if (fd == -1) {
    perror("SPI Channel setup failed");
    abort();
  }

  struct timespec gettime_now;
  long start;
  long frame = 0;
  while(frames < 0 || frame < frames) {
    clock_gettime(CLOCK_REALTIME, &gettime_now);
    start = gettime_now.tv_nsec;

    send_color(12,192, (255-frame*16)%255, frame*16, 0);

    frame++;
    clock_gettime(CLOCK_REALTIME, &gettime_now);
    useconds_t toSleep = ((1/fps)*1000000 - (gettime_now.tv_nsec - start)/1000.0);
    usleep(toSleep);
  }
}
