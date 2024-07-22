
#include "strip_handler.h"
#include "wifi_connection_handler.h"
#include "nvs_flash.h" //non volatile storage
#include "mdns_service.h"


static const char *TAG = "main";

static esp_err_t get_handler(httpd_req_t *req)
{
    char *response_message = "URI Response ...";
    httpd_resp_send(req, response_message, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

void start_http_server()
{
    httpd_config_t server_config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server_handle = NULL;
    httpd_start(&server_handle, &server_config);

    httpd_uri_t uri_get = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = get_handler,
        .user_ctx = NULL};
    httpd_register_uri_handler(server_handle, &uri_get);
}

void app_main(void)
{
    ESP_LOGI(TAG, "flashing the non volatile storage");
    nvs_flash_init();
    ESP_LOGI(TAG, "connecting to wifi");
    wifi_connect();
    ESP_LOGI(TAG, "starting mdns");
    start_mdns_service();
    ESP_LOGI(TAG, "starting http server");
    start_http_server();
    ESP_LOGI(TAG, "setting up the led strip");
    strip_setup();
    bool led_on_off = true;

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

