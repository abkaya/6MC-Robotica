//========================================
//
//  Robotics Lab Project
//  Written by Abdil Kaya & Robin Janssens
//
//========================================

#include "robotapp.h"

#define DEBUG_ABORT
//#define testQR
//#define testTag
//#define testDijkstra
//#define testCC1101send
//#define testCC1101receive
#define testDrive
//#define program



void RobotApp(int argc, char *argv[])
{
    //int readTagNode;                            // hold return status values
    LegoMotorSetup(&LegoMotor,1,0,0);   // motor, channel, brake, mode
    LegoMotorSetup(&LegoMotor,2,0,0);
    system("./mkRamdisk.sh");           // checks on whether or not the /mnt/ramdisk directory exist and a ramdisk has been made -- script reports back on screen
#ifdef DEBUG_ABORT
    printf ("Initialisation complete\n");
    //system ("espeak -ven+f2 -k5 -a50 -s150 \"Initialasition complete\" --stdout | aplay");
#endif

    //==============================
    // Test Procedures
    //==============================
#ifdef testQR     // Try to scan a QR-code every 5 seconds

    //system ("espeak -ven+f2 -k5 -a50 -s150 \"QR\" --stdout | aplay");
    res = QRCodeDecode(qr_data, maxContentLength);   // scan for QR code
    printf("QR status: %i   data: %s\n",res,qr_data);                      // print status

#endif
#ifdef testTag     // Try to scan a QR-code every 5 seconds
    for(i=0; i<3; i++)
    {
        //system ("espeak -ven+f2 -k5 -a50 -s150 \"Testing Tag scanning\" --stdout | aplay");
        res = TagReaderGetUID( tag_data );                         // scan tag
        printf("tag node: %i   tag data: %s\n",res,tag_data);       // print statusl

        _delay_ms(5000);
    }                                           // wait 5 seconds
#endif
#ifdef testDijkstra     // Test dijkstra algorithm
    Start=2;
    Finish=10;
    int PathLength;
    int CurrentNode=Start;
    //Create an array of nodes describing the map
    //NodeStruct* Nodes = malloc(MapSize * sizeof(NodeStruct));
    NodeStruct Nodes[MapSize];
    PathLength=Dijkstra(Nodes,MapSize,Start,Finish);
    printf("\nShortest Path: \n");
    printf("-> %d ", Start);

    for(i=0; i<PathLength; i++)
    {
        printf("NRD: %d ",Nodes[CurrentNode].NextRelDir);
        printf("\n");

        CurrentNode=Nodes[CurrentNode].Next;
    }

    int Current=Start;
    while(Nodes[Current].Next!=-1)
    {
        printf("-> %d ", Nodes[Current].Next);
        Current=Nodes[Current].Next;
    }
#endif
#ifdef testCC1101send     // Send data package every 5 seconds
    while (1)
    {
        //system ("espeak -ven+f2 -k5 -a50 -s150 \"Testing sending wireless communication\" --stdout | aplay");

        RfCommsPacket package;      // create package
        package.DstRfAddr = 2;      // destination RF address
        package.SrcRfAddr = 7;      // source RF address
        package.DataLen = 4;        // number of data bytes in packet (Data array)
        package.Data[0] = 1;        // set data (1337)
        package.Data[1] = 3;        // set data (1337)
        package.Data[2] = 3;        // set data (1337)
        package.Data[3] = 7;        // set data (1337)

        res = RfCommsSendPacket( &package );   // Send data
        printf("send status: %i\n",res);                      // print status
        _delay_ms(5000);
    }
#endif
#ifdef testCC1101receive     // Receive data package every 2 seconds
    RfCC1101.RFAddr = 2;            // receive as robot 2
    res = RfCommsInit();        // check for available data
    printf("init status: %i\n",res);               // print status
    while (1)
    {
        //system ("espeak -ven+f2 -k5 -a50 -s150 \"Testing receiving wireless communication\" --stdout | aplay");
        RfCommsPacket package;                          // create package
        uint8 PollStatus;
        res = RfCommsReceivePoll( &PollStatus );        // check for available data
        printf("receive status: %i\n",res);               // print status

        if ( PollStatus == 1 )                          // packet ready
        {
            res = RfCommsReceivePacket( &package );     // receive data
            printf("Packet received! (Rssi: %i, Lqi: %i) data: ",package.Rssi,package.Lqi);
            for(i=0; i<package.DataLen; i++)
            {
                printf("%c",package.Data[i]);
            }
            printf("\n");
        }
        else
        {
            _delay_ms(2000);                            // wait 2 seconds
        }
    }
#endif
#ifdef testDrive
    printf("%d \n",AssessStubNode(4));
    DriveInit();
    //Finish=10;
    speed=80;
    turnSpeed=60;
    MapSize=12;

    int ladingFound = 0;
    int endpointsSize = 5;
    int endpoints[] = {0,2,6,8,10};
    int pickerStart = -1;

    char* qr_data_objective = "lading1";

    while ( ladingFound == 0 ) {
        Finish = endpoints[endpointsSize-1];
        DriveToDest(Finish);
        if ( pickerStart == -1 )     // if first time drive
            pickerStart = Start;     // save picker location
        res = QRCodeDecode(qr_data, maxContentLength);   // scan for QR code
        printf("\nQR status: %i   data: %s\n",res,qr_data);                      // print status
        DriveRotateCenter(-180, (turnSpeed));
        ladingFound = (qr_data == qr_data_objective);
        if ( !ladingFound )
            if ( endpointsSize > 0 )
                endpointsSize--;
    }

    // ------------------------------
    // Send instructions to picker
    // ->   |        1         |        0        |
    // ->   | 1B = finish node | 1B = start node |
    // ------------------------------
    RfCommsPacket package;					    // create package
    package.DstRfAddr = pickerRfAddress;		// destination RF address
    package.SrcRfAddr = seekerRfAddress;		// source RF address
    package.DataLen = 2;						// number of data bytes in packet (Data array)
    package.Data[0] = pickerStart;  			// start node
    package.Data[1] = Finish;					// finish node
    res = RfCommsSendPacket( &package );	// send data
    printf("send status: %d\n",res);			// print status

    // ------------------------------
    // Park
    // ------------------------------
    int parking = 0;
    while ( endpoints[parking] == Finish || parking == endpoints[pickerStart] )
        parking++;
    DriveToDest( endpoints[parking] );

    // ------------------------------
    // Celebrate
    // ------------------------------
    system ("espeak -ven+f2 -k5 -a50 -s150 \"Good job big boys\" --stdout | aplay");
    system("mpg123 rickroll.mp3");



#endif
#ifdef program

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


    //==============================
    // Read QR code
    //==============================
    //char* qr_data[maxContentLength];
    res = QRCodeDecode(qr_data, maxContentLength);   // Scan for QR code
    switch(res)
    {
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
        printf ("Error while scanning QR code\n");  // log to console
#endif
        break;
    default :
        break;
        //code
    }

    printf ("Ready.\n");
#endif
}

