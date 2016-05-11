/*
#include "drive.h"

//writen by Dries Blontrock

sint16 kp;
sint16 kd;
sint16 ki;
sint16 imax;
//mm nog berekenen, omtrek (2*r*pi) delen door 360°
float mmPerDeg;
int correction;
int minimum;
int maximum;

int DriveInit()
{
    //printf("DriveInit\n");
    kp = 1800;
    kd = 4200;
    ki = 0;
    imax = 0;
    //mm nog berekenen, omtrek (2*r*pi) delen door 360°
    mmPerDeg = (53*M_PI)/360;

    //dan staat deze zeker stil
    LegoMotorSetup(&LegoMotor,1,1,1);
    LegoMotorSetup(&LegoMotor,2,1,1);

    //set podPos to 0
    LegoMotorPosPIDControl(&LegoMotor, 1, 0);
    LegoMotorPosPIDControl(&LegoMotor, 2, 0);

    //zet hoekcallibratie op 0
    LegoMotorSetPos(&LegoMotor, 1, 0);
    LegoMotorSetPos(&LegoMotor, 2, 0);

    LegoSensorSetupLSensor(&LegoSensor, 1, 1);
    LegoSensorSetupLSensor(&LegoSensor, 2, 1);

    correction = callibration();

    return 0;
}

//distance is in mm, speed is in mm/s
void DriveStraightDistance(float Distance,float Speed)
{
    //printf("DriveStraightDistance\n");
    LegoMotorSetPos(&LegoMotor, 1, 0);
    LegoMotorSetPos(&LegoMotor, 2, 0);
    //te bepalen, mm per graden (via omtrek van wiel) meet diameter. Distance wordt doorgegeven in cm
    //times 2 for half degrees.
    float TotalMoveAngle = (Distance/mmPerDeg)*2;
    //speed wordt doorgegeven in mm/s
    float rotateSpeed = ((fabs(Speed)/100)/mmPerDeg)*2;
    float currentAngle = 0;

    TimeStepInit(10000);
    if(Distance<0)
    {
        while(currentAngle>TotalMoveAngle)
        {
            currentAngle -= rotateSpeed;
            LegoMotorPIDControl(&LegoMotor, 1, currentAngle, kp, kd, ki, imax);
            LegoMotorPIDControl(&LegoMotor, 2, currentAngle, kp, kd, ki, imax);
            TimeStep(0);
        }
    }
    else
    {
        while(currentAngle<TotalMoveAngle)
        {
            currentAngle += rotateSpeed;
            LegoMotorPIDControl(&LegoMotor, 1, currentAngle, kp, kd, ki, imax);
            LegoMotorPIDControl(&LegoMotor, 2, currentAngle, kp, kd, ki, imax);
            TimeStep(0);
        }
    }

    LegoMotorSetup(&LegoMotor,1,1,1);
    LegoMotorSetup(&LegoMotor,2,1,1);

}

void DriveRotateRWheel(float Angle,float Speed)
{
    //printf("DriveRotateRWheel\n");
    LegoMotorSetup(&LegoMotor,1,0,1);
    LegoMotorSetup(&LegoMotor,2,1,1);
    LegoMotorSetPos(&LegoMotor, 1, 0);
    LegoMotorSetPos(&LegoMotor, 2, 0);
    LegoMotorPosPIDControl(&LegoMotor, 1, 0);
    LegoMotorPosPIDControl(&LegoMotor, 2, 0);

    //times 2 for half degrees.
    float TotalMoveAngle = (Angle*2)/mmPerDeg;
    //speed wordt doorgegeven in mm/s
    float rotateSpeed = ((fabs(Speed)/100)/mmPerDeg);
    float currentAngle = 0;

    TimeStepInit(10000);
    if(Angle<0)
    {
        while(currentAngle>TotalMoveAngle)
        {
            currentAngle -= rotateSpeed;
            LegoMotorPIDControl(&LegoMotor, 1, currentAngle*2, kp, kd, ki, imax);
            TimeStep(0);
        }
    }
    else
    {
        while(currentAngle<TotalMoveAngle)
        {
            currentAngle += rotateSpeed;
            LegoMotorPIDControl(&LegoMotor, 1, currentAngle*2, kp, kd, ki, imax);
            TimeStep(0);
        }
    }

    LegoMotorSetup(&LegoMotor,1,1,1);
    LegoMotorSetup(&LegoMotor,2,1,1);
}

void DriveRotateLWheel(float Angle,float Speed)
{
    //printf("DriveRotateLWheel\n");
    LegoMotorSetup(&LegoMotor,1,1,1);
    LegoMotorSetup(&LegoMotor,2,0,1);
    LegoMotorSetPos(&LegoMotor, 1, 0);
    LegoMotorSetPos(&LegoMotor, 2, 0);
    LegoMotorPosPIDControl(&LegoMotor, 1, 0);
    LegoMotorPosPIDControl(&LegoMotor, 2, 0);

    //times 2 for half degrees.
    float TotalMoveAngle = (Angle*2)/mmPerDeg;
    //speed wordt doorgegeven in mm/s
    float rotateSpeed = ((fabs(Speed)/100)/mmPerDeg);
    float currentAngle = 0;

    TimeStepInit(10000);
    if(Angle<0)
    {
        while(currentAngle>TotalMoveAngle)
        {
            currentAngle -= rotateSpeed;
            LegoMotorPIDControl(&LegoMotor, 2, currentAngle*2, kp, kd, ki, imax);
            TimeStep(0);
        }
    }
    else
    {
        while(currentAngle<TotalMoveAngle)
        {
            currentAngle += rotateSpeed;
            LegoMotorPIDControl(&LegoMotor, 2, currentAngle*2, kp, kd, ki, imax);
            TimeStep(0);
        }
    }

    LegoMotorSetup(&LegoMotor,1,1,1);
    LegoMotorSetup(&LegoMotor,2,1,1);
}

void DriveRotateCenter(float Angle,float Speed)
{
    //printf("DriveRotateCenter\n");
    LegoMotorSetup(&LegoMotor,1,1,1);
    LegoMotorSetup(&LegoMotor,2,1,1);

    LegoMotorSetPos(&LegoMotor, 1, 0);
    LegoMotorSetPos(&LegoMotor, 2, 0);

    LegoMotorPosPIDControl(&LegoMotor, 1, 0);
    LegoMotorPosPIDControl(&LegoMotor, 2, 0);

    //times 2 for half degrees.
    float TotalMoveAngle = (Angle*2)/mmPerDeg;
    //speed wordt doorgegeven in mm/s
    float rotateSpeed = ((fabs(Speed)/100)/mmPerDeg);
    float currentAngle = 0;

    TimeStepInit(10000);
    if(Angle<0)
    {
        while(currentAngle>TotalMoveAngle)
        {
            currentAngle -= rotateSpeed;
            LegoMotorPIDControl(&LegoMotor, 1, -currentAngle, kp, kd, ki, imax);
            LegoMotorPIDControl(&LegoMotor, 2, currentAngle, kp, kd, ki, imax);
            TimeStep(0);
        }
    }
    else
    {
        while(currentAngle<TotalMoveAngle)
        {
            currentAngle += rotateSpeed;
            LegoMotorPIDControl(&LegoMotor, 1, -currentAngle, kp, kd, ki, imax);
            LegoMotorPIDControl(&LegoMotor, 2, currentAngle, kp, kd, ki, imax);
            TimeStep(0);
        }
    }

    LegoMotorSetup(&LegoMotor,1,1,1);
    LegoMotorSetup(&LegoMotor,2,1,1);
}

int callibration()
{
    uint16 right = 0;
    uint16 left = 0;
    int i;

    float diff[100] = {0};
    int avg = 0;

    for(i=0; i<100; i++)
    {
        LegoSensorGetLSensorData(&LegoSensor, 1, &right);
        LegoSensorGetLSensorData(&LegoSensor, 2, &left);
        diff[i] = right-left;
        avg += right+left;
    }
    float sum = 0;
    for(i=0; i<100; i++)
    {
        sum += diff[i];
    }

    int totCall = sum/100.0;
    avg = avg/200;
    printf("average: %d\n", avg);
    minimum = avg;
    maximum = avg;

    return totCall;
}

void DriveLineFollowDistance(int Distance, float Speed)
{
    //printf("DriveLineFollowDistance\n");
    LegoMotorSetup(&LegoMotor,1,0,1);
    LegoMotorSetup(&LegoMotor,2,0,1);

    LegoMotorSetPos(&LegoMotor, 1, 0);
    LegoMotorSetPos(&LegoMotor, 2, 0);

    LegoMotorPosPIDControl(&LegoMotor, 1, 0);
    LegoMotorPosPIDControl(&LegoMotor, 2, 0);

    //channel 1
    uint16 RSensor = 0;
    //channel 2
    uint16 LSensor = 0;
    LegoSensorSetupLSensor(&LegoSensor, 1, 1);
    LegoSensorSetupLSensor(&LegoSensor, 2, 1);

    //int correction = callibration();
    //printf("correction: %f", correction);

    float TotalMoveAngle = (Distance/mmPerDeg)*2;
    //speed wordt doorgegeven in mm/s
    float rotateSpeed = ((fabs(Speed)/100)/mmPerDeg)*2;
    float currentAngleR = 0;
    float currentAngleL = 0;
    float currentAngle = 0;
    float correctionL = 0;
    float correctionR = 0;
    float correctieTerm = 1100;

    TimeStepInit(10000);
    if(Distance<0)
    {
        while(currentAngle<TotalMoveAngle)
        {
            LegoSensorGetLSensorData(&LegoSensor, 1, &RSensor);
            LegoSensorGetLSensorData(&LegoSensor, 2, &LSensor);
            LSensor +=correction;


            if(RSensor != LSensor)
            {
                correctionL = (RSensor - LSensor)/correctieTerm;
                correctionR = -correctionL;
                //printf("correction left: %f\n", correctionL);
            }
            else
            {
                correctionL = 0;
                correctionR = 0;
            }

            currentAngleR -= (rotateSpeed + correctionR);
            currentAngleL -= (rotateSpeed + correctionL);
            currentAngle -= rotateSpeed;
            LegoMotorPIDControl(&LegoMotor, 1, currentAngleR, kp, kd, ki, imax);
            LegoMotorPIDControl(&LegoMotor, 2, currentAngleL, kp, kd, ki, imax);
            TimeStep(0);
        }
    }
    else
    {
        while(currentAngle<TotalMoveAngle)
        {
            LegoSensorGetLSensorData(&LegoSensor, 1, &RSensor);
            LegoSensorGetLSensorData(&LegoSensor, 2, &LSensor);
            LSensor +=correction;


            if(RSensor != LSensor)
            {
                correctionL = (RSensor - LSensor)/correctieTerm;
                correctionR = -correctionL;
                //printf("correction left: %f\n", correctionL);
            }
            else
            {
                correctionL = 0;
                correctionR = 0;
            }

            currentAngleR += (rotateSpeed + correctionR);
            currentAngleL += (rotateSpeed + correctionL);
            currentAngle += rotateSpeed;
            LegoMotorPIDControl(&LegoMotor, 1, currentAngleR, kp, kd, ki, imax);
            LegoMotorPIDControl(&LegoMotor, 2, currentAngleL, kp, kd, ki, imax);
            TimeStep(0);
        }
    }

    LegoMotorSetup(&LegoMotor,1,1,1);
    LegoMotorSetup(&LegoMotor,2,1,1);
}

void DriveLineFollow(float Speed)
{
    //printf("DriveLineFollow\n");
    LegoMotorSetup(&LegoMotor,1,0,1);
    LegoMotorSetup(&LegoMotor,2,0,1);

    LegoMotorSetPos(&LegoMotor, 1, 0);
    LegoMotorSetPos(&LegoMotor, 2, 0);

    LegoMotorPosPIDControl(&LegoMotor, 1, 0);
    LegoMotorPosPIDControl(&LegoMotor, 2, 0);

    //channel 1
    uint16 RSensor = 0;
    //channel 2
    uint16 LSensor = 0;
    LegoSensorSetupLSensor(&LegoSensor, 1, 1);
    LegoSensorSetupLSensor(&LegoSensor, 2, 1);

    //int correction = callibration();
    //printf("correction: %f", correction);

    //speed wordt doorgegeven in mm/s
    float rotateSpeed = ((fabs(Speed)/100)/mmPerDeg)*2;
    float currentAngleR = 0;
    float currentAngleL = 0;
    float correctionL = 0;
    float correctionR = 0;
    float correctieTerm = 1100;
    int stop = 0;
    int threshold = 0;
    int stopvalue = 0;

    TimeStepInit(10000);
    while(stop == 0)
    {
        LegoSensorGetLSensorData(&LegoSensor, 1, &RSensor);
        LegoSensorGetLSensorData(&LegoSensor, 2, &LSensor);
        LSensor +=correction;
        //printf("left: %d\n", LSensor);
        //printf("right: %d\n", RSensor);

        if(RSensor != LSensor)
        {
            correctionL = (RSensor - LSensor)/correctieTerm;
            correctionR = -correctionL;
        }
        else
        {
            correctionL = 0;
            correctionR = 0;
        }
        currentAngleR += (rotateSpeed + correctionR);
        currentAngleL += (rotateSpeed + correctionL);
        LegoMotorPIDControl(&LegoMotor, 1, currentAngleR, kp, kd, ki, imax);
        LegoMotorPIDControl(&LegoMotor, 2, currentAngleL, kp, kd, ki, imax);

        if(RSensor> maximum || RSensor<minimum || LSensor>maximum || LSensor<minimum)
        {
            threshold = adaptiveThreshold(RSensor, LSensor);
            stopvalue = threshold-threshold/15;
            printf("stopvalue: %d\n", stopvalue);
        }
        //if(LSensor<14000 && RSensor<14000)
        if(LSensor<stopvalue && RSensor<stopvalue)
        {
            stop = 1;
            printf("stop!\n");
        }
        TimeStep(0);
    }

    LegoMotorSetup(&LegoMotor,1,1,1);
    LegoMotorSetup(&LegoMotor,2,1,1);
}

int adaptiveThreshold(int RSensor, int LSensor)
{
    if(RSensor > maximum)
    {
        maximum = RSensor;
    }
    else if(RSensor < minimum)
    {
        minimum = RSensor;
    }

    if(LSensor > maximum)
    {
        maximum = LSensor;
    }
    else if(LSensor < minimum)
    {
        minimum = LSensor;
    }

    //printf("minimum: %d\n", minimum);
    //printf("maximum: %d\n", maximum);

    int threshold = (minimum + maximum)/2;
    return threshold;
}
*/
