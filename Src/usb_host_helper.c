#include <stdio.h>
#include "usbh_conf.h"
#include "usbh_core.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "version.h"

/*
 * buffers for PRINT REG macros
 */
static char print_reg_buf[64];

/*
 * basic definitions
 */
#define PRT_REGBITS(prefix, regname, bitname)           \
sprintf(print_reg_buf, " " #bitname " %lX", ((reg & prefix##_##regname##_##bitname) >> bitmask_shift(prefix##_##regname##_##bitname))); \
width = strlen(print_reg_buf) < 14 ? 1 : 2;\
if (count + width < 12) { count += width; } else { printf(NEW_LINE "%-20s:", ""); count = width; }\
if (width == 1) { printf("%-15s", print_reg_buf); } else { printf("%-30s", print_reg_buf); }


#define PRTREG_BEGIN(prefix, regname)                   \
void PRT_##prefix##_##regname (uint32_t reg)            \
{                                                       \
int count = 0, width = 0;                               \
sprintf(print_reg_buf, "  " #prefix "_" #regname);      \
printf("%-20s:", print_reg_buf);


#define PRTREG_END                                      \
printf(NEW_LINE);                                       \
}

#define DEFINE_PRTREG3(prefix, regname, bm1)            \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRTREG_END

#define DEFINE_PRTREG4(prefix, regname, bm1, bm2)       \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRTREG_END

#define DEFINE_PRTREG5(prefix, regname, bm1, bm2, bm3)  \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRTREG_END

#define DEFINE_PRTREG6(prefix, regname, bm1, bm2, bm3,  \
    bm4)                                                \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRTREG_END

#define DEFINE_PRTREG7(prefix, regname, bm1, bm2, bm3,  \
    bm4, bm5)                                           \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRTREG_END

#define DEFINE_PRTREG8(prefix, regname, bm1, bm2, bm3,  \
    bm4, bm5, bm6)                                      \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRTREG_END


#define DEFINE_PRTREG9(prefix, regname, bm1, bm2, bm3,  \
    bm4, bm5, bm6, bm7)                                 \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRTREG_END

#define DEFINE_PRTREG10(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8)                            \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRTREG_END

#define DEFINE_PRTREG11(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9)                       \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRTREG_END

#define DEFINE_PRTREG12(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10)                 \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRTREG_END

#define DEFINE_PRTREG13(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11)           \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRTREG_END

#define DEFINE_PRTREG14(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12)     \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRTREG_END

#define DEFINE_PRTREG15(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13)                                               \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRTREG_END

#define DEFINE_PRTREG16(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13, bm14)                                         \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRT_REGBITS(prefix, regname, bm14)                      \
PRTREG_END

#define DEFINE_PRTREG17(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13, bm14, bm15)                                   \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRT_REGBITS(prefix, regname, bm14)                      \
PRT_REGBITS(prefix, regname, bm15)                      \
PRTREG_END

#define DEFINE_PRTREG18(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13, bm14, bm15, bm16)                             \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRT_REGBITS(prefix, regname, bm14)                      \
PRT_REGBITS(prefix, regname, bm15)                      \
PRT_REGBITS(prefix, regname, bm16)                      \
PRTREG_END

#define DEFINE_PRTREG19(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13, bm14, bm15, bm16, bm17)                       \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRT_REGBITS(prefix, regname, bm14)                      \
PRT_REGBITS(prefix, regname, bm15)                      \
PRT_REGBITS(prefix, regname, bm16)                      \
PRT_REGBITS(prefix, regname, bm17)                      \
PRTREG_END

#define DEFINE_PRTREG20(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13, bm14, bm15, bm16, bm17, bm18)                 \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRT_REGBITS(prefix, regname, bm14)                      \
PRT_REGBITS(prefix, regname, bm15)                      \
PRT_REGBITS(prefix, regname, bm16)                      \
PRT_REGBITS(prefix, regname, bm17)                      \
PRT_REGBITS(prefix, regname, bm18)                      \
PRTREG_END

#define DEFINE_PRTREG21(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13, bm14, bm15, bm16, bm17, bm18, bm19)           \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRT_REGBITS(prefix, regname, bm14)                      \
PRT_REGBITS(prefix, regname, bm15)                      \
PRT_REGBITS(prefix, regname, bm16)                      \
PRT_REGBITS(prefix, regname, bm17)                      \
PRT_REGBITS(prefix, regname, bm18)                      \
PRT_REGBITS(prefix, regname, bm19)                      \
PRTREG_END

#define DEFINE_PRTREG22(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13, bm14, bm15, bm16, bm17, bm18, bm19, bm20)     \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRT_REGBITS(prefix, regname, bm14)                      \
PRT_REGBITS(prefix, regname, bm15)                      \
PRT_REGBITS(prefix, regname, bm16)                      \
PRT_REGBITS(prefix, regname, bm17)                      \
PRT_REGBITS(prefix, regname, bm18)                      \
PRT_REGBITS(prefix, regname, bm19)                      \
PRT_REGBITS(prefix, regname, bm20)                      \
PRTREG_END

#define DEFINE_PRTREG23(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13, bm14, bm15, bm16, bm17, bm18, bm19, bm20,     \
    bm21)                                               \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRT_REGBITS(prefix, regname, bm14)                      \
PRT_REGBITS(prefix, regname, bm15)                      \
PRT_REGBITS(prefix, regname, bm16)                      \
PRT_REGBITS(prefix, regname, bm17)                      \
PRT_REGBITS(prefix, regname, bm18)                      \
PRT_REGBITS(prefix, regname, bm19)                      \
PRT_REGBITS(prefix, regname, bm20)                      \
PRT_REGBITS(prefix, regname, bm21)                      \
PRTREG_END


#define DEFINE_PRTREG24(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13, bm14, bm15, bm16, bm17, bm18, bm19, bm20,     \
    bm21, bm22)                                         \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRT_REGBITS(prefix, regname, bm14)                      \
PRT_REGBITS(prefix, regname, bm15)                      \
PRT_REGBITS(prefix, regname, bm16)                      \
PRT_REGBITS(prefix, regname, bm17)                      \
PRT_REGBITS(prefix, regname, bm18)                      \
PRT_REGBITS(prefix, regname, bm19)                      \
PRT_REGBITS(prefix, regname, bm20)                      \
PRT_REGBITS(prefix, regname, bm21)                      \
PRT_REGBITS(prefix, regname, bm22)                      \
PRTREG_END


#define DEFINE_PRTREG25(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13, bm14, bm15, bm16, bm17, bm18, bm19, bm20,     \
    bm21, bm22, bm23)                                   \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRT_REGBITS(prefix, regname, bm14)                      \
PRT_REGBITS(prefix, regname, bm15)                      \
PRT_REGBITS(prefix, regname, bm16)                      \
PRT_REGBITS(prefix, regname, bm17)                      \
PRT_REGBITS(prefix, regname, bm18)                      \
PRT_REGBITS(prefix, regname, bm19)                      \
PRT_REGBITS(prefix, regname, bm20)                      \
PRT_REGBITS(prefix, regname, bm21)                      \
PRT_REGBITS(prefix, regname, bm22)                      \
PRT_REGBITS(prefix, regname, bm23)                      \
PRTREG_END


#define DEFINE_PRTREG26(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13, bm14, bm15, bm16, bm17, bm18, bm19, bm20,     \
    bm21, bm22, bm23, bm24)                             \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRT_REGBITS(prefix, regname, bm14)                      \
PRT_REGBITS(prefix, regname, bm15)                      \
PRT_REGBITS(prefix, regname, bm16)                      \
PRT_REGBITS(prefix, regname, bm17)                      \
PRT_REGBITS(prefix, regname, bm18)                      \
PRT_REGBITS(prefix, regname, bm19)                      \
PRT_REGBITS(prefix, regname, bm20)                      \
PRT_REGBITS(prefix, regname, bm21)                      \
PRT_REGBITS(prefix, regname, bm22)                      \
PRT_REGBITS(prefix, regname, bm23)                      \
PRT_REGBITS(prefix, regname, bm24)                      \
PRTREG_END


#define DEFINE_PRTREG27(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13, bm14, bm15, bm16, bm17, bm18, bm19, bm20,     \
    bm21, bm22, bm23, bm24, bm25)                       \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRT_REGBITS(prefix, regname, bm14)                      \
PRT_REGBITS(prefix, regname, bm15)                      \
PRT_REGBITS(prefix, regname, bm16)                      \
PRT_REGBITS(prefix, regname, bm17)                      \
PRT_REGBITS(prefix, regname, bm18)                      \
PRT_REGBITS(prefix, regname, bm19)                      \
PRT_REGBITS(prefix, regname, bm20)                      \
PRT_REGBITS(prefix, regname, bm21)                      \
PRT_REGBITS(prefix, regname, bm22)                      \
PRT_REGBITS(prefix, regname, bm23)                      \
PRT_REGBITS(prefix, regname, bm24)                      \
PRT_REGBITS(prefix, regname, bm25)                      \
PRTREG_END


#define DEFINE_PRTREG28(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13, bm14, bm15, bm16, bm17, bm18, bm19, bm20,     \
    bm21, bm22, bm23, bm24, bm25, bm26)                 \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRT_REGBITS(prefix, regname, bm14)                      \
