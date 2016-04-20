//========================================
//
//  rfcomms.c
//  Written by Abdil Kaya & Robin Janssens
//
//========================================

#include "rfcomms.h"

#define DEBUG_ABORT

/**
 * \brief Initialize RF communications system
 * \return
 * 0:OK \n
 * 1:Error \n
*/
int RfCommsInit(void) {
    int status = RfCC1101Init( &RfCC1101 ); // send settings to init function
    // status return
    if (status == 0) return 0;              // if status is 0 return 'ok'
    else return 1;                          // if status is 'other' return 'error'
}

/**
 * \brief Send data packet in RfCommsPacket struct form
 * \param Packet :Pointer to packet to be transmitted
 * \return
 * 0:OK \n
 * 1:Error \n
*/
int RfCommsSendPacket(RfCommsPacket *Packet) {
    int i;                                      // initialize variable for loop counters
    uint8 Data[RFCOMMS_MAX_DATA_LEN];           // initialize array to hold data
    for(i=0;i<RFCOMMS_MAX_DATA_LEN;i++) {       // copy array value by value because "Data = Packet->Data;" doesnt work
        Data[i] = Packet->Data[i];
    }

    //==============================
    // Log
    //==============================
    #ifdef DEBUG_ABORT
    printf("Send data before stuffin: ");
    for(i=0;i<RFCOMMS_MAX_DATA_LEN;i++) {       // print data to send before bytestuffing
        printf("%i ",Data[i]);
    }
    printf ("\n\n");
    #endif

    //==============================
    // Bytestuffing
    //==============================
    //------------------------------
    // Shift Right
    //------------------------------
    for(i=RFCOMMS_MAX_DATA_LEN;i>=1;i--) {
        Data[i] = Data[i-1];
    }
    Data[0] = 0;
    //------------------------------
    // Stuff
    //------------------------------
    for(i=0;i<(Packet->DataLen+1);i++) {                            // iterate all fields
        if ( Data[i] == 0 ) {                                       // if field == 0
            int j = i,replacement=0;                                // initialze variables
            do {
                j++;                                                // remember place of next zero's
                replacement++;                                      // count places between zero's
            } while ( Data[j] != 0 && j < (Packet->DataLen+1) );    // while ( (data != 0) or !(end of data) )
            Data[i] = replacement;                                  // replace zero by (places till next zero)
            i = j-1;                                                // jump directly to next zero
        }
    }

    //==============================
    // Log
    //==============================
    #ifdef DEBUG_ABORT
    printf("Send data after stuffin: ");
    for(i=0;i<RFCOMMS_MAX_DATA_LEN;i++) {       // print data to send after bytestuffing
        printf("%i ",Data[i]);
    }
    printf ("\n\n");
    #endif
    //==============================
    // Send Data
    //==============================
    int status = RfCC1101FIFOSendData( &RfCC1101, Data, Packet->DstRfAddr );
    // status return
    if (status == 0) return 0;              // if status is 0 return 'ok'
    else return 1;                          // if status is 'other' return 'error'
}

/**
 * \brief Receive data packet in RfCommsPacket struct form. Blocks until packet is received.
 * \param Packet :Pointer to packet receive buffer
 * \return
 * 0:OK \n
 * 1:Error \n
*/
int RfCommsReceivePacket(RfCommsPacket *Packet) {
    int i;
    //==============================
    // Receive Data
    //==============================
    int status = RfCC1101FIFOReceiveData( &RfCC1101, Packet->Data, &Packet->Rssi, &Packet->Lqi );   // receive data

    //==============================
    // Log
    //==============================
    #ifdef DEBUG_ABORT
    printf("Received data before destuffin: ");
    for(i=0;i<RFCOMMS_MAX_DATA_LEN;i++) {           // print received bytestuffed data
        printf("%i ",Packet->Data[i]);
    }
    printf ("\n\n");
    #endif

    //==============================
    // Undo Bitstuffing
    //==============================
    //------------------------------
    // Unstuff
    //------------------------------
    int index = 0;
    int distance = 0;
    do {
        distance = Packet->Data[index];     // data of value holds places till next zero
        Packet->Data[index] = 0;            // replace by 0 again
        index += distance;                  // next index = current index + value of next zero
    } while (index<RFCOMMS_MAX_DATA_LEN && Packet->Data[index] != 0);   // do as long as not 0 (end of stream delimiter) or MAX length reached
    //------------------------------
    // Shift Left
    //------------------------------
    for(i=0;i<RFCOMMS_MAX_DATA_LEN-1;i++) {
        Packet->Data[i] = Packet->Data[i+1];
    }
    Packet->Data[RFCOMMS_MAX_DATA_LEN-1] = 0;


    //==============================
    // Log
    //==============================
    #ifdef DEBUG_ABORT
    printf("Received data after destuffin: ");
    for(i=0;i<RFCOMMS_MAX_DATA_LEN;i++) {       // print received dat without bytestuffing
        printf("%i ",Packet->Data[i]);
    }
    printf ("\n\n");
    #endif

    // status return
    if (status == 0) return 0;              // if status is 0 return 'ok'
    else return 1;                          // if status is 'other' return 'error'
}


/**
 * \brief Polls whether packet is ready in receive buffer
 * \param PollStatus :Pointer to poll status variable. 0: No packet ready 1: Packet ready
 * \return
 * 0:OK \n
 * 1:Error \n
*/
int RfCommsReceivePoll(uint8 *PollStatus) {
    int status = RfCC1101FIFOReceivePoll( &RfCC1101, PollStatus );  // check if data is available in receive buffer
    // status return
    if (status == 0 || status == 1) return 0;              // if status is 0 return 'ok'
    else return 1;                          // if status is 'other' return 'error'
}
