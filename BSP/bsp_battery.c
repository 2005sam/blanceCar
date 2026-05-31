#include "bsp_battery.h"
#include <stdint.h>

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;

uint32_t BspBatteryInit(void)
{
  HAL_TIM_Base_Start(&htim2);
  HAL_ADCEx_InjectedStart(&hadc1);
  return 0;
}
uint32_t BspBatteryGetVoltage(float *voltage)
{
  uint32_t adc_value;
  adc_value = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_1);
  *voltage = (float)adc_value * 8.4 / 4095;
  return adc_value;
}