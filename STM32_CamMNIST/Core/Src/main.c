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
#include "main.h"
#include "dcmi.h"
#include "dma.h"
#include "i2c.h"
#include "lptim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "OV7670.h"
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
extern DCMI_HandleTypeDef hdcmi;
extern DMA_HandleTypeDef hdma_dcmi;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
Camera_settings OV7670_settings={
		.resolution = QVGA, 	    //Resolution
		.format = RGB565, 	//Format
		.effect = NORMAL, 	//Effect
		.AEC = ON,			//AEC
		.AGC = ON, 		//AGC
		.AWB = ON, 		//AWB
		.cbar = OFF,		//Color bar
		OFF,		//vertical flip
		OFF,		//Horizontal flip
		OFF,		//Night mode
		OFF,		//ASC
		ON,			//De-noise
		ON,			//Banding filter
		HISTOGRAM,	//AEC algorithm
		QUARTER_FPS,//Min. fps in night mode
		F_AUTO,		//Auto detect banding freq.
		256, 		//Exposure - 2 bytes
		4, 			//Gain	[0-7]=[1-128]
		160,		//Brightness - byte
		64, 		//Contrast - byte
		80, 		//Saturation - byte
		2,			//Sharpness	- [0-31]
		0,			//De-noise strength - byte
		x16, 		//Gain ceiling
		77, 		//R channel gain - byte
		103, 		//G channel gain - byte
		153			//B channel gain - byte
};

typedef struct
{
	//uint32_t 	*address;	// address/pointer to image data
	uint16_t 	*address;	// address/pointer to image data
	uint16_t 	width;		// image width
	uint16_t 	height;		// image height
	uint8_t 	format;		// format in which image data are stored
}Image_info;

Image_info image;

//#define X_MAX	352 //CIF width
//#define Y_MAX	288	//CIF height
//
//static uint32_t image_data[X_MAX*(Y_MAX>>1)];	//Max resolution is CIF(352*288)

#define X_MAX	320 //CIF width
#define Y_MAX	240	//CIF height

//static uint32_t image_data[X_MAX*(Y_MAX>>1)];	//Max resolution is QQQVGA(80*60)
//static uint32_t image_data[X_MAX*(Y_MAX/2)];	//Max resolution is QQQVGA(80*60)
//static uint16_t image_data[X_MAX*(Y_MAX/2)];	//Max resolution is QQQVGA(80*60)
static uint16_t image_data[X_MAX*Y_MAX]; //uint16_t =2 bytes per color


//uint32_t* getImageAddress(void)
uint16_t* getImageAddress(void)
{
	return image_data;
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
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_DCMI_Init();
  MX_USB_DEVICE_Init();
  MX_I2C1_Init();
  MX_LPTIM1_Init();
  /* USER CODE BEGIN 2 */
  OV7670_Init(&hdcmi, &hdma_dcmi, &hi2c1, &hlptim1);

//  OV7670_PowerUp();
////
//  OV7670_UpdateSettings(OV7670_settings);
//  OV7670_SetFrameRate(XCLK_DIV(1), PLL_x4);//seems less stable for higher pll
//  HAL_Delay(50);
//
//  image.address=getImageAddress();
//  OV7670_Start(CONTINUOUS,image.address);

//  uint8_t temp=0;
//  OV7670_ReadSCCB(0x01, &temp);
//  HAL_Delay(50);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	OV7670_PowerUp();
	OV7670_UpdateSettings(OV7670_settings);
	OV7670_SetFrameRate(XCLK_DIV(1), PLL_x4);//seems less stable for higher pll
	HAL_Delay(10);
//
    image.address=getImageAddress();
    OV7670_Start(SNAPSHOT,image.address);
	OV7670_getImageInfo(&image.width, &image.height, &image.format);

    //uint8_t buffer[] = "Hello, World!\r\n";
    //CDC_Transmit_FS(buffer, sizeof(buffer));
	//CDC_Transmit_FS(image_data, sizeof(image_data));

	HAL_UART_Transmit(&huart3, (const uint8_t*)"Hello from UART3\r\n", 18, HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart3, (const uint8_t*)"Hello from UART3\r\n", 18, HAL_MAX_DELAY);



    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    HAL_Delay(2000);

    //OV7670_Stop();
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
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
