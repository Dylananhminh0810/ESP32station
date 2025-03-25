/* LEDC (LED Controller) basic example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "driver/gpio.h"
#include "ledc_app.h"
#include "driver/ledc.h"

// #define LEDC_TIMER              LEDC_TIMER_0
// #define LEDC_MODE               LEDC_LOW_SPEED_MODE
// #define LEDC_OUTPUT_IO          (5) // Define the output GPIO
// #define LEDC_CHANNEL            LEDC_CHANNEL_0
// #define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits // --> co the set duty cycle 8192 muc --> tu 0 -> 1
// #define LEDC_DUTY               (4095) // Set duty to 50%. ((2 ** 13) - 1) * 50% = 4095
// #define LEDC_FREQUENCY          (5000) // Frequency in Hertz. Set frequency at 5 kHz


void ledc_init(void){
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .timer_num        = LEDC_TIMER_1,
        .duty_resolution  = LEDC_TIMER_13_BIT,
        .freq_hz          = 5000,  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
}
void ledc_add_pin(int pin, int channel){
    
    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel= {
        .speed_mode     = LEDC_HIGH_SPEED_MODE,
        .channel        = channel,
        .timer_sel      = LEDC_TIMER_1,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = pin,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0 // den thoi diem hpoint nay thi no se tu muc 0 len 1 
        //PWM tu ban dau xuat muc cao
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}
//0 - 8191
//0 - 100
void ledc_make_duty(int channel, int duty){
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, channel, duty*8192/100); 
    // Update duty to apply the new value
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, channel);
}

