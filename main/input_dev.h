#ifndef __INPUT_DEV_H
#define __INPUT_DEV_H
#include "esp_err.h"
#include "hal/gpio_types.h"

#define BUTTON0 GPIO_NUM_0

typedef enum {
    LO_TO_HI = 1,
    HI_TO_LO = 2,
    ANY_EDGE = 3 
}   input_int_type_t;
typedef void (*input_callback_t) (int,uint64_t);
typedef void (*timeoutButton_t) (int);
void input_io_create(int pin, input_int_type_t type);
void input_set_callback(void *cb);
void input_set_timeout_callback(void * cb);
#endif