
#include "strip_handler.h"
#include "wifi_connection_handler.h"
#include "nvs_flash.h" //non volatile storage




static const char *TAG = "main";


void app_main(void)
{

    nvs_flash_init();
    wifi_connect();
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

