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
#include "adc.h"
#include "usart.h"
#include "gpio.h"

#include "stdio.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */


// Variables globales
char msg[10];
uint32_t adc_value; //short int adcval=0; dans notre prog


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void ESP8266_SendCommand(char* cmd);
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
  MX_ADC1_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */

  // Initialisation du module ESP8266
  /*ESP8266_SendCommand ("AT+RST\r\n");
   HAL_Delay(2000);*/
   ESP8266_SendCommand("AT\r\n");             // Test AT
   HAL_Delay(1000);
   ESP8266_SendCommand("AT+CWMODE=1\r\n");    // Set mode to STA
   HAL_Delay(1000);
   ESP8266_SendCommand("AT+CWJAP=\"Galaxy A12 C227\",\"dfay3938\"\r\n"); // Connect to Wi-Fi
   HAL_Delay(5000); // Attendre la connexion

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
// Démarrer l'ADC et lire la valeur
 /* HAL_ADC_Start(&hadc1);
	 if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
	 	   adc_value = HAL_ADC_GetValue(&hadc1);
	 	    }
	 	HAL_ADC_Stop(&hadc1);
	 	// Convertir la valeur ADC en chaîne de caractères
	 	   sprintf(msg, "%ld\r\n", adc_value);*/ // qu'il existe dans le prog principale de chat

  if (HAL_ADC_Start(&hadc1)!= HAL_OK)
   {
     Error_Handler();
   }
 if(HAL_ADC_PollForConversion(&hadc1,1)!= HAL_OK)
		   {
		     Error_Handler();
		   }
 if (HAL_ADC_Stop(&hadc1)!= HAL_OK)
    {
      Error_Handler();
    }
  adc_value=(short int)HAL_ADC_GetValue(&hadc1);
// Convertir la valeur ADC en chaîne de caractères
 sprintf(msg, "%ld", adc_value);

if(adc_value>4000){
HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);
 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_SET);}
 else if(adc_value>3000){
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);}
 else if(adc_value>2000){
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);}
 else {
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);}

// Envoyer les données à SpeakThings via ESP8266
    char sendCmd[200];
    sprintf(sendCmd, "AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n");


    ESP8266_SendCommand(sendCmd);
    HAL_Delay(2000);

    sprintf(sendCmd, "AT+CIPSEND=%d\r\n", strlen(msg)+40); // Taille des données à envoyer
    ESP8266_SendCommand(sendCmd);
    HAL_Delay(2000);

    sprintf(sendCmd, "POST /update HTTP/1.1\r\ GET https://api.thingspeak.com/update?api_key=3S65J6GTYPVBCCMD&field1%d=0\r\n\r\nvalue=%s", strlen(msg), msg);
    ESP8266_SendCommand(sendCmd);
    HAL_Delay(2000);
    // Attendre un peu avant la prochaine lecture
    HAL_Delay(500);


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
// Fonction pour envoyer une commande au module ESP8266
void ESP8266_SendCommand(char* cmd) {
  HAL_UART_Transmit(&huart2, (uint8_t*)cmd, strlen(cmd), HAL_MAX_DELAY);
}
/**
  * @brief System Clock Configuration
  * @retval None
  */

// Configuration de l'horloge système
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
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
