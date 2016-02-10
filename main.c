#include "lin-delay.h"
#include "lin-gpio.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include "robotapp.h"
#include "ansi.h"

//#define DEBUG_ABORT

RS485ClientStruct RS485Client;
LegoMotorStruct LegoMotor;
LegoSensorStruct LegoSensor;
RfCC1101Struct RfCC1101;
PwrLiIon1AStruct PwrLiIion1A;
ImuStruct Imu;

pthread_t AbortThread;
static uint8 AbortRequest=0;
pthread_t MasterThread;

void* AbortHandler(void *arg)
{
    uint8 val;
    int res;
    while (1)
    {
    _delay_ms(100);
    val=GPIOGetIn(24);
    if (!val || AbortRequest)
    {
        pthread_t CurThread= pthread_self();

        #ifdef DEBUG_ABORT
        printf ("SIGINT handler\n");
        printf ("Master thread:%d   Current thread:%d\n",(int) MasterThread,(int) CurThread);
        #endif

        printf("\nABORT. Shutting down motors.\n");
        printf ("1\n");
        sem_wait(&RS485Client.Busy); //Block RS485 system
        printf ("2\n");
        if (MasterThread!=CurThread) pthread_cancel(MasterThread); //Now kill the master thread in case the abort button has been pressed
        printf ("3\n");
        sem_post(&RS485Client.Busy); //And release control to the termination calls below
        printf ("4\n");
        res=LegoMotorSetup(&LegoMotor,1,0,0);
        if (res>0) printf ("Abort handler: LegoMotorSetup() CH1 fail.\n");
        res=LegoMotorSetup(&LegoMotor,2,0,0);
        if (res>0) printf ("Abort handler: LegoMotorSetup() CH2 fail.\n");
        res=LegoMotorSetup(&LegoMotor,3,0,0);
        if (res>0) printf ("Abort handler: LegoMotorSetup() CH3 fail.\n");
        res=LegoMotorDirectControl(&LegoMotor,1,0);
        if (res>0) printf ("Abort handler: LegoMotorDirectControl() CH1 fail.\n");
        res=LegoMotorDirectControl(&LegoMotor,2,0);
        if (res>0) printf ("Abort handler: LegoMotorDirectControl() CH2 fail.\n");
        res=LegoMotorDirectControl(&LegoMotor,3,0);
        if (res>0) printf ("Abort handler: LegoMotorDirectControl() CH3 fail.\n");

        printf ("ABORT complete\n");
        system ("espeak -ven+f2 -k5 -a100 -s140 \"Safety abort.\" --stdout | aplay");

        fflush(stdout);
        exit (-1);
    }
    //printf ("V:%d\n",val);

    }
}

/* Signal Handler for SIGINT */
void sigintHandler(int sig_num)
{
    AbortRequest=1;
    signal(SIGINT, sigintHandler);
    /* Reset handler to catch SIGINT next time.
       Refer http://en.cppreference.com/w/c/program/signal */
}


void LogCsvSint16(char *FileName,sint16 *Data,uint16 Num);

