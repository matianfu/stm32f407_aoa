#include "debug.h"

void restore_debug_defaults(void)
{
  int i;

  debug_hal_hcd_hc_submitrequest_print = DEBUG_HAL_HCD_HC_SUBMITREQUEST_DEFAULT;
  debug_usbh_allocpipe = DEBUG_USBH_ALLOCPIPE_DEFAULT;

  for (i = 0; i < 16; i++)
  {
    debug_hc_hcintx_mask[16] = DEBUG_HC_HCINTX_MASK_DEFAULT;
  }
}
