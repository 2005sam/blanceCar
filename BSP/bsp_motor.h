#ifndef BSP_MOTOR_H_
#define BSP_MOTOR_H_

#define MOTOR_COUNT 2
#define MOTOR_DIRECTION_COUNT 3
typedef enum
{
  BSP_MOTOR_LEFT = 0,
  BSP_MOTOR_RIGHT = 1
} MotorId;

typedef enum
{
  BSP_MOTOR_DIRECTION_BRAKE = 0,
  BSP_MOTOR_DIRECTION_FORWARD = 1,
  BSP_MOTOR_DIRECTION_BACKWARD = 2
} MotorDirection;

uint32_t BspMotorStart(MotorId motor);
uint32_t BspMotorStop(MotorId motor);
uint32_t BspMotorDection(MotorId motor, MotorDirection direction);

#endif