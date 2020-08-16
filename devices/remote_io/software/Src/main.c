/* USER CODE BEGIN Header */
/**
 * *****************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * This notice applies to any and all portions of this file
 * that are not between comment pairs USER CODE BEGIN and
 * USER CODE END. Other portions of this file, whether
 * inserted by the user or by software development tools
 * are owned by their respective copyright owners.
 *
 * Copyright (c) 2019 STMicroelectronics International N.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other
 *    contributors to this software may be used to endorse or promote products
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under
 *    this license is void and will automatically terminate your rights under
 *    this license.
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A~
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "mbedtls.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lwip.h"
#include "lwip/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/ip_addr.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"

#include "netif/ethernet.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>

#include "colors.h"
#include "logging.h"

#include "ping.h"

#include "u8g2.h"
#include "u8x8_stm32_HAL.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PING_USE_SOCKETS 1
#define LWIP_RAW 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c1;

RNG_HandleTypeDef hrng;

UART_HandleTypeDef huart3;

osThreadId defaultTaskHandle;
osThreadId httpTaskHandle;
osThreadId ledTaskHandle;
osThreadId loggingTaskHandle;
osThreadId mbTaskHandle;
osThreadId displayTaskHandle;
osThreadId ioTaskHandle;
/* USER CODE BEGIN PV */

SemaphoreHandle_t alloc_mutex;
SemaphoreHandle_t net_mutex;
QueueHandle_t loggingQueue;

int errno = 0;
unsigned int deviceID = 0;

/* Variables needed to change PHY config */
extern char lwip_initialized;
struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
uint8_t IP_ADDRESS[4];
uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];

/* Modbus Variables */
char discrete_input_buf;
char discrete_output_buf;

/* Global Variable*/
uint8_t setLED;

/* Display */
static u8g2_t u8g2;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RNG_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_I2C1_Init(void);
void StartDefaultTask(void const * argument);
void httpFunc(void const * argument);
void ledFunc(void const * argument);
void loggingFunc(void const * argument);
void mbFunc(void const * argument);
void displayFunc(void const * argument);
void ioFunc(void const * argument);

