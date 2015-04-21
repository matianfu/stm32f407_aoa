#include <iostream>
#include <functional>
#include <cstdarg>
#include "stm32f4xx_hal.h"

using namespace std;

#define CONCAT01(a, b)                      a##b
#define CONCAT02(a, b)                      CONCAT01(a, b)
#define YIELD()                             \
  do {                                      \
    CONCAT02(ENTRY, __LINE__):              \
    if (EP == && CONCAT02(ENTRY, __LINE__)) \
      EP = 0;                               \
    else                                    \
    {                                       \
      EP = && CONCAT02(ENTRY, __LINE__);    \
      return true;                          \
    }                                       \
  } while (0)

typedef std::function<bool(int)> coroutine;

coroutine add(int a, int b, int * ret)
{
  uint32_t start = HAL_GetTick();
  return [=](int sig) mutable -> bool
  {
    uint32_t now;

    if (sig) return false;
    while(1)
    {
      now = HAL_GetTick();
      if (now - start > 2000) break;
      return true;
    }
    * ret = a + b;
    return false;
  };
}

coroutine sum(int a, int b, int c, int * ret)
{
  void* EP = 0;
  int tmp = 0;
  coroutine co;

  if (a < 0 || b < 0 || c < 0) return NULL;
  return [=] (int sig) mutable -> bool
  {
    if (sig) return co(sig);
    if (EP) goto *EP;

    co = add(a, b, &tmp);
    while(co(0))
    YIELD();

    co = add(tmp, c, ret);
    while(co(0))
    YIELD();

    return false;
  };
}

int inited = 0;
int finished = 0;
int ret = 0;

coroutine co = NULL;

// timer
uint32_t start = 0;

extern "C"
{

void test_closure()
{
  if (!finished)
  {
    if (!inited)
    {
      if (co == NULL)
      {
        co = sum(1, 2, 3, &ret);
        start = HAL_GetTick();
        inited = 1;
      }
    }

    if (HAL_GetTick() - start > 3000)
    {
      co(1);
      finished = 1;
    }
    else if (co(0)) return;

    finished = 1;
  }
}

}

