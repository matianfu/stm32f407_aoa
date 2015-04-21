/*
 * play.c
 *
 *  Created on: Apr 15, 2015
 *      Author: ma
 */
#include <stdio.h>
#include "funk.h"
#include "usbh_core.h"
#include "usbh_core_helper.h"



static Continuation* delay(Continuation* co,
    uint32_t duration)
{
  VAR_BEGIN
  uint32_t start;
  uint32_t duration;
  VAR_END

  this->duration = duration;
  this->start = HAL_GetTick();

  while(HAL_GetTick() - this->start < this->duration)
  {
    YIELD();
  }
  EXIT();
}

static Continuation* periodical(Continuation* co)
{
  VAR_BEGIN
  Continuation* sub;
  VAR_END

  while(1)
  {
    while((this->sub = delay(this->sub, 1000)))
      YIELD();

    printf("another second passed." NEW_LINE);
  }
}

static Continuation* peri = 0;

extern void test_closure();

void task()
{
  peri = periodical(peri);
  test_closure();
};
