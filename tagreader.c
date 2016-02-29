//========================================
//
//  tagreader.c | C library to get data from tagreader on Raspberry Pi
//  Written by Abdil Kaya & Robin Janssens
//  Commissioned by University of Antwerp
//
//========================================

#include <stdio.h>

#define DEBUG_ABORT

int TagReaderGetUID(char *Data) {


    FILE *fp;        // create file object
    int status;      // status value
    char line[32];   // data from read


    fp = popen("./tagreader.sh", "r");
    if (fp == NULL) {
        printf("no file\n");
        /* Handle error */;
    }

    while ( fgets(line, 32, fp) != NULL)  {
        printf("line: %s\n", line);
    }


    status = pclose(fp);
    if (status == -1) {
        #ifdef DEBUG_ABORT
            printf("error at pclose()\n");  // print status to debug console
        #endif
        // Error reported by pclose()
    }
    else {
        #ifdef DEBUG_ABORT
            printf("succeeded at pclose()\n");  // print status to debug console
        #endif
        // succeeded
    }

    return 0;

}
