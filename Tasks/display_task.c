#include "display_task.h"
#include "tft_display.h"
#include "config.h"

extern QueueHandle_t screen_queue;

void DisplayTask(void *pvParameters) {
    uint8_t screen_buffer[SCREEN_BUFFER_SIZE];
    BaseType_t ret;
    
    /* Initialize TFT display */
    TFT_Init();
    TFT_Clear(0x0000);  /* Black background */
    
    while (1) {
        /* Wait for screen data */
        ret = xQueueReceive(screen_queue, screen_buffer, pdMS_TO_TICKS(100));
        
        if (ret == pdTRUE) {
            /* Render screen buffer to TFT */
            TFT_RenderScreen(screen_buffer);
        } else {
            /* No data - keep display as is */
        }
        
        vTaskDelay(pdMS_TO_TICKS(33));  /* ~30 FPS */
    }
}
