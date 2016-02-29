//========================================
//
//  Robotics Lab Project
//  Written by Abdil Kaya & Robin Janssens
//  Commissioned by University of Antwerp
//
//========================================

#include "robotapp.h"
#include "qrcode.h"

#define maxContentLength 30   // max amount of chars in content

#define DEBUG_ABORT
//#define testQR
#define testTag

void RobotApp(int argc, char *argv[])
{
    //==============================
    // Initialization
    //==============================
    char* qr_data[maxContentLength];    // create QR data holder
    char* tag_data;                     // create tag data holder
    int res;                            // hold return status values
    LegoMotorSetup(&LegoMotor,1,0,0);   // motor, channel, brake, mode
    LegoMotorSetup(&LegoMotor,2,0,0);
    #ifdef DEBUG_ABORT
        printf ("Initialisation complete\n");
        system ("espeak -ven+f2 -k5 -a50 -s150 \"Initialasition complete\" --stdout | aplay");
    #endif

    //==============================
    // Test Procedures
    //==============================
    #ifdef testQR     // Try to scan a QR-code every 5 seconds
        while (1) {
            system ("espeak -ven+f2 -k5 -a50 -s150 \"Testing QR code scanning\" --stdout | aplay");
            res = QRCodeDecode(*qr_data, maxContentLength);   // Scan for QR code
            printf("QR status: %i",res);                      // print status
            _delay_ms(5000);
        }
    #endif
    #ifdef testTag     // Try to scan a QR-code every 5 seconds
        while (1) {
            system ("espeak -ven+f2 -k5 -a50 -s150 \"Testing Tag scanning\" --stdout | aplay");
            res = TagReaderGetUID(*tag_data);   // Scan for QR code
            printf("tag status: %i",res);                      // print status
            _delay_ms(5000);
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

    system ("espeak -ven+f2 -k5 -a50 -s150 \"Let's get ready to rumble Robin, are you ready?.\" --stdout | aplay");

    //==============================
    // Read QR code
    //==============================
    //char* qr_data[maxContentLength];
    res = QRCodeDecode(*qr_data, maxContentLength);   // Scan for QR code
    switch(res) {
      case 0 :   // OK
        // code
        #ifdef DEBUG_ABORT
          printf ("QR code = %s\n",qr_data);  // log to console
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
          printf ("Erro while scanning QR code\n");  // log to console
        #endif
        break;
      default :
        break;
      //code
    }



    printf ("Ready.\n");
}
