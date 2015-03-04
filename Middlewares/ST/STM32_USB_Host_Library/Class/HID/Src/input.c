#include <stdlib.h>
#include "kinput.h"

/**
 * input_alloc_absinfo - allocates array of input_absinfo structs
 * @dev: the input device emitting absolute events
 *
 * If the absinfo struct the caller asked for is already allocated, this
 * functions will not do anything.
 */
void input_alloc_absinfo(struct input_dev *dev)
{
  if (!dev->absinfo)
  {
    // dev->absinfo = kcalloc(ABS_CNT, sizeof(struct input_absinfo), GFP_KERNEL);
    dev->absinfo = malloc(ABS_CNT * sizeof(struct input_absinfo));
  }

  // WARN(!dev->absinfo, "%s(): kcalloc() failed?\n", __func__);
}
// EXPORT_SYMBOL( input_alloc_absinfo);

void input_set_abs_params(struct input_dev *dev, unsigned int axis, int min,
    int max, int fuzz, int flat)
{
  struct input_absinfo *absinfo;

  input_alloc_absinfo(dev);
  if (!dev->absinfo)
    return;

  absinfo = &dev->absinfo[axis];
  absinfo->minimum = min;
  absinfo->maximum = max;
  absinfo->fuzz = fuzz;
  absinfo->flat = flat;

  dev->absbit[BIT_WORD(axis)] |= BIT_MASK(axis);
}
// EXPORT_SYMBOL( input_set_abs_params);
