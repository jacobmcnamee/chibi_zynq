/*
 * mio.c
 *
 *  Created on: Mar 14, 2016
 *      Author: jacob
 */

#include "mio.h"

#include "slcr_mio.h"

void mio_configure(uint8_t mio, uint8_t function, bool tristate,
                   bool pullup, bool fast)
{
  uint32_t clear_mask = (SLCR_MIO_PIN_PULLUP_Msk |
                         SLCR_MIO_PIN_SPEED_Msk |
                         SLCR_MIO_PIN_L3_SEL_Msk |
                         SLCR_MIO_PIN_L2_SEL_Msk |
                         SLCR_MIO_PIN_L1_SEL_Msk |
                         SLCR_MIO_PIN_L0_SEL_Msk |
                         SLCR_MIO_PIN_TRI_ENABLE_Msk);

  uint32_t set_mask = (function << SLCR_MIO_PIN_L0_SEL_Pos);
  if (tristate)
    set_mask |= SLCR_MIO_PIN_TRI_ENABLE_Msk;
  if (pullup)
    set_mask |= SLCR_MIO_PIN_PULLUP_Msk;
  if (fast)
    set_mask |= SLCR_MIO_PIN_SPEED_Msk;

  uint32_t mio_pin_reg = SLCR_MIO->MIO_PIN[mio];
  mio_pin_reg &= ~clear_mask;
  mio_pin_reg |= set_mask;
  SLCR_MIO->MIO_PIN[mio] = mio_pin_reg;
}
