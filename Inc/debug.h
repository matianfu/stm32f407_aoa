#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdint.h>



/*
 * Do NVIC_SystemReset after usb disconnect
 */
// #define CONFIG_USBH_SYSRESET_AFTER_DISCONNECT

/**
 * Do CoreReset after usb disconnect forcefully
 */
// #define CONFIG_USBH_FORCE_CORERESET_AFTER_DISCONNECT

#define CHANNEL_HCINTX_ALL                                ((uint32_t)0x000007FF)  /** low 11 bits **/
#define CHANNEL_HCINTX_NONE                               ((uint32_t)0)

typedef struct {

  /* Print Options */
  int print_hcd_event;
  int print_debouncer_event;
  int print_device_descriptor;
  int print_aoa_recvdata;

  int handle_control_level;


  uint32_t channel_hcintx_default;
  uint32_t channel_hcintx_mask[16];

  /* Behavioral Options */
  /*
   * This option is used for DEBUG ONLY! be sure to switch it off in delivered code.
   * This option is intended for generate control pipe error.
   */
  int dont_request_core_reset_after_control_error;      /* set 0 for production code */

  /* default behavior is to call usb_stophost */
  int do_nothing_after_port_down;                       /* set 0 for production code */
  int do_only_disable_global_int_after_port_down;       /* set 0 for production code */

  /* default behavior is to call usb_stophost */
  int do_nothing_after_disconnect;                      /* set 0 for production code */
  int do_only_disable_global_int_after_disconnect;      /* set 0 for production code */

  /* do EPDIR change in usb_stophost, highly possible to trigger control error
   * after reconnect
   */
  int malicious_attack_channel_clean_up;                /* !!! must set 0 for production code */

} DebugConfigTypeDef;

extern DebugConfigTypeDef DebugConfig;

#define DEBUG_HAL_HCD_HC_SUBMITREQUEST_DEFAULT              (0)
extern int debug_hal_hcd_hc_submitrequest_print;
extern int debug_hal_hcd_hc_submitrequest_halt;





#define DEBUG_USBH_ALLOCPIPE_DEFAULT                        (0)
extern int debug_usbh_allocpipe;

void restore_debug_defaults(void);

extern char __DEBUG_LL_PRINT_BUF[256];
extern void USBH_PutMessage(const char* buf);

#ifndef NEW_LINE
#define NEW_LINE                "\r\n"
#endif

#define ASSERT(eval)            do { if ((!eval))                                       \
                                printf("assert fail:" " #eval " "@ %s, %s, %d\r\n",     \
                                __FILE__, __func__, __LINE__); } while (0)

#define DEBUG_LL_LOG(...)       do { snprintf(__DEBUG_LL_PRINT_BUF, 255, __VA_ARGS__);  \
                                __DEBUG_LL_PRINT_BUF[255] = '\0';                       \
                                USBH_PutMessage(__DEBUG_LL_PRINT_BUF); } while (0)

#define DEBUG_LOG(...)          do { printf("%08u  ", (unsigned int)HAL_GetTick());     \
                                printf(__VA_ARGS__);                                    \
                                printf(NEW_LINE); } while (0)

#endif /* __DEBUG_H__ */

