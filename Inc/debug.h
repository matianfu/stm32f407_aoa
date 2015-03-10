#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdint.h>

#define DEBUG_HAL_HCD_HC_SUBMITREQUEST_DEFAULT              (1)
extern int DEBUG_HAL_HCD_HC_SubmitRequest;

#define DEBUG_HC_HCINTX_ALL                                 ((uint32_t)0x000007FF)  /** low 11 bits **/
#define DEBUG_HC_HCINTX_NONE                                ((uint32_t)0)
#define DEBUG_HC_HCINTX_MASK_DEFAULT                        DEBUG_HC_HCINTX_ALL
extern uint32_t debug_hc_hcintx_mask[16];

#define DEBUG_USBH_ALLOCPIPE_DEFAULT                        (0)
extern int debug_usbh_allocpipe;

#endif /* __DEBUG_H__ */

