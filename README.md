# STM32_I2C_Slave_Example

This repo contains a simple library and an example to use a STM as I2C slave. It allows you to define a register map with auto-incremental read and write requests.


### Usage

Before you include and set up the library as below, the I2S peripheral has to be configured (e.g. via CubeMX). This is not covered here.

```
#include "i2c_slave.h"
#include "i2c_registermap.h"

i2c_slave_init_t i2c_init = {.hi2c = &hi2c1, .error_handler = Error_Handler};
```

`hi2c1` and `Error_Handler` may be replaced with the interface and error function of your choice.

The interface can be started via

```
I2C_Slave_Init(&i2c_init);
```

### Todo

 - [ ] Improve register mapping
