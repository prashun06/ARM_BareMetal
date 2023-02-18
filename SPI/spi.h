
#include <stdio.h>
#include <stdint.h>
#include "stm32f446xx.h"

#define SPI1EN  (1 << 12)
#define GPIOAEN  (1<<0)
#define GPIOBEN  (1<<1)

#define SR_TXE  (1<<1)


