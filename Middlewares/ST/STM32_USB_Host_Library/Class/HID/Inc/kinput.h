#ifndef __KERNEL_INPUT_H
#define __KERNEL_INPUT_H

#include "non-atomic.h"
#include "uinput.h"

struct hid_device;

/*
 * This is extracted from input.h (kernel), the original one is huge.
 */
struct input_dev
{

  unsigned long evbit[BITS_TO_LONGS(EV_CNT)];
  unsigned long keybit[BITS_TO_LONGS(KEY_CNT)];
  unsigned long relbit[BITS_TO_LONGS(REL_CNT)];
  unsigned long absbit[BITS_TO_LONGS(ABS_CNT)];
  unsigned long mscbit[BITS_TO_LONGS(MSC_CNT)];
  unsigned long ledbit[BITS_TO_LONGS(LED_CNT)];
  unsigned long sndbit[BITS_TO_LONGS(SND_CNT)];
  unsigned long ffbit[BITS_TO_LONGS(FF_CNT)];
  unsigned long swbit[BITS_TO_LONGS(SW_CNT)];

  unsigned int hint_events_per_packet;

  struct input_absinfo *absinfo;

  struct hid_device *hiddev;
};


/**
 * input_set_events_per_packet - tell handlers about the driver event rate
 * @dev: the input device used by the driver
 * @n_events: the average number of events between calls to input_sync()
 *
 * If the event rate sent from a device is unusually large, use this
 * function to set the expected event rate. This will allow handlers
 * to set up an appropriate buffer size for the event stream, in order
 * to minimize information loss.
 */
static inline void input_set_events_per_packet(struct input_dev *dev, int n_events)
{
        dev->hint_events_per_packet = n_events;
}

void input_alloc_absinfo(struct input_dev *dev);
void input_set_abs_params(struct input_dev *dev, unsigned int axis, int min,
    int max, int fuzz, int flat);

#define INPUT_GENERATE_ABS_ACCESSORS(_suffix, _item)                    \
static inline int input_abs_get_##_suffix(struct input_dev *dev,        \
                                          unsigned int axis)            \
{                                                                       \
        return dev->absinfo ? dev->absinfo[axis]._item : 0;             \
}                                                                       \
                                                                        \
static inline void input_abs_set_##_suffix(struct input_dev *dev,       \
                                           unsigned int axis, int val)  \
{                                                                       \
        input_alloc_absinfo(dev);                                       \
        if (dev->absinfo)                                               \
                dev->absinfo[axis]._item = val;                         \
}

INPUT_GENERATE_ABS_ACCESSORS(val, value)
INPUT_GENERATE_ABS_ACCESSORS(min, minimum)
INPUT_GENERATE_ABS_ACCESSORS(max, maximum)
INPUT_GENERATE_ABS_ACCESSORS(fuzz, fuzz)
INPUT_GENERATE_ABS_ACCESSORS(flat, flat)
INPUT_GENERATE_ABS_ACCESSORS(res, resolution)

#endif

