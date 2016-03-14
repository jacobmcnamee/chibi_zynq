/*
 * mio.h
 *
 *  Created on: Mar 14, 2016
 *      Author: jacob
 */

#ifndef SRC_MIO_H_
#define SRC_MIO_H_

#include <stdint.h>
#include <stdbool.h>

void mio_configure(uint8_t mio, uint8_t function, bool tristate,
                   bool pullup, bool fast);

#endif /* SRC_MIO_H_ */
