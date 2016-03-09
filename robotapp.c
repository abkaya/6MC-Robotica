//========================================
//
//  Robotics Lab Project
//  Written by Abdil Kaya & Robin Janssens
//  Commissioned by University of Antwerp
//
//========================================

#include "robotapp.h"
#include "qrcode.h"
#include "tagreader.h"
//#include "dijkstra.h"
#include "rfcomms.h"

#define maxContentLength 32   // max amount of chars in content

#define DEBUG_ABORT
//#define testQR
#define testTag
//#define testDijkstra
//#define testCC1101send
//#define testCC1101receive

void RobotApp(int argc, char *argv[])
{
    //==============================
    // Initialization
    //==============================
    char* qr_data[maxContentLength];    // create QR data holder
    char* tag_data[maxContentLength];   // create tag data holder
    int i;                              // holds counter value
    int res;                            // hold return status values
    LegoMotorSetup(&LegoMotor,1,0,0);   // motor, channel, brake, mode
    LegoMotorSetup(&LegoMotor,2,0,0);
    #ifdef DEBUG_ABORT
        printf ("Initialisation complete\n");
        //system ("espeak -ven+f2 -k5 -a50 -s150 \"Initialasition complete\" --stdout | aplay");
    #endif

    //==============================
    // Test Procedures
    //==============================
    #ifdef testQR     // Try to scan a QR-code every 5 seconds
        while (1) {
            system ("espeak -ven+f2 -k5 -a50 -s150 \"Testing QR code scanning\" --stdout | aplay");
            res = QRCodeDecode(*qr_data, maxContentLength);   // scan for QR code
            printf("QR status: %i\n",res);                      // print status
            _delay_ms(5000);                                  // wait 5 seconds
        }
    #endif
    #ifdef testTag     // Try to scan a QR-code every 5 seconds
        while (1) {
            //system ("espeak -ven+f2 -k5 -a50 -s150 \"Testing Tag scanning\" --stdout | aplay");
            res = TagReaderGetUID( *tag_data );                         // scan tag
            printf("tag status: %d   tag data: %s\n",res,*tag_data);       // print status
            _delay_ms(5000);                                            // wait 5 seconds
        }
    #endif
    #ifdef testDijkstr     // Test dijkstra algorithm

    #endif
    #ifdef testCC1101send     // Send data package every 5 seconds
        while (1) {
            system ("espeak -ven+f2 -k5 -a50 -s150 \"Testing sending wireless communication\" --stdout | aplay");

            RfCommsPacket package;      // create package
            package.DstRfAddr = 5;      // destination RF address
            package.SrcRfAddr = 7;      // source RF address
            package.DataLen = 4;        // number of data bytes in packet (Data array)
            package.Data[0] = 1;        // set data (1337)
            package.Data[1] = 3;        // set data (1337)
            package.Data[2] = 3;        // set data (1337)
            package.Data[3] = 7;        // set data (1337)

            res = RfCommsSendPacket( &package );   // Send data
            printf("send status: %i",res);                      // print status
            _delay_ms(5000);
        }
    #endif
        #ifdef testCC1101receive     // Receive data package every 2 seconds
        while (1) {
            system ("espeak -ven+f2 -k5 -a50 -s150 \"Testing receiving wireless communication\" --stdout | aplay");
            RfCommsPacket package;                          // create package
            uint8 PollStatus;
            res = RfCommsReceivePoll( &PollStatus );        // check for available data
            printf("receive status: %i",res);               // print status

            if ( PollStatus == 1 ) {                        // packet ready
                res = RfCommsReceivePacket( &package );     // receive data
                printf("Packet received! (Rssi: %i, Lqi: %i) data: ",package.Rssi,package.Lqi);
                for(i=0;i<package.DataLen;i++) {
                    printf("%c",package.Data[i]);
                }
                printf("\n");
            }
            else {
                _delay_ms(2000);                            // wait 2 seconds
            }
        }
    #endif

    //==============================
    // Drive
    //==============================
    LegoMotorDirectControl(&LegoMotor,1,25000);
    LegoMotorDirectControl(&LegoMotor,2,25000);

    _delay_ms(4000);

    LegoMotorDirectControl(&LegoMotor,1,-30000);
    LegoMotorDirectControl(&LegoMotor,2,0);

    _delay_ms(2000);

    LegoMotorDirectControl(&LegoMotor,1,-30000);
    LegoMotorDirectControl(&LegoMotor,2,-30000);

    _delay_ms(2000);

    LegoMotorSetup(&LegoMotor,1,1,0);
    LegoMotorSetup(&LegoMotor,2,1,0);

    LegoMotorDirectControl(&LegoMotor,1,0);
    LegoMotorDirectControl(&LegoMotor,2,0);

    //system ("espeak -ven+f2 -k5 -a50 -s150 \"Let's get ready to rumble Robin, are you ready?.\" --stdout | aplay");

    //==============================
    // Read QR code
    //==============================
    //char* qr_data[maxContentLength];
    res = QRCodeDecode(*qr_data, maxContentLength);   // Scan for QR code
    switch(res) {
      case 0 :   // OK
        // code
        #ifdef DEBUG_ABORT
          printf ("QR code = %s\n",*qr_data);  // log to console
        #endif
        break;
      case 1 :   // camera error
        // code
        #ifdef DEBUG_ABORT
          printf ("Camera Error\n");  // log to console
        #endif
        break;
      case 2 :   // no QR code detected
        // code
        #ifdef DEBUG_ABORT
          printf ("No QR code\n");  // log to console
        #endif
        break;
      case 3 :   // other error
        // code
        #ifdef DEBUG_ABORT
          printf ("Error while scanning QR code\n");  // log to console
        #endif
        break;
      default :
        break;
      //code
    }



    printf ("Ready.\n");
}