PRT_REGBITS(prefix, regname, bm15)                      \
PRT_REGBITS(prefix, regname, bm16)                      \
PRT_REGBITS(prefix, regname, bm17)                      \
PRT_REGBITS(prefix, regname, bm18)                      \
PRT_REGBITS(prefix, regname, bm19)                      \
PRT_REGBITS(prefix, regname, bm20)                      \
PRT_REGBITS(prefix, regname, bm21)                      \
PRT_REGBITS(prefix, regname, bm22)                      \
PRT_REGBITS(prefix, regname, bm23)                      \
PRT_REGBITS(prefix, regname, bm24)                      \
PRT_REGBITS(prefix, regname, bm25)                      \
PRT_REGBITS(prefix, regname, bm26)                      \
PRTREG_END


#define DEFINE_PRTREG29(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13, bm14, bm15, bm16, bm17, bm18, bm19, bm20,     \
    bm21, bm22, bm23, bm24, bm25, bm26, bm27)           \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRT_REGBITS(prefix, regname, bm14)                      \
PRT_REGBITS(prefix, regname, bm15)                      \
PRT_REGBITS(prefix, regname, bm16)                      \
PRT_REGBITS(prefix, regname, bm17)                      \
PRT_REGBITS(prefix, regname, bm18)                      \
PRT_REGBITS(prefix, regname, bm19)                      \
PRT_REGBITS(prefix, regname, bm20)                      \
PRT_REGBITS(prefix, regname, bm21)                      \
PRT_REGBITS(prefix, regname, bm22)                      \
PRT_REGBITS(prefix, regname, bm23)                      \
PRT_REGBITS(prefix, regname, bm24)                      \
PRT_REGBITS(prefix, regname, bm25)                      \
PRT_REGBITS(prefix, regname, bm26)                      \
PRT_REGBITS(prefix, regname, bm27)                      \
PRTREG_END


#define DEFINE_PRTREG30(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13, bm14, bm15, bm16, bm17, bm18, bm19, bm20,     \
    bm21, bm22, bm23, bm24, bm25, bm26, bm27, bm28)     \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRT_REGBITS(prefix, regname, bm14)                      \
PRT_REGBITS(prefix, regname, bm15)                      \
PRT_REGBITS(prefix, regname, bm16)                      \
PRT_REGBITS(prefix, regname, bm17)                      \
PRT_REGBITS(prefix, regname, bm18)                      \
PRT_REGBITS(prefix, regname, bm19)                      \
PRT_REGBITS(prefix, regname, bm20)                      \
PRT_REGBITS(prefix, regname, bm21)                      \
PRT_REGBITS(prefix, regname, bm22)                      \
PRT_REGBITS(prefix, regname, bm23)                      \
PRT_REGBITS(prefix, regname, bm24)                      \
PRT_REGBITS(prefix, regname, bm25)                      \
PRT_REGBITS(prefix, regname, bm26)                      \
PRT_REGBITS(prefix, regname, bm27)                      \
PRT_REGBITS(prefix, regname, bm28)                      \
PRTREG_END


#define DEFINE_PRTREG31(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13, bm14, bm15, bm16, bm17, bm18, bm19, bm20,     \
    bm21, bm22, bm23, bm24, bm25, bm26, bm27, bm28,     \
    bm29)                                               \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRT_REGBITS(prefix, regname, bm14)                      \
PRT_REGBITS(prefix, regname, bm15)                      \
PRT_REGBITS(prefix, regname, bm16)                      \
PRT_REGBITS(prefix, regname, bm17)                      \
PRT_REGBITS(prefix, regname, bm18)                      \
PRT_REGBITS(prefix, regname, bm19)                      \
PRT_REGBITS(prefix, regname, bm20)                      \
PRT_REGBITS(prefix, regname, bm21)                      \
PRT_REGBITS(prefix, regname, bm22)                      \
PRT_REGBITS(prefix, regname, bm23)                      \
PRT_REGBITS(prefix, regname, bm24)                      \
PRT_REGBITS(prefix, regname, bm25)                      \
PRT_REGBITS(prefix, regname, bm26)                      \
PRT_REGBITS(prefix, regname, bm27)                      \
PRT_REGBITS(prefix, regname, bm28)                      \
PRT_REGBITS(prefix, regname, bm29)                      \
PRTREG_END


#define DEFINE_PRTREG32(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13, bm14, bm15, bm16, bm17, bm18, bm19, bm20,     \
    bm21, bm22, bm23, bm24, bm25, bm26, bm27, bm28,     \
    bm29, bm30)                                         \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRT_REGBITS(prefix, regname, bm14)                      \
PRT_REGBITS(prefix, regname, bm15)                      \
PRT_REGBITS(prefix, regname, bm16)                      \
PRT_REGBITS(prefix, regname, bm17)                      \
PRT_REGBITS(prefix, regname, bm18)                      \
PRT_REGBITS(prefix, regname, bm19)                      \
PRT_REGBITS(prefix, regname, bm20)                      \
PRT_REGBITS(prefix, regname, bm21)                      \
PRT_REGBITS(prefix, regname, bm22)                      \
PRT_REGBITS(prefix, regname, bm23)                      \
PRT_REGBITS(prefix, regname, bm24)                      \
PRT_REGBITS(prefix, regname, bm25)                      \
PRT_REGBITS(prefix, regname, bm26)                      \
PRT_REGBITS(prefix, regname, bm27)                      \
PRT_REGBITS(prefix, regname, bm28)                      \
PRT_REGBITS(prefix, regname, bm29)                      \
PRT_REGBITS(prefix, regname, bm30)                      \
PRTREG_END


#define DEFINE_PRTREG33(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13, bm14, bm15, bm16, bm17, bm18, bm19, bm20,     \
    bm21, bm22, bm23, bm24, bm25, bm26, bm27, bm28,     \
    bm29, bm30, bm31)                                   \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRT_REGBITS(prefix, regname, bm14)                      \
PRT_REGBITS(prefix, regname, bm15)                      \
PRT_REGBITS(prefix, regname, bm16)                      \
PRT_REGBITS(prefix, regname, bm17)                      \
PRT_REGBITS(prefix, regname, bm18)                      \
PRT_REGBITS(prefix, regname, bm19)                      \
PRT_REGBITS(prefix, regname, bm20)                      \
PRT_REGBITS(prefix, regname, bm21)                      \
PRT_REGBITS(prefix, regname, bm22)                      \
PRT_REGBITS(prefix, regname, bm23)                      \
PRT_REGBITS(prefix, regname, bm24)                      \
PRT_REGBITS(prefix, regname, bm25)                      \
PRT_REGBITS(prefix, regname, bm26)                      \
PRT_REGBITS(prefix, regname, bm27)                      \
PRT_REGBITS(prefix, regname, bm28)                      \
PRT_REGBITS(prefix, regname, bm29)                      \
PRT_REGBITS(prefix, regname, bm30)                      \
PRT_REGBITS(prefix, regname, bm31)                      \
PRTREG_END


#define DEFINE_PRTREG34(prefix, regname, bm1, bm2, bm3, \
    bm4, bm5, bm6, bm7, bm8, bm9, bm10, bm11, bm12,     \
    bm13, bm14, bm15, bm16, bm17, bm18, bm19, bm20,     \
    bm21, bm22, bm23, bm24, bm25, bm26, bm27, bm28,     \
    bm29, bm30, bm31, bm32)                             \
PRTREG_BEGIN(prefix, regname)                           \
PRT_REGBITS(prefix, regname, bm1)                       \
PRT_REGBITS(prefix, regname, bm2)                       \
PRT_REGBITS(prefix, regname, bm3)                       \
PRT_REGBITS(prefix, regname, bm4)                       \
PRT_REGBITS(prefix, regname, bm5)                       \
PRT_REGBITS(prefix, regname, bm6)                       \
PRT_REGBITS(prefix, regname, bm7)                       \
PRT_REGBITS(prefix, regname, bm8)                       \
PRT_REGBITS(prefix, regname, bm9)                       \
PRT_REGBITS(prefix, regname, bm10)                      \
PRT_REGBITS(prefix, regname, bm11)                      \
PRT_REGBITS(prefix, regname, bm12)                      \
PRT_REGBITS(prefix, regname, bm13)                      \
PRT_REGBITS(prefix, regname, bm14)                      \
PRT_REGBITS(prefix, regname, bm15)                      \
PRT_REGBITS(prefix, regname, bm16)                      \
PRT_REGBITS(prefix, regname, bm17)                      \
PRT_REGBITS(prefix, regname, bm18)                      \
PRT_REGBITS(prefix, regname, bm19)                      \
PRT_REGBITS(prefix, regname, bm20)                      \
PRT_REGBITS(prefix, regname, bm21)                      \
PRT_REGBITS(prefix, regname, bm22)                      \
PRT_REGBITS(prefix, regname, bm23)                      \
PRT_REGBITS(prefix, regname, bm24)                      \
PRT_REGBITS(prefix, regname, bm25)                      \
PRT_REGBITS(prefix, regname, bm26)                      \
PRT_REGBITS(prefix, regname, bm27)                      \
PRT_REGBITS(prefix, regname, bm28)                      \
PRT_REGBITS(prefix, regname, bm29)                      \
PRT_REGBITS(prefix, regname, bm30)                      \
PRT_REGBITS(prefix, regname, bm31)                      \
PRT_REGBITS(prefix, regname, bm32)                      \
PRTREG_END










