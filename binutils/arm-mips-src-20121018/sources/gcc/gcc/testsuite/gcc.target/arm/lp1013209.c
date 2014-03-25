/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-add-options arm_neon } */
#include "arm_neon.h"

void foo (void)
{
  int16_t buffer[2048];
  int f;
  for (f = 0; f < 128; f += 8)
    vst1q_u16 (&buffer[f], (uint16x8_t){0});

}
