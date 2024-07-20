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

led_strip_handle_t led_strip;

uint8_t pixels[LED_STRIP_LED_NUMBERS][COLORS];
uint8_t color[COLORS];
uint8_t max_intensity = 30;
uint8_t speed = 0;
uint8_t update = 0;

uint8_t r_mod = 1;
uint8_t g_mod = 2;
uint8_t b_mod = 3;
uint8_t w_mod = 4;

led_strip_handle_t configure_led(void);
void motion();
void fade_out(int i);
void update_map();
void strip_setup();