/*
 * macro function overloading
 */
#define GET_MACRO(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,   \
    _12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,\
    _25,_26,_27,_28,_29,_30,_31,_32,_33,_34,NAME,...)   \
    NAME

#define DEFINE_PRTREG(...) GET_MACRO(__VA_ARGS__,       \
    DEFINE_PRTREG34, \
    DEFINE_PRTREG33, \
    DEFINE_PRTREG32, \
    DEFINE_PRTREG31, \
    DEFINE_PRTREG30, \
    DEFINE_PRTREG29, \
    DEFINE_PRTREG28, \
    DEFINE_PRTREG27, \
    DEFINE_PRTREG26, \
    DEFINE_PRTREG25, \
    DEFINE_PRTREG24, \
    DEFINE_PRTREG23, \
    DEFINE_PRTREG22, \
    DEFINE_PRTREG21, \
    DEFINE_PRTREG20, \
    DEFINE_PRTREG19, \
    DEFINE_PRTREG18, \
    DEFINE_PRTREG17, \
    DEFINE_PRTREG16, \
    DEFINE_PRTREG15, \
    DEFINE_PRTREG14, \
    DEFINE_PRTREG13, \
    DEFINE_PRTREG12, \
    DEFINE_PRTREG11, \
    DEFINE_PRTREG10, \
    DEFINE_PRTREG9, \
    DEFINE_PRTREG8, \
    DEFINE_PRTREG7, \
    DEFINE_PRTREG6, \
    DEFINE_PRTREG5, \
    DEFINE_PRTREG4, \
    DEFINE_PRTREG3, \
    DEFINE_PRTREG2)(__VA_ARGS__)


extern HCD_HandleTypeDef hhcd_USB_OTG_HS;

/*
 * return 0 for valid bitmask, 1 for invalid
 */
//static int checkbit(uint32_t bits)
//{
//  int i, j = 0;
//  int up = 0, down = 0, prev_bit = 0, post_bit = 0;
//
//  for (i = 0; i < sizeof(uint32_t); i++)
//  {
//    if (prev_bit == 0) {
//      if (bits & (i << i))
//        up++;
//    }
//    else {
//      if (!(bits & (i << i)))
//        down++;
//    }
//  }
//
//  if (prev_bit == 1)
//    down++;
//
//  if (up == 1 && down == 1)
//    return 0;
//
//  return -1;
//}

static int bitmask_shift(uint32_t bitmask)
{
  int i = 0;

  while ((((uint32_t)0x00000001) & bitmask) == 0){
    bitmask = (bitmask >> 1);
    i++;
  }

  return i;
}

#if 0
/********************  Bit definition forUSB_OTG_GOTGCTL register  ********************/
#define USB_OTG_GOTGCTL_SRQSCS                  ((uint32_t)0x00000001)            /*!< Session request success */
#define USB_OTG_GOTGCTL_SRQ                     ((uint32_t)0x00000002)            /*!< Session request */
#define USB_OTG_GOTGCTL_HNGSCS                  ((uint32_t)0x00000100)            /*!< Host negotiation success */
#define USB_OTG_GOTGCTL_HNPRQ                   ((uint32_t)0x00000200)            /*!< HNP request */
#define USB_OTG_GOTGCTL_HSHNPEN                 ((uint32_t)0x00000400)            /*!< Host set HNP enable */
#define USB_OTG_GOTGCTL_DHNPEN                  ((uint32_t)0x00000800)            /*!< Device HNP enabled */
#define USB_OTG_GOTGCTL_CIDSTS                  ((uint32_t)0x00010000)            /*!< Connector ID status */
#define USB_OTG_GOTGCTL_DBCT                    ((uint32_t)0x00020000)            /*!< Long/short debounce time */
#define USB_OTG_GOTGCTL_ASVLD                   ((uint32_t)0x00040000)            /*!< A-session valid */
#define USB_OTG_GOTGCTL_BSVLD                   ((uint32_t)0x00080000)            /*!< B-session valid */
#endif

DEFINE_PRTREG(USB_OTG, GOTGCTL, BSVLD, ASVLD, DBCT, CIDSTS, DHNPEN, HSHNPEN, HNPRQ, HNGSCS, SRQ, SRQSCS);

#if 0
/********************  Bit definition forUSB_OTG_GOTGINT register  ********************/
#define USB_OTG_GOTGINT_SEDET                   ((uint32_t)0x00000004)            /*!< Session end detected */
#define USB_OTG_GOTGINT_SRSSCHG                 ((uint32_t)0x00000100)            /*!< Session request success status change */
#define USB_OTG_GOTGINT_HNSSCHG                 ((uint32_t)0x00000200)            /*!< Host negotiation success status change */
#define USB_OTG_GOTGINT_HNGDET                  ((uint32_t)0x00020000)            /*!< Host negotiation detected */
#define USB_OTG_GOTGINT_ADTOCHG                 ((uint32_t)0x00040000)            /*!< A-device timeout change */
#define USB_OTG_GOTGINT_DBCDNE                  ((uint32_t)0x00080000)            /*!< Debounce done */
#endif

DEFINE_PRTREG(USB_OTG, GOTGINT, DBCDNE, ADTOCHG, HNGDET, HNSSCHG, SRSSCHG, SEDET);

#if 0
/********************  Bit definition forUSB_OTG_GAHBCFG register  ********************/
#define USB_OTG_GAHBCFG_GINT                    ((uint32_t)0x00000001)            /*!< Global interrupt mask */

#define USB_OTG_GAHBCFG_HBSTLEN                 ((uint32_t)0x0000001E)            /*!< Burst length/type */
#define USB_OTG_GAHBCFG_HBSTLEN_0               ((uint32_t)0x00000002)            /*!<Bit 0 */
#define USB_OTG_GAHBCFG_HBSTLEN_1               ((uint32_t)0x00000004)            /*!<Bit 1 */
#define USB_OTG_GAHBCFG_HBSTLEN_2               ((uint32_t)0x00000008)            /*!<Bit 2 */
#define USB_OTG_GAHBCFG_HBSTLEN_3               ((uint32_t)0x00000010)            /*!<Bit 3 */
#define USB_OTG_GAHBCFG_DMAEN                   ((uint32_t)0x00000020)            /*!< DMA enable */
#define USB_OTG_GAHBCFG_TXFELVL                 ((uint32_t)0x00000080)            /*!< TxFIFO empty level */
#define USB_OTG_GAHBCFG_PTXFELVL                ((uint32_t)0x00000100)            /*!< Periodic TxFIFO empty level */
#endif

DEFINE_PRTREG(USB_OTG, GAHBCFG, PTXFELVL, TXFELVL, DMAEN, HBSTLEN, GINT);

#if 0
/********************  Bit definition forUSB_OTG_GUSBCFG register  ********************/

#define USB_OTG_GUSBCFG_TOCAL                   ((uint32_t)0x00000007)            /*!< FS timeout calibration */
#define USB_OTG_GUSBCFG_TOCAL_0                 ((uint32_t)0x00000001)            /*!<Bit 0 */
#define USB_OTG_GUSBCFG_TOCAL_1                 ((uint32_t)0x00000002)            /*!<Bit 1 */
#define USB_OTG_GUSBCFG_TOCAL_2                 ((uint32_t)0x00000004)            /*!<Bit 2 */
#define USB_OTG_GUSBCFG_PHYSEL                  ((uint32_t)0x00000040)            /*!< USB 2.0 high-speed ULPI PHY or USB 1.1 full-speed serial transceiver select */
#define USB_OTG_GUSBCFG_SRPCAP                  ((uint32_t)0x00000100)            /*!< SRP-capable */
#define USB_OTG_GUSBCFG_HNPCAP                  ((uint32_t)0x00000200)            /*!< HNP-capable */

#define USB_OTG_GUSBCFG_TRDT                    ((uint32_t)0x00003C00)            /*!< USB turnaround time */
#define USB_OTG_GUSBCFG_TRDT_0                  ((uint32_t)0x00000400)            /*!<Bit 0 */
#define USB_OTG_GUSBCFG_TRDT_1                  ((uint32_t)0x00000800)            /*!<Bit 1 */
#define USB_OTG_GUSBCFG_TRDT_2                  ((uint32_t)0x00001000)            /*!<Bit 2 */
#define USB_OTG_GUSBCFG_TRDT_3                  ((uint32_t)0x00002000)            /*!<Bit 3 */
#define USB_OTG_GUSBCFG_PHYLPCS                 ((uint32_t)0x00008000)            /*!< PHY Low-power clock select */
#define USB_OTG_GUSBCFG_ULPIFSLS                ((uint32_t)0x00020000)            /*!< ULPI FS/LS select */
#define USB_OTG_GUSBCFG_ULPIAR                  ((uint32_t)0x00040000)            /*!< ULPI Auto-resume */
#define USB_OTG_GUSBCFG_ULPICSM                 ((uint32_t)0x00080000)            /*!< ULPI Clock SuspendM */
#define USB_OTG_GUSBCFG_ULPIEVBUSD              ((uint32_t)0x00100000)            /*!< ULPI External VBUS Drive */
#define USB_OTG_GUSBCFG_ULPIEVBUSI              ((uint32_t)0x00200000)            /*!< ULPI external VBUS indicator */
#define USB_OTG_GUSBCFG_TSDPS                   ((uint32_t)0x00400000)            /*!< TermSel DLine pulsing selection */
#define USB_OTG_GUSBCFG_PCCI                    ((uint32_t)0x00800000)            /*!< Indicator complement */
#define USB_OTG_GUSBCFG_PTCI                    ((uint32_t)0x01000000)            /*!< Indicator pass through */
#define USB_OTG_GUSBCFG_ULPIIPD                 ((uint32_t)0x02000000)            /*!< ULPI interface protect disable */
#define USB_OTG_GUSBCFG_FHMOD                   ((uint32_t)0x20000000)            /*!< Forced host mode */
#define USB_OTG_GUSBCFG_FDMOD                   ((uint32_t)0x40000000)            /*!< Forced peripheral mode */
#define USB_OTG_GUSBCFG_CTXPKT                  ((uint32_t)0x80000000)            /*!< Corrupt Tx packet */
#endif

