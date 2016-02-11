#include <time.h>
#include <string.h>
#include <stdio.h>

#define DEBUG_ABORT

void main() {
  time_t currentTime = time(NULL);                 // get current time from <time.h>
  char* currentTimeString = ctime(&currentTime);   // cast time_t to string
  char hours[3]   = "";
  char minutes[3] = "";
  char seconds[3] = "";
  char time[7]    = "";
  strncpy(hours,   &currentTimeString[11],2);   // get hours   from currentTimeString
  strncpy(minutes, &currentTimeString[14],2);   // get minutes from currentTimeString
  strncpy(seconds, &currentTimeString[17],2);   // get seconds from currentTimeString
  strcat(time, hours);         // add hours   to time string
  strcat(time, minutes);       // add minutes to time string
  strcat(time, seconds);       // add seconds to time string
  int time_int = atoi(time);   // cast time string to an signed integer

  #ifdef DEBUG_ABORT
    printf("time: %i\n",time_int);   // print integer value to debug console
  #endif

  system("./test.sh");   // run a script
  //system("./QRDecoder.sh");
}
