#include "bsp_button.h"

USER_BUTTON_STATE UserButtonState = OFF;

uint32_t BspUserButtonHandeler(void)
{
  UserButtonState = !UserButtonState;
  return 0;
}
uint32_t BspUserButtonGetState(USER_BUTTON_STATE *state)
{
  *state = UserButtonState;
  return 0;
}