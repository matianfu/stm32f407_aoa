#include "usbh_core.h"
#include "usbh_core_helper.h"

extern USBH_HandleTypeDef hUsbHostHS;

/*
 * local constants
 */
const static char* event_string[] =
{ "USBH_EVT_NULL", "USBH_EVT_CONNECT", "USBH_EVT_DISCONNECT",
    "USBH_EVT_PORTUP", "USBH_EVT_PORTDOWN", "USBH_EVT_OVERFLOW",
    "USBH_EVT_HCINT" };

const static char* pstate_string[] =
{ "PORT_IDLE", "PORT_WAIT_PORT_UP", "PORT_UP", "PORT_DOWN" };

const static char* gstate_string[] =
{ "HOST_IDLE", "HOST_DEV_WAIT_FOR_ATTACHMENT", "HOST_DEV_ATTACHED",
    "HOST_DEV_DISCONNECTED", "HOST_DETECT_DEVICE_SPEED", "HOST_ENUMERATION",
    "HOST_CLASS_REQUEST", "HOST_INPUT", "HOST_SET_CONFIGURATION",
    "HOST_CHECK_CLASS", "HOST_HAND_SHAKE", "HOST_CLASS", "HOST_SUSPENDED",
    "HOST_ABORT_STATE" };

char* usbh_helper_gstate_string(HOST_StateTypeDef gstate)
{
  return gstate_string[gstate];
}

const static char* enum_state_string[] =
{ "ENUM_IDLE", "ENUM_GET_FULL_DEV_DESC", "ENUM_SET_ADDR", "ENUM_GET_CFG_DESC",
    "ENUM_GET_FULL_CFG_DESC", "ENUM_GET_MFC_STRING_DESC",
    "ENUM_GET_PRODUCT_STRING_DESC", "ENUM_GET_SERIALNUM_STRING_DESC" };

const static char* request_state_string[] =
{ "CMD_IDLE", "CMD_SEND", "CMD_WAIT" };

const static char* control_state_string[] =
{ "CTRL_IDLE", "CTRL_SETUP", "CTRL_SETUP_WAIT", "CTRL_DATA_IN",
    "CTRL_DATA_IN_WAIT", "CTRL_DATA_OUT", "CTRL_DATA_OUT_WAIT",
    "CTRL_STATUS_IN", "CTRL_STATUS_IN_WAIT", "CTRL_STATUS_OUT",
    "CTRL_STATUS_OUT_WAIT", "CTRL_ERROR", "CTRL_STALLED", "CTRL_COMPLETE" };
/**
  * @brief  URB States definition
  */
const static char * urb_state_string[] =
{ "URB_IDLE", // = 0,
    "URB_DONE", "URB_NOTREADY", "URB_NYET", "URB_ERROR", "URB_STALL" };

/**
  * @brief  Host channel States  definition
  */
const static char * channel_state_string[] =
{ "HC_IDLE", "HC_XFRC", "HC_HALTED", "HC_NAK", "HC_NYET", "HC_STALL",
    "HC_XACTERR", "HC_BBLERR", "HC_DATATGLERR" };

void USBH_Print_DeviceDescriptor(USBH_HandleTypeDef *phost)
{
  int i;

  USBH_DevDescTypeDef* devdesc = &phost->device.DevDesc;
  USBH_CfgDescTypeDef* cfgdesc = &phost->device.CfgDesc;
  USBH_InterfaceDescTypeDef* itfdesc;
  int bNumInterfaces =  cfgdesc->bNumInterfaces;

  USBH_UsrLog("Device Descriptor:");
  USBH_UsrLog("  bLength:            %d",   devdesc->bLength);
  USBH_UsrLog("  bDescriptionType:   %d",   devdesc->bDescriptorType);
  USBH_UsrLog("  bcdUSB:             0x%04x", devdesc->bcdUSB);
  USBH_UsrLog("  bDeviceClass:       %d",   devdesc->bDeviceClass);
  USBH_UsrLog("  bDeviceSubClass:    %d",   devdesc->bDeviceSubClass);
  USBH_UsrLog("  bDeviceProtocol:    %d",   devdesc->bDeviceProtocol);
  USBH_UsrLog("  bMaxPacketSize:     %d",   devdesc->bMaxPacketSize);
  USBH_UsrLog("  idVendor:           0x%04x", devdesc->idVendor);
  USBH_UsrLog("  idProduct:          0x%04x", devdesc->idProduct);
  USBH_UsrLog("  bcdDevice:          0x%04x", devdesc->bcdDevice);
  USBH_UsrLog("  iManufacturer:      %d",   devdesc->iManufacturer);
  USBH_UsrLog("  iProduct:           %d",   devdesc->iProduct);
  USBH_UsrLog("  iSerial:            %d",   devdesc->iSerialNumber);
  USBH_UsrLog("  bNumConfigurations: %d",   devdesc->bNumConfigurations);

  USBH_UsrLog("  Configuration Descriptor:");
  USBH_UsrLog("    bLength:          %d",   cfgdesc->bLength);
  USBH_UsrLog("    bDescriptorType:  %d",     cfgdesc->bDescriptorType);
  USBH_UsrLog("    wTotalLength:     %d",     cfgdesc->wTotalLength);
  USBH_UsrLog("    bNumInterfaces:   %d",     cfgdesc->bNumInterfaces);
  USBH_UsrLog("    bConfiguration:   %d",   cfgdesc->bConfigurationValue);
  USBH_UsrLog("    iConfiguration:   %d",     cfgdesc->iConfiguration);
  USBH_UsrLog("    bmAttributes:     0x%04x", cfgdesc->bmAttributes);
  USBH_UsrLog("    bMaxPower:        %d",     cfgdesc->bMaxPower);

for (i = 0; i < bNumInterfaces; i++) {
  itfdesc = &cfgdesc->Itf_Desc[i];

  USBH_UsrLog("    Interface Descriptor:");
  USBH_UsrLog("      bLength:                %d",   itfdesc->bLength);
  USBH_UsrLog("      bDescriptionType:       %d",   itfdesc->bDescriptorType);
  USBH_UsrLog("      bInterfaceNumber:       %d",   itfdesc->bInterfaceNumber);
  USBH_UsrLog("      bAlternateSetting:      %d",   itfdesc->bAlternateSetting);
  USBH_UsrLog("      bNumEndpoints:          %d",   itfdesc->bNumEndpoints);
  USBH_UsrLog("      bInterfaceClass:        %d",   itfdesc->bInterfaceClass);
  USBH_UsrLog("      bInterfaceSubClass:     %d",   itfdesc->bInterfaceSubClass);
  USBH_UsrLog("      bInterfaceProtocol:     %d",   itfdesc->bInterfaceProtocol);
  USBH_UsrLog("      iInterface:             %d",   itfdesc->iInterface);
}


}

