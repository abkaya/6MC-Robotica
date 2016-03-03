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
    int status = RfCC1101Init( &RfCC1101 );
    // status return
    if (status == 0) return 0;
    else return 1;
}

/**
 * \brief Send data packet in RfCommsPacket struct form
 * \param Packet :Pointer to packet to be transmitted
 * \return
 * 0:OK \n
 * 1:Error \n
*/
int RfCommsSendPacket(RfCommsPacket *Packet) {
    int i;
    uint8 Data[RFCOMMS_MAX_DATA_LEN];
    for(i=0;i<RFCOMMS_MAX_DATA_LEN;i++) {       // because "Data = Packet->Data;" doesnt work
        Data[i] = Packet->Data[i];
    }

    //==============================
    // Log
    //==============================
    #ifdef DEBUG_ABORT
    printf("Send data before stuffin: ");
    for(i=0;i<RFCOMMS_MAX_DATA_LEN;i++) {
        printf("%i ",Data[i]);
    }
    printf ("\n");
    #endif

    //==============================
    // Bitstuffing
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
    for(i=0;i<(Packet->DataLen+1);i++) {
        if ( Data[i] == 0 ) {
            int j = i,replacement=0;
            do {
                j++;
                replacement++;
            } while ( Data[j] != 0 && j < (Packet->DataLen+1) );
            Data[i] = replacement;
            i = j-1;
        }
    }

    //==============================
    // Log
    //==============================
    #ifdef DEBUG_ABORT
    printf("Send data after stuffin: ");
    for(i=0;i<RFCOMMS_MAX_DATA_LEN;i++) {
        printf("%i ",Data[i]);
    }
    printf ("\n");
    #endif
    //==============================
    // Send Data
    //==============================
    int status = RfCC1101FIFOSendData( &RfCC1101, Packet->Data, Packet->DstRfAddr );
    // status return
    if (status == 0) return 0;
    else return 1;
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
    int status = RfCC1101FIFOReceiveData( &RfCC1101, Packet->Data, &Packet->Rssi, &Packet->Lqi );

    //==============================
    // Log
    //==============================
    #ifdef DEBUG_ABORT
    printf("Received data before destuffin: ");
    for(i=0;i<RFCOMMS_MAX_DATA_LEN;i++) {
        printf("%i ",Packet->Data[i]);
    }
    printf ("\n");
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
        distance = Packet->Data[index];
        Packet->Data[index] = 0;
        index += distance;
    } while (index<RFCOMMS_MAX_DATA_LEN && Packet->Data[index] != 0);
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
    for(i=0;i<RFCOMMS_MAX_DATA_LEN;i++) {
        printf("%i ",Packet->Data[i]);
    }
    printf ("\n");
    #endif

    // status return
    if (status == 0) return 0;
    else return 1;
}


/**
 * \brief Polls whether packet is ready in receive buffer
 * \param PollStatus :Pointer to poll status variable. 0: No packet ready 1: Packet ready
 * \return
 * 0:OK \n
 * 1:Error \n
*/
int RfCommsReceivePoll(uint8 *PollStatus) {
    int status = RfCC1101FIFOReceivePoll( &RfCC1101, PollStatus );
    // status return
    if (status == 0) return 0;
    else return 1;
}
