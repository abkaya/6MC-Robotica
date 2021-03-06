//========================================
//
//  qrcode.c | Reading QR data and error codes from
//  the ramdisk-written QR.dat file
//
//  Written by:
//    Abdil Kaya
//      - mkRamDisk.sh
//      - QRDecoder.sh
//    Robin Janssens:
//      - qrcode.c
//
//========================================

#include <time.h>
#include "qrcode.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lin-delay.h"

#define DEBUG_ABORT

int QRCodeDecode(char* Data,int MaxDataLen)
{

    char status[16];   // status code to return

    //==============================
    // Get system time
    //==============================
    int time_int= getCurrentTime();
#ifdef DEBUG_ABORT
    printf("system time on start: %i\n",time_int);   // print integer value to debug console
#endif

    //==============================
    // Scan QR code
    //==============================
    system("./QRDecoder.sh");   // run a script

    int time_dif = -1;       // init on a negative number

    FILE *fp;

    do
    {
        //==============================
        // Get time from .dat file
        //==============================
        fp = fopen("/mnt/ramdisk/QR.dat", "r");  // read from QR.dat
        char time_file[32];
        //fgets(time_file, 32, (FILE*)fp);      // get the scan time from the first line
        fgets(time_file, 32, (FILE*)fp);     // get the scan time from the first line
        int time_file_int = atoi(time_file);    // cast string to integer
#ifdef DEBUG_ABORT
        printf("time in file: %i\n", time_file_int);  // print integer value to debug console
        // print data to debug console
#endif
        fclose(fp);  // close QR.dat

        //==============================
        // Compare our time with the time written in QR.dat. -- Integrity check
        //==============================
        time_dif = time_file_int - time_int;   // calculate the difference
#ifdef DEBUG_ABORT
        printf("time difference: %i\n", time_dif);   // print integer value to debug console
        _delay_ms(2000);
#endif
#ifdef DEBUG_ABORT
        printf("time difference: %i\n", time_dif);   // print integer value to debug console
#endif
    }
    while (abs(time_dif) > 5);

    char time_file[32];
    fp = fopen("/mnt/ramdisk/QR.dat", "r");  // read from QR.dat
    fgets(time_file, 32, fp);     // get the scan time from the first line
    fgets(Data, MaxDataLen, fp);  // get the content of the QR code
    fgets(status, 16, fp);        // get the status of the QR code scanning
    fclose(fp);  // close QR.dat

    int res = atoi(status);   // convert to integer
#ifdef DEBUG_ABORT
    printf("status: %i\n",res);
#endif
    return res;

}

int getCurrentTime()
{
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
    return time_int;
}
