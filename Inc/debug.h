#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdint.h>

#define  ASSERT(eval)                                       if ((!eval)) \
  printf("assert fail:" " #eval " "@ %s, %s, %d\r\n",  __FILE__, __func__, __LINE__)

/*
 * Do NVIC_SystemReset after usb disconnect
 */
// #define CONFIG_USBH_SYSRESET_AFTER_DISCONNECT

/**
 * Do CoreReset after usb disconnect forcefully
 */
// #define CONFIG_USBH_FORCE_CORERESET_AFTER_DISCONNECT



#define DEBUG_HAL_HCD_HC_SUBMITREQUEST_DEFAULT              (0)
extern int debug_hal_hcd_hc_submitrequest_print;
extern int debug_hal_hcd_hc_submitrequest_halt;


#define DEBUG_HC_HCINTX_ALL                                 ((uint32_t)0x000007FF)  /** low 11 bits **/
#define DEBUG_HC_HCINTX_NONE                                ((uint32_t)0)
#define DEBUG_HC_HCINTX_MASK_DEFAULT                        0
extern uint32_t debug_hc_hcintx_mask[16];

#define DEBUG_USBH_ALLOCPIPE_DEFAULT                        (0)
extern int debug_usbh_allocpipe;

void restore_debug_defaults(void);

#endif /* __DEBUG_H__ */

