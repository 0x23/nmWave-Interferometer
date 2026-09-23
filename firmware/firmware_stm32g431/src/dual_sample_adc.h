#pragma once

#include "stm32g4xx_hal.h"

class CDualSampleADC {
public:
    CDualSampleADC(GPIO_TypeDef* port1, uint8_t pin1, uint32_t adc1_channel,
                   GPIO_TypeDef* port2, uint8_t pin2, uint32_t adc2_channel);

    void begin();
    void read(uint16_t* value1, uint16_t* value2);

private:
    void init_gpio();
    void init_adc1();
    void init_adc2();

    GPIO_TypeDef* _port1;
    uint8_t _pin1;
    uint32_t _adc1_channel;

    GPIO_TypeDef* _port2;
    uint8_t _pin2;
    uint32_t _adc2_channel;

    ADC_HandleTypeDef _hadc1;
    ADC_HandleTypeDef _hadc2;
};