DEFINE_PRTREG(USB_OTG, GUSBCFG, CTXPKT, FDMOD, FHMOD, ULPIIPD, PTCI, PCCI, TSDPS, ULPIEVBUSI, ULPIEVBUSD,
    ULPICSM, ULPIAR, ULPIFSLS, PHYLPCS, TRDT)

#if 0
/********************  Bit definition forUSB_OTG_GRSTCTL register  ********************/
#define USB_OTG_GRSTCTL_CSRST                   ((uint32_t)0x00000001)            /*!< Core soft reset */
#define USB_OTG_GRSTCTL_HSRST                   ((uint32_t)0x00000002)            /*!< HCLK soft reset */
#define USB_OTG_GRSTCTL_FCRST                   ((uint32_t)0x00000004)            /*!< Host frame counter reset */
#define USB_OTG_GRSTCTL_RXFFLSH                 ((uint32_t)0x00000010)            /*!< RxFIFO flush */
#define USB_OTG_GRSTCTL_TXFFLSH                 ((uint32_t)0x00000020)            /*!< TxFIFO flush */

#define USB_OTG_GRSTCTL_TXFNUM                  ((uint32_t)0x000007C0)            /*!< TxFIFO number */
#define USB_OTG_GRSTCTL_TXFNUM_0                ((uint32_t)0x00000040)            /*!<Bit 0 */
#define USB_OTG_GRSTCTL_TXFNUM_1                ((uint32_t)0x00000080)            /*!<Bit 1 */
#define USB_OTG_GRSTCTL_TXFNUM_2                ((uint32_t)0x00000100)            /*!<Bit 2 */
#define USB_OTG_GRSTCTL_TXFNUM_3                ((uint32_t)0x00000200)            /*!<Bit 3 */
#define USB_OTG_GRSTCTL_TXFNUM_4                ((uint32_t)0x00000400)            /*!<Bit 4 */
#define USB_OTG_GRSTCTL_DMAREQ                  ((uint32_t)0x40000000)            /*!< DMA request signal */
#define USB_OTG_GRSTCTL_AHBIDL                  ((uint32_t)0x80000000)            /*!< AHB master idle */
#endif

DEFINE_PRTREG(USB_OTG, GRSTCTL, AHBIDL, DMAREQ, TXFNUM, TXFFLSH, RXFFLSH, FCRST, HSRST, CSRST);

#if 0
/********************  Bit definition forUSB_OTG_GINTSTS register  ********************/
#define USB_OTG_GINTSTS_CMOD                    ((uint32_t)0x00000001)            /*!< Current mode of operation */
#define USB_OTG_GINTSTS_MMIS                    ((uint32_t)0x00000002)            /*!< Mode mismatch interrupt */
#define USB_OTG_GINTSTS_OTGINT                  ((uint32_t)0x00000004)            /*!< OTG interrupt */
#define USB_OTG_GINTSTS_SOF                     ((uint32_t)0x00000008)            /*!< Start of frame */
#define USB_OTG_GINTSTS_RXFLVL                  ((uint32_t)0x00000010)            /*!< RxFIFO nonempty */
#define USB_OTG_GINTSTS_NPTXFE                  ((uint32_t)0x00000020)            /*!< Nonperiodic TxFIFO empty */
#define USB_OTG_GINTSTS_GINAKEFF                ((uint32_t)0x00000040)            /*!< Global IN nonperiodic NAK effective */
#define USB_OTG_GINTSTS_BOUTNAKEFF              ((uint32_t)0x00000080)            /*!< Global OUT NAK effective */
#define USB_OTG_GINTSTS_ESUSP                   ((uint32_t)0x00000400)            /*!< Early suspend */
#define USB_OTG_GINTSTS_USBSUSP                 ((uint32_t)0x00000800)            /*!< USB suspend */
#define USB_OTG_GINTSTS_USBRST                  ((uint32_t)0x00001000)            /*!< USB reset */
#define USB_OTG_GINTSTS_ENUMDNE                 ((uint32_t)0x00002000)            /*!< Enumeration done */
#define USB_OTG_GINTSTS_ISOODRP                 ((uint32_t)0x00004000)            /*!< Isochronous OUT packet dropped interrupt */
#define USB_OTG_GINTSTS_EOPF                    ((uint32_t)0x00008000)            /*!< End of periodic frame interrupt */
#define USB_OTG_GINTSTS_IEPINT                  ((uint32_t)0x00040000)            /*!< IN endpoint interrupt */
#define USB_OTG_GINTSTS_OEPINT                  ((uint32_t)0x00080000)            /*!< OUT endpoint interrupt */
#define USB_OTG_GINTSTS_IISOIXFR                ((uint32_t)0x00100000)            /*!< Incomplete isochronous IN transfer */
#define USB_OTG_GINTSTS_PXFR_INCOMPISOOUT       ((uint32_t)0x00200000)            /*!< Incomplete periodic transfer */
#define USB_OTG_GINTSTS_DATAFSUSP               ((uint32_t)0x00400000)            /*!< Data fetch suspended */
#define USB_OTG_GINTSTS_HPRTINT                 ((uint32_t)0x01000000)            /*!< Host port interrupt */
#define USB_OTG_GINTSTS_HCINT                   ((uint32_t)0x02000000)            /*!< Host channels interrupt */
#define USB_OTG_GINTSTS_PTXFE                   ((uint32_t)0x04000000)            /*!< Periodic TxFIFO empty */
#define USB_OTG_GINTSTS_CIDSCHG                 ((uint32_t)0x10000000)            /*!< Connector ID status change */
#define USB_OTG_GINTSTS_DISCINT                 ((uint32_t)0x20000000)            /*!< Disconnect detected interrupt */
#define USB_OTG_GINTSTS_SRQINT                  ((uint32_t)0x40000000)            /*!< Session request/new session detected interrupt */
#define USB_OTG_GINTSTS_WKUINT                  ((uint32_t)0x80000000)            /*!< Resume/remote wakeup detected interrupt */
#endif

DEFINE_PRTREG(USB_OTG, GINTSTS, WKUINT, SRQINT, DISCINT, CIDSCHG, HCINT, HPRTINT, DATAFSUSP, PXFR_INCOMPISOOUT,
    IISOIXFR, OEPINT, IEPINT, EOPF, ISOODRP, ENUMDNE, USBRST, USBSUSP, ESUSP, BOUTNAKEFF, GINAKEFF, NPTXFE,
    RXFLVL, SOF, OTGINT, MMIS, CMOD);

