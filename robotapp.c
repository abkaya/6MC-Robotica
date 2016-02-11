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

void RobotApp(int argc, char *argv[])
{
    int MaxDataLen=20;
    char* Data[MaxDataLen]

    LegoMotorSetup(&LegoMotor,1,0,0);
    LegoMotorSetup(&LegoMotor,2,0,0);


    printf ("Initialisation complete\n");


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
    char qr_data[maxContentLength] = "";
    int res = QRCodeDecode(char *qr_data,int maxContentLength);   // Scan for QR code
    switch(res) {
      case 0 :   // OK
        // code
        #ifdef DEBUG_ABORT
          printf ("QR code = %s\n",qr_data);  // log to console
        #endif
        break:
      case 1 :   // camera error
        // code
        #ifdef DEBUG_ABORT
          printf ("Camera Error\n");  // log to console
        #endif
        break:
      case 2 :   // no QR code detected
        // code
        #ifdef DEBUG_ABORT
          printf ("No QR code\n");  // log to console
        #endif
        break:
      case 3 :   // other error
        // code
        #ifdef DEBUG_ABORT
          printf ("Erro while scanning QR code\n");  // log to console
        #endif
        break:
      default :
      statement(s);
    }



    printf ("Ready.\n");
}
