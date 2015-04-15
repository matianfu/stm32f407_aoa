/*
 * funk.h
 *
 *  Created on: Apr 15, 2015
 *      Author: ma
 */

#ifndef FUNK_H_
#define FUNK_H_

#include <string.h>

#define CONCAT01(a, b)        a##b
#define CONCAT02(a, b)        CONCAT01(a, b)

#define EXIT()                          \
  do {                                  \
    if (this) free(this);               \
    return 0;                           \
  } while(0)

#define YIELD()                         \
  CONCAT02(ENTRY, __LINE__):            \
  do {                                  \
    if (this->co.ep ==                  \
        && CONCAT02(ENTRY, __LINE__))   \
    {                                   \
      this->co.ep = 0;                  \
    }                                   \
    else                                \
    {                                   \
      this->co.ep =                     \
        && CONCAT02(ENTRY, __LINE__);   \
      return (Continuation*)this;       \
    }                                   \
  } while (0)

#define CALL_FUNK(co, name,...)         \
    (co = name(co, __VA_ARGS__))        \

#define VAR_BEGIN                       \
  struct _func_data {                   \
    Continuation co;

#define VAR_END                         \
  }* this = (struct _func_data *) co;   \
  if (this) goto *this->co.ep;          \
  if ((this = malloc(sizeof(*this))))   \
    memset(this, 0, sizeof(*this));

typedef struct Continuation {
  void* ep;                   /** entry point **/

} Continuation;

#endif /* FUNK_H_ */
