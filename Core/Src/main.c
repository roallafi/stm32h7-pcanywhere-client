#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "uart_serial.h"
#include "tft_display.h"
#include "serial_task.h"
#include "keyboard_task.h"
#include "display_task.h"
#include "usb_host_keyboard.h"

/* Peripheral Handles */
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
HCD_HandleTypeDef hhcd_USB_OTG_HS;
LTDC_HandleTypeDef hltdc;
DMA2D_HandleTypeDef hdma2d;

/* Task Handles */
TaskHandle_t serialTaskHandle = NULL;
TaskHandle_t keyboardTaskHandle = NULL;
TaskHandle_t displayTaskHandle = NULL;

/* Queue Handles */
QueueHandle_t screen_queue = NULL;
QueueHandle_t keyboard_queue = NULL;

int main(void) {
    /* MCU Configuration */
    HAL_Init();
    SystemClock_Config();
    
    /* Initialize Peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART1_UART_Init();
    MX_FSMC_Init();
    MX_DMA2D_Init();
    MX_USB_HOST_Init();
    
    /* Initialize Drivers */
    LED_Init();
    UART_Init(&huart1);
    TFT_Init();
    USB_Keyboard_Init();
    
    /* Create FreeRTOS Queues */
    screen_queue = xQueueCreate(2, SCREEN_BUFFER_SIZE);
    keyboard_queue = xQueueCreate(16, sizeof(uint8_t));
    
    if (screen_queue == NULL || keyboard_queue == NULL) {
        LED_SetRed(1);
        Error_Handler();
    }
    
    /* Create FreeRTOS Tasks */
    if (xTaskCreate(SerialTask, "SerialTask", configMINIMAL_STACK_SIZE * 4,
                   NULL, SERIAL_TASK_PRIO, &serialTaskHandle) != pdPASS) {
        LED_SetRed(1);
        Error_Handler();
    }
    
    if (xTaskCreate(KeyboardTask, "KeyboardTask", configMINIMAL_STACK_SIZE * 2,
                   NULL, KEYBOARD_TASK_PRIO, &keyboardTaskHandle) != pdPASS) {
        LED_SetRed(1);
        Error_Handler();
    }
    
    if (xTaskCreate(DisplayTask, "DisplayTask", configMINIMAL_STACK_SIZE * 4,
                   NULL, DISPLAY_TASK_PRIO, &displayTaskHandle) != pdPASS) {
        LED_SetRed(1);
        Error_Handler();
    }
    
    /* LED Green - System Ready */
    LED_SetGreen(1);
    
    /* Start FreeRTOS Scheduler */
    vTaskStartScheduler();
    
    /* Should never reach here */
    while (1);
    
    return 0;
}

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    
    /** Supply configuration update enable */
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
    
    /** Configure the main internal regulator output voltage */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    
    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
    
    /** Initializes the RCC Oscillators according to the specified parameters */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 5;
    RCC_OscInitStruct.PLL.PLLN = 160;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    
    /** Initializes the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |
                                  RCC_CLOCKTYPE_D3PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV4;
    
    if (HAL_RCC_ClkConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
        Error_Handler();
    }
    
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USB;
    PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_D2PCLK2;
    PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
    
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        Error_Handler();
    }
}

static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    
    /* LED Configuration */
    GPIO_InitStruct.Pin = LED_GREEN_PIN | LED_RED_PIN | LED_BLUE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* UART1 Pins Configuration */
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;  /* PA9=TX, PA10=RX */
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void MX_USART1_UART_Init(void) {
    huart1.Instance = USART1;
    huart1.Init.BaudRate = UART_BAUD_RATE;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
}

static void MX_DMA_Init(void) {
    /* DMA Controller Clock Enable */
    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();
    
    /* UART DMA Configuration */
    hdma_usart1_rx.Instance = DMA2_Stream1;
    hdma_usart1_rx.Init.Request = DMA_REQUEST_USART1_RX;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
    
    HAL_DMA_Init(&hdma_usart1_rx);
    __HAL_LINKDMA(&huart1, hdmarx, hdma_usart1_rx);
}

static void MX_FSMC_Init(void) {
    /* FSMC Clock Enable for TFT Interface */
    __HAL_RCC_FMC_CLK_ENABLE();
    
    /* TFT FSMC Configuration would be generated by STM32CubeMX */
    /* This initializes the parallel interface for 320x240 TFT LCD */
}

static void MX_DMA2D_Init(void) {
    /* DMA2D Clock Enable for graphics acceleration */
    __HAL_RCC_DMA2D_CLK_ENABLE();
    
    hdma2d.Instance = DMA2D;
    hdma2d.Init.Mode = DMA2D_M2M;
    hdma2d.Init.ColorMode = DMA2D_RGB565;
    hdma2d.Init.OutputOffset = 0;
    
    HAL_DMA2D_Init(&hdma2d);
}

static void MX_USB_HOST_Init(void) {
    /* USB OTG HS Clock Enable */
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
    __HAL_RCC_USB_OTG_HS_ULPI_CLK_ENABLE();
    
    hhcd_USB_OTG_HS.Instance = USB_OTG_HS;
    hhcd_USB_OTG_HS.Init.Host_channels = 16;
    hhcd_USB_OTG_HS.Init.speed = HCD_SPEED_HIGH;
    hhcd_USB_OTG_HS.Init.dma_enable = ENABLE;
    hhcd_USB_OTG_HS.Init.phy_itface = HCD_PHY_ULPI;
    hhcd_USB_OTG_HS.Init.Sof_enable = DISABLE;
    hhcd_USB_OTG_HS.Init.low_power_enable = DISABLE;
    hhcd_USB_OTG_HS.Init.vbus_sensing_enable = DISABLE;
    hhcd_USB_OTG_HS.Init.use_dedicated_ep1 = DISABLE;
    hhcd_USB_OTG_HS.Init.use_external_vbus = DISABLE;
    hhcd_USB_OTG_HS.Init.dma_enable = ENABLE;
    
    if (HAL_HCD_Init(&hhcd_USB_OTG_HS) != HAL_OK) {
        Error_Handler();
    }
}

void LED_Init(void) {
    LED_SetGreen(0);
    LED_SetRed(0);
    LED_SetBlue(0);
}

void LED_SetGreen(uint8_t state) {
    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void LED_SetRed(uint8_t state) {
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void LED_SetBlue(uint8_t state) {
    HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Error_Handler(void) {
    LED_SetRed(1);
    while(1) {
        HAL_Delay(500);
    }
}

void assert_failed(uint8_t* file, uint32_t line) {
    LED_SetRed(1);
    while(1);
}