void TurnRobot(int RelativeDirection, int speed)
{
//Relative direction to next node (-1:no direction, 1=turn left, 2=forward, 3=turn right)
    switch(RelativeDirection)

    {
    case 1:
        DriveRotateLWheel(90, speed);
        DriveStraightDistance(20,speed);
        DriveLineFollow(speed);
        _delay_ms(10);
        break;
    case 2:
        DriveStraightDistance(40,speed);
        DriveLineFollow(speed);
        _delay_ms(10);
        break;
    case 3:
        //turn right
        DriveRotateRWheel(-90, speed);
        DriveStraightDistance(20,speed);
        DriveLineFollow(speed);
        _delay_ms(10);
        break;
    default :
        break;
        //code
    }
}

int AssessStubNode(int FirstScannedNode)
{
    switch(FirstScannedNode)
    {
    case 4:
        return 0;
        break;
    case 3:
        return 2;
        break;
    case 7:
        return 6;
        break;
    case 11:
        return 10;
        break;
    case 9:
        return 8;
        break;
    default:
        return -1;
        break;
    }
    return -1;
}

void DriveToDest(int Destination)
{
    DriveLineFollow(speed);

    currentNode=TagReaderGetUID(tag_data);
    Start=AssessStubNode(currentNode);
    Dijkstra(Nodes,MapSize,Start,Destination);

    printf("\nTHE START NODE: %d\n",Start);
    printf("THE SECOND NODE: %d\n",currentNode);
    NextRelDir=Nodes[currentNode].NextRelDir;
    TurnRobot(NextRelDir,speed); //turn robot depending on the relative direction returned by dijkstra

    while(Nodes[currentNode].Next!=Destination)
    {
        currentNode=Nodes[currentNode].Next;
        printf("Node: %d , NOW TURNING TO: %d \n",currentNode,Nodes[currentNode].NextRelDir);
        NextRelDir=Nodes[currentNode].NextRelDir;
        TurnRobot(NextRelDir,speed);
    }
}
