#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>

class MCP3426_Adc {
  public:
    enum Channel { CH1 = 0, CH2 = 1 };
    enum Gain    { GAIN_1 = 0, GAIN_2 = 1, GAIN_4 = 2, GAIN_8 = 3 };
    enum Mode    { ONE_SHOT = 0, CONTINUOUS = 1 };
    enum Resolution { RES_12BIT = 0, RES_14BIT = 1, RES_16BIT = 2};

    // Constructor
    MCP3426_Adc(int pin_sda, int pin_scl, uint8_t address = 0x68);

    void begin();

    // Configure ADC gain and mode
    bool configure(Gain gain, Mode mode, Resolution res);

    // Start conversion on a specific channel (non-blocking)
    bool start_conversion(Channel channel);

    // Check if conversion is ready and read raw ADC value (non-blocking)
    bool read_raw(int16_t &value);

    // Read value in range -1 to 1 (non-blocking), returns true if a new value is available
    bool read(float& value);

    // Read voltage in range -1 to 1 (non-blocking), returns true if a new value is available
    bool read_voltage(float& voltage);

  private:
    bool write_config();

  public:
    TwoWire m_i2c;   // I2C bus to use
    uint8_t m_address;
    uint8_t m_config;
    Resolution m_resolution;
    float m_scale;
    float m_scale_voltage;
};