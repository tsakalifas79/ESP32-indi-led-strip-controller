/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "esp_log.h"
#include "esp_err.h"

// GPIO assignment
#define LED_STRIP_BLINK_GPIO 12
// Numbers of the LED in the strip
#define LED_STRIP_LED_NUMBERS 240
// 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define LED_STRIP_RMT_RES_HZ (10 * 1000 * 1000)

#define COLORS 4

static const char *TAG = "example";

led_strip_handle_t led_strip;


uint8_t pixels[LED_STRIP_LED_NUMBERS][COLORS];
uint8_t color[COLORS];
uint8_t max_intensity = 30;
uint8_t speed = 0;
uint8_t update = 0;

led_strip_handle_t configure_led(void)
{
    // LED strip general initialization, according to your led board design
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_STRIP_BLINK_GPIO,    // The GPIO that connected to the LED strip's data line
        .max_leds = LED_STRIP_LED_NUMBERS,         // The number of LEDs in the strip,
        .led_pixel_format = LED_PIXEL_FORMAT_GRBW, // Pixel format of your LED strip
        .led_model = LED_MODEL_SK6812,             // LED strip model
        .flags.invert_out = false,                 // whether to invert the output signal
    };

    // LED strip backend configuration: RMT
    led_strip_rmt_config_t rmt_config = {
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
        .rmt_channel = 0,
#else
        .clk_src = RMT_CLK_SRC_DEFAULT,        // different clock source can lead to different power consumption
        .resolution_hz = LED_STRIP_RMT_RES_HZ, // RMT counter clock frequency
        .flags.with_dma = false,               // DMA feature is available on ESP target like ESP32-S3
#endif
    };

    // LED Strip object handle
    led_strip_handle_t led_strip;
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    ESP_LOGI(TAG, "Created LED strip object with RMT backend");
    return led_strip;
}

uint8_t r_mod = 1;
uint8_t g_mod = 2;
uint8_t b_mod = 3;
uint8_t w_mod = 4;

void motion()
{
        color[0] = (r_mod + color[0]) % max_intensity;
        color[1] = (g_mod + color[1]) % max_intensity;
        color[2] = (b_mod + color[2]) % max_intensity;
        color[3] = (w_mod + color[3]) % max_intensity;
}

void fade_out(int i)
{            
    for (size_t j = 0; j < COLORS; j++)
    {  
        if(pixels[i][COLORS] > 0){
            pixels[i][COLORS] = pixels[i][COLORS] - 1;
            pixels[i][COLORS] %= max_intensity;
        };
    }
}


void update_map()
{
    update = (update + 1) % LED_STRIP_LED_NUMBERS;

    /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
    for (int i = 0; i < LED_STRIP_LED_NUMBERS; i++)
    {
        fade_out(i);
        if(i == update){
            motion();
            for (size_t j = 0; j < COLORS; j++)
            {  
                pixels[i][j] = color[j];
            }
        }

        ESP_ERROR_CHECK(led_strip_set_pixel_rgbw(led_strip, i, pixels[i][0], pixels[i][1], pixels[i][2], pixels[i][3]));
    }

    /* Refresh the strip to send data */
    ESP_ERROR_CHECK(led_strip_refresh(led_strip));
}

void app_main(void)
{
    led_strip = configure_led();
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
