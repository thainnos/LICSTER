/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include "open62541.h"

#include "u8g2.h"
#include "u8x8_stm32_HAL.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart3;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 2048 * 4
};
/* Definitions for opcuaTask */
osThreadId_t opcuaTaskHandle;
const osThreadAttr_t opcuaTask_attributes = {
  .name = "opcuaTask",
  .priority = (osPriority_t) osPriorityBelowNormal,
  .stack_size = 8000 * 4
};
/* Definitions for ledTask */
osThreadId_t ledTaskHandle;
const osThreadAttr_t ledTask_attributes = {
  .name = "ledTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for displayTask */
osThreadId_t displayTaskHandle;
const osThreadAttr_t displayTask_attributes = {
  .name = "displayTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 512 * 4
};
/* USER CODE BEGIN PV */
static UA_Boolean running = true; // OPA-UA running

// OPC-UA Login data
static UA_UsernamePasswordLogin logins[2] = {
    {UA_STRING_STATIC("admin"), UA_STRING_STATIC("admin")},
    {UA_STRING_STATIC("root"), UA_STRING_STATIC("root")}
};

static u8g2_t u8g2; // for OLED display

extern uint8_t IP_ADDRESS[4]; // IP Address 
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_I2C1_Init(void);
void StartDefaultTask(void *argument);
void opcuaFunc(void *argument);
void ledFunc(void *argument);
void displayFunc(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

unsigned int deviceID = 0;

/**
  * @brief  Define the printf function.
  * @retval HAL_OK
  */
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
    u8g2_DrawStr(&u8g2, 0, 0, "OPC/UA RemoteIO");
    u8g2_SetFont(&u8g2, u8g2_font_profont10_tf);
    snprintf(message, sizeof(message), "IP:      %03d.%03d.%03d.%03d",
	    IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
    u8g2_DrawStr(&u8g2, 10, 15, message);
    snprintf(message, sizeof(message), "INPUTs:  %1d %1d %1d %1d",
	    HAL_GPIO_ReadPin(in1_GPIO_Port, in1_Pin),
	    HAL_GPIO_ReadPin(in2_GPIO_Port, in2_Pin),
	    HAL_GPIO_ReadPin(in3_GPIO_Port, in3_Pin),
	    HAL_GPIO_ReadPin(in4_GPIO_Port, in4_Pin));
   u8g2_DrawStr(&u8g2, 10, 25, message);
   snprintf(message, sizeof(message), "OUTPUTs: %1d %1d %1d %1d",
	    HAL_GPIO_ReadPin(out1_GPIO_Port, out1_Pin),
	    HAL_GPIO_ReadPin(out2_GPIO_Port, out2_Pin),
	    HAL_GPIO_ReadPin(out3_GPIO_Port, out3_Pin),
	    HAL_GPIO_ReadPin(out4_GPIO_Port, out4_Pin));
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

static void
addVariable(UA_Server *server) {
    /* Define the attribute of the myInteger variable node */
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    UA_Int32 myInteger = 42;
    UA_Variant_setScalar(&attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);
    attr.description = UA_LOCALIZEDTEXT("en-US","STM32 OPC-UA Server");
    attr.displayName = UA_LOCALIZEDTEXT("en-US","STM32 OPC-UA Server");
    attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    /* Add the variable node to the information model */
    UA_NodeId myIntegerNodeId = UA_NODEID_STRING(1, "stm32.opcua.server");
    UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME(1, "stm32.opcua.server");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_Server_addVariableNode(server, myIntegerNodeId, parentNodeId,
                              parentReferenceNodeId, myIntegerName,
                              UA_NODEID_NULL, attr, NULL, NULL);
}

/**
 * Now we change the value with the write service. This uses the same service
 * implementation that can also be reached over the network by an OPC UA client.
 */

static void
writeVariable(UA_Server *server) {
    UA_NodeId myIntegerNodeId = UA_NODEID_STRING(1, "stm32.opcua.server");

    /* Write a different integer value */
    UA_Int32 myInteger = 43;
    UA_Variant myVar;
    UA_Variant_init(&myVar);
    UA_Variant_setScalar(&myVar, &myInteger, &UA_TYPES[UA_TYPES_INT32]);
    UA_Server_writeValue(server, myIntegerNodeId, myVar);

    /* Set the status code of the value to an error code. The function
     * UA_Server_write provides access to the raw service. The above
     * UA_Server_writeValue is syntactic sugar for writing a specific node
     * attribute with the write service. */
    UA_WriteValue wv;
    UA_WriteValue_init(&wv);
    wv.nodeId = myIntegerNodeId;
    wv.attributeId = UA_ATTRIBUTEID_VALUE;
    wv.value.status = UA_STATUSCODE_BADNOTCONNECTED;
    wv.value.hasStatus = true;
    UA_Server_write(server, &wv);

    /* Reset the variable to a good statuscode with a value */
    wv.value.hasStatus = false;
    wv.value.value = myVar;
    wv.value.hasValue = true;
    UA_Server_write(server, &wv);
}


/* LED Method */

/**
  * @brief  Check the status of the RED LED
  * @retval UA_StatusCode
  */
UA_StatusCode
readLedRedState(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue) {
    UA_Boolean ledred_State = HAL_GPIO_ReadPin(LD3_GPIO_Port, LD3_Pin);
    UA_Variant_setScalarCopy(&dataValue->value, &ledred_State,
                             &UA_TYPES[UA_TYPES_BOOLEAN]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

/**
  * @brief  Check the status of Output 1
  * @retval UA_StatusCode
  */
UA_StatusCode
readOut1State(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue) {
    UA_Boolean out1_State = HAL_GPIO_ReadPin(out1_GPIO_Port, out1_Pin);
    UA_Variant_setScalarCopy(&dataValue->value, &out1_State,
                             &UA_TYPES[UA_TYPES_BOOLEAN]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

/**
  * @brief  Check the status of Output 2
  * @retval UA_StatusCode
  */
UA_StatusCode
readOut2State(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue) {
    UA_Boolean out2_State = HAL_GPIO_ReadPin(out2_GPIO_Port, out2_Pin);
    UA_Variant_setScalarCopy(&dataValue->value, &out2_State,
                             &UA_TYPES[UA_TYPES_BOOLEAN]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

/**
  * @brief  Check the status of Input 1
  * @retval UA_StatusCode
  */
UA_StatusCode
readIn1State(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue) {
    UA_Boolean in1_State = HAL_GPIO_ReadPin(in1_GPIO_Port, in1_Pin);
    UA_Variant_setScalarCopy(&dataValue->value, &in1_State,
                             &UA_TYPES[UA_TYPES_BOOLEAN]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

/**
  * @brief  Check the status of Input 2
  * @retval UA_StatusCode
  */
UA_StatusCode
readIn2State(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue) {
    UA_Boolean in2_State = HAL_GPIO_ReadPin(in2_GPIO_Port, in2_Pin);
    UA_Variant_setScalarCopy(&dataValue->value, &in2_State,
                             &UA_TYPES[UA_TYPES_BOOLEAN]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}


/**
  * @brief  Set the RED LED
  * @retval UA_StatusCode
  */
UA_StatusCode
setLedRedState(UA_Server *server,
                  const UA_NodeId *sessionId, void *sessionContext,
                  const UA_NodeId *nodeId, void *nodeContext,
                 const UA_NumericRange *range, const UA_DataValue *data) {
    UA_Boolean currentState = HAL_GPIO_ReadPin(LD3_GPIO_Port, LD3_Pin);
    int level = currentState == true ? 0:1;
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, level);
    UA_Boolean ledred_state_after_write = HAL_GPIO_ReadPin(LD3_GPIO_Port, LD3_Pin);
    UA_StatusCode status = ledred_state_after_write == level ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;
    return status;
}

/**
  * @brief  Set Output 1
  * @retval UA_StatusCode
  */
UA_StatusCode
setOut1State(UA_Server *server,
                  const UA_NodeId *sessionId, void *sessionContext,
                  const UA_NodeId *nodeId, void *nodeContext,
                 const UA_NumericRange *range, const UA_DataValue *data) {
    //UA_Boolean currentState = HAL_GPIO_ReadPin(out1_GPIO_Port, out1_Pin);
    printf("\rOutput 1 will be set to %d\n", *(UA_Boolean*) data->value.data); 
    if(*(UA_Boolean*) data->value.data == UA_TRUE){
        HAL_GPIO_WritePin(out1_GPIO_Port, out1_Pin, 1);
    }
    else if(*(UA_Boolean*) data->value.data == UA_FALSE){
        HAL_GPIO_WritePin(out1_GPIO_Port, out1_Pin, 0);
    }
    UA_Boolean out1_state_after_write = HAL_GPIO_ReadPin(out1_GPIO_Port, out1_Pin);
    UA_StatusCode status = out1_state_after_write == *(UA_Boolean*) data->value.data ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;
    return status;
}

/**
  * @brief  Set Output 2
  * @retval UA_StatusCode
  */
UA_StatusCode
setOut2State(UA_Server *server,
                  const UA_NodeId *sessionId, void *sessionContext,
                  const UA_NodeId *nodeId, void *nodeContext,
                 const UA_NumericRange *range, const UA_DataValue *data) {
    //UA_Boolean currentState = HAL_GPIO_ReadPin(out2_GPIO_Port, out2_Pin);
    printf("\rOutput 2 will be set to %d\n", *(UA_Boolean*) data->value.data); 
    if(*(UA_Boolean*) data->value.data == UA_TRUE){
        HAL_GPIO_WritePin(out2_GPIO_Port, out2_Pin, 1);
    }
    else if(*(UA_Boolean*) data->value.data == UA_FALSE){
        HAL_GPIO_WritePin(out2_GPIO_Port, out2_Pin, 0);
    }
    UA_Boolean out2_state_after_write = HAL_GPIO_ReadPin(out2_GPIO_Port, out2_Pin);
    UA_StatusCode status = out2_state_after_write == *(UA_Boolean*) data->value.data ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;
    return status;
}

/**
  * @brief  Add the RED LED to the OPC-UA Node
  * @retval void
  */
void
addLedRedControlNode(UA_Server *server) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "LED Red");
    attr.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "Control Red LED.");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "Control Red LED.");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource ledred;
    ledred.read = readLedRedState;
    ledred.write = setLedRedState;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        ledred, NULL, NULL);
}

/**
  * @brief  Add Output 1 to the OPC-UA Node
  * @retval void
  */
void
addOut1ControlNode(UA_Server *server) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Output 1");
    attr.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "Control Output 1.");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "Control Output 1.");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource out1;
    out1.read = readOut1State;
    out1.write = setOut1State;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        out1, NULL, NULL);
}

/**
  * @brief  Add Output 2 to the OPC-UA Node
  * @retval void
  */
void
addOut2ControlNode(UA_Server *server) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Output 2");
    attr.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "Control Output 2.");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "Control Output 2.");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource out2;
    out2.read = readOut2State;
    out2.write = setOut2State;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        out2, NULL, NULL);
}