int main(int argc, char *argv[])
{
    int res;

    AnsiCls();
    AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_GREEN);
    printf ("Initializing RS485 modules...\n");

    //Initialize RS485Client
    AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_WHITE);
    printf ("RS485 bus...");
    strcpy(RS485Client.RS485Port,SER_PORT);
    RS485Client.BaudRate=512000;
    RS485Client.Timeout=20E6;
    res=RS485ClientInit(&RS485Client);
    if (res!=0)
    {
        AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_RED);
        printf ("FAIL: RS485ClientInit() error %d\n",res);
    }
    else
    {
        AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_GREEN);
        printf ("PASS\n");
    }



    //Lego motor init
    AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_WHITE);
    printf ("Lego motor init @ RS485 addr %d...",RS485_LEGO_MOTOR_ADDR);
    LegoMotor.RS485Client=&RS485Client;
    LegoMotor.Address=RS485_LEGO_MOTOR_ADDR;
    res=LegoMotorInit(&LegoMotor);
    if (res!=0)
    {
        AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_RED);
        printf ("FAIL: LegoMotorInit() error code %d\n",res);
    }
    else
    {
        AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_GREEN);
        printf ("PASS\n");
    }

    //Lego sensor init
    AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_WHITE);
    printf ("Lego sensor init @ RS485 addr %d...",RS485_LEGO_SENSOR_ADDR);
    LegoSensor.RS485Client=&RS485Client;
    LegoSensor.Address=RS485_LEGO_SENSOR_ADDR;
    res=LegoSensorInit(&LegoSensor);
    if (res!=0)
    {
        AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_RED);
        printf ("FAIL: LegoSensorInit() error code %d\n",res);
    }
    else
    {
        AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_GREEN);
        printf ("PASS\n");
    }

    AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_WHITE);
    printf ("Lego light sensor on CH1...");
    res=LegoSensorSetup(&LegoSensor,1,CFG_LSENSOR);
    if (res!=0)
    {
        AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_RED);
        printf ("FAIL: LegoSensorSetup for CH1 error code %d\n",res);
    }
    else
    {
        AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_GREEN);
        printf ("PASS\n");
    }

    AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_WHITE);
    printf ("Lego light sensor on CH2...");
    res=LegoSensorSetup(&LegoSensor,2,CFG_LSENSOR);
    if (res!=0)
    {
        AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_RED);
        printf ("FAIL: LegoSensorSetup for CH2 error code %d\n",res);
    }
    else
    {
        AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_GREEN);
        printf ("PASS\n");
    }

    AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_WHITE);
    printf ("Lego contact sensor on CH3...");
    res=LegoSensorSetup(&LegoSensor,3,CFG_SWITCH);
    if (res!=0)
    {
        AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_RED);
        printf ("FAIL: LegoSensorSetup for CH3 error code %d\n",res);
    }
    else
    {
        AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_GREEN);
        printf ("PASS\n");
    }

    //RF-CC1101 module init
    AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_WHITE);
    printf ("RF module init @ RS485 addr:%d...",RS485_RF_ADDR);
    RfCC1101.RS485Client=&RS485Client;
    RfCC1101.Address=RS485_RF_ADDR;
    RfCC1101.Channel=1;
    RfCC1101.Frequency=433920000;
    RfCC1101.ModType=CC1101_MOD_FSK;
    RfCC1101.Rate=CC1101_RATE_1200;
    res=RfCC1101Init(&RfCC1101);
    if (res!=0)
    {
        AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_RED);
        printf ("FAIL: RfCC1101Init error code %d\n",res);
    }
    else
    {
        AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_GREEN);
        printf ("PASS\n");
    }

    //PWR-LIION1A init
    AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_WHITE);
    printf ("Battery module init @ RS485 addr:%d...",RS485_BAT_ADDR);
    PwrLiIion1A.RS485Client=&RS485Client;
    PwrLiIion1A.Address=RS485_BAT_ADDR;
    res=PwrLiIon1AInit(&PwrLiIion1A);
    if (res!=0)
    {
        AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_RED);
        printf ("PwrLiIion1A error code %d\n",res);
    }
    else
    {
        AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_GREEN);
        printf ("PASS\n");
    }

    //IMU init
    AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_WHITE);
    printf ("IMU module init @ RS485 addr:%d...",RS485_IMU_ADDR);
    Imu.RS485Client=&RS485Client;
    Imu.Address=RS485_IMU_ADDR;
    res=ImuInit(&Imu);
    if (res!=0)
    {
        AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_RED);
        printf ("IMU error code %d\n",res);
    }
    else
    {
        AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_GREEN);
        printf ("PASS\n");
    }

    //CTRL-C handler
    signal(SIGINT, sigintHandler);

    //Abort button handler
    GPIOSetDir(24,0);
    MasterThread=pthread_self();
    res = pthread_create(&AbortThread, NULL, &AbortHandler, NULL);
    if (res != 0)
    {
        printf("Can't create abort button thread. Error:%s\n", strerror(res));
        return 1;
    }

    //Now block main thread from processing SIGINT
    sigset_t set;
    //int s;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    //pthread_sigmask(SIG_BLOCK, &set, NULL);

    AnsiSetColor(ANSI_ATTR_OFF,ANSI_BLACK,ANSI_WHITE);
    printf ("Init complete\n");

    //Main application code
    RobotApp(argc,argv);

    //Stop motors
    LegoMotorSetup(&LegoMotor,1,0,0);
    LegoMotorSetup(&LegoMotor,2,0,0);
    LegoMotorSetup(&LegoMotor,3,0,0);
    LegoMotorDirectControl(&LegoMotor,1,0);
    LegoMotorDirectControl(&LegoMotor,2,0);
    LegoMotorDirectControl(&LegoMotor,3,0);

    //Terminate RS485Client
    res=RS485ClientDeinit(&RS485Client);
    if (res!=0)
    {
        printf ("RS485ClientDeinit error code %d\n",res);
        return 1;
    }

    return 0;
}
