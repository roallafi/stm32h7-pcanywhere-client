#include "tft_init.h"
#include "config.h"

/* TFT Control Pins */
#define TFT_CS_PORT     GPIOD
#define TFT_CS_PIN      GPIO_PIN_7
#define TFT_DC_PORT     GPIOD
#define TFT_DC_PIN      GPIO_PIN_11
#define TFT_WR_PORT     GPIOD
#define TFT_WR_PIN      GPIO_PIN_5
#define TFT_RD_PORT     GPIOD
#define TFT_RD_PIN      GPIO_PIN_4
#define TFT_RST_PORT    GPIOE
#define TFT_RST_PIN     GPIO_PIN_1

/* FSMC Configuration */
#define FSMC_ADDR_BASE  0x60000000
#define FSMC_DATA_ADDR  0x60020000

void TFT_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    /* TFT Control Pins */
    GPIO_InitStruct.Pin = TFT_CS_PIN | TFT_DC_PIN | TFT_WR_PIN | TFT_RD_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = TFT_RST_PIN;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

void TFT_FSMC_Config(void) {
    FSMC_NORSRAM_TimingTypeDef Timing = {0};
    FSMC_NORSRAM_InitTypeDef Init = {0};
    
    __HAL_RCC_FMC_CLK_ENABLE();
    
    Init.NSBank = FSMC_NORSRAM_BANK1;
    Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
    Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
    Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
    Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
    Init.WaitSignalActive = FSMC_WAIT_TIMING_DURING_ALL_CYCLES;
    Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
    Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
    Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
    Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
    Init.ContinuousClock = FSMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
    
    Timing.AddressSetupTime = 15;
    Timing.AddressHoldTime = 15;
    Timing.DataSetupTime = 60;
    Timing.BusTurnAroundDuration = 0;
    Timing.CLKDivision = 0;
    Timing.DataLatency = 0;
    Timing.AccessMode = FSMC_ACCESS_MODE_A;
    
    HAL_FSMC_NORSRAM_Init(FSMC_NORSRAM_DEVICE, &Init);
    HAL_FSMC_NORSRAM_Timing_Init(FSMC_NORSRAM_DEVICE, &Timing, FSMC_NORSRAM_BANK1);
}

void TFT_Reset(void) {
    HAL_GPIO_WritePin(TFT_RST_PORT, TFT_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(TFT_RST_PORT, TFT_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(100);
}

void TFT_WriteCommand(uint16_t cmd) {
    HAL_GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, GPIO_PIN_RESET);  /* DC=0 for command */
    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_RESET);  /* CS=0 */
    *(volatile uint16_t *)FSMC_ADDR_BASE = cmd;
    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET);    /* CS=1 */
}

void TFT_WriteData(uint16_t data) {
    HAL_GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, GPIO_PIN_SET);    /* DC=1 for data */
    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_RESET);  /* CS=0 */
    *(volatile uint16_t *)FSMC_DATA_ADDR = data;
    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET);    /* CS=1 */
}

void TFT_SetWindow(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    /* Set column address window */
    TFT_WriteCommand(0x2A);  /* Column Address Set */
    TFT_WriteData((x >> 8) & 0xFF);
    TFT_WriteData(x & 0xFF);
    TFT_WriteData(((x + width - 1) >> 8) & 0xFF);
    TFT_WriteData((x + width - 1) & 0xFF);
    
    /* Set row address window */
    TFT_WriteCommand(0x2B);  /* Row Address Set */
    TFT_WriteData((y >> 8) & 0xFF);
    TFT_WriteData(y & 0xFF);
    TFT_WriteData(((y + height - 1) >> 8) & 0xFF);
    TFT_WriteData((y + height - 1) & 0xFF);
    
    /* Start Memory Write */
    TFT_WriteCommand(0x2C);
}
