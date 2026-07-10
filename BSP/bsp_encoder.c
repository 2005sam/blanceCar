#include "bsp_encoder.h"
#include "bsp_config.h"
#include "error_codes.h"
#include "stm32f1xx_hal.h"

volatile int32_t EncoderAngleLift = 0;
volatile int32_t EncoderAngleRight = 0;

void BspEncoderHandleLift(void)
{
  GPIO_PinState phase_a = HAL_GPIO_ReadPin(BSP_ENCODER_LEFT_A_GPIO_PORT,
                                           BSP_ENCODER_LEFT_A_PIN);
  GPIO_PinState phase_b = HAL_GPIO_ReadPin(BSP_ENCODER_LEFT_B_GPIO_PORT,
                                           BSP_ENCODER_LEFT_B_PIN);

  if (phase_a == GPIO_PIN_SET)
  {
    if (phase_b == GPIO_PIN_RESET)
    {
      EncoderAngleLift--;
    }
    else
    {
      EncoderAngleLift++;
    }
  }
  else
  {
    if (phase_b == GPIO_PIN_RESET)
    {
      EncoderAngleLift++;
    }
    else
    {
      EncoderAngleLift--;
    }
  }
}

void BspEncoderHandleRight(void)
{
  GPIO_PinState phase_a = HAL_GPIO_ReadPin(BSP_ENCODER_RIGHT_A_GPIO_PORT,
                                           BSP_ENCODER_RIGHT_A_PIN);
  GPIO_PinState phase_b = HAL_GPIO_ReadPin(BSP_ENCODER_RIGHT_B_GPIO_PORT,
                                           BSP_ENCODER_RIGHT_B_PIN);

  if (phase_a == GPIO_PIN_SET)
  {
    if (phase_b == GPIO_PIN_RESET)
    {
      EncoderAngleRight++;
    }
    else
    {
      EncoderAngleRight--;
    }
  }
  else
  {
    if (phase_b == GPIO_PIN_RESET)
    {
      EncoderAngleRight--;
    }
    else
    {
      EncoderAngleRight++;
    }
  }
}

uint32_t BspGetEncoderAngleLift(float *angle)
{
  if (angle == NULL)
  {
    return RET_BSP_INVALID_PARAM;
  }
  *angle = (float)EncoderAngleLift / (30613.0f / 1500.0f * 22.0f) * 360.0f;
  return RET_OK;
}

uint32_t BspGetEncoderAngleRight(float *angle)
{
  if (angle == NULL)
  {
    return RET_BSP_INVALID_PARAM;
  }
  *angle = (float)EncoderAngleRight / (30613.0f / 1500.0f * 22.0f) * 360.0f;
  return RET_OK;
}