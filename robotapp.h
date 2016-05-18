//========================================
//
//  Robotics Lab Project
//  Written by Abdil Kaya & Robin Janssens
//
//========================================
#ifndef ROBOTAPP_H
#define ROBOTAPP_H

#include "project.h"
#include "rs485client.h"
#include "lego-motor.h"
#include "lin-lego-motor-log.h"
#include "lego-sensor.h"
#include "imu.h"
#include "rf-cc1101.h"
#include "pwr-liion1a.h"
#include "lin-delay.h"
#include "TimeSupport.h"
#include <stdlib.h>
#include <stdio.h>

#define RS485_LEGO_MOTOR_ADDR 1
#define RS485_LEGO_SENSOR_ADDR 2
#define RS485_RF_ADDR 4
#define RS485_BAT_ADDR 3
#define RS485_IMU_ADDR 5

// ==============================
// Settings
// ==============================
#define seekerRfAddress 7		// address of seeker ( this robot )
#define pickerRfAddress 9		// address of picker

#define MOTOR_RIGHT 1
#define MOTOR_LEFT 2
#define MOTOR_LIFT 3

#define SENSOR_LIFT 3

#include "dijkstra.h"
#define maxContentLength 32   // max amount of chars in content
#include "qrcode.h"
#include "tagreader.h"
#include "rfcomms.h"
#include "drive.h"
#include <stdbool.h>


extern RS485ClientStruct RS485Client;
extern LegoMotorStruct LegoMotor;
extern LegoSensorStruct LegoSensor;
extern RfCC1101Struct RfCC1101;
extern PwrLiIon1AStruct PwrLiIion1A;
extern ImuStruct Imu;

void RobotApp(int argc, char *argv[]);

/**
* \brief Rotates the robot depending on the relative direction
* \param RelativeDirection: returned by the .RelativeDirection method in dijkstra
*/
void TurnRobot(int RelativeDirection, int speed);

/**
* \brief Used to return the first node depending on the first scanned Tag
* \param FirstScannedNode is the first scanned node, used to find the stub node.
* \return Returns the first node (the stub node it left from)
*/
int AssessStubNode(int FirstScannedNode);
#endif

/**
* \brief Moves the robot to the destination. The start position is assumed : Facing FORWARD towards path.
* \param Finish: Destination node or stub
*/
void DriveToDest(int Destination);


//MapSize=12;
char qr_data[maxContentLength];    // create QR data holder
int res;
int speed;
int turnSpeed;
char tag_data[maxContentLength];   // create tag data holder
int NextRelDir;
int i;                              // holds counter value


int PathLength;
int currentNode;
int Start;
//Create an array of nodes describing the map
//NodeStruct* Nodes = malloc(MapSize * sizeof(NodeStruct));
NodeStruct Nodes[12]; //12 is map size

int PathLength;
int currentNode;
int Start;
//Create an array of nodes describing the map
//NodeStruct* Nodes = malloc(MapSize * sizeof(NodeStruct));