#if 0
/********************  Bit definition forUSB_OTG_GINTMSK register  ********************/
#define USB_OTG_GINTMSK_MMISM                   ((uint32_t)0x00000002)            /*!< Mode mismatch interrupt mask */
#define USB_OTG_GINTMSK_OTGINT                  ((uint32_t)0x00000004)            /*!< OTG interrupt mask */
#define USB_OTG_GINTMSK_SOFM                    ((uint32_t)0x00000008)            /*!< Start of frame mask */
#define USB_OTG_GINTMSK_RXFLVLM                 ((uint32_t)0x00000010)            /*!< Receive FIFO nonempty mask */
#define USB_OTG_GINTMSK_NPTXFEM                 ((uint32_t)0x00000020)            /*!< Nonperiodic TxFIFO empty mask */
#define USB_OTG_GINTMSK_GINAKEFFM               ((uint32_t)0x00000040)            /*!< Global nonperiodic IN NAK effective mask */
#define USB_OTG_GINTMSK_GONAKEFFM               ((uint32_t)0x00000080)            /*!< Global OUT NAK effective mask */
#define USB_OTG_GINTMSK_ESUSPM                  ((uint32_t)0x00000400)            /*!< Early suspend mask */
#define USB_OTG_GINTMSK_USBSUSPM                ((uint32_t)0x00000800)            /*!< USB suspend mask */
#define USB_OTG_GINTMSK_USBRST                  ((uint32_t)0x00001000)            /*!< USB reset mask */
#define USB_OTG_GINTMSK_ENUMDNEM                ((uint32_t)0x00002000)            /*!< Enumeration done mask */
#define USB_OTG_GINTMSK_ISOODRPM                ((uint32_t)0x00004000)            /*!< Isochronous OUT packet dropped interrupt mask */
#define USB_OTG_GINTMSK_EOPFM                   ((uint32_t)0x00008000)            /*!< End of periodic frame interrupt mask */
#define USB_OTG_GINTMSK_EPMISM                  ((uint32_t)0x00020000)            /*!< Endpoint mismatch interrupt mask */
#define USB_OTG_GINTMSK_IEPINT                  ((uint32_t)0x00040000)            /*!< IN endpoints interrupt mask */
#define USB_OTG_GINTMSK_OEPINT                  ((uint32_t)0x00080000)            /*!< OUT endpoints interrupt mask */
#define USB_OTG_GINTMSK_IISOIXFRM               ((uint32_t)0x00100000)            /*!< Incomplete isochronous IN transfer mask */
#define USB_OTG_GINTMSK_PXFRM_IISOOXFRM         ((uint32_t)0x00200000)            /*!< Incomplete periodic transfer mask */
#define USB_OTG_GINTMSK_FSUSPM                  ((uint32_t)0x00400000)            /*!< Data fetch suspended mask */
#define USB_OTG_GINTMSK_PRTIM                   ((uint32_t)0x01000000)            /*!< Host port interrupt mask */
#define USB_OTG_GINTMSK_HCIM                    ((uint32_t)0x02000000)            /*!< Host channels interrupt mask */
#define USB_OTG_GINTMSK_PTXFEM                  ((uint32_t)0x04000000)            /*!< Periodic TxFIFO empty mask */
#define USB_OTG_GINTMSK_CIDSCHGM                ((uint32_t)0x10000000)            /*!< Connector ID status change mask */
#define USB_OTG_GINTMSK_DISCINT                 ((uint32_t)0x20000000)            /*!< Disconnect detected interrupt mask */
#define USB_OTG_GINTMSK_SRQIM                   ((uint32_t)0x40000000)            /*!< Session request/new session detected interrupt mask */
#define USB_OTG_GINTMSK_WUIM                    ((uint32_t)0x80000000)            /*!< Resume/remote wakeup detected interrupt mask */
#endif

DEFINE_PRTREG(USB_OTG, GINTMSK, WUIM, SRQIM, DISCINT, CIDSCHGM, PTXFEM, HCIM, PRTIM, FSUSPM, PXFRM_IISOOXFRM, IISOIXFRM,
    OEPINT, IEPINT, EPMISM, EOPFM, ISOODRPM, ENUMDNEM, USBRST, USBSUSPM, ESUSPM, GONAKEFFM, GINAKEFFM, NPTXFEM,
    RXFLVLM, SOFM, OTGINT, MMISM);

#if 0
/********************  Bit definition for USB_OTG_GRXSTSP register  ********************/
#define USB_OTG_GRXSTSP_EPNUM                    ((uint32_t)0x0000000F)            /*!< IN EP interrupt mask bits */
#define USB_OTG_GRXSTSP_BCNT                     ((uint32_t)0x00007FF0)            /*!< OUT EP interrupt mask bits */
#define USB_OTG_GRXSTSP_DPID                     ((uint32_t)0x00018000)            /*!< OUT EP interrupt mask bits */
#define USB_OTG_GRXSTSP_PKTSTS                   ((uint32_t)0x001E0000)            /*!< OUT EP interrupt mask bits */
#endif


DEFINE_PRTREG(USB_OTG, GRXSTSP, PKTSTS, DPID, BCNT, EPNUM);

#if 0
/********************  Bit definition forUSB_OTG_GRXFSIZ register  ********************/
#define USB_OTG_GRXFSIZ_RXFD                    ((uint32_t)0x0000FFFF)            /*!< RxFIFO depth */
#endif

DEFINE_PRTREG(USB_OTG, GRXFSIZ, RXFD);

#if 0
/********************  Bit definition for OTG register  ********************/
#define USB_OTG_NPTXFSA                 ((uint32_t)0x0000FFFF)            /*!< Nonperiodic transmit RAM start address */
#define USB_OTG_NPTXFD                  ((uint32_t)0xFFFF0000)            /*!< Nonperiodic TxFIFO depth */
#define USB_OTG_TX0FSA                  ((uint32_t)0x0000FFFF)            /*!< Endpoint 0 transmit RAM start address */
#define USB_OTG_TX0FD                   ((uint32_t)0xFFFF0000)            /*!< Endpoint 0 TxFIFO depth */
#endif

#define USB_OTG_GNPTXFSIZ_NPTXFSA                 ((uint32_t)0x0000FFFF)            /*!< Nonperiodic transmit RAM start address */
#define USB_OTG_GNPTXFSIZ_NPTXFD                  ((uint32_t)0xFFFF0000)            /*!< Nonperiodic TxFIFO depth */


DEFINE_PRTREG(USB_OTG, GNPTXFSIZ, NPTXFD, NPTXFSA);

#if 0
/********************  Bit definition forUSB_OTG_GNPTXSTS register  ********************/
#define USB_OTG_GNPTXSTS_NPTXFSAV                ((uint32_t)0x0000FFFF)            /*!< Nonperiodic TxFIFO space available */

#define USB_OTG_GNPTXSTS_NPTQXSAV                ((uint32_t)0x00FF0000)            /*!< Nonperiodic transmit request queue space available */
#define USB_OTG_GNPTXSTS_NPTQXSAV_0              ((uint32_t)0x00010000)            /*!<Bit 0 */
#define USB_OTG_GNPTXSTS_NPTQXSAV_1              ((uint32_t)0x00020000)            /*!<Bit 1 */
#define USB_OTG_GNPTXSTS_NPTQXSAV_2              ((uint32_t)0x00040000)            /*!<Bit 2 */
#define USB_OTG_GNPTXSTS_NPTQXSAV_3              ((uint32_t)0x00080000)            /*!<Bit 3 */
#define USB_OTG_GNPTXSTS_NPTQXSAV_4              ((uint32_t)0x00100000)            /*!<Bit 4 */
#define USB_OTG_GNPTXSTS_NPTQXSAV_5              ((uint32_t)0x00200000)            /*!<Bit 5 */
#define USB_OTG_GNPTXSTS_NPTQXSAV_6              ((uint32_t)0x00400000)            /*!<Bit 6 */
#define USB_OTG_GNPTXSTS_NPTQXSAV_7              ((uint32_t)0x00800000)            /*!<Bit 7 */

#define USB_OTG_GNPTXSTS_NPTXQTOP                ((uint32_t)0x7F000000)            /*!< Top of the nonperiodic transmit request queue */
#define USB_OTG_GNPTXSTS_NPTXQTOP_0              ((uint32_t)0x01000000)            /*!<Bit 0 */
#define USB_OTG_GNPTXSTS_NPTXQTOP_1              ((uint32_t)0x02000000)            /*!<Bit 1 */
#define USB_OTG_GNPTXSTS_NPTXQTOP_2              ((uint32_t)0x04000000)            /*!<Bit 2 */
#define USB_OTG_GNPTXSTS_NPTXQTOP_3              ((uint32_t)0x08000000)            /*!<Bit 3 */
#define USB_OTG_GNPTXSTS_NPTXQTOP_4              ((uint32_t)0x10000000)            /*!<Bit 4 */
#define USB_OTG_GNPTXSTS_NPTXQTOP_5              ((uint32_t)0x20000000)            /*!<Bit 5 */
#define USB_OTG_GNPTXSTS_NPTXQTOP_6              ((uint32_t)0x40000000)            /*!<Bit 6 */
#endif

DEFINE_PRTREG(USB_OTG, GNPTXSTS, NPTXQTOP, NPTXFSAV);

#if 0
/********************  Bit definition forUSB_OTG_GCCFG register  ********************/
#define USB_OTG_GCCFG_PWRDWN                  ((uint32_t)0x00010000)            /*!< Power down */
#define USB_OTG_GCCFG_I2CPADEN                ((uint32_t)0x00020000)            /*!< Enable I2C bus connection for the external I2C PHY interface */
#define USB_OTG_GCCFG_VBUSASEN                ((uint32_t)0x00040000)            /*!< Enable the VBUS sensing device */
#define USB_OTG_GCCFG_VBUSBSEN                ((uint32_t)0x00080000)            /*!< Enable the VBUS sensing device */
#define USB_OTG_GCCFG_SOFOUTEN                ((uint32_t)0x00100000)            /*!< SOF output enable */
#define USB_OTG_GCCFG_NOVBUSSENS              ((uint32_t)0x00200000)            /*!< VBUS sensing disable option */
#endif

DEFINE_PRTREG(USB_OTG, GCCFG, NOVBUSSENS, SOFOUTEN, VBUSBSEN, VBUSASEN, I2CPADEN, PWRDWN);

#if 0
/********************  Bit definition forUSB_OTG_CID register  ********************/
#define USB_OTG_CID_PRODUCT_ID              ((uint32_t)0xFFFFFFFF)            /*!< Product ID field */
#endif

DEFINE_PRTREG(USB_OTG, CID, PRODUCT_ID);

#if 0
/********************  Bit definition forUSB_OTG_HPTXFSIZ register  ********************/
#define USB_OTG_HPTXFSIZ_PTXSA                   ((uint32_t)0x0000FFFF)            /*!< Host periodic TxFIFO start address */
#define USB_OTG_HPTXFSIZ_PTXFD                   ((uint32_t)0xFFFF0000)            /*!< Host periodic TxFIFO depth */
#endif

