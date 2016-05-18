//========================================
//
//  tagreader.c | C library to get data from tagreader on Raspberry Pi
//  Written by Robin Janssens
//
//========================================
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#define DEBUG_ABORT

int TagReaderGetUID(char *Data) {

    //==============================
    // Initialize Variables
    //==============================
    FILE *fp;        // create file object
    int status;      // status value
    int tag_node;

    //==============================
    // Scan Tag
    //==============================
    fp = popen("./tagreader.sh", "r");
    if (fp == NULL) {
        printf("no file\n");
        /* Handle error */;
    }
    fgets(Data, 15, fp);
    printf("Data: %s\n", Data);
    status = pclose(fp);

    if(strcmp(Data,"0447E422A93480")==0)
        tag_node=3;
    else if(strcmp(Data,"04ECD612A93480")==0)
        tag_node=4;
    else if(strcmp(Data,"04631322A93484")==0)
        tag_node=5;
    else if(strcmp(Data,"0488FE22A93480")==0)
        tag_node=7;
    else if(strcmp(Data,"0424E622A93480")==0)
        tag_node=9;
    else if(strcmp(Data,"04D00412A93484")==0)
        tag_node=11;
    else
        tag_node=-1;



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



    return tag_node;

}
