//========================================
//
//  qrcode.c | C library to get data from QR code on Raspberry Pi
//  Written by Abdil Kaya & Robin Janssens
//  Commissioned by University of Antwerp
//
//========================================

#include <time.h>
#include <string.h>
#include <stdio.h>

#define DEBUG_ABORT

int QRCodeDecode(char* Data,int MaxDataLen) {

    char status[16];   // status code to return

    //==============================
    // Get system time
    //==============================
    int time_int = getCurrentTime();
    #ifdef DEBUG_ABORT
        printf("system time on start: %i\n",time_int);   // print integer value to debug console
    #endif

    //==============================
    // Scan QR code
    //==============================
    system("./QRDecoder.sh");   // run a script

    int time_dif = -1;       // init on a negative number
    do {
        //==============================
        // Get time from .dat file
        //==============================
        FILE *fp;
        fp = fopen("/mnt/ramdisk/QR.dat", "r");  // read from QR.dat
        char time_file[32];
        //fgets(time_file, 32, (FILE*)fp);      // get the scan time from the first line
        fgets(time_file, 32, (FILE*)fp);     // get the scan time from the first line
        fgets(Data, MaxDataLen, (FILE*)fp);  // get the content of the QR code
        fgets(status, 16, (FILE*)fp);        // get the status of the QR code scanning
        int time_file_int = atoi(time_file);    // cast string to integer
        #ifdef DEBUG_ABORT
            printf("time in file: %i\n", time_file_int);  // print integer value to debug console
            printf("content: %s\n",Data);                 // print data to debug console
        #endif
        fclose(fp);  // close QR.dat

        //==============================
        // Check if data is valid
        //==============================
        time_dif = time_file_int - time_int;   // calculate the difference
        #ifdef DEBUG_ABORT
            printf("time difference: %i\n", time_dif);   // print integer value to debug console
            _delay_ms(2000);
        #endif
    } while (time_dif < 0);

    int res = atoi(status);   // convert to integer
        #ifdef DEBUG_ABORT
            printf("status: %i\n",res);
        #endif
    return res;

}

int getCurrentTime() {
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
}
