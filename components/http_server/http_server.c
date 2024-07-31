#include <stdio.h>
#include "http_server.h"


static esp_err_t light_on_handler(httpd_req_t *req)
{
    char *response_message = "Light ON!";
    httpd_resp_send(req, response_message, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t light_off_handler(httpd_req_t *req)
{
    char *response_message = "Light OFF!";
    httpd_resp_send(req, response_message, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

void start_http_server()
{
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