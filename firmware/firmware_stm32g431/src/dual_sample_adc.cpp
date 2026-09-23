#include "dual_sample_adc.h"

CDualSampleADC::CDualSampleADC(GPIO_TypeDef* port1, uint8_t pin1, uint32_t adc1_channel,
                               GPIO_TypeDef* port2, uint8_t pin2, uint32_t adc2_channel)
    : _port1(port1), _pin1(pin1), _adc1_channel(adc1_channel),
      _port2(port2), _pin2(pin2), _adc2_channel(adc2_channel)
{}

void CDualSampleADC::begin() {
  __HAL_RCC_ADC12_CLK_ENABLE();

  init_gpio();
  init_adc2();  // slave first
  init_adc1();  // then master

//  HAL_ADCEx_Calibration_Start(&_hadc1, ADC_SINGLE_ENDED);
//  HAL_ADCEx_Calibration_Start(&_hadc2, ADC_SINGLE_ENDED);
  HAL_Delay(20);  // Delay 1ms to be safe
}

void CDualSampleADC::read(uint16_t* value1, uint16_t* value2) {
    HAL_ADC_Start(&_hadc2);                  // Start master ADC
    HAL_ADC_Start(&_hadc1);                  // Start master ADC

    HAL_ADC_PollForConversion(&_hadc2, HAL_MAX_DELAY);

    *value1 = HAL_ADC_GetValue(&_hadc1);
    *value2 = HAL_ADC_GetValue(&_hadc2);

 //   HAL_ADC_Stop(&_hadc2);
 //   HAL_ADC_Stop(&_hadc1);
}

void CDualSampleADC::init_gpio() {
    // Enable GPIO clocks as needed
    if (_port1 == GPIOA || _port2 == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    if (_port1 == GPIOB || _port2 == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    if (_port1 == GPIOC || _port2 == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
    // Add more ports if your MCU has more GPIO ports used
    return;
    GPIO_InitTypeDef gpio_init = {0};
    gpio_init.Mode = GPIO_MODE_ANALOG;
    gpio_init.Pull = GPIO_NOPULL;

    gpio_init.Pin = 1 << _pin1;  // HAL expects pin mask, so 1 << pin number
    HAL_GPIO_Init(_port1, &gpio_init);

    gpio_init.Pin = 1 << _pin2;
    HAL_GPIO_Init(_port2, &gpio_init);
}

void CDualSampleADC::init_adc1() {
    ADC_MultiModeTypeDef multimode = {0};
    ADC_ChannelConfTypeDef sConfig = {0};

    _hadc1.Instance = ADC1;
    _hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;  // fastest ADC clock, APB2 must ≤ 80 MHz
    _hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    _hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    _hadc1.Init.ContinuousConvMode = DISABLE;
    _hadc1.Init.DiscontinuousConvMode = DISABLE;
    _hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    _hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    _hadc1.Init.NbrOfConversion = 1;
    _hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    _hadc1.Init.LowPowerAutoWait = DISABLE;            // recommended explicit disable
    if(HAL_ADC_Init(&_hadc1) != HAL_OK) {// Start ADC1 only
      // Serial.println("HAL_ADC_Init failed");  
    }
    

    sConfig.Channel = _adc1_channel;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_6CYCLES_5;  // minimal sample time for fastest conversion
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    if(HAL_ADC_ConfigChannel(&_hadc1, &sConfig) != HAL_OK) { // Start ADC1 only
      // Serial.println("HAL_ADC_ConfigChannel failed");
    }

    multimode.Mode = ADC_DUALMODE_REGSIMULT;
    multimode.DMAAccessMode = ADC_DMAACCESSMODE_DISABLED;
    multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_1CYCLE;  // minimal delay between dual samples
    HAL_ADCEx_MultiModeConfigChannel(&_hadc1, &multimode);
}

void CDualSampleADC::init_adc2() {
    ADC_ChannelConfTypeDef sConfig = {0};

    _hadc2.Instance = ADC2;
    _hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;  // match ADC1 clock
    _hadc2.Init.Resolution = ADC_RESOLUTION_12B;
    _hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
    _hadc2.Init.ContinuousConvMode = DISABLE;
    _hadc2.Init.DiscontinuousConvMode = DISABLE;
    _hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    _hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    _hadc2.Init.NbrOfConversion = 1;
    _hadc2.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    _hadc2.Init.LowPowerAutoWait = DISABLE;            // recommended explicit disable
    HAL_ADC_Init(&_hadc2);

    sConfig.Channel = _adc2_channel;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_6CYCLES_5;  // minimal sample time
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    HAL_ADC_ConfigChannel(&_hadc2, &sConfig);
}