/*
 * state mapping
 * PORT IDLE -> HOST_IDLE
 * PORT_WAIT_PORT_UP -> HOST_WAIT_FOR_ATTACHMENT
 * PORT_UP -> All other states
 * PORT_DOWN -> DISCONNECTED
 */
PORT_StateTypeDef mapped_port_state(USBH_HandleTypeDef *phost)
{
  if (phost->gState == HOST_IDLE)
    return PORT_IDLE;
  else if (phost->gState == HOST_DEV_WAIT_FOR_ATTACHMENT || phost->gState == HOST_DEV_ATTACHED)
    return PORT_WAIT_PORT_UP;
  else if (phost->gState == HOST_DEV_DISCONNECTED)
    return PORT_DOWN;
  else
  {
    return PORT_UP;
  }
}

void HAL_HCD_URB_Monitor(void)
{
  int idx;
  USBH_HandleTypeDef* phost;
  HCD_HandleTypeDef* hhcd;

  phost = &hUsbHostHS;
  hhcd = phost->pData;

  if (hhcd == NULL)
    return;

  for (idx = 0 ; idx < 11 ; idx++)
  {
    if ((phost->Pipes[idx] & 0x8000) == 0)
    {
      continue;
    }

    if (hhcd->hc[idx].urb_requested == 1 &&
        (HAL_GetTick() - hhcd->hc[idx].urb_timer) > 500) {
      printf("+++++ URB Time out, channel: %d, state: %s, urb_state: %s +++++\r\n",
          idx,
          channel_state_string[hhcd->hc[idx].state],
          urb_state_string[hhcd->hc[idx].urb_state]);

      hhcd->hc[idx].urb_requested = 0;
    }
  }
}

/*
 * @param force, forcefully print, otherwise successive same event print only once
 */
void USBH_DebugOutput(USBH_HandleTypeDef* phost, USBH_EventTypeDef event, int force)
{
  static PORT_StateTypeDef ps = -1;
  static HOST_StateTypeDef gs = -1;
  static ENUM_StateTypeDef es = -1;
  static CMD_StateTypeDef rs = -1;
  static CTRL_StateTypeDef cs = -1;
  static USBH_EventTypeDef e = {-1, 0};
  static char time_strbuf[16];

  if (!force) {
    /** print only once for successive state/event **/
    if ( /** (phost->pState == ps) && **/
        (phost->gState == gs) &&
        (phost->EnumState == es) &&
        (phost->RequestState == rs) &&
        (phost->Control.state == cs) &&
        (e.evt == event.evt))
    {
      return;
    }
  }

  // ps = phost->pState;
  ps = mapped_port_state(phost);
  gs = phost->gState;
  es = phost->EnumState;
  rs = phost->RequestState;
  cs = phost->Control.state;
  e.evt = event.evt;
  e.timestamp = event.timestamp;

  if (ps < SIZE_OF_ARRAY(pstate_string) &&
      gs < SIZE_OF_ARRAY(gstate_string) &&
      es < SIZE_OF_ARRAY(enum_state_string) &&
      rs < SIZE_OF_ARRAY(request_state_string) &&
      cs < SIZE_OF_ARRAY(control_state_string) &&
      e.evt < SIZE_OF_ARRAY(event_string))
  {
    if (e.evt == USBH_EVT_NULL) {
      time_strbuf[0] = '\0';
    }
    else {
      snprintf(time_strbuf, 16, "@ %08u", (unsigned int)e.timestamp);
    }

    USBH_UsrLog("- %s, %s, %s, %s, %s, %s %s",
        pstate_string[ps],
        gstate_string[gs],
        enum_state_string[es],
        request_state_string[rs],
        control_state_string[cs],
        event_string[e.evt],
        time_strbuf);
  }
  else
  {
    USBH_UsrLog("!!!! Illegal USBH States, s %d, es %d, rs %d, cs %d, e %d @ %010u",
        gs, es, rs, cs, e.evt, (unsigned int )e.timestamp);
  }
}


