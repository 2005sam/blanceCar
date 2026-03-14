#include "settings.h"
#include "bsp_config.h"
#include "bsp_motor.h"

char motor_status[MOTOR_COUNT] = {0};
/**
 * @brief  Start the specified motor
 * @param  motor: Motor identifier (MOTOR_LEFT or MOTOR_RIGHT)
 * @retval Return code indicating success or error
 */
uint32_t BspMotorStart(MotorId motor)
{
  if (motor == MOTOR_LEFT || motor == MOTOR_RIGHT)
  {
    if (motor_status[motor] == 0)
    {
      HAL_GPIO_WritePin(BSP_MOTOR_STBY_GPIO_PORT, BSP_MOTOR_STBY_PIN, GPIO_PIN_SET);
      motor_status[motor] = 1;
      return 0;
    }
    motor_status[MOTOR_LEFT] = 1;
    motor_status[MOTOR_RIGHT] = 1;
    return 0;
  }
  return MAKE_RETURN_CODE(RETURN_ERROR, ERROR_LEVEL_MINOR, MODULE_HAL, ERR_TYPE_BUSY, 0);
}

/**
 * @brief  Stop the specified motor
 * @param  motor: Motor identifier (MOTOR_LEFT or MOTOR_RIGHT)
 * @retval Return code indicating success or error
 */
uint32_t BspMotorStop(MotorId motor)
{
  if (motor == MOTOR_LEFT || motor == MOTOR_RIGHT)
  {
    if (motor_status[motor] == 1)
    {
      HAL_GPIO_WritePin(BSP_MOTOR_STBY_GPIO_PORT, BSP_MOTOR_STBY_PIN, GPIO_PIN_RESET);
      motor_status[motor] = 0;
      return 0;
    }
    motor_status[MOTOR_LEFT] = 0;
    motor_status[MOTOR_RIGHT] = 0;
    return 0;
  }
  return MAKE_RETURN_CODE(RETURN_ERROR, ERROR_LEVEL_MINOR, MODULE_HAL, ERR_TYPE_BUSY, 0);
}