/**
  * @brief  Add Input 1 to the OPC-UA Node
  * @retval void
  */
void
addIn1ControlNode(UA_Server *server) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Input 1");
    attr.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "Read Input 1.");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "Read Input 1.");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource in1;
    in1.read = readIn1State;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        in1, NULL, NULL);
}

/**
  * @brief  Add Input 2 to the OPC-UA Node
  * @retval void
  */
void
addIn2ControlNode(UA_Server *server) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Input 2");
    attr.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "Read Input 2.");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "Read Input 2.");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource in2;
    in2.read = readIn2State;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        in2, NULL, NULL);
}


/* Node features */
static UA_Boolean
allowAddNode(UA_Server *server, UA_AccessControl *ac,
             const UA_NodeId *sessionId, void *sessionContext,
             const UA_AddNodesItem *item) {
    printf("Called allowAddNode\n");
    return UA_TRUE;
}

static UA_Boolean
allowAddReference(UA_Server *server, UA_AccessControl *ac,
                  const UA_NodeId *sessionId, void *sessionContext,
                  const UA_AddReferencesItem *item) {
    printf("Called allowAddReference\n");
    return UA_TRUE;
}

static UA_Boolean
allowDeleteNode(UA_Server *server, UA_AccessControl *ac,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_DeleteNodesItem *item) {
    printf("Called allowDeleteNode\n");
    return UA_FALSE; // Do not allow deletion from client
}

