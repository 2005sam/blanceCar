#include "settings.h"
#include "bsp_config.h"
#include "bsp_motor.h"

char motor_status[MOTOR_COUNT] = {0};
char motor_direction[MOTOR_COUNT] = {0};
/**
 * @brief  Start the specified motor
 * @param  motor: Motor identifier (MOTOR_LEFT or MOTOR_RIGHT)
 * @retval Return code indicating success or error
 */
uint32_t BspMotorStart(MotorId motor)
{
  if (motor == BSP_MOTOR_LEFT || motor == BSP_MOTOR_RIGHT)
  {
    if (motor_status[motor] == 0)
    {
      HAL_GPIO_WritePin(BSP_MOTOR_STBY_GPIO_PORT, BSP_MOTOR_STBY_PIN, GPIO_PIN_SET);
      motor_status[motor] = 1;
      return 0;
    }
    motor_status[BSP_MOTOR_LEFT] = 1;
    motor_status[BSP_MOTOR_RIGHT] = 1;
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
  if (motor == BSP_MOTOR_LEFT || motor == BSP_MOTOR_RIGHT)
  {
    if (motor_status[motor] == 1)
    {
      HAL_GPIO_WritePin(BSP_MOTOR_STBY_GPIO_PORT, BSP_MOTOR_STBY_PIN, GPIO_PIN_RESET);
      motor_status[motor] = 0;
      return 0;
    }
    motor_status[BSP_MOTOR_LEFT] = 0;
    motor_status[BSP_MOTOR_RIGHT] = 0;
    return 0;
  }
  return MAKE_RETURN_CODE(RETURN_ERROR, ERROR_LEVEL_MINOR, MODULE_HAL, ERR_TYPE_BUSY, 0);
}

static uint32_t SetLeftMotorDirection(MotorDirection direction);
static uint32_t SetRightMotorDirection(MotorDirection direction);

/**
 * @brief  Set the direction of the specified motor
 * @param  motor: Motor identifier (MOTOR_LEFT or MOTOR_RIGHT)
 * @param  direction: Direction of the motor (BSP_MOTOR_DIRECTION_FORWARD, BSP_MOTOR_DIRECTION_BACKWARD, BSP_MOTOR_DIRECTION_BRAKE)
 * @retval Return code indicating success or error
 */
uint32_t BspMotorDection(MotorId motor, MotorDirection direction)
{
  uint32_t result;
  switch (motor)
  {
  case BSP_MOTOR_LEFT:
    result = SetLeftMotorDirection(direction);
    break;
  case BSP_MOTOR_RIGHT:
    result = SetRightMotorDirection(direction);
    break;
  default:
    return MAKE_RETURN_CODE(RETURN_ERROR, ERROR_LEVEL_MINOR,
                            MOUDLE_MOTOR, ERR_TYPE_INVALID_PARAM, 0);
  }

  if (result != 0)
  {
    return result;
  }

  motor_direction[motor] = direction;
  return 0;
}

/**
 * @brief  Get the current direction of the specified motor
 * @param  motor: Motor identifier (MOTOR_LEFT or MOTOR_RIGHT)
 * @param  direction: Pointer to store the current direction of the motor
 * @retval Return code indicating success or error
 */
static void SetMotorPins(GPIO_TypeDef *in1_port, uint16_t in1_pin,
                         GPIO_TypeDef *in2_port, uint16_t in2_pin,
                         GPIO_PinState in1_state, GPIO_PinState in2_state)
{
  HAL_GPIO_WritePin(in1_port, in1_pin, in1_state);
  HAL_GPIO_WritePin(in2_port, in2_pin, in2_state);
}

/**
 * @brief  Set the direction of the left motor
 * @param  direction: Direction of the motor (BSP_MOTOR_DIRECTION_FORWARD, BSP_MOTOR_DIRECTION_BACKWARD, BSP_MOTOR_DIRECTION_BRAKE)
 * @retval Return code indicating success or error
 */
static uint32_t SetLeftMotorDirection(MotorDirection direction)
{
  switch (direction)
  {
  case BSP_MOTOR_DIRECTION_FORWARD:
    SetMotorPins(BSP_MOTOR_LEFT_IN1_GPIO_PORT, BSP_MOTOR_LEFT_IN1_PIN,
                 BSP_MOTOR_LEFT_IN2_GPIO_PORT, BSP_MOTOR_LEFT_IN2_PIN,
                 GPIO_PIN_SET, GPIO_PIN_RESET);
    return 0;
  case BSP_MOTOR_DIRECTION_BACKWARD:
    SetMotorPins(BSP_MOTOR_LEFT_IN1_GPIO_PORT, BSP_MOTOR_LEFT_IN1_PIN,
                 BSP_MOTOR_LEFT_IN2_GPIO_PORT, BSP_MOTOR_LEFT_IN2_PIN,
                 GPIO_PIN_RESET, GPIO_PIN_SET);
    return 0;
  case BSP_MOTOR_DIRECTION_BRAKE:
    SetMotorPins(BSP_MOTOR_LEFT_IN1_GPIO_PORT, BSP_MOTOR_LEFT_IN1_PIN,
                 BSP_MOTOR_LEFT_IN2_GPIO_PORT, BSP_MOTOR_LEFT_IN2_PIN,
                 GPIO_PIN_RESET, GPIO_PIN_RESET);
    return 0;
  default:
    return MAKE_RETURN_CODE(RETURN_ERROR, ERROR_LEVEL_MINOR,
                            MOUDLE_MOTOR, ERR_TYPE_INVALID_PARAM, 0);
  }
}

/**
 * @brief  Set the direction of the right motor
 * @param  direction: Direction of the motor (BSP_MOTOR_DIRECTION_FORWARD, BSP_MOTOR_DIRECTION_BACKWARD, BSP_MOTOR_DIRECTION_BRAKE)
 * @retval Return code indicating success or error
 */
static uint32_t SetRightMotorDirection(MotorDirection direction)
{
  switch (direction)
  {
  case BSP_MOTOR_DIRECTION_FORWARD:
    SetMotorPins(BSP_MOTOR_RIGHT_IN1_GPIO_PORT, BSP_MOTOR_RIGHT_IN1_PIN,
                 BSP_MOTOR_RIGHT_IN2_GPIO_PORT, BSP_MOTOR_RIGHT_IN2_PIN,
                 GPIO_PIN_SET, GPIO_PIN_RESET);
    return 0;
  case BSP_MOTOR_DIRECTION_BACKWARD:
    SetMotorPins(BSP_MOTOR_RIGHT_IN1_GPIO_PORT, BSP_MOTOR_RIGHT_IN1_PIN,
                 BSP_MOTOR_RIGHT_IN2_GPIO_PORT, BSP_MOTOR_RIGHT_IN2_PIN,
                 GPIO_PIN_RESET, GPIO_PIN_SET);
    return 0;
  case BSP_MOTOR_DIRECTION_BRAKE:
    SetMotorPins(BSP_MOTOR_RIGHT_IN1_GPIO_PORT, BSP_MOTOR_RIGHT_IN1_PIN,
                 BSP_MOTOR_RIGHT_IN2_GPIO_PORT, BSP_MOTOR_RIGHT_IN2_PIN,
                 GPIO_PIN_RESET, GPIO_PIN_RESET);
    return 0;
  default:
    return MAKE_RETURN_CODE(RETURN_ERROR, ERROR_LEVEL_MINOR,
                            MOUDLE_MOTOR, ERR_TYPE_INVALID_PARAM, 0);
  }
}