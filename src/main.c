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

#include "zynq7000.h"

const ioline_t led_gpio_line = PAL_LINE(GPIO1, 15);
const ioline_t button_gpio_line = PAL_LINE(GPIO1, 19);

const PALConfig pal_default_config;

void button_ext_callback(EXTDriver *extp, expchannel_t channel)
{
  (void)extp;
  (void)channel;
  palToggleLine(led_gpio_line);
}

const EXTConfig ext_config =
{
  {
    {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART,
      button_ext_callback, GPIO1, 19}
  }
};

int _write(int file, char *ptr, int len) __attribute__((used));
int _write(int file, char *ptr, int len)
{
  (void)file;

  sdWrite(&SD2, (uint8_t *)ptr, len);
  return len;
}

static THD_WORKING_AREA(wa_reg_test_thread, 2048);
extern THD_FUNCTION(reg_test_thread, arg);

static THD_WORKING_AREA(wa_rx_thread, 2048);
static THD_FUNCTION(rx_thread, arg) {

  (void)arg;

  chRegSetThreadName("RX");

  while (true) {

    uint32_t count = 0;
    while(1) {
      msg_t b = sdGetTimeout(&SD2, TIME_IMMEDIATE);
      if (b >= Q_OK) {
        //printf("rx: %u\r\n", b);
        printf("%c", (char)(b & 0xff));
        count++;
      } else {
        if (count > 0) {
          printf("\r\n");
        }
        break;
      }
    }

    chThdSleep(MS2ST(10));
  }
}

/*
 * Application entry point.
 */
int main(void) {

  setvbuf(stdout, NULL, _IONBF, 0);
  printf("Hello ChibiOS\r\n");

  /*
   * System initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  palSetLineMode(button_gpio_line, PAL_MODE_INPUT);

  palSetLineMode(led_gpio_line, PAL_MODE_OUTPUT_PUSHPULL);
  palWriteLine(led_gpio_line, PAL_HIGH);

  extStart(&EXTD1, &ext_config);

  sdStart(&SD2, 0);


  (void) chThdCreateStatic(wa_rx_thread, sizeof(wa_rx_thread),
                           HIGHPRIO, rx_thread, NULL);
  (void) chThdCreateStatic(wa_reg_test_thread, sizeof(wa_reg_test_thread),
                           NORMALPRIO, reg_test_thread, NULL);



  while (true) {
    printf("Hello main\r\n");

    chThdSleepMilliseconds(1000);
  }
}
