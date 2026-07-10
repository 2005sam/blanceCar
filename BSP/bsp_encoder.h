#ifndef BSP_ENCODER_H_
#define BSP_ENCODER_H_

#include <stdint.h>

extern volatile int32_t EncoderAngleLift;
extern volatile int32_t EncoderAngleRight;

void BspEncoderHandleLift(void);
void BspEncoderHandleRight(void);
uint32_t BspGetEncoderAngleLift(float *angle);
uint32_t BspGetEncoderAngleRight(float *angle);

#endif