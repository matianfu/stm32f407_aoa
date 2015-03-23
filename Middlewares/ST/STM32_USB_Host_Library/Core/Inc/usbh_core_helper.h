#ifndef USBH_CORE_HELPER_H
#define USBH_CORE_HELPER_H

#include "usbh_core.h"

#define SIZE_OF_ARRAY(array)                    (sizeof(array) / sizeof(array[0]))
#define USBH_ERRORSTATE(s, e)                   printf(NEW_LINE "!!! ERROR STATE: %d" NEW_LINE, __LINE__);  \
                                                USBH_DebugOutput(s, e, 1);                                  \
                                                printf(NEW_LINE NEW_LINE);

USBH_EventTypeDef USBH_GetFilteredEvent(USBH_HandleTypeDef * phost);
void USBH_DebugOutput(USBH_HandleTypeDef* phost, USBH_EventTypeDef event, int force);

USBH_EventTypeDef* USBH_AllocEvent(void);
void USBH_SendEvent(USBH_EventTypeDef* ep);
void USBH_SendSimpleEvent(USBH_EventTypeTypeDef type);

#endif /* USBH_CORE_HELPER_H */
