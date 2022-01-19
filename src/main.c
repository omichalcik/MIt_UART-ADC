#include "stm8s.h"
#include "milis.h"
#include "spse_stm8.h"
/*#include "delay.h"*/

#include <stdio.h>
#include "stm8s_adc2.h"
#include "uart1.h"

#define _ISOC99_SOURCE
#define _GNU_SOURCE

#define LED_PORT GPIOC
#define LED_PIN  GPIO_PIN_5
#define LED_HIGH   GPIO_WriteHigh(LED_PORT, LED_PIN)
#define LED_LOW  GPIO_WriteLow(LED_PORT, LED_PIN)
#define LED_REVERSE GPIO_WriteReverse(LED_PORT, LED_PIN)

#define BTN_PORT GPIOE
#define BTN_PIN  GPIO_PIN_4
#define BTN_PUSH (GPIO_ReadInputPin(BTN_PORT, BTN_PIN)==RESET) 


void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz
    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(BTN_PORT, BTN_PIN, GPIO_MODE_IN_FL_NO_IT);
    ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL4, DISABLE); //PB4
    ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL5, DISABLE); //PB5
    // nastavíme clock pro ADC (16MHz / 4 = 4MHz)
    ADC2_PrescalerConfig(ADC2_PRESSEL_FCPU_D4);
    // volíme zarovnání výsledku (typicky vpravo, jen vyjmečně je výhodné vlevo)
    ADC2_AlignConfig(ADC2_ALIGN_RIGHT);
    // nasatvíme multiplexer na některý ze vstupních kanálů
    ADC2_Select_Channel(ADC2_CHANNEL_4);
    // rozběhneme AD převodník
    ADC2_Cmd(ENABLE);
    // počkáme než se AD převodník rozběhne (~7us)
    ADC2_Startup_Wait();

    init_milis();
    init_uart1();
}


int main(void)
{
    uint32_t time = 0;
    uint16_t ADCx = ADC_get(ADC2_CHANNEL_4);

    setup();

    while (1) {

        if (milis() - time > 333 && BTN_PUSH) {
            LED_REVERSE; 
            time = milis();
            printf("%ld %d\r\n", time, ADCx);
        }

        /*LED_REVERSE;*/
        /*delay_ms(333);*/
        /*printf("Funguje to!!!\n");*/
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
