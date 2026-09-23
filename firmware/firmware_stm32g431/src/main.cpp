#include "config.h"
#include "dual_sample_adc.h"
#include "hardware/MCP3426_adc.h"
#include "stm32g4xx_hal.h"
#include "Arduino.h"
#include "Wire.h"
#include "utilities/logging.h"


CDualSampleADC phase_adc(
                        GPIOB, 0, ADC_CHANNEL_15,   // ADC1_IN15
                        GPIOA, 7, ADC_CHANNEL_4
                        );

MCP3426_Adc temperature_adc(PC9, PC8, 0x68); // MCP3426 default address

void blink(int count, int period_ms) {
  for(int i=0; i<count; i++) {
    digitalWrite(PIN_LED1, true);
    delay(period_ms / 2);
    digitalWrite(PIN_LED1, false);
    delay(period_ms / 2); 
  }
}

//extern "C" void SysTick_Handler(void) {
//    HAL_IncTick();
//}

#include <Wire.h>

void i2c_scan(TwoWire &wire) {
  Serial.println("I2C scan...");

  uint8_t count = 0;
  for (uint8_t addr = 1; addr < 127; addr++) {
    wire.beginTransmission(addr);
    if (wire.endTransmission() == 0) {
      Serial.print("Found device at 0x");
      if (addr < 16) Serial.print("0");
      Serial.println(addr, HEX);
      count++;
    }
  }

  if (count == 0)
    Serial.println("No I2C devices found");
}



void setup() {
  Serial.begin(115200);
 // while(!Serial);
  Serial.println("STM32G431 Started...");

  Serial.print("SystemCoreClock: ");
  Serial.println(SystemCoreClock);
  Serial.print("Crystal Freq: ");
  Serial.println(HSE_VALUE);

  // setup temperature adc
  Serial.println("Configure Temperature sensing");
  temperature_adc.begin();
  if(temperature_adc.configure(MCP3426_Adc::GAIN_1, MCP3426_Adc::ONE_SHOT, MCP3426_Adc::RES_16BIT)) 
    Serial.println("[ok]");
  else
    Serial.println("[failed]");
  temperature_adc.start_conversion(MCP3426_Adc::CH1);

  // setup photodiode adc 
  phase_adc.begin();
  analogReadResolution(12);  // STM32 ADC is 12-bit by default, but just in case

  // setup laser current dac 
  pinMode(LD_CURRENT, OUTPUT); 
  analogWriteResolution(12);
  analogWrite(PA5, 1024);

  // --- PWM Configuration (Explicitly specifying PWM functions for clarity) ---
  pinMode(PIN_HEATER, OUTPUT);
  analogWriteFrequency(100);

  analogWrite(PIN_LED1, 20);
  analogWrite(PIN_HEATER, 2048);
}

#include <cmath>

float ntc_voltage_to_temp(float voltage, float R_pullup, float vcc=3.3, float R0 = 100000.0f, float B = 3950.0f) {
    if (voltage >= vcc) voltage = vcc-1e-7f;

    // Step 1: Compute NTC resistance
    float R_ntc = (voltage * R_pullup) / (vcc - voltage);

    // Step 2: Apply Beta formula
    float T0 = 298.15f; // 25°C in Kelvin
    float tempK = 1.0f / ( (1.0f/T0) + (1.0f/B) * log(R_ntc / R0) );

    // Step 3: Convert to Celsius
    float tempC = tempK - 273.15f;
    return tempC;
}


float t = 0.0f;
void loop() {
  uint16_t phase_a=0, phase_b=0;
  phase_adc.read(&phase_b, &phase_a);

 // phase_a = analogRead(PIN_PHASE_A);
 // phase_b = analogRead(PIN_PHASE_B);
  //phase_a = ADC1_Read();

  analogWrite(PA5, int(sin(t*5.0f)*128+2048));

  float tv = 0.0f;
  //temperature_adc.start_conversion(MCP3426_Adc::CH1);
  //delay(100);
  if(temperature_adc.read_voltage(tv)) {
    temperature_adc.start_conversion(MCP3426_Adc::CH1);
    tv = ntc_voltage_to_temp(tv, 82000);
    Serial.print(">T: ");
    Serial.println(tv, 12);
  }

  Serial.print(">PhaseA: ");
  Serial.println(phase_a);
  Serial.print(">PhaseB: ");
  Serial.println(phase_b);  
  // Serial.println("");  
  delay(10);
  t += 0.01f;
}