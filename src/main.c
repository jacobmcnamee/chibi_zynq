/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"

#include <stdio.h>

int _write(int file, char *ptr, int len) __attribute__((used));

int _write(int file, char *ptr, int len)
{
  (void)file;

  int i = 0;
  while(i < len) {
    /* Wait for UART1 FIFO not full */
    while(*(volatile unsigned int *)(0xE0001000 + 0x002C) & 0x00000010U);
    /* Write to UART1 TX FIFO */
    *(volatile unsigned int *)(0xE0001000 + 0x0030) = ptr[i++];
  }
  return 0;
}

/*
 * This is a periodic thread that does absolutely nothing except sleeping.
 */
static THD_WORKING_AREA(waThread1, 2048);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;

  chRegSetThreadName("sleeper");

  while (true) {
    printf("Hello sleeper\r\n");
    chThdSleepMilliseconds(1000);
  }
}

/*
 * Application entry point.
 */
int main(void) {

  printf("Hello ChibiOS\r\n");

  /*
   * System initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Creates the example thread.
   */
  (void) chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it just sleeps.
   */
  while (true) {
    printf("Hello main\r\n");
    chThdSleepMilliseconds(1000);
  }
}
