#include <stdio.h>
#include "pico/stdlib.h"   // stdlib 
#include "hardware/irq.h"  // interrupts
#include "hardware/pwm.h"  // pwm 
#include "hardware/sync.h" // wait for interrupt 
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/uart.h"
#include "pico/binary_info.h"
 
#define AUDIO_PIN 28  


#define PIN_ADC 26;
#define ADC_VREF 3.3
#define ADC_RANGE (1 << 12)
#define ADC_CONVERT (ADC_VREF / (ADC_RANGE - 1))

int BTN_PIN = 14;

int main(void) {

    stdio_init_all();
    gpio_set_function(AUDIO_PIN, GPIO_FUNC_PWM);

    int audio_pin_slice = pwm_gpio_to_slice_num(AUDIO_PIN);

    // Setup PWM interrupt to fire when PWM cycle is complete
    pwm_clear_irq(audio_pin_slice);
    pwm_set_irq_enabled(audio_pin_slice, true);

    pwm_config config = pwm_get_default_config();

    pwm_config_set_clkdiv(&config, 8.0f); 
    pwm_config_set_wrap(&config, 250); 
    pwm_init(audio_pin_slice, &config, true);
    pwm_set_gpio_level(AUDIO_PIN, 0);
    uint16_t valor_adc;

    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_up(BTN_PIN);
    adc_init();
    adc_gpio_init(PIN_ADC);
    adc_select_input(0);
    while(1) {
        if (!gpio_get(BTN_PIN)) {
        valor_adc = adc_read();
            pwm_set_gpio_level(AUDIO_PIN, valor_adc);
        } else {
                pwm_set_gpio_level(AUDIO_PIN, 0);
        }
        sleep_us(120);
    }
}