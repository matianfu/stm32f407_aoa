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

typedef struct {

  /* Print Options */
  int print_hcd_event;
  int print_debouncer_event;
  int print_device_descriptor;

  int handle_control_level;



  /* Behavioral Options */
  /*
   * This option is used for DEBUG ONLY! be sure to switch it off in delivered code.
   * This option is intended for generate control pipe error.
   */
  int no_clean_after_port_down;

} DebugConfigTypeDef;

extern DebugConfigTypeDef DebugConfig;

#define DEBUG_HAL_HCD_HC_SUBMITREQUEST_DEFAULT              (0)
extern int debug_hal_hcd_hc_submitrequest_print;
extern int debug_hal_hcd_hc_submitrequest_halt;


#define DEBUG_HC_HCINTX_ALL                                 ((uint32_t)0x000007FF)  /** low 11 bits **/
#define DEBUG_HC_HCINTX_NONE                                ((uint32_t)0)
#define DEBUG_HC_HCINTX_MASK_DEFAULT                        DEBUG_HC_HCINTX_NONE
extern uint32_t debug_hc_hcintx_mask[16];

#define DEBUG_USBH_ALLOCPIPE_DEFAULT                        (0)
extern int debug_usbh_allocpipe;

void restore_debug_defaults(void);

#endif /* __DEBUG_H__ */

