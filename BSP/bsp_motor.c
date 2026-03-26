#include "settings.h"
#include "bsp_config.h"
#include "bsp_motor.h"

extern TIM_HandleTypeDef htim1;          // left motor PWM timer
extern TIM_HandleTypeDef htim4;          // right motor PWM timer
char motor_status[MOTOR_COUNT] = {0};    // status of motor (0: stop, 1: run)
char motor_direction[MOTOR_COUNT] = {0}; // direction of motor (0: brake, 1: forward, 2: backward)
float motor_pwm[MOTOR_COUNT] = {0};      // PWM duty cycle of motor (0.0 to 1.0)

static uint32_t SetLeftMotorDirection(MotorDirection direction);
static uint32_t SetRightMotorDirection(MotorDirection direction);
static uint32_t SetMotorPWM(TIM_HandleTypeDef *htim, uint32_t channel, float dutyCycle);
static uint32_t SetLeftMotorPWM(float dutyCycle);
static uint32_t SetRightMotorPWM(float dutyCycle);

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
      HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
      HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
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
      HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
      HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
    }
    motor_status[BSP_MOTOR_LEFT] = 0;
    motor_status[BSP_MOTOR_RIGHT] = 0;
    return 0;
  }
  return MAKE_RETURN_CODE(RETURN_ERROR, ERROR_LEVEL_MINOR, MODULE_HAL, ERR_TYPE_BUSY, 0);
}

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
 * @brief  Set the PWM duty cycle of the specified motor
 * @param  motor: Motor identifier (MOTOR_LEFT or MOTOR_RIGHT)
 * @param  dutyCycle: Duty cycle (0.0 to 1.0)
 * @retval Return code indicating success or error
 */
uint32_t BspMotorPWM(MotorId motor, float dutyCycle)
{
  switch (motor)
  {
  case BSP_MOTOR_LEFT:
    return SetLeftMotorPWM(dutyCycle);
    break;
  case BSP_MOTOR_RIGHT:

    return SetRightMotorPWM(dutyCycle);
    break;
  default:
    return MAKE_RETURN_CODE(RETURN_ERROR, ERROR_LEVEL_MINOR,
                            MOUDLE_MOTOR, ERR_TYPE_INVALID_PARAM, 0);
  }
  motor_pwm[motor] = dutyCycle;
  return 0;
}

/**
 * @brief  Get the current status of the specified motor
 * @param  motor: Motor identifier (MOTOR_LEFT or MOTOR_RIGHT)
 * @retval Return motor Id
 */
MotorId BspMotorGetId(MotorId motor) { return motor_status[motor]; }

/**
 * @brief  Get the current direction of the specified motor
 * @param  motor: Motor identifier (MOTOR_LEFT or MOTOR_RIGHT)
 * @retval Return motor direction
 */
MotorDirection BspMotorGetDirection(MotorId motor) { return motor_direction[motor]; }

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

/**
 * @brief  Set the PWM duty cycle of the specified motor
 * @param  htim: Pointer to the TIM handle
 * @param  channel: Timer channel
 * @param  dutyCycle: Duty cycle (0.0 to 1.0)
 * @retval Return code indicating success or error
 */
static uint32_t SetMotorPWM(TIM_HandleTypeDef *htim, uint32_t channel, float dutyCycle)
{
  uint32_t auto_reload_value = __HAL_TIM_GET_AUTORELOAD(htim);
  uint32_t pulse = (uint32_t)(dutyCycle * auto_reload_value);
  if (pulse > auto_reload_value)
  {
    return MAKE_RETURN_CODE(RETURN_ERROR, ERROR_LEVEL_MINOR,
                            MOUDLE_MOTOR, ERR_TYPE_INVALID_PARAM, 0);
  }
  __HAL_TIM_SET_COMPARE(htim, channel, pulse);
  return 0;
}

static uint32_t SetLeftMotorPWM(float dutyCycle) { return SetMotorPWM(&htim1, BSP_MOTOR_LEFT_PWM_CHANNEL, dutyCycle); }   // set left motor pwm
static uint32_t SetRightMotorPWM(float dutyCycle) { return SetMotorPWM(&htim4, BSP_MOTOR_RIGHT_PWM_CHANNEL, dutyCycle); } // set right motor pwm