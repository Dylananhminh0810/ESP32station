#include <stdio.h>
#include <esp_log.h>
#include "input_dev.h"
#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
input_callback_t input_callback = NULL;
static uint64_t _start, _stop, _pressTick;

static TimerHandle_t xTimers;
timeoutButton_t timeoutButton_callback = NULL;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    int gpio_num = (uint32_t) arg;
    uint32_t rtc = xTaskGetTickCountFromISR(); //ms
    if (gpio_get_level(gpio_num) == 0){ // bat dau bam
        _start = rtc;
        xTimerStart(xTimers,0);
    } else { // tha tay ra
        xTimerStop(xTimers,0);
        _stop = rtc;
        _pressTick = _stop - _start;
        input_callback(gpio_num, _pressTick);
    }
}

 static void vTimerCallback( TimerHandle_t xTimer )
 {
    uint32_t ID;

    /* Optionally do something if the pxTimer parameter is NULL. */
    configASSERT( xTimer );

    /* The number of times this timer has expired is saved as the
    timer's ID.  Obtain the count. */
    ID = ( uint32_t ) pvTimerGetTimerID( xTimer );
    if (ID == 0){
        timeoutButton_callback(BUTTON0);
    }
 }
void input_io_create(int pin, input_int_type_t type)
{
    gpio_config_t io_conf;
    //interrupt of rising edge
    io_conf.intr_type = type;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = (1ULL<<pin);
    //set as input mode    
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    io_conf.pull_down_en = 0;

    gpio_config(&io_conf);    

    //install gpio isr service
    gpio_install_isr_service(0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(pin, gpio_isr_handler, (void*) pin);    

    xTimers = xTimerCreate
                   ( /* Just a text name, not used by the RTOS
                     kernel. */
                     "TimerForTimeout",
                     /* The timer period in ticks, must be
                     greater than 0. */
                     5000/portTICK_PERIOD_MS,
                     /* The timers will auto-reload themselves
                     when they expire. */
                     pdTRUE,
                     /* The ID is used to store a count of the
                     number of times the timer has expired, which
                     is initialised to 0. */
                     ( void * ) 0,
                     /* Each timer calls the same callback when
                     it expires. */
                     vTimerCallback
    );

}

void input_set_callback(void * cb)
{
    input_callback = cb;
}
void input_set_timeout_callback(void * cb)
{
    timeoutButton_callback = cb;
}