static UA_Boolean
allowDeleteReference(UA_Server *server, UA_AccessControl *ac,
                     const UA_NodeId *sessionId, void *sessionContext,
                     const UA_DeleteReferencesItem *item) {
    printf("Called allowDeleteReference\n");
    return UA_TRUE;
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
  MX_USART3_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  printf("\n\n\r+++++++++++++++++++++++++++++++++++++++\n");
 
  printf("\rBuild: %s %s\n", __DATE__, __TIME__);
  printf("\rInit done!\n");
  printf("\rWaiting for network switch...\n");
    
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

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  osDelay(30000);
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of opcuaTask */
  opcuaTaskHandle = osThreadNew(opcuaFunc, NULL, &opcuaTask_attributes);

  /* creation of ledTask */
  ledTaskHandle = osThreadNew(ledFunc, NULL, &ledTask_attributes);

  /* creation of displayTask */
  displayTaskHandle = osThreadNew(displayFunc, NULL, &displayTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C1;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(out4_GPIO_Port, out4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(out3_GPIO_Port, out3_Pin, GPIO_PIN_RESET);

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

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : device_4_Pin in1_Pin in4_Pin */
  GPIO_InitStruct.Pin = device_4_Pin|in1_Pin|in4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : out4_Pin */
  GPIO_InitStruct.Pin = out4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(out4_GPIO_Port, &GPIO_InitStruct);

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

  /*Configure GPIO pins : USB_SOF_Pin USB_ID_Pin USB_DM_Pin USB_DP_Pin */
  GPIO_InitStruct.Pin = USB_SOF_Pin|USB_ID_Pin|USB_DM_Pin|USB_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_VBUS_Pin */
  GPIO_InitStruct.Pin = USB_VBUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_VBUS_GPIO_Port, &GPIO_InitStruct);

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
void StartDefaultTask(void *argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN 5 */
  printf("\rNetwork initialized!\n");
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_opcuaFunc */
/**
* @brief Function implementing the opcuaTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_opcuaFunc */
void opcuaFunc(void *argument)
{
  /* USER CODE BEGIN opcuaFunc */
  osDelay(30000);
  printf("\rOPC-UA Function started!\n");
          //The default 64KB of memory for sending and receicing buffer caused problems to many users. With the code below, they are reduced to ~16KB
        UA_UInt32 sendBufferSize = 16000;       //64 KB is the regular buffer size
        UA_UInt32 recvBufferSize = 16000;       //64 KB is the regular buffer size
        UA_UInt16 portNumber = 4840;
        UA_StatusCode retval = UA_STATUSCODE_GOOD;
        
        UA_Server* mUaServer = UA_Server_new();
        UA_ServerConfig *uaServerConfig = UA_Server_getConfig(mUaServer);
        UA_ServerConfig_setMinimalCustomBuffer(uaServerConfig, portNumber, 0, sendBufferSize, recvBufferSize);

        //VERY IMPORTANT: Set the hostname with your IP before starting the server
        uint8_t ip;
        char buffer [15];
        ip = 50 + deviceID;
        snprintf ( buffer, sizeof(buffer), "192.168.0.%i", ip);
        UA_ServerConfig_setCustomHostname(uaServerConfig, UA_STRING(buffer));

        //The rest is the same as the example

        UA_Boolean running = true;


        /* Disable anonymous logins, enable two user/password logins */
        uaServerConfig->accessControl.clear(&uaServerConfig->accessControl);
        retval = UA_AccessControl_default(uaServerConfig, false,
             &uaServerConfig->securityPolicies[uaServerConfig->securityPoliciesSize-1].policyUri, 2, logins);
        if(retval != UA_STATUSCODE_GOOD)
            printf("\rLogins could not be set\n");

        /* Set accessControl functions for nodeManagement */
        uaServerConfig->accessControl.allowAddNode = allowAddNode;
        uaServerConfig->accessControl.allowAddReference = allowAddReference;
        uaServerConfig->accessControl.allowDeleteNode = allowDeleteNode;
        uaServerConfig->accessControl.allowDeleteReference = allowDeleteReference;

        // add a variable node to the addresspace
        addVariable(mUaServer);
        writeVariable(mUaServer);

        // add the red LED to the addressspace
        addLedRedControlNode(mUaServer);
        
        // add outputs to the addressspace
        addOut1ControlNode(mUaServer);
        addOut2ControlNode(mUaServer);
        
        // add inputs to the addressspace
        addIn1ControlNode(mUaServer);
        addIn2ControlNode(mUaServer);


        retval = UA_Server_run(mUaServer, &running);
        if (retval == UA_STATUSCODE_GOOD){
          while (running){
            UA_Server_run_iterate(mUaServer, false);
            taskYIELD();
          }
        }
        else{
              printf("\rProblem starting OPC-UA Server\n");
        }
        UA_Server_run_shutdown(mUaServer);

        UA_Server_delete(mUaServer);



  printf("\rOPC-UA Function terminated\n");  
  /* USER CODE END opcuaFunc */
}

/* USER CODE BEGIN Header_ledFunc */
/**
* @brief Function implementing the ledTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ledFunc */
void ledFunc(void *argument)
{
  /* USER CODE BEGIN ledFunc */
  osDelay(10);
  printf("\rLED Toggle Task started\n");
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	osDelay(100);
  }
  /* USER CODE END ledFunc */
}

/* USER CODE BEGIN Header_displayFunc */
/**
* @brief Function implementing the displayTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_displayFunc */
void displayFunc(void *argument)
{
  /* USER CODE BEGIN displayFunc */
  osDelay(100);
  //u8g2_Setup_ssd1306_i2c_128x64_noname_1(&u8g2, U8G2_R0, u8x8_byte_hw_i2c, u8x8_stm32_gpio_and_delay_cb); // init u8g2 structure
  u8g2_Setup_sh1106_i2c_128x64_noname_1(&u8g2, U8G2_R0, u8x8_byte_hw_i2c, u8x8_stm32_gpio_and_delay_cb); // init u8g2 structure
  u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
  u8g2_SetDisplayRotation(&u8g2, U8G2_R2);
  u8g2_SetPowerSave(&u8g2, 0); // wake up display
  /* Infinite loop */
  for(;;){
    u8g2_FirstPage(&u8g2);
    do{
      draw();
    }
    while ( u8g2_NextPage(&u8g2) ); // 8 times running

    if (step <= max)
      step += 3;
    else{
      step = 0;
      picture++;
      if ( picture >= 6)
        picture = 0;
    }

    osDelay(100);
  }
  /* USER CODE END displayFunc */
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
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
