/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "main.h"
#include "usb_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include "stdio.h"
#include "stdint.h"
#include "FLASH_PROCESS_lib.h"
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

I2S_HandleTypeDef hi2s3;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
RTC_TimeTypeDef RTC_Time;
RTC_TimeTypeDef RTC_AlarmTime;
RTC_DateTypeDef RTC_Date;
RTC_AlarmTypeDef RTC_Alarm;

bool AlarmState = false;
bool AlarmTriggered =false;
int button1_flag=0;
int button2_flag=0;

int PickerHours1=0;
int PickerHours2=0;
int PickerMinutes1=0;
int PickerMinutes2=0;

uint32_t flash_hours;
uint32_t flash_minutes;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2S3_Init(void);
static void MX_SPI1_Init(void);
static void MX_RTC_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void process_alarm(bool AlarmState, int time){
	if(AlarmState == true){
		HAL_GPIO_TogglePin(buzzer_GPIO_Port, buzzer_Pin);

//		06:00 - 11:59 → Sabah
//		12:00 - 17:59 → Öğle
//		18:00 - 23:59 → Akşam
//		00:00 - 05:59 → Gece
			if (6 <= time && time < 12) {
			    // Sabah
				HAL_GPIO_TogglePin(led1_GPIO_Port, led1_Pin);
				HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_RESET);
			}
			else if (12 <= time && time < 18) {
			    // Öğle
				HAL_GPIO_TogglePin(led2_GPIO_Port, led2_Pin);
				HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_RESET);
			}
			else if (18 <= time && time < 24) {
			    // Akşam
				HAL_GPIO_TogglePin(led3_GPIO_Port, led3_Pin);
				HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_RESET);
			}
			else {
			    // Gece
				HAL_GPIO_TogglePin(led4_GPIO_Port, led4_Pin);
				HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_RESET);
			}

	}
	else{
		HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_RESET);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if (GPIO_Pin == GPIO_PIN_0)
    {
    	AlarmTriggered = true;
    }

}

void SevenSDisplay1(int value) {

    HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(dp1_GPIO_Port, dp1_Pin, GPIO_PIN_RESET);
    switch (value) {
        case 0:
            HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_SET);
            break;
        case 1:
            HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_SET);
            break;
        case 2:
            HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_SET);
            break;
        case 3:
            HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_SET);
            break;
        case 4:
            HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_SET);
            break;
        case 5:
            HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_SET);
            break;
        case 6:
            HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_SET);
            break;
        case 7:
            HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_SET);
            break;
        case 8:
            HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_SET);
            break;
        case 9:
            HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_SET);
            break;
        default:
            HAL_GPIO_WritePin(dp1_GPIO_Port, dp1_Pin, GPIO_PIN_RESET);
            break;
    }
}

void SevenSDisplay1_null(){
	  HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(dp1_GPIO_Port, dp1_Pin, GPIO_PIN_SET);
}

