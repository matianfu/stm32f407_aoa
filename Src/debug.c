#include "debug.h"

char __DEBUG_LL_PRINT_BUF[256];

DebugConfigTypeDef DebugConfig = {

    .print_hcd_event = 0,
    .print_debouncer_event = 1,
    .print_device_descriptor = 0,
    .print_aoa_recvdata = 1,

    .handle_control_level = 0,

    .channel_hcintx_default = CHANNEL_HCINTX_NONE,


    .dont_request_core_reset_after_control_error = 1,

    .do_nothing_after_port_down = 1,
    .do_only_disable_global_int_after_port_down = 0,

    .do_nothing_after_disconnect = 1,
    .do_only_disable_global_int_after_disconnect = 0,

    .malicious_attack_channel_clean_up = 0,
};

void restore_debug_defaults(void)
{
  int i;

  debug_hal_hcd_hc_submitrequest_print = DEBUG_HAL_HCD_HC_SUBMITREQUEST_DEFAULT;
  debug_usbh_allocpipe = DEBUG_USBH_ALLOCPIPE_DEFAULT;

  for (i = 0; i < 16; i++)
  {
    DebugConfig.channel_hcintx_mask[i] = DebugConfig.channel_hcintx_default;
  }
}


