/*
 * slcr_mio.h
 *
 *  Created on: Mar 14, 2016
 *      Author: jacob
 */

#ifndef SRC_SLCR_MIO_H_
#define SRC_SLCR_MIO_H_

#include <stdint.h>

/* Registers */
typedef struct {
  volatile uint32_t MIO_PIN[54];
  volatile uint32_t RESERVED0[11];
  volatile uint32_t MIO_LOOPBACK;
  volatile uint32_t MIO_MST_TRI[2];
  volatile uint32_t SD_WP_CD_SEL[2];
} slcr_mio_t;

/* Instances */
#define SLCR_MIO_BASE 0xF8000700U
#define SLCR_MIO ((slcr_mio_t *)SLCR_MIO_BASE)

/* Bitfields */
#define SLCR_MIO_PIN_TRI_ENABLE_Pos (0U)
#define SLCR_MIO_PIN_TRI_ENABLE_Msk (0x1U << SLCR_MIO_PIN_TRI_ENABLE_Pos)

#define SLCR_MIO_PIN_L0_SEL_Pos (1U)
#define SLCR_MIO_PIN_L0_SEL_Msk (0x1U << SLCR_MIO_PIN_L0_SEL_Pos)

#define SLCR_MIO_PIN_L1_SEL_Pos (2U)
#define SLCR_MIO_PIN_L1_SEL_Msk (0x1U << SLCR_MIO_PIN_L1_SEL_Pos)

#define SLCR_MIO_PIN_L2_SEL_Pos (3U)
#define SLCR_MIO_PIN_L2_SEL_Msk (0x3U << SLCR_MIO_PIN_L2_SEL_Pos)

#define SLCR_MIO_PIN_L3_SEL_Pos (5U)
#define SLCR_MIO_PIN_L3_SEL_Msk (0x7U << SLCR_MIO_PIN_L3_SEL_Pos)

#define SLCR_MIO_PIN_SPEED_Pos (8U)
#define SLCR_MIO_PIN_SPEED_Msk (0x1U << SLCR_MIO_PIN_SPEED_Pos)

#define SLCR_MIO_PIN_IO_TYPE_Pos (9U)
#define SLCR_MIO_PIN_IO_TYPE_Msk (0x7U << SLCR_MIO_PIN_IO_TYPE_Pos)

#define SLCR_MIO_PIN_PULLUP_Pos (12U)
#define SLCR_MIO_PIN_PULLUP_Msk (0x1U << SLCR_MIO_PIN_PULLUP_Pos)

#endif /* SRC_SLCR_MIO_H_ */
