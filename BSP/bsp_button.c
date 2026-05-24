#include "bsp_button.h"

static UserButtonState user_button_state = BSP_BUTTON_OFF;

uint32_t BspUserButtonHandeler(void)
{
  user_button_state = !user_button_state;
  return 0;
}
uint32_t BspUserButtonGetState(UserButtonState *state)
{
  *state = user_button_state;
  return 0;
}
