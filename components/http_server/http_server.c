#include <stdio.h>
#include "http_server.h"

static const char *TAG = "strip_handler";
bool* exposed_led_on_off_pointer;

static esp_err_t light_on_handler(httpd_req_t *req)
{
    char *response_message = "Light ON!";
    httpd_resp_send(req, response_message, HTTPD_RESP_USE_STRLEN);
    *exposed_led_on_off_pointer = true;

    return ESP_OK;
}

static esp_err_t light_off_handler(httpd_req_t *req)
{
    char *response_message = "Light OFF!";
    *exposed_led_on_off_pointer = false;
    httpd_resp_send(req, response_message, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

void start_http_server(bool* pointer_led_on_off)
{
    exposed_led_on_off_pointer = pointer_led_on_off;
    ESP_LOGI(TAG, "pointer led_on_off %d", *pointer_led_on_off);
    // ESP_LOGI(TAG, "value led_on_off %d", *pointer_led_on_off);
    httpd_config_t server_config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server_handle = NULL;
    httpd_start(&server_handle, &server_config);

    httpd_uri_t uri_light_on = {
        .uri = "/on",
        .method = HTTP_GET,
        .handler = light_on_handler,
        .user_ctx = NULL};
    httpd_register_uri_handler(server_handle, &uri_light_on);

    httpd_uri_t uri_light_off = {
        .uri = "/off",
        .method = HTTP_GET,
        .handler = light_off_handler,
        .user_ctx = NULL};
    httpd_register_uri_handler(server_handle, &uri_light_off);

}