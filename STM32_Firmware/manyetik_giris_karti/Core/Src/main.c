/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "ds1302.h"
#include "rc522.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// Pin Tanımlamaları
#define GPIO_PIN_ROW1 GPIO_PIN_0
#define GPIO_PIN_ROW2 GPIO_PIN_1
#define GPIO_PIN_ROW3 GPIO_PIN_2
#define GPIO_PIN_ROW4 GPIO_PIN_3

#define GPIO_PIN_COL1 GPIO_PIN_4
#define GPIO_PIN_COL2 GPIO_PIN_5
#define GPIO_PIN_COL3 GPIO_PIN_6
#define GPIO_PIN_COL4 GPIO_PIN_7
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

char keypad_map[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

char Keypad_Oku(void)
{
    uint16_t ROW_PINS[4] = {GPIO_PIN_ROW1, GPIO_PIN_ROW2, GPIO_PIN_ROW3, GPIO_PIN_ROW4};
    uint16_t COL_PINS[4] = {GPIO_PIN_COL1, GPIO_PIN_COL2, GPIO_PIN_COL3, GPIO_PIN_COL4};

    for(int i = 0; i < 4; i++)
    {
        // 1. ÖNCE TÜM SATIRLARI '1' YAP (Elektriği Kes)
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_ROW1 | GPIO_PIN_ROW2 | GPIO_PIN_ROW3 | GPIO_PIN_ROW4, GPIO_PIN_SET);

        // 2. Sadece o anki satırı '0' YAP (Aktif et)
        HAL_GPIO_WritePin(GPIOC, ROW_PINS[i], GPIO_PIN_RESET);

        // 3. Voltajın oturması için bekle (Hayalet tuşları engeller)
        HAL_Delay(2);

        // 4. Sütunları tara
        for(int j = 0; j < 4; j++)
        {
            // Tuşa basıldı mı? (0 mı okundu?)
            if(HAL_GPIO_ReadPin(GPIOC, COL_PINS[j]) == GPIO_PIN_RESET)
            {
                // Emin olmak için bekle (Debounce)
                HAL_Delay(50);

                // Hala basılı mı?
                if(HAL_GPIO_ReadPin(GPIOC, COL_PINS[j]) == GPIO_PIN_RESET)
                {
                    // Tuşu döndür
                    return keypad_map[i][j];
                }
            }
        }
    }
    return 0; // Tuşa basılmadı
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint8_t CardID[5];
  char buffer[100];
  DS1302_Time time;

  uint32_t son_rfid_kontrol = 0;
  uint32_t son_kart_okuma_zamani = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */
  DS1302_Init();
  MFRC522_Init();
  HAL_UART_Transmit(&huart2, (uint8_t*)"Sistem Baslatildi...\r\n", 22, 100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      // =========================================================
      // 1. KEYPAD KONTROLÜ
      // =========================================================
      char basilanTus = Keypad_Oku();

      if(basilanTus != 0)
      {
          // 1. Veriyi Gönder
          char keyMsg[20];
          sprintf(keyMsg, "KEY:%c\r\n", basilanTus);
          HAL_UART_Transmit(&huart2, (uint8_t*)keyMsg, strlen(keyMsg), 100);

          // 2. Ses Çıkar (Bip)
          HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
          HAL_Delay(50);
          HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);

          // 3. Tuştan parmağını çekene kadar bekle (Spam engelleme)
          while(Keypad_Oku() == basilanTus)
          {
        	  HAL_Delay(10); // İşlemciyi yorma
          }
      }

      // =========================================================
      // 2. RFID KONTROLÜ (ZAMANLAYICI İLE)
      // =========================================================

      uint32_t simdi = HAL_GetTick();

      // 200ms geçtiyse ve son kart okumanın üzerinden 2 saniye geçtiyse
      if ((simdi - son_rfid_kontrol > 200) && (simdi - son_kart_okuma_zamani > 2000))
      {
          son_rfid_kontrol = simdi;

          if (MFRC522_Check(CardID) == MI_OK)
          {
              // Kart bulundu!
              DS1302_GetTime(&time);

              // Ses
              HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
              HAL_Delay(100);
              HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);

              // Veri Gönderimi
              sprintf(buffer, "UID:%02X-%02X-%02X-%02X-%02X,TARIH:%02d/%02d/20%02d %02d:%02d:%02d\r\n",
              CardID[0], CardID[1], CardID[2], CardID[3], CardID[4],
              time.dayofmonth, time.month, time.year,
              time.hour, time.minutes, time.seconds);

              HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

              // Kart okundu, cooldown başlat
              son_kart_okuma_zamani = HAL_GetTick();
          }
      }

      // Döngü çok hızlı dönüp işlemciyi yormasın
      HAL_Delay(1);
  }
  /* USER CODE END WHILE */
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RC522_RST_Pin|DS1302_CLK_Pin|DS1302_DAT_Pin|DS1302_RST_Pin
                          |BUZZER_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : RC522_CS_Pin */
  GPIO_InitStruct.Pin = RC522_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RC522_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RC522_RST_Pin DS1302_CLK_Pin DS1302_DAT_Pin DS1302_RST_Pin
                           BUZZER_Pin */
  GPIO_InitStruct.Pin = RC522_RST_Pin|DS1302_CLK_Pin|DS1302_DAT_Pin|DS1302_RST_Pin
                          |BUZZER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* --- KEYPAD AYARLARI BURADA YAPILIYOR --- */

  /* 1. KEYPAD ROW (SATIR) PİNLERİ: Output (Çıkış) */
  /* PC0, PC1, PC2, PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* 2. KEYPAD COL (SÜTUN) PİNLERİ: Input (Giriş) ve PULL-UP */
  /* PC4, PC5, PC6, PC7 */
  /* Bu Pull-Up ayarı hayalet tuşları engeller! */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

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
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */
