
#include "strip_handler.h"
#include "wifi_connection_handler.h"
#include "nvs_flash.h" //non volatile storage
#include "mdns.h" 

static const char *TAG = "main";

void start_mdns_service()
{
    //initialize mDNS service
    esp_err_t err = mdns_init();
    if (err) {
        printf("MDNS Init failed: %d\n", err);
        return;
    }

    //set hostname
    mdns_hostname_set("esp-hero");
    //set default instance
    mdns_instance_name_set("ESP smart light controller");
}

void app_main(void)
{

    nvs_flash_init();
    wifi_connect();
    start_mdns_service();
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

