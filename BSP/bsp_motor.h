#ifndef BSP_MOTOR_H_
#define BSP_MOTOR_H_

#define MOTOR_COUNT 2

typedef enum
{
  MOTOR_LEFT = 0,
  MOTOR_RIGHT = 1
} MotorId;

uint32_t BspMotorStart(MotorId motor);
uint32_t BspMotorStop(MotorId motor);

#endif