int TimePicker(int value){
	switch(value){
		case 1:
			if(HAL_GPIO_ReadPin(button2_GPIO_Port, button2_Pin) == 1){
				PickerHours1 =+1;
			}
			break;
		case 2:
			if(HAL_GPIO_ReadPin(button2_GPIO_Port, button2_Pin) == 1){
				PickerHours2 =+1;
			}
			break;
		case 3:
            HAL_GPIO_WritePin(dp1_GPIO_Port, dp1_Pin, GPIO_PIN_RESET);
			if(HAL_GPIO_ReadPin(button2_GPIO_Port, button2_Pin) == 1){
				PickerMinutes1 =+1;
				SevenSDisplay1(PickerMinutes1);
			}
			break;
		case 4:
            HAL_GPIO_WritePin(dp1_GPIO_Port, dp1_Pin, GPIO_PIN_RESET);
			if(HAL_GPIO_ReadPin(button2_GPIO_Port, button2_Pin) == 1){
				PickerMinutes2 =+1;
				SevenSDisplay1(PickerMinutes1);
			}
			break;
		default:
			break;

	}
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
  MX_I2C1_Init();
  MX_I2S3_Init();
  MX_SPI1_Init();
  MX_USB_HOST_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  RTC_Time.Hours = 21;
  RTC_Time.Minutes = 15;
  RTC_Time.Seconds = 0;
  HAL_RTC_SetTime(&hrtc, &RTC_Time, RTC_FORMAT_BIN);

  RTC_Date.Year = 25;
  RTC_Date.Month = RTC_MONTH_FEBRUARY;
  RTC_Date.Date = 1;
  RTC_Date.WeekDay = RTC_WEEKDAY_SUNDAY;
  HAL_RTC_SetDate(&hrtc, &RTC_Date, RTC_FORMAT_BIN);

  RTC_AlarmTime.Hours = 21;
  RTC_AlarmTime.Minutes = 16;
  RTC_AlarmTime.Seconds = 0;

  SevenSDisplay1_null();

//  RTC_AlarmTime.Hours = 21;
//  RTC_AlarmTime.Minutes = 16;
//  RTC_Alarm.AlarmTime = RTC_AlarmTime;
//  HAL_RTC_SetAlarm(&hrtc, &RTC_Alarm, RTC_FORMAT_BIN);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    MX_USB_HOST_Process();

    /* USER CODE BEGIN 3 */

    HAL_RTC_GetTime(&hrtc, &RTC_Time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &RTC_Date, RTC_FORMAT_BIN);


    if(HAL_GPIO_ReadPin(button1_GPIO_Port, button1_Pin) == GPIO_PIN_SET){
    	button1_flag +=1;
    	if (button1_flag >= 5){
    		int setMinutes = (PickerMinutes1*10) + PickerMinutes2;
    		int setHours = (PickerHours1*10) + PickerHours2;
    		Flash_WR(flash_hours, setHours);
    		Flash_WR(flash_minutes, setMinutes);
    		button1_flag = 0;
    	}
    }
    if(button1_flag == 0){
        if(!AlarmTriggered && RTC_AlarmTime.Hours==RTC_Time.Hours && RTC_AlarmTime.Minutes == RTC_Time.Minutes){
        	AlarmState = true;
        }
        else{
        	AlarmState = false;
        }
        if(RTC_AlarmTime.Hours==RTC_Time.Hours && (RTC_AlarmTime.Minutes + 1)  == RTC_Time.Minutes ){
        	AlarmTriggered = false;
        }
        process_alarm(AlarmState, RTC_AlarmTime.Hours);
        HAL_Delay(100);
    }


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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2S3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2S3_Init(void)
{

  /* USER CODE BEGIN I2S3_Init 0 */

  /* USER CODE END I2S3_Init 0 */

  /* USER CODE BEGIN I2S3_Init 1 */

  /* USER CODE END I2S3_Init 1 */
  hi2s3.Instance = SPI3;
  hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_96K;
  hi2s3.Init.CPOL = I2S_CPOL_LOW;
  hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  if (HAL_I2S_Init(&hi2s3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2S3_Init 2 */

  /* USER CODE END I2S3_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 17;
  sTime.Minutes = 5;
  sTime.Seconds = 1;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_SUNDAY;
  sDate.Month = RTC_MONTH_FEBRUARY;
  sDate.Date = 2;
  sDate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0;
  sAlarm.AlarmTime.Minutes = 0;
  sAlarm.AlarmTime.Seconds = 10;
  sAlarm.AlarmTime.SubSeconds = 0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|g1_Pin|f1_Pin|a1_Pin
                          |b1_Pin|e1_Pin|d1_Pin|dp1_Pin
                          |c1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, buzzer_Pin|led4_Pin|led3_Pin|led2_Pin
                          |led1_Pin|Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : CS_I2C_SPI_Pin g1_Pin f1_Pin a1_Pin
                           b1_Pin e1_Pin d1_Pin dp1_Pin
                           c1_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin|g1_Pin|f1_Pin|a1_Pin
                          |b1_Pin|e1_Pin|d1_Pin|dp1_Pin
                          |c1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PDM_OUT_Pin */
  GPIO_InitStruct.Pin = PDM_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CLK_IN_Pin */
  GPIO_InitStruct.Pin = CLK_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : button2_Pin button1_Pin */
  GPIO_InitStruct.Pin = button2_Pin|button1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : buzzer_Pin led4_Pin led3_Pin led2_Pin
                           led1_Pin Audio_RST_Pin */
  GPIO_InitStruct.Pin = buzzer_Pin|led4_Pin|led3_Pin|led2_Pin
                          |led1_Pin|Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


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
