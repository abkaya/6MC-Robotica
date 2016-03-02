//========================================
//
//  tagreader.c | C library to get data from tagreader on Raspberry Pi
//  Written by Abdil Kaya & Robin Janssens
//  Commissioned by University of Antwerp
//
//========================================
#include <stdlib.h>
#include <stdio.h>

#define DEBUG_ABORT

int TagReaderGetUID(char *Data) {

    //==============================
    // Initialize Variables
    //==============================
    FILE *fp;        // create file object
    int status;      // status value
    char line[32];   // data from read

    //==============================
    // Scan Tag
    //==============================
    fp = popen("./tagreader.sh", "r");
    if (fp == NULL) {
        printf("no file\n");
        /* Handle error */;
    }
    while ( fgets(line, 32, fp) != NULL)  {
        printf("line: %s\n", line);
    }
    status = pclose(fp);

    //==============================
    // Catch pclose error
    //==============================
    if (status == -1) {
        #ifdef DEBUG_ABORT
            printf("error at pclose()\n");  // print status to debug console
        #endif
    }
    else {
        #ifdef DEBUG_ABORT
            printf("succeeded at pclose()\n");  // print status to debug console
        #endif
    }

    return 0;

}
