/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/


#include <main.h>

#include <fifoUart.h>
#include <hardware.h>
#include <link.h>
#include <loraTcp.h>
#include <loraApp.h>
#include <tcp.h>
#include <app.h>


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
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_rx;
DMA_HandleTypeDef hdma_i2c1_tx;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
DMA_HandleTypeDef hdma_uart4_rx;
DMA_HandleTypeDef hdma_uart4_tx;
DMA_HandleTypeDef hdma_uart5_rx;
DMA_HandleTypeDef hdma_uart5_tx;

TIM_HandleTypeDef htim6;

IWDG_HandleTypeDef hiwdg;

/* USER CODE BEGIN PV */


bool flagBoton;

///////////////
// GPS INPUT //
///////////////

gpsInput gpsInput;		// Instancia de gpsInput

fifoUart	gpsIn;		//
uint8_t	rxGps[2];		//

float latitud;			//
int latitud2;			//
float longitud;			//
int longitud2;			//

bool gpsValid;			//

bool startGps	= 1;	//
bool stopGps;			//
bool savedGps;			//
bool enableGps	= 1;	//

////////////////
// LORA INPUT //
////////////////

uint16_t l;
uint8_t rxLora[1];			// Buffer que recibe datos del DMA y traspasa a FIFO
fifoUart loraIn;			// FIFO que recibe los datos
loraInput loraInput;		// OBjeto que detecta si hay
loraCheck loraCheck;		// Objeto que verifica si es una respuesta valida
loraDecision loraDecision;	//
loraSelect loraSelect;		//

loraRutine setupRN2903;
loraRutine chSetRN2903;
loraRutine loopRN2903;


uint8_t bufferLoraIn[50];

// LORA //

wdTimeout loraTimeWd(4000);

uint8_t *loraTxCommand;
uint8_t sizeTxCommand;

//////////
// LORA //
//////////

uint8_t loraRetries	= 2;

/////////////
// DISPLAY //
/////////////

displayLink displayLink;
displayPhysical displayPhy;

/////////
// I2C //
/////////

bool flagI2C_DMA;

uint8_t rxI2C[]	= "0";

#define DEV_ADDR 0xa0
uint8_t dataw1[] = "hello world from EEPROM";
uint8_t dataw2[] = "This is the second string from EEPROM";
float dataw3 = 1234.5678;

uint8_t datar1[sizeof(dataw1)];
uint8_t datar2[sizeof(dataw2)];
float datar3;

////////////
// EEPROM //
////////////

uint8_t nameSensor0[] 	= "HCL B1 ALPHASENSE";
uint8_t idSensor0[]		= "0005";
uint8_t rangeSensor0[]	= "0020";
uint8_t amplifier1_0[]	= "0020";
uint8_t amplifier2_0[]	= "0011";

uint8_t groundSensor0[]	= "1000";
uint8_t curveSensor0[]	= "0035";

uint8_t dateCalib0[]	= "09/04/2024";

uint8_t nameSensor1[] 	= "HCL A1 ALPHASENSE";
uint8_t idSensor1[]		= "1005";
uint8_t rangeSensor1[]	= "0100";
uint8_t amplifier1_1[]	= "0020";
uint8_t amplifier2_1[]	= "0015";

uint8_t groundSensor1[]	= "1000";
uint8_t curveSensor1[]	= "0035";

uint8_t dateCalib1[]	= "09/04/2024";

/////////////
// CONTENT //
/////////////

fifoCommands fifoContent;

///////////////
// SUPERLOOP //
///////////////

static bool flagSuperloop;
uint8_t superloop	= 2;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_UART4_Init(void);
static void MX_UART5_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM6_Init(void);
static void MX_ADC1_Init(void);
static void MX_IWDG_Init(void);

