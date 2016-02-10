#ifndef ROBOT_APP
#define ROBOT_APP

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


#define RS485_LEGO_MOTOR_ADDR 1
#define RS485_LEGO_SENSOR_ADDR 2
#define RS485_RF_ADDR 4
#define RS485_BAT_ADDR 3
#define RS485_IMU_ADDR 5

#define MOTOR_RIGHT 1
#define MOTOR_LEFT 2
#define MOTOR_LIFT 3

#define SENSOR_LIFT 3


extern RS485ClientStruct RS485Client;
extern LegoMotorStruct LegoMotor;
extern LegoSensorStruct LegoSensor;
extern RfCC1101Struct RfCC1101;
extern PwrLiIon1AStruct PwrLiIion1A;
extern ImuStruct Imu;

void RobotApp(int argc, char *argv[]);


#endif