DEFINE_PRTREG(USB_OTG, HPTXFSIZ, PTXFD, PTXSA);

#if 0
/********************  Bit definition forUSB_OTG_DIEPTXF register  ********************/
#define USB_OTG_DIEPTXF_INEPTXSA                 ((uint32_t)0x0000FFFF)            /*!< IN endpoint FIFOx transmit RAM start address */
#define USB_OTG_DIEPTXF_INEPTXFD                 ((uint32_t)0xFFFF0000)            /*!< IN endpoint TxFIFO depth */
#endif

DEFINE_PRTREG(USB_OTG, DIEPTXF, INEPTXFD, INEPTXSA);

/*****************************************************************************/

#if 0
typedef struct
{
  __IO uint32_t GOTGCTL;      /*!<  USB_OTG Control and Status Register    000h*/
  __IO uint32_t GOTGINT;      /*!<  USB_OTG Interrupt Register             004h*/
  __IO uint32_t GAHBCFG;      /*!<  Core AHB Configuration Register    008h*/
  __IO uint32_t GUSBCFG;      /*!<  Core USB Configuration Register    00Ch*/
  __IO uint32_t GRSTCTL;      /*!<  Core Reset Register                010h*/
  __IO uint32_t GINTSTS;      /*!<  Core Interrupt Register            014h*/
  __IO uint32_t GINTMSK;      /*!<  Core Interrupt Mask Register       018h*/
  __IO uint32_t GRXSTSR;      /*!<  Receive Sts Q Read Register        01Ch*/
  __IO uint32_t GRXSTSP;      /*!<  Receive Sts Q Read & POP Register  020h*/
  __IO uint32_t GRXFSIZ;      /* Receive FIFO Size Register         024h*/
  __IO uint32_t DIEPTXF0_HNPTXFSIZ;   /*!<  EP0 / Non Periodic Tx FIFO Size Register 028h*/
  __IO uint32_t HNPTXSTS;     /*!<  Non Periodic Tx FIFO/Queue Sts reg 02Ch*/
  uint32_t Reserved30[2];     /* Reserved                           030h*/
  __IO uint32_t GCCFG;        /* General Purpose IO Register        038h*/
  __IO uint32_t CID;          /* User ID Register                   03Ch*/
  uint32_t  Reserved40[48];   /* Reserved                      040h-0FFh*/
  __IO uint32_t HPTXFSIZ; /* Host Periodic Tx FIFO Size Reg     100h*/
  __IO uint32_t DIEPTXF[0x0F];/* dev Periodic Transmit FIFO */
}
USB_OTG_GlobalTypeDef;
#endif

static void print_usb_otg_global_registers(void) {

//  int i;

  HCD_HandleTypeDef *hhcd = &hhcd_USB_OTG_HS;

  PRT_USB_OTG_GOTGCTL(hhcd->Instance->GOTGCTL);
  PRT_USB_OTG_GOTGINT(hhcd->Instance->GOTGINT);
  PRT_USB_OTG_GAHBCFG(hhcd->Instance->GAHBCFG);
  PRT_USB_OTG_GUSBCFG(hhcd->Instance->GUSBCFG);
  PRT_USB_OTG_GRSTCTL(hhcd->Instance->GRSTCTL);
  PRT_USB_OTG_GINTSTS(hhcd->Instance->GINTSTS);
  PRT_USB_OTG_GINTMSK(hhcd->Instance->GINTMSK);
  PRT_USB_OTG_GRXSTSP(hhcd->Instance->GRXSTSR);               // read
  PRT_USB_OTG_GRXSTSP(hhcd->Instance->GRXSTSP);               // pop
  PRT_USB_OTG_GRXFSIZ(hhcd->Instance->GRXFSIZ);
  PRT_USB_OTG_GNPTXFSIZ(hhcd->Instance->DIEPTXF0_HNPTXFSIZ);
  PRT_USB_OTG_GNPTXSTS(hhcd->Instance->HNPTXSTS);
  PRT_USB_OTG_GCCFG(hhcd->Instance->GCCFG);
  PRT_USB_OTG_CID(hhcd->Instance->CID);
  PRT_USB_OTG_HPTXFSIZ(hhcd->Instance->HPTXFSIZ);

  /** seems to be device only regs **/
//  for (i = 0; i < 0x0F; i++)
//    PRT_USB_OTG_DIEPTXF(hhcd->Instance->DIEPTXF[i]);
}

/*****************************************************************************/

#if 0
/********************  Bit definition forUSB_OTG_HCFG register  ********************/

#define USB_OTG_HCFG_FSLSPCS                 ((uint32_t)0x00000003)            /*!< FS/LS PHY clock select */
#define USB_OTG_HCFG_FSLSPCS_0               ((uint32_t)0x00000001)            /*!<Bit 0 */
#define USB_OTG_HCFG_FSLSPCS_1               ((uint32_t)0x00000002)            /*!<Bit 1 */
#define USB_OTG_HCFG_FSLSS                   ((uint32_t)0x00000004)            /*!< FS- and LS-only support */
#endif

DEFINE_PRTREG(USB_OTG, HCFG, FSLSS, FSLSPCS);

#if 0
/********************  Bit definition forUSB_OTG_HFIR register  ********************/
#define USB_OTG_HFIR_FRIVL                   ((uint32_t)0x0000FFFF)            /*!< Frame interval */
#endif

DEFINE_PRTREG(USB_OTG, HFIR, FRIVL);

#if 0
/********************  Bit definition forUSB_OTG_HFNUM register  ********************/
#define USB_OTG_HFNUM_FRNUM                   ((uint32_t)0x0000FFFF)            /*!< Frame number */
#define USB_OTG_HFNUM_FTREM                   ((uint32_t)0xFFFF0000)            /*!< Frame time remaining */
#endif

DEFINE_PRTREG(USB_OTG, HFNUM, FTREM, FRNUM);

#if 0
/********************  Bit definition forUSB_OTG_HPTXSTS register  ********************/
#define USB_OTG_HPTXSTS_PTXFSAVL                ((uint32_t)0x0000FFFF)            /*!< Periodic transmit data FIFO space available */

#define USB_OTG_HPTXSTS_PTXQSAV                 ((uint32_t)0x00FF0000)            /*!< Periodic transmit request queue space available */
#define USB_OTG_HPTXSTS_PTXQSAV_0               ((uint32_t)0x00010000)            /*!<Bit 0 */
#define USB_OTG_HPTXSTS_PTXQSAV_1               ((uint32_t)0x00020000)            /*!<Bit 1 */
#define USB_OTG_HPTXSTS_PTXQSAV_2               ((uint32_t)0x00040000)            /*!<Bit 2 */
#define USB_OTG_HPTXSTS_PTXQSAV_3               ((uint32_t)0x00080000)            /*!<Bit 3 */
#define USB_OTG_HPTXSTS_PTXQSAV_4               ((uint32_t)0x00100000)            /*!<Bit 4 */
#define USB_OTG_HPTXSTS_PTXQSAV_5               ((uint32_t)0x00200000)            /*!<Bit 5 */
#define USB_OTG_HPTXSTS_PTXQSAV_6               ((uint32_t)0x00400000)            /*!<Bit 6 */
#define USB_OTG_HPTXSTS_PTXQSAV_7               ((uint32_t)0x00800000)            /*!<Bit 7 */

#define USB_OTG_HPTXSTS_PTXQTOP                 ((uint32_t)0xFF000000)            /*!< Top of the periodic transmit request queue */
#define USB_OTG_HPTXSTS_PTXQTOP_0               ((uint32_t)0x01000000)            /*!<Bit 0 */
#define USB_OTG_HPTXSTS_PTXQTOP_1               ((uint32_t)0x02000000)            /*!<Bit 1 */
#define USB_OTG_HPTXSTS_PTXQTOP_2               ((uint32_t)0x04000000)            /*!<Bit 2 */
#define USB_OTG_HPTXSTS_PTXQTOP_3               ((uint32_t)0x08000000)            /*!<Bit 3 */
#define USB_OTG_HPTXSTS_PTXQTOP_4               ((uint32_t)0x10000000)            /*!<Bit 4 */
#define USB_OTG_HPTXSTS_PTXQTOP_5               ((uint32_t)0x20000000)            /*!<Bit 5 */
#define USB_OTG_HPTXSTS_PTXQTOP_6               ((uint32_t)0x40000000)            /*!<Bit 6 */
#define USB_OTG_HPTXSTS_PTXQTOP_7               ((uint32_t)0x80000000)            /*!<Bit 7 */
#endif

DEFINE_PRTREG(USB_OTG, HPTXSTS, PTXQTOP, PTXQSAV, PTXFSAVL);

#if 0
/********************  Bit definition forUSB_OTG_HAINT register  ********************/
#define USB_OTG_HAINT_HAINT                   ((uint32_t)0x0000FFFF)            /*!< Channel interrupts */
#endif

DEFINE_PRTREG(USB_OTG, HAINT, HAINT);

#define USB_OTG_HAINTMSK_HAINTMSK             ((uint32_t)0x0000FFFF)            /*!< Channel interrupts */