void hcint2string(char* buf, uint32_t hcint)
{
  int i ;

  static const char* name[] =
  { "XFRC", "CHH", "AHBERR", "STALL", "NAK", "ACK", "NYET", "TXERR", "BBERR",
      "FRMOR", "DTERR" };

  for (i = 0; i < 11; i++) {

    if (hcint & (1 << i)) {
      strcpy(buf, name[i]);
      buf += strlen(name[i]);
      *buf = ' ';
      buf ++;
    }
  }

  /* remove last space */
  if (strlen(buf) > 0) {
    buf--;
    *buf = 0;
  }
}

/*
 * a ring buffer for asynchronous USBH event
 *
 * TODO these data should be changed to struct, since each USB (HS, FS) need an instance
 */

static USBH_EventTypeDef USBH_Events[USBH_EVENT_RING_SIZE] = { {0, 0, {0}} };
static int get_event_index = 0;
static int put_event_index = 0;

static inline int next_event_index(int i) {

  if (i == USBH_EVENT_RING_SIZE - 1)
    return 0;

  return ++i;
}

static USBH_EventTypeDef USBH_GetEvent(void) {

  USBH_EventTypeDef e;

  if (get_event_index == put_event_index) {
    e.evt = USBH_EVT_NULL;
    e.timestamp = 0;
    return e;
  }

  e = USBH_Events[get_event_index];
  USBH_Events[get_event_index].evt = 0;
  USBH_Events[get_event_index].timestamp = 0;

  get_event_index = next_event_index(get_event_index);

  return e;
}

USBH_EventTypeDef* USBH_AllocEvent(void)
{
  if (next_event_index(put_event_index) == get_event_index)
    return NULL ;

  return &USBH_Events[put_event_index];
}

void USBH_SendEvent(USBH_EventTypeDef* ep)
{
  if (ep != &USBH_Events[put_event_index])
    return;

  ep->timestamp = HAL_GetTick();
  put_event_index = next_event_index(put_event_index);
}

void USBH_SendSimpleEvent(USBH_EventTypeTypeDef type)
{
  USBH_EventTypeDef* ep = USBH_AllocEvent();
  if (ep)
  {
    ep->evt = type;
    USBH_SendEvent(ep);
  }
}

void USBH_PutEvent(USBH_EventTypeDef e)
{
  int i;
  if (next_event_index(put_event_index) == get_event_index)
  {

    for (i = 0; i < USBH_EVENT_RING_SIZE; i++)
      USBH_Events[i].evt = USBH_EVT_OVERFLOW;

    return;
  }

  USBH_Events[put_event_index] = e;
  put_event_index = next_event_index(put_event_index);
}

/** this can be safely called in interrupt context **/
void USBH_PutMessage(const char* buf)
{
  USBH_Events[put_event_index].evt = USBH_EVT_MESSAGE;
  USBH_Events[put_event_index].timestamp = HAL_GetTick();
  strncpy(USBH_Events[put_event_index].data.message.buf, buf, 64);
  USBH_Events[put_event_index].data.message.buf[63] = '\0';

  put_event_index = next_event_index(put_event_index);
}

USBH_EventTypeDef USBH_GetFilteredEvent(USBH_HandleTypeDef * phost)
{
  static USBH_EventTypeDef e;
  static char buf[128];

pop:
  e = USBH_GetEvent();

  if (e.evt == USBH_EVT_HCINT) {

    memset(buf, 0, 128);

    hcint2string(buf, e.data.hcint.hcint_reg);

    printf(" :: HCINT %08x, %02x, %04x, %s, %s, %s %s, %s %s, %d %d, - %08u" NEW_LINE,
        e.data.hcint.uid,
        (unsigned int)e.data.hcint.interrupt,
        (unsigned int)e.data.hcint.hcint_reg,
        buf,
        ((e.data.hcint.direction == 0) ? "OUT" : "IN"),
        channel_state_string[e.data.hcint.in_state],
        channel_state_string[e.data.hcint.out_state],
        urb_state_string[e.data.hcint.in_urbstate],
        urb_state_string[e.data.hcint.out_urbstate],
        e.data.hcint.in_err_count,
        e.data.hcint.out_err_count,
        (unsigned int)e.timestamp);

    goto pop;
  }
  else if (e.evt == USBH_EVT_MESSAGE) {

    e.data.message.buf[63] = '\0';
    USBH_UsrLog("++++++++++++++++ %s", e.data.message.buf);

    goto pop;
  }

  USBH_DebugOutput(phost, e, 0);

  return e;
}
