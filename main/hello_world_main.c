

#include "nvs_flash.h" //non volatile storage
#include "mdns_service.h"
#include "http_server.h"
#include "strip_handler.h"
#include "wifi_connection_handler.h"





static const char *TAG = "main";




void app_main(void){
    ESP_LOGI(TAG, "flashing the non volatile storage");
    nvs_flash_init();
    ESP_LOGI(TAG, "connecting to wifi");
    wifi_connect();
    ESP_LOGI(TAG, "starting mdns");
    start_mdns_service();
    ESP_LOGI(TAG, "starting http server");
    bool led_on_off = true;
    start_http_server(&led_on_off);
    ESP_LOGI(TAG, "setting up the led strip");
    strip_setup();

    ESP_LOGI(TAG, "Start applying logic on the LED strip");
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