DEFINE_PRTREG(USB_OTG, HAINTMSK, HAINTMSK);

#if 0
typedef struct
{
  __IO uint32_t HCFG;             /* Host Configuration Register    400h*/
  __IO uint32_t HFIR;      /* Host Frame Interval Register   404h*/
  __IO uint32_t HFNUM;         /* Host Frame Nbr/Frame Remaining 408h*/
  uint32_t Reserved40C;                   /* Reserved                       40Ch*/
  __IO uint32_t HPTXSTS;   /* Host Periodic Tx FIFO/ Queue Status 410h*/
  __IO uint32_t HAINT;   /* Host All Channels Interrupt Register 414h*/
  __IO uint32_t HAINTMSK;   /* Host All Channels Interrupt Mask 418h*/
}
USB_OTG_HostTypeDef;
#endif

static void print_usb_host_registers(void) {

  HCD_HandleTypeDef *hhcd = &hhcd_USB_OTG_HS;
  USB_OTG_GlobalTypeDef *USBx = hhcd->Instance;

  PRT_USB_OTG_HCFG(USBx_HOST->HCFG);
  PRT_USB_OTG_HFIR(USBx_HOST->HFIR);
  PRT_USB_OTG_HFNUM(USBx_HOST->HFNUM);
  PRT_USB_OTG_HPTXSTS(USBx_HOST->HPTXSTS);
  PRT_USB_OTG_HAINT(USBx_HOST->HAINT);
  PRT_USB_OTG_HAINTMSK(USBx_HOST->HAINTMSK);
}

/*****************************************************************************/


#if 0
/********************  Bit definition forUSB_OTG_HPRT register  ********************/
#define USB_OTG_HPRT_PCSTS                   ((uint32_t)0x00000001)            /*!< Port connect status */
#define USB_OTG_HPRT_PCDET                   ((uint32_t)0x00000002)            /*!< Port connect detected */
#define USB_OTG_HPRT_PENA                    ((uint32_t)0x00000004)            /*!< Port enable */
#define USB_OTG_HPRT_PENCHNG                 ((uint32_t)0x00000008)            /*!< Port enable/disable change */
#define USB_OTG_HPRT_POCA                    ((uint32_t)0x00000010)            /*!< Port overcurrent active */
#define USB_OTG_HPRT_POCCHNG                 ((uint32_t)0x00000020)            /*!< Port overcurrent change */
#define USB_OTG_HPRT_PRES                    ((uint32_t)0x00000040)            /*!< Port resume */
#define USB_OTG_HPRT_PSUSP                   ((uint32_t)0x00000080)            /*!< Port suspend */
#define USB_OTG_HPRT_PRST                    ((uint32_t)0x00000100)            /*!< Port reset */

#define USB_OTG_HPRT_PLSTS                   ((uint32_t)0x00000C00)            /*!< Port line status */
#define USB_OTG_HPRT_PLSTS_0                 ((uint32_t)0x00000400)            /*!<Bit 0 */
#define USB_OTG_HPRT_PLSTS_1                 ((uint32_t)0x00000800)            /*!<Bit 1 */
#define USB_OTG_HPRT_PPWR                    ((uint32_t)0x00001000)            /*!< Port power */

#define USB_OTG_HPRT_PTCTL                   ((uint32_t)0x0001E000)            /*!< Port test control */
#define USB_OTG_HPRT_PTCTL_0                 ((uint32_t)0x00002000)            /*!<Bit 0 */
#define USB_OTG_HPRT_PTCTL_1                 ((uint32_t)0x00004000)            /*!<Bit 1 */
#define USB_OTG_HPRT_PTCTL_2                 ((uint32_t)0x00008000)            /*!<Bit 2 */
#define USB_OTG_HPRT_PTCTL_3                 ((uint32_t)0x00010000)            /*!<Bit 3 */

#define USB_OTG_HPRT_PSPD                    ((uint32_t)0x00060000)            /*!< Port speed */
#define USB_OTG_HPRT_PSPD_0                  ((uint32_t)0x00020000)            /*!<Bit 0 */
#define USB_OTG_HPRT_PSPD_1                  ((uint32_t)0x00040000)            /*!<Bit 1 */
#endif

DEFINE_PRTREG(USB_OTG, HPRT, PSPD, PTCTL, PLSTS, PRST, PSUSP, PRES, POCCHNG, POCA, PENCHNG, PENA, PCDET, PCSTS);

static void print_usb_port_registers(void) {

  HCD_HandleTypeDef *hhcd = &hhcd_USB_OTG_HS;
  USB_OTG_GlobalTypeDef *USBx = hhcd->Instance;

  PRT_USB_OTG_HPRT(USBx_HPRT0);
}

/*****************************************************************************/

#if 0
/********************  Bit definition forUSB_OTG_HCCHAR register  ********************/
#define USB_OTG_HCCHAR_MPSIZ                   ((uint32_t)0x000007FF)            /*!< Maximum packet size */

#define USB_OTG_HCCHAR_EPNUM                   ((uint32_t)0x00007800)            /*!< Endpoint number */
#define USB_OTG_HCCHAR_EPNUM_0                 ((uint32_t)0x00000800)            /*!<Bit 0 */
#define USB_OTG_HCCHAR_EPNUM_1                 ((uint32_t)0x00001000)            /*!<Bit 1 */
#define USB_OTG_HCCHAR_EPNUM_2                 ((uint32_t)0x00002000)            /*!<Bit 2 */
#define USB_OTG_HCCHAR_EPNUM_3                 ((uint32_t)0x00004000)            /*!<Bit 3 */
#define USB_OTG_HCCHAR_EPDIR                   ((uint32_t)0x00008000)            /*!< Endpoint direction */
#define USB_OTG_HCCHAR_LSDEV                   ((uint32_t)0x00020000)            /*!< Low-speed device */

#define USB_OTG_HCCHAR_EPTYP                   ((uint32_t)0x000C0000)            /*!< Endpoint type */
#define USB_OTG_HCCHAR_EPTYP_0                 ((uint32_t)0x00040000)            /*!<Bit 0 */
#define USB_OTG_HCCHAR_EPTYP_1                 ((uint32_t)0x00080000)            /*!<Bit 1 */

#define USB_OTG_HCCHAR_MC                      ((uint32_t)0x00300000)            /*!< Multi Count (MC) / Error Count (EC) */
#define USB_OTG_HCCHAR_MC_0                    ((uint32_t)0x00100000)            /*!<Bit 0 */
#define USB_OTG_HCCHAR_MC_1                    ((uint32_t)0x00200000)            /*!<Bit 1 */

#define USB_OTG_HCCHAR_DAD                     ((uint32_t)0x1FC00000)            /*!< Device address */
#define USB_OTG_HCCHAR_DAD_0                   ((uint32_t)0x00400000)            /*!<Bit 0 */
#define USB_OTG_HCCHAR_DAD_1                   ((uint32_t)0x00800000)            /*!<Bit 1 */
#define USB_OTG_HCCHAR_DAD_2                   ((uint32_t)0x01000000)            /*!<Bit 2 */
#define USB_OTG_HCCHAR_DAD_3                   ((uint32_t)0x02000000)            /*!<Bit 3 */
#define USB_OTG_HCCHAR_DAD_4                   ((uint32_t)0x04000000)            /*!<Bit 4 */
#define USB_OTG_HCCHAR_DAD_5                   ((uint32_t)0x08000000)            /*!<Bit 5 */
#define USB_OTG_HCCHAR_DAD_6                   ((uint32_t)0x10000000)            /*!<Bit 6 */
#define USB_OTG_HCCHAR_ODDFRM                  ((uint32_t)0x20000000)            /*!< Odd frame */
#define USB_OTG_HCCHAR_CHDIS                   ((uint32_t)0x40000000)            /*!< Channel disable */
#define USB_OTG_HCCHAR_CHENA                   ((uint32_t)0x80000000)            /*!< Channel enable */
#endif

DEFINE_PRTREG(USB_OTG, HCCHAR, CHENA, CHDIS, ODDFRM, DAD, MC, EPTYP, LSDEV, EPDIR, EPNUM, MPSIZ);

#if 0

/********************  Bit definition forUSB_OTG_HCSPLT register  ********************/

#define USB_OTG_HCSPLT_PRTADDR                 ((uint32_t)0x0000007F)            /*!< Port address */
#define USB_OTG_HCSPLT_PRTADDR_0               ((uint32_t)0x00000001)            /*!<Bit 0 */
#define USB_OTG_HCSPLT_PRTADDR_1               ((uint32_t)0x00000002)            /*!<Bit 1 */
#define USB_OTG_HCSPLT_PRTADDR_2               ((uint32_t)0x00000004)            /*!<Bit 2 */
#define USB_OTG_HCSPLT_PRTADDR_3               ((uint32_t)0x00000008)            /*!<Bit 3 */
#define USB_OTG_HCSPLT_PRTADDR_4               ((uint32_t)0x00000010)            /*!<Bit 4 */
#define USB_OTG_HCSPLT_PRTADDR_5               ((uint32_t)0x00000020)            /*!<Bit 5 */
#define USB_OTG_HCSPLT_PRTADDR_6               ((uint32_t)0x00000040)            /*!<Bit 6 */