/* USER CODE BEGIN PFP */


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint32_t analog[3];
bool flagAnalog;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  //loraDecition.setRetries( loraRetries );

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
  MX_DMA_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_I2C1_Init();
  MX_TIM6_Init();
  MX_ADC1_Init();
  MX_IWDG_Init();

  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start_IT	(&htim6);
  HAL_UART_Receive_DMA	(&huart4,	rxLora, sizeof(rxLora));
  HAL_UART_Receive_DMA	(&huart5,	rxGps, sizeof(rxGps));
  HAL_ADC_Start_DMA		(&hadc1, analog, 3);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  setupLoraApp();

  chSetRN2903.addCommand( &macSetCh8 );
  chSetRN2903.addCommand( &macSetCh9 );
  chSetRN2903.addCommand( &macSetCh10 );

  setupRN2903.addCommand( &radioSetMod	);
  setupRN2903.addCommand( &radioSetSf 	);
  setupRN2903.addCommand( &radioSetBw 	);
  setupRN2903.addCommand( &radioSetPwr 	);

  setupRN2903.addCommand( &macSetDeveui );
  setupRN2903.addCommand( &macSetDevAddr );
  setupRN2903.addCommand( &macSetNwkskey );
  setupRN2903.addCommand( &macSetAppskey );
  setupRN2903.addCommand( &macJoinAbp );
  setupRN2903.addCommand( &macSetSync );
  setupRN2903.addCommand( &macSave );

  loopRN2903.addCommand( &macTxData );
  loopRN2903.addCommand( &macTxData );

  loraCheck.setResponsePointer( rxCommand, 13);
  loraDecision.setRetries(3);

  loraInput.insertBuffer( bufferLoraIn, sizeof(bufferLoraIn));
  //displayLink.inputDisplay(8888, 3);
  //
  HAL_IWDG_Init(&hiwdg);
  while (1)
  {
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

	  hwInput();
	  linkInput();
	  tcpInput();
	  app();
	  tcpOutput();
	  linkOutput();
	  hwOutput();

	  /* USER CODE END WHILE */

	  /* USER CODE BEGIN 3 */
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

	  HAL_SuspendTick();
	  HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);

	  while (!flagSuperloop){	// SI NO SE HA CUMPLIDO L TIMER
	  }							// ESPERA ETERNAMENTE
	  flagSuperloop	= 0;		// REINICIA FLAG

	  HAL_ResumeTick();
	  HAL_IWDG_Refresh(&hiwdg);

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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}
/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
void MX_IWDG_Init(void)
{
  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* Initialize the IWDG peripheral */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_4;
  hiwdg.Init.Reload = 399;
  hiwdg.Init.Window	= 390;

  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
}
/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{
	  /* USER CODE BEGIN ADC1_Init 0 */

	  /* USER CODE END ADC1_Init 0 */

	  ADC_MultiModeTypeDef multimode = {0};
	  ADC_ChannelConfTypeDef sConfig = {0};

	  /* USER CODE BEGIN ADC1_Init 1 */

	  /* USER CODE END ADC1_Init 1 */

	  /** Common config
	  */
	  hadc1.Instance = ADC1;
	  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
	  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	  hadc1.Init.LowPowerAutoWait = DISABLE;
	  hadc1.Init.ContinuousConvMode = ENABLE;
	  hadc1.Init.NbrOfConversion = 3;
	  hadc1.Init.DiscontinuousConvMode = DISABLE;
	  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	  hadc1.Init.DMAContinuousRequests = DISABLE;
	  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	  hadc1.Init.OversamplingMode = DISABLE;
	  if (HAL_ADC_Init(&hadc1) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /** Configure the ADC multi-mode
	  */
	  multimode.Mode = ADC_MODE_INDEPENDENT;
	  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /** Configure Regular Channel
	  */
	  sConfig.Channel = ADC_CHANNEL_1;
	  sConfig.Rank = ADC_REGULAR_RANK_1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_24CYCLES_5;
	  sConfig.SingleDiff = ADC_SINGLE_ENDED;
	  sConfig.OffsetNumber = ADC_OFFSET_NONE;
	  sConfig.Offset = 0;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /** Configure Regular Channel
	  */
	  sConfig.Channel = ADC_CHANNEL_2;
	  sConfig.Rank = ADC_REGULAR_RANK_2;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /** Configure Regular Channel
	  */
	  sConfig.Channel = ADC_CHANNEL_3;
	  sConfig.Rank = ADC_REGULAR_RANK_3;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  /* USER CODE BEGIN ADC1_Init 2 */

	  /* USER CODE END ADC1_Init 2 */
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
  hi2c1.Init.Timing = 0x2000090E;
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
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}
/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

	  TIM_MasterConfigTypeDef sMasterConfig = {0};

	  /* USER CODE BEGIN TIM6_Init 1 */

	  /* USER CODE END TIM6_Init 1 */
	  htim6.Instance = TIM6;
	  htim6.Init.Prescaler = 1;
	  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim6.Init.Period = 7999;
	  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 57600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity =  UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  huart5.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
  /* DMA1_Channel7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
  /* DMA2_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel1_IRQn);
  /* DMA2_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel2_IRQn);
  /* DMA2_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel3_IRQn);
  /* DMA2_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2 | GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pins : PE2 PE3 PE0 PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PA2 PA4 PA5 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD8 PD9 PD10 PD11
                           PD12 PD13 PD14 PD15
                           PD0 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}


/* USER CODE BEGIN 4 */


// when DMA conversion is completed, HAL_ADC_ConvCpltCallback function
// will interrupt the processor. You can find this function in
// Drivers>STM32F4xx_HAL_Drivers>stm32f4xx_hal_adc.c file as __weak attribute
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	// I set adc_conv_complete_flag variable to 1 when,
	// HAL_ADC_ConvCpltCallback function is call.
	if ( hadc->Instance == ADC1 ){
		flagAnalog = 1;
	}
	//HAL_ADC_Start_DMA(&hadc1, (uint32_t *) analog, 3);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == UART4){
		loraIn.insertElement(rxLora[0]);
		HAL_UART_Receive_DMA(&huart4, rxLora, sizeof(rxLora));
	}

	if(huart->Instance == UART5){
		gpsIn.insertElement(rxGps[0]);
		gpsIn.insertElement(rxGps[1]);
		//HAL_UARTEx_ReceiveToIdle_DMA(&huart5, rxGps, sizeof(rxGps));
		//__HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);
		HAL_UART_Receive_DMA(&huart5, rxGps, sizeof(rxGps));
		__HAL_UART_CLEAR_IDLEFLAG(&huart5);
		//__HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);
	}
}


/////////////////////////
// TIMER 6 - SUPERLOOP //
/////////////////////////

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM6){
		flagSuperloop	= 1;
	}
}

///////////////////////
// I2C INTERRUPTIONS //
///////////////////////

void HAL_I2C_MasterTxCpltCallback (I2C_HandleTypeDef *hi2c)
{
	flagI2C_DMA	= 1;;
}

void HAL_I2C_MasterRxCpltCallback (I2C_HandleTypeDef * hi2c)
{

}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
