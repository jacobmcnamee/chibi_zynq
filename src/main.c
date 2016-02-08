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

#define LED_GPIO_LINE PAL_LINE(GPIO1, 15)
#define BUTTON_GPIO_LINE PAL_LINE(GPIO1, 19)

#define SPI_MOSI_GPIO_LINE PAL_LINE(GPIO0, 10)
#define SPI_MISO_GPIO_LINE PAL_LINE(GPIO0, 11)
#define SPI_CLK_GPIO_LINE PAL_LINE(GPIO0, 12)
#define SPI_SS_GPIO_LINE PAL_LINE(GPIO0, 13)

const PALConfig pal_default_config;

const WDGConfig wdg_config = {3000};

const SPIConfig spi_config = {0, SPI_MODE_0,
                               SPI_CLK_DIV_4, SPI_SS_GPIO_LINE};

void gpt_callback(GPTDriver *gptp)
{
  (void)gptp;
  palToggleLine(LED_GPIO_LINE);
}

const GPTConfig gpt_config = {CPU_1x_FREQUENCY_Hz / (1<<16), gpt_callback};

void button_ext_callback(EXTDriver *extp, expchannel_t channel)
{
  (void)extp;
  (void)channel;
  palToggleLine(LED_GPIO_LINE);
}

const EXTConfig ext_config =
{
  {
    {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART,
      button_ext_callback, PAL_PORT(BUTTON_GPIO_LINE), PAL_PAD(BUTTON_GPIO_LINE)}
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

  wdgStart(&WDGD1, &wdg_config);

  palSetLineMode(BUTTON_GPIO_LINE, PAL_MODE_INPUT);

  palSetLineMode(LED_GPIO_LINE, PAL_MODE_OUTPUT_PUSHPULL);
  palWriteLine(LED_GPIO_LINE, PAL_HIGH);

  extStart(&EXTD1, &ext_config);

  sdStart(&SD2, 0);

  /* Enable SPI0 and SPI1 clocks */
  *(volatile uint32_t *)0xF800012C |= (1<<14) | (1 << 15);

  /* SPI REFCLK = 1GHz / 20 = 50MHz */
  *(volatile uint32_t *)0xF8000158 &= ~(0x3F << 8);
  *(volatile uint32_t *)0xF8000158 |= (20 << 8);

  palSetLineMode(SPI_MOSI_GPIO_LINE, PAL_MODE_CUSTOM(PAL_DIR_PERICTRL, PAL_PULL_NONE, PAL_SPEED_SLOW, PAL_PIN_FUNCTION(5 << 4)));
  palSetLineMode(SPI_MISO_GPIO_LINE, PAL_MODE_CUSTOM(PAL_DIR_INPUT, PAL_PULL_NONE, PAL_SPEED_SLOW, PAL_PIN_FUNCTION(5 << 4)));
  palSetLineMode(SPI_CLK_GPIO_LINE, PAL_MODE_CUSTOM(PAL_DIR_PERICTRL, PAL_PULL_NONE, PAL_SPEED_SLOW, PAL_PIN_FUNCTION(5 << 4)));
  palSetLineMode(SPI_SS_GPIO_LINE, PAL_MODE_OUTPUT_PUSHPULL);

  spiStart(&SPID2, &spi_config);

  gptStart(&GPTD1, &gpt_config);
  gptStartContinuous(&GPTD1, 1000);


  (void) chThdCreateStatic(wa_rx_thread, sizeof(wa_rx_thread),
                           HIGHPRIO, rx_thread, NULL);
  (void) chThdCreateStatic(wa_reg_test_thread, sizeof(wa_reg_test_thread),
                           NORMALPRIO, reg_test_thread, NULL);



  while (true) {
    printf("Hello main\r\n");

    static uint8_t txbuf[200];
    static uint8_t rxbuf[200];

    spiAcquireBus(&SPID2);
    spiSelect(&SPID2);

    for (uint32_t i=0; i<sizeof(txbuf); i++) {
      txbuf[i] = i;
      rxbuf[i] = 0x55;
    }

    spiExchange(&SPID2, sizeof(txbuf), txbuf, rxbuf);
    for (uint32_t i=0; i<sizeof(txbuf); i++) {
      if (txbuf[i] != rxbuf[i]) {
        printf("mismatch: %02x %02x\r\n", txbuf[i], rxbuf[i]);
      }
    }

    printf("rx: %02x %02x %02x %02x\r\n", rxbuf[0], rxbuf[1], rxbuf[2], rxbuf[3]);

    spiUnselect(&SPID2);
    spiReleaseBus(&SPID2);

    wdgReset(&WDGD1);
    chThdSleepMilliseconds(1000);
  }
}
