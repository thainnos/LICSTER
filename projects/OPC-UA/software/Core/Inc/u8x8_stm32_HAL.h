#ifndef _U8X8_STM32_HAL_H
#define _U8X8_STM32_HAL_H
   
   
 #include "u8g2.h"
 #include "stm32f7xx_hal.h"
   
 #define DATA_BUFFER_SIZE 128 	//the size of buffer depends on how many pages are transfered at once e.g. one page are 128byte
 #define I2C_TIMEOUT 1000
 #define DEVICE_ADDRESS 0x78 	//device address is added
 #define I2C_HANDLER hi2c1
   
 extern I2C_HandleTypeDef hi2c1; // use your i2c handler
   
   
 uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
 uint8_t u8x8_stm32_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
   
 #endif  
