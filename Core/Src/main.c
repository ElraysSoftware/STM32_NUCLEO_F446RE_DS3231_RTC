/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#ifdef DEBUG_LOG
#include "logger.h"
#endif
#include "ds3231.h"
#include "serial_menu.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART2_UART_Init();
  MX_I2C3_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  /* Serial Menu RS-232 Initialization */
  rs_232_rx_init();

#ifdef DEBUG_LOG
      /* Debugging via Virtual COM Serial Port */
      LOG(LOG_MSG, "DS3241 RTC Example by Elray's Software LLC");
#endif
      /* Initialize the RTC */
      ds3231_init(&hi2c3);

//      /* Disable interrupts while RTC is configured */
//      __disable_irq();
//      /* Initialize the RTC Interrupt mode to Alarm mode */
//      ds3231_set_interruptMode(DS3231_ALARM_INTERRUPT);
//      /* Enable the Alarms */
//      ds3231_enable_alarm_1(DS3231_ENABLED);
//      ds3231_enable_alarm_2(DS3231_ENABLED);
//      /* Set the Alarm 1 */
//      ds3231_set_alarm_1_mode(DS3231_A1_MATCH_S_M_H);
//      ds3231_set_alarm_1_second(3);
//      ds3231_set_alarm_1_minute(30);
//      ds3231_set_alarm_1_hour(13);
//      ds3231_set_alarm_1_date(2);
//
//      /* Set the Alarm 2 Mode */
//      ds3231_set_alarm_2_mode(DS3231_A2_MATCH_M_H);
//      ds3231_set_alarm_2_minute(31);
//      ds3231_set_alarm_2_hour(13);
//      ds3231_set_alarm_2_day(2);
//
//      /* Set time */
//      ds3231_set_full_time(13, 30, 0);
//      /* Set date */
//      ds3231_set_full_date(2, 9, 1, 2024);
//      /* Enable interrupts after configured */
//      __enable_irq();

//      char *day[7] = { "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN" };
//      LOG(LOG_MSG, "ISO8601 FORMAT: %04d-%02d-%02dT%02d:%02d:%02d %s %d.%02d",
//          ds3231_get_year(),
//          ds3231_get_month(),
//          ds3231_get_date(),
//          ds3231_get_hour(),
//          ds3231_get_minute(),
//          ds3231_get_second(),
//          day[ds3231_get_day_of_week()-1],
//          ds3231_get_temperature_integer(),
//          ds3231_get_temperature_fraction());

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      rs_232_menu();
      HAL_Delay(10);
      LOG(LOG_MSG, "Tick");
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
 * errorHandler
 * @brief Custom Error Handler
 * @param error_num - error number
 * @retval - always 0
 * @note This function never returns
 */
uint8_t errorHandler(uint32_t error_num)
{
    uint32_t j;
    uint32_t green_blink_cnt;

    /* determine the LED blink rate */
    switch (error_num)
    {
    default:
    /* intentional fall through */
    case ERR_CODE_SYS:
        green_blink_cnt = 2;
        break;
    }

    /* turn the LEDs OFF */
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

    /* blink the LED */
    while (1)
    {
        for (j = 0; j < green_blink_cnt; j++)
        {
            /* Toggle the Controller Board MCU Green LED */
            HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
            HAL_Delay(500);
        }
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    errorHandler(ERR_CODE_SYS);
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
