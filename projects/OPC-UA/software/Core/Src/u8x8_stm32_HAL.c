/*
  ******************************************************************************
  * File Name          : u8x8_stm32_HAL.c
  * Description        : Main program body
  ******************************************************************************
  * µC-spezifische Anweisungen zw. u8g2/u8x8 und stm32
  * Hal-Driver im Einsatz
  ******************************************************************************
*/

#include "u8g2.h"
#include "stm32f7xx_hal.h"
#include "u8x8_stm32_HAL.h"

uint8_t u8x8_stm32_gpio_and_delay_cb(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
  switch(msg)							// there is no need for any delay
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:	// called once during init phase of u8g2/u8x8 -> Make with HAL_init
      //__NOP();
      break;							// can be used to setup pins
    case U8X8_MSG_DELAY_NANO:			// delay arg_int * 1 nano second
      //__NOP();
      break;    
    case U8X8_MSG_DELAY_100NANO:		// delay arg_int * 100 nano seconds
      //__NOP();
      //__NOP();
      //__NOP();
      break;
    case U8X8_MSG_DELAY_10MICRO:		// delay arg_int * 10 micro seconds
     // for (uint16_t n = 0; n < (320*arg_int); n++)
    	//	{
    	//	 __NOP();
    	//	}
     break;
    case U8X8_MSG_DELAY_MILLI:			// delay arg_int * 1 milli second
    //  HAL_Delay(arg_int);


     break;
    case U8X8_MSG_DELAY_I2C:			// arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
    	//for (uint16_t n = 0; n < (160*arg_int); n++)
    	  //  		{
    	    //		 __NOP();
    	    	//	}
      break;							// arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
    case U8X8_MSG_GPIO_D0:				// D0 or SPI clock pin: Output level in arg_int
    //case U8X8_MSG_GPIO_SPI_CLOCK:
      break;
    case U8X8_MSG_GPIO_D1:				// D1 or SPI data pin: Output level in arg_int
    //case U8X8_MSG_GPIO_SPI_DATA:
      break;
    case U8X8_MSG_GPIO_D2:				// D2 pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_D3:				// D3 pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_D4:				// D4 pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_D5:				// D5 pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_D6:				// D6 pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_D7:				// D7 pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_E:				// E/WR pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_CS:				// CS (chip select) pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_DC:				// DC (data/cmd, A0, register select) pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_RESET:			// Reset pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_CS1:				// CS1 (chip select) pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_CS2:				// CS2 (chip select) pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_I2C_CLOCK:		// arg_int=0: Output low at I2C clock pin
      break;							// arg_int=1: Input dir with pullup high for I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA:			// arg_int=0: Output low at I2C data pin
      break;							// arg_int=1: Input dir with pullup high for I2C data pin

  }
  return 1;
}
 //   case U8X8_MSG_GPIO_MENU_SELECT:
 //     u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
 //     break;
 //   case U8X8_MSG_GPIO_MENU_NEXT:
 //     u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
 //     break;
 //   case U8X8_MSG_GPIO_MENU_PREV:
 //     u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
 //     break;
 //   case U8X8_MSG_GPIO_MENU_HOME:
 //     u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
 //     break;
 //   default:
 //     u8x8_SetGPIOResult(u8x8, 1);	*/		// default return value


uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t *ptr;
  static uint8_t buffer_count;
  static uint8_t buffer[DATA_BUFFER_SIZE+1];	//the size of buffer depends on how many pages are transfered at once
  	  	  	  	  	  	  	  	  	  	  	  	//e.g. one page are 128byte and one byte more for command

  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:					//collect
      {   ptr = arg_ptr;
    	  for (int i=1; i <= arg_int; i++)
    	  {	  buffer[buffer_count] = *(ptr++);
    		  buffer_count++;
    	  }
      }
      break;
    case U8X8_MSG_BYTE_INIT:
      break;
    case U8X8_MSG_BYTE_SET_DC:
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
    	buffer_count = 0;						// start
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:			// send all at once
    	 HAL_I2C_Master_Transmit(&I2C_HANDLER, DEVICE_ADDRESS, (uint8_t *)buffer, buffer_count, I2C_TIMEOUT);
      break;
    default:
      return 0;
  }
  return 1;
}

