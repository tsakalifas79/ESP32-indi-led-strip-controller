idf_component_register(
    SRCS "wifi_connection_handler.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_wifi
    REQUIRES nvs_flash)
