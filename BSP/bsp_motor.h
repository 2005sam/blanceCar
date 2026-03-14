#ifndef BSP_MOTOR_H_
#define BSP_MOTOR_H_

#define MOTOR_COUNT 2
#define MOTOR_DIRECTION_COUNT 3

typedef enum // motor identifier
{
  BSP_MOTOR_LEFT = 0, // left motor
  BSP_MOTOR_RIGHT = 1 // right motor
} MotorId;

typedef enum // motor direction
{
  BSP_MOTOR_DIRECTION_BRAKE = 0,   // motor brake
  BSP_MOTOR_DIRECTION_FORWARD = 1, // motor forward
  BSP_MOTOR_DIRECTION_BACKWARD = 2 // motor backward
} MotorDirection;

uint32_t BspMotorStart(MotorId motor);
uint32_t BspMotorStop(MotorId motor);
uint32_t BspMotorDection(MotorId motor, MotorDirection direction);
uint32_t BspMotorPWM(MotorId motor, float dutyCycle);
MotorId BspMotorGetId(MotorId motor);
MotorDirection BspMotorGetDirection(MotorId motor);

#endif