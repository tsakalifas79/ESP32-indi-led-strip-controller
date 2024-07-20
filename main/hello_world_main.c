
#include "strip_handler.h"

static const char *TAG = "main";

void app_main(void)
{
    strip_setup();
    bool led_on_off = true;

    ESP_LOGI(TAG, "Start LED strip");
    while (1)
    {
        if (led_on_off)
        {
            update_map();
        }
        else
        {
            /* Set all LED off to clear all pixels */
            ESP_ERROR_CHECK(led_strip_clear(led_strip));
            ESP_LOGI(TAG, "LED OFF!");
        }
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}