#define USB_OTG_HCSPLT_HUBADDR                 ((uint32_t)0x00003F80)            /*!< Hub address */
#define USB_OTG_HCSPLT_HUBADDR_0               ((uint32_t)0x00000080)            /*!<Bit 0 */
#define USB_OTG_HCSPLT_HUBADDR_1               ((uint32_t)0x00000100)            /*!<Bit 1 */
#define USB_OTG_HCSPLT_HUBADDR_2               ((uint32_t)0x00000200)            /*!<Bit 2 */
#define USB_OTG_HCSPLT_HUBADDR_3               ((uint32_t)0x00000400)            /*!<Bit 3 */
#define USB_OTG_HCSPLT_HUBADDR_4               ((uint32_t)0x00000800)            /*!<Bit 4 */
#define USB_OTG_HCSPLT_HUBADDR_5               ((uint32_t)0x00001000)            /*!<Bit 5 */
#define USB_OTG_HCSPLT_HUBADDR_6               ((uint32_t)0x00002000)            /*!<Bit 6 */

#define USB_OTG_HCSPLT_XACTPOS                 ((uint32_t)0x0000C000)            /*!< XACTPOS */
#define USB_OTG_HCSPLT_XACTPOS_0               ((uint32_t)0x00004000)            /*!<Bit 0 */
#define USB_OTG_HCSPLT_XACTPOS_1               ((uint32_t)0x00008000)            /*!<Bit 1 */
#define USB_OTG_HCSPLT_COMPLSPLT               ((uint32_t)0x00010000)            /*!< Do complete split */
#define USB_OTG_HCSPLT_SPLITEN                 ((uint32_t)0x80000000)            /*!< Split enable */
#endif

DEFINE_PRTREG(USB_OTG, HCSPLT, SPLITEN, COMPLSPLT, XACTPOS, HUBADDR, PRTADDR);

#if 0
/********************  Bit definition forUSB_OTG_HCINT register  ********************/
#define USB_OTG_HCINT_XFRC                    ((uint32_t)0x00000001)            /*!< Transfer completed */
#define USB_OTG_HCINT_CHH                     ((uint32_t)0x00000002)            /*!< Channel halted */
#define USB_OTG_HCINT_AHBERR                  ((uint32_t)0x00000004)            /*!< AHB error */
#define USB_OTG_HCINT_STALL                   ((uint32_t)0x00000008)            /*!< STALL response received interrupt */
#define USB_OTG_HCINT_NAK                     ((uint32_t)0x00000010)            /*!< NAK response received interrupt */
#define USB_OTG_HCINT_ACK                     ((uint32_t)0x00000020)            /*!< ACK response received/transmitted interrupt */
#define USB_OTG_HCINT_NYET                    ((uint32_t)0x00000040)            /*!< Response received interrupt */
#define USB_OTG_HCINT_TXERR                   ((uint32_t)0x00000080)            /*!< Transaction error */
#define USB_OTG_HCINT_BBERR                   ((uint32_t)0x00000100)            /*!< Babble error */
#define USB_OTG_HCINT_FRMOR                   ((uint32_t)0x00000200)            /*!< Frame overrun */
#define USB_OTG_HCINT_DTERR                   ((uint32_t)0x00000400)            /*!< Data toggle error */
#endif

DEFINE_PRTREG(USB_OTG, HCINT, DTERR, FRMOR, BBERR, TXERR, NYET, ACK, NAK, STALL, AHBERR, CHH, XFRC);

#if 0
/********************  Bit definition forUSB_OTG_HCINTMSK register  ********************/
#define USB_OTG_HCINTMSK_XFRCM                   ((uint32_t)0x00000001)            /*!< Transfer completed mask */
#define USB_OTG_HCINTMSK_CHHM                    ((uint32_t)0x00000002)            /*!< Channel halted mask */
#define USB_OTG_HCINTMSK_AHBERR                  ((uint32_t)0x00000004)            /*!< AHB error */
#define USB_OTG_HCINTMSK_STALLM                  ((uint32_t)0x00000008)            /*!< STALL response received interrupt mask */
#define USB_OTG_HCINTMSK_NAKM                    ((uint32_t)0x00000010)            /*!< NAK response received interrupt mask */
#define USB_OTG_HCINTMSK_ACKM                    ((uint32_t)0x00000020)            /*!< ACK response received/transmitted interrupt mask */
#define USB_OTG_HCINTMSK_NYET                    ((uint32_t)0x00000040)            /*!< response received interrupt mask */
#define USB_OTG_HCINTMSK_TXERRM                  ((uint32_t)0x00000080)            /*!< Transaction error mask */
#define USB_OTG_HCINTMSK_BBERRM                  ((uint32_t)0x00000100)            /*!< Babble error mask */
#define USB_OTG_HCINTMSK_FRMORM                  ((uint32_t)0x00000200)            /*!< Frame overrun mask */
#define USB_OTG_HCINTMSK_DTERRM                  ((uint32_t)0x00000400)            /*!< Data toggle error mask */
#endif

DEFINE_PRTREG(USB_OTG, HCINTMSK, DTERRM, FRMORM, BBERRM, TXERRM, NYET, ACKM, NAKM, STALLM, AHBERR, CHHM, XFRCM);

#if 0
/********************  Bit definition forUSB_OTG_HCTSIZ register  ********************/
#define USB_OTG_HCTSIZ_XFRSIZ                    ((uint32_t)0x0007FFFF)            /*!< Transfer size */
#define USB_OTG_HCTSIZ_PKTCNT                    ((uint32_t)0x1FF80000)            /*!< Packet count */
#define USB_OTG_HCTSIZ_DOPING                    ((uint32_t)0x80000000)            /*!< Do PING */
#define USB_OTG_HCTSIZ_DPID                      ((uint32_t)0x60000000)            /*!< Data PID */
#define USB_OTG_HCTSIZ_DPID_0                    ((uint32_t)0x20000000)            /*!<Bit 0 */
#define USB_OTG_HCTSIZ_DPID_1                    ((uint32_t)0x40000000)            /*!<Bit 1 */
#endif

DEFINE_PRTREG(USB_OTG, HCTSIZ, DPID, DOPING, PKTCNT, XFRSIZ);

#if 0
/********************  Bit definition forUSB_OTG_HCDMA register  ********************/
#define USB_OTG_HCDMA_DMAADDR                    ((uint32_t)0xFFFFFFFF)            /*!< DMA address */
#endif

DEFINE_PRTREG(USB_OTG, HCDMA, DMAADDR);

#if 0
typedef struct
{
  __IO uint32_t HCCHAR;
  __IO uint32_t HCSPLT;
  __IO uint32_t HCINT;
  __IO uint32_t HCINTMSK;
  __IO uint32_t HCTSIZ;
  __IO uint32_t HCDMA;
  uint32_t Reserved[2];
}
USB_OTG_HostChannelTypeDef;
#endif

void print_usb_channel_registers(int chnum)
{
  HCD_HandleTypeDef *hhcd = &hhcd_USB_OTG_HS;
  USB_OTG_GlobalTypeDef *USBx = hhcd->Instance;

  PRT_USB_OTG_HCCHAR(USBx_HC(chnum)->HCCHAR);
  PRT_USB_OTG_HCSPLT(USBx_HC(chnum)->HCSPLT);
  PRT_USB_OTG_HCINT(USBx_HC(chnum)->HCINT);
  PRT_USB_OTG_HCINTMSK(USBx_HC(chnum)->HCINTMSK);
  PRT_USB_OTG_HCTSIZ(USBx_HC(chnum)->HCTSIZ);
  PRT_USB_OTG_HCDMA(USBx_HC(chnum)->HCDMA);
}

/*****************************************************************************/

/*
 * pa - print all
 * pg - print global
 * ph - print host
 * pt - print port
 * pc - print channel
 */
int usb_host_cmd(const char* cmd)
{
  int i;

  if (0 == strcmp(cmd, "pg"))
  {
    printf("<< USB GLOBAL >>" NEW_LINE);
    print_usb_otg_global_registers();
  }
  else if (0 == strcmp(cmd, "ph"))
  {
    printf(NEW_LINE "<< USB HOST >>" NEW_LINE);
    print_usb_host_registers();
  }
  else if (0 == strcmp(cmd, "pp"))
  {
    printf(NEW_LINE "<< PORT >>" NEW_LINE);
    print_usb_port_registers();
  }
  else if (0 == strcmp(cmd, "pc"))
  {
    for (i = 0; i < 4; i++)
    {
      printf(NEW_LINE "<< CHANNEL %d >>" NEW_LINE, i);
      print_usb_channel_registers(i);
    }
  }
  else if (0 == strcmp(cmd, "pa"))
  {
    printf("<< USB GLOBAL >>" NEW_LINE);
    print_usb_otg_global_registers();
    printf(NEW_LINE "<< USB HOST >>" NEW_LINE);
    print_usb_host_registers();
    printf(NEW_LINE "<< PORT >>" NEW_LINE);
    print_usb_port_registers();
    for (i = 0; i < 4; i++)
    {
      printf(NEW_LINE "<< CHANNEL %d >>" NEW_LINE, i);
      print_usb_channel_registers(i);
    }
  }
  else if (0 == strcmp(cmd, "version"))
  {
    PrintGitVersionControl();
  }
  else
  {
    return -1;
  }

  return 0;
}