/* USER CODE BEGIN PFP */
extern void httpd_task(void* );
extern void modbus_cmd_handler_task(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* Function which responds for drawing  */
void u8g2_prepare()
{
    u8g2_SetFont(&u8g2, u8g2_font_profont10_tf);
    u8g2_SetFontRefHeightExtendedText(&u8g2);
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_SetFontPosTop(&u8g2);
    u8g2_SetFontDirection(&u8g2, 0);
}


void u8g2_disc_circle(uint8_t shift){
    char message[64];
    u8g2_SetFont(&u8g2, u8g2_font_profont15_tf);
    u8g2_DrawStr(&u8g2, 0, 0, "MODBUS/TCP Device");
    u8g2_SetFont(&u8g2, u8g2_font_profont10_tf);
    snprintf(message, sizeof(message), "IP:     %03d.%03d.%03d.%03d",
	    IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
    u8g2_DrawStr(&u8g2, 10, 15, message);
    snprintf(message, sizeof(message), "Inputs: %1d %1d %1d %1d",
	    discrete_input_buf>>0 & 1,
	    discrete_input_buf>>1 & 1,
	    discrete_input_buf>>2 & 1,
	    discrete_input_buf>>3 & 1);
    u8g2_DrawStr(&u8g2, 10, 25, message);
    snprintf(message, sizeof(message), "COILs:  %1d %1d %1d %1d",
	    discrete_output_buf>>0 & 1,
	    discrete_output_buf>>1 & 1,
	    discrete_output_buf>>2 & 1,
	    discrete_output_buf>>3 & 1);
    u8g2_DrawStr(&u8g2, 10, 35, message);
    u8g2_DrawCircle(&u8g2,0+shift,20+32,2,U8G2_DRAW_ALL);
    u8g2_DrawCircle(&u8g2,5+shift,20+32,2,U8G2_DRAW_ALL);
    u8g2_DrawCircle(&u8g2,10+shift,20+32,2,U8G2_DRAW_ALL);
}


uint8_t max = 150;
uint8_t step = 0;
uint8_t picture =0;

void draw(void)
{
    u8g2_prepare();
    u8g2_disc_circle(step);
}


int _write(int file, char * data, int len) {
    if ((file != STDOUT_FILENO) && (file != STDERR_FILENO)) {
        errno = EBADF;
        return -1;
    }

    // arbitrary timeout 1
    HAL_StatusTypeDef status = HAL_OK;
    status = HAL_UART_Transmit( & huart3, (uint8_t * ) data, len, 100);

    // return # of bytes written - as best we can tell
    return (status == HAL_OK ? len : 0);
}


/* changeIP function is like lwip_init */
/**
 * @brief 	This halts code execution for the supplied amount of seconds
 * 			while printing out the passed and remaining amount of wait.
 * @param  	seconds: The amount of seconds to wait.
 * @retval 	None
 */
void verbose_wait(int seconds)
{
	printf("\rWaiting %i seconds...\n", seconds);
	for(int i = 0; i < seconds; i++)
	{
		printf("\r%i/%i", i, seconds);
		fflush(stdout);
		HAL_Delay(1000);
	}
	printf("\rWating done!\n\r");

}

/* changeIP function is like lwip_init */
/**
 * @brief This sets the IP Address of the device
 * @param  None
 * @retval None
 */
void changeIP(void){

	while (xSemaphoreTake(net_mutex, 10) != pdTRUE);
	if(lwip_initialized)
		return;

	lwip_initialized = 1;
    /* IP addresses initialization */
    IP_ADDRESS[0] = 192;
    IP_ADDRESS[1] = 168;
    IP_ADDRESS[2] = 0;
    IP_ADDRESS[3] = 50;
    NETMASK_ADDRESS[0] = 255;
    NETMASK_ADDRESS[1] = 255;
    NETMASK_ADDRESS[2] = 255;
    NETMASK_ADDRESS[3] = 0;
    GATEWAY_ADDRESS[0] = 192;
    GATEWAY_ADDRESS[1] = 168;
    GATEWAY_ADDRESS[2] = 0;
    GATEWAY_ADDRESS[3] = 1;

    IP_ADDRESS[3] += deviceID;

    /* Initilialize the LwIP stack with RTOS */
    tcpip_init( NULL, NULL );

    /* IP addresses initialization without DHCP (IPv4) */
    IP4_ADDR(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
    IP4_ADDR(&netmask, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1] , NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
    IP4_ADDR(&gw, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);

    /* add the network interface (IPv4/IPv6) with RTOS */
    netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

    /* Registers the default network interface */
    netif_set_default(&gnetif);

    if (netif_is_link_up(&gnetif))
    {
	/* When the netif is fully configured this function must be called */
	netif_set_up(&gnetif);
    }
    else
    {
	/* When the netif link is down this function must be called */
	netif_set_down(&gnetif);
    }

    printf("\rIP Address set to: %03d.%03d.%03d.%03d\n",
	    IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);

	xSemaphoreGive(net_mutex);
    return;
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RNG_Init();
  MX_USART3_UART_Init();
  MX_MBEDTLS_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
    printf("\n\n\r+++++++++++++++++++++++++++++++++++++++\n");
#ifdef USE_TLS
    printf("\rBuild: TLS Enabled!\n");
#else
    printf("\rBuild: TLS Disabled!\n");
#endif
    printf("\rBuild: %s %s\n", __DATE__, __TIME__);
    printf("\rLOGLEVEL set to: %i!\n", showLogLevel);
    printf("\r%sInit done!\n", CRST);
    printf("\rWaiting for network switch...\n");
    verbose_wait(30); // necessary for the network switch
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)) {
        printf("\rGPIO_PIN_A05 is high\n");
        deviceID += 1;
    }
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)) {
        printf("\rGPIO_PIN_A06 is high\n");
        deviceID += 2;
    }
    if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_14)) {
        printf("\rGPIO_PIN_D14 is high\n");
        deviceID += 4;
    }
    if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15)) {
        printf("\rGPIO_PIN_D15 is high\n");
        deviceID += 8;
    }
    if (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_12)) {
        printf("\rGPIO_PIN_F12 is high\n");
        deviceID += 16;
    }

    if ((deviceID == 0) || (deviceID == 31)) {
        printf("\rError: No shield connected!\n");
    }
    printf("\rDeviceID: %u\n", (unsigned int) deviceID);
    printf("\rDeviceSE: %u\n", (unsigned int) HAL_GetDEVID);
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    alloc_mutex = xSemaphoreCreateMutex();
    net_mutex = xSemaphoreCreateMutex();
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    loggingQueue = xQueueCreate(6, sizeof(char[LOGLEN]));
    printf("\rStarting Scheduler!\n");
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of httpTask */
  osThreadDef(httpTask, httpFunc, osPriorityBelowNormal, 0, 8192);
  httpTaskHandle = osThreadCreate(osThread(httpTask), NULL);

  /* definition and creation of ledTask */
  osThreadDef(ledTask, ledFunc, osPriorityRealtime, 0, 128);
  ledTaskHandle = osThreadCreate(osThread(ledTask), NULL);

  /* definition and creation of loggingTask */
  osThreadDef(loggingTask, loggingFunc, osPriorityIdle, 0, 2048);
  loggingTaskHandle = osThreadCreate(osThread(loggingTask), NULL);

  /* definition and creation of mbTask */
  osThreadDef(mbTask, mbFunc, osPriorityNormal, 0, 4096);
  mbTaskHandle = osThreadCreate(osThread(mbTask), NULL);

  /* definition and creation of displayTask */
  osThreadDef(displayTask, displayFunc, osPriorityLow, 0, 512);
  displayTaskHandle = osThreadCreate(osThread(displayTask), NULL);

  /* definition and creation of ioTask */
  osThreadDef(ioTask, ioFunc, osPriorityHigh, 0, 128);
  ioTaskHandle = osThreadCreate(osThread(ioTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00200922;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /** I2C Enable Fast Mode Plus
  */
  HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RNG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, device_4_Pin|out4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(out3_GPIO_Port, out3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, USB_PowerSwitchOn_Pin|out1_Pin|out2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : device_0_Pin device_1_Pin */
  GPIO_InitStruct.Pin = device_0_Pin|device_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : device_4_Pin out4_Pin */
  GPIO_InitStruct.Pin = device_4_Pin|out4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : in1_Pin in4_Pin */
  GPIO_InitStruct.Pin = in1_Pin|in4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : in2_Pin in3_Pin */
  GPIO_InitStruct.Pin = in2_Pin|in3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : out3_Pin */
  GPIO_InitStruct.Pin = out3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(out3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : device_2_Pin device_3_Pin */
  GPIO_InitStruct.Pin = device_2_Pin|device_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_PowerSwitchOn_Pin out1_Pin out2_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin|out1_Pin|out2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */

    osDelay(10);
    changeIP();
    /* Infinite loop */
    for (;;) {
        osDelay(1);
    }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_httpFunc */
/**
* @brief Function implementing the httpTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_httpFunc */
void httpFunc(void const * argument)
{
  /* USER CODE BEGIN httpFunc */
    /* Infinite loop */
    osDelay(400);
    for(;;)
	{
		printf("\rStarting httpd!\n");
		//httpd_task("http_server");
		httpd_task("http_server");
		printf("\rhttpd task returned unexpected\n");
		osDelay(10);
    }
  /* USER CODE END httpFunc */
}

/* USER CODE BEGIN Header_ledFunc */
/**
* @brief Function implementing the ledTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ledFunc */
void ledFunc(void const * argument)
{
  /* USER CODE BEGIN ledFunc */
    osDelay(10);
    printf("\rLED Toggle Task started\n");
    /* Infinite loop */
    for(;;)
    {
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		osDelay(100);

		if(setLED != 0){
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
		}
		else{
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
		}

    }
  /* USER CODE END ledFunc */
}

/* USER CODE BEGIN Header_loggingFunc */
/**
 * @brief Function implementing the loggingTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_loggingFunc */
void loggingFunc(void const * argument)
{
  /* USER CODE BEGIN loggingFunc */
    osDelay(100);
    char logmessage[LOGLEN];
    printf("\rLogging Task started!\n");
    /* Infinite loop */
    for (;;) {
        /* Check if logging queue exists */
        if (loggingQueue != 0) {
            /* Check if content is in logging queue */
            if (xQueueReceive(loggingQueue, logmessage, (TickType_t) 1)) {
                printf("\r%s\n", (char * ) logmessage);
            }
        } else {
            printf("\rError loggingQueue does not exist!\n");
        }
        osDelay(1);
    }
  /* USER CODE END loggingFunc */
}

/* USER CODE BEGIN Header_mbFunc */
/**
* @brief Function implementing the mbTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_mbFunc */
void mbFunc(void const * argument)
{
  /* USER CODE BEGIN mbFunc */
    osDelay(800);
    modbus_cmd_handler_task();
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END mbFunc */
}

/* USER CODE BEGIN Header_displayFunc */
/**
* @brief Function implementing the displayTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_displayFunc */
void displayFunc(void const * argument)
{
  /* USER CODE BEGIN displayFunc */
	osDelay(100);
	//u8g2_Setup_ssd1306_i2c_128x64_noname_1(&u8g2, U8G2_R0, u8x8_byte_hw_i2c, u8x8_stm32_gpio_and_delay_cb); // init u8g2 structure
	u8g2_Setup_sh1106_i2c_128x64_noname_1(&u8g2, U8G2_R0, u8x8_byte_hw_i2c, u8x8_stm32_gpio_and_delay_cb); // init u8g2 structure
	u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
	u8g2_SetDisplayRotation(&u8g2, U8G2_R2);
	u8g2_SetPowerSave(&u8g2, 0); // wake up display
	/* Infinite loop */
	for(;;)
	{
	u8g2_FirstPage(&u8g2);
	do
	{
		draw();
	}
	while ( u8g2_NextPage(&u8g2) ); // 8 times running

	if (step <= max)
		step += 3;
	else
	{
		step = 0;
		picture++;
		if ( picture >= 6)
		picture = 0;
	}

		osDelay(100);
	}
  /* USER CODE END displayFunc */
}

/* USER CODE BEGIN Header_ioFunc */
/**
* @brief Function implementing the ioTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ioFunc */
void ioFunc(void const * argument)
{
  /* USER CODE BEGIN ioFunc */
    osDelay(10);
    /* Infinite loop */
    for(;;)
    {
	// READ INPUTS
	if(HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_13)){
	    discrete_input_buf |= 1UL;
	}
	else {
	    discrete_input_buf &= ~(1UL);
	}
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9)){
	    discrete_input_buf |= (1UL<<1);
	}
	else {
	    discrete_input_buf &= ~((1UL)<<1);
	}
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_11)){
	    discrete_input_buf |= (1UL<<2);
	}
	else {
	    discrete_input_buf &= ~((1UL)<<2);
	}
	if(HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_14)){
	    discrete_input_buf |= (1UL<<3);
	}
	else {
	    discrete_input_buf &= ~((1UL)<<3);
	}

	//WRITE OUTPUTS
	if(discrete_output_buf & (1 << 0)){
	    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_SET);
	}
	else{
	    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_RESET);
	}
	if(discrete_output_buf & (1 << 1)){
	    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
	}
	else{
	    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);
	}
	if(discrete_output_buf & (1 << 2)){
	    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15, GPIO_PIN_SET);
	}
	else{
	    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15, GPIO_PIN_RESET);
	}
	if(discrete_output_buf & (1 << 3)){
	    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
	}
	else{
	    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
	}
	osDelay(10);
    }
  /* USER CODE END ioFunc */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    while(1)
    {
    	printf("\n\rIn error handler?!\n");
    }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
    char sendBuf[256];
    snprintf(sendBuf, sizeof(sendBuf), "Wrong parameters value: file %s on line %d\r\n", file, line);
    logger(LOG_ERR, 1, sendBuf);
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
