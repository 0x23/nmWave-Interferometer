#include "MCP3426_adc.h"

#include "MCP3426_adc.h"

MCP3426_Adc::MCP3426_Adc(int pin_sda, int pin_scl, uint8_t address)
    : m_i2c(pin_sda, pin_scl), m_address(address), m_config(0), m_scale(1), m_scale_voltage(1), m_resolution(RES_16BIT)
{}

void MCP3426_Adc::begin() {
    m_i2c.setClock(400000); // 400 kHz
    m_i2c.begin();
}

bool MCP3426_Adc::configure(Gain gain, Mode mode, Resolution res) {
    m_config = 0;
    m_config |= (gain & 0x03);        // bits 0-1: G1-G0 PGA gain
    m_config |= (res & 0x03) << 2;    // bits 2-3: S1-S0 sample rate / resolution
    m_config |= (mode & 0x01) << 4;   // bit 4: O/C conversion mode
    m_config |= 0x80;                 // bit 7 RDY=1 starts conversion (one-shot mode)


    m_resolution = res;
    switch (m_resolution) {
      case RES_12BIT: m_scale = 1.0f/(1<<11); break;
      case RES_14BIT: m_scale = 1.0f/(1<<13); break;
      case RES_16BIT: m_scale = 1.0f/(1<<15); break;
    }

    float v_ref=2.048f;
    switch (gain) {
      case GAIN_1: m_scale_voltage = v_ref*m_scale; break;
      case GAIN_2: m_scale_voltage = v_ref*m_scale*2; break;
      case GAIN_4: m_scale_voltage = v_ref*m_scale*4; break;
      case GAIN_8: m_scale_voltage = v_ref*m_scale*8; break;
    }

    return write_config();
}

bool MCP3426_Adc::start_conversion(Channel channel) {
    m_config &= ~0x10;               // clear channel bit
    if (channel == CH2)
        m_config |= 0x10;

    m_config |= 0x80;                // RDY=1 to start conversion
    return write_config();
}

bool MCP3426_Adc::read_raw(int16_t &value) {
    m_i2c.requestFrom((int)m_address, 3);
    if (m_i2c.available() < 3) return false; // conversion not ready

    uint8_t msb = m_i2c.read();
    uint8_t lsb = m_i2c.read();
    uint8_t config = m_i2c.read();

    m_config = config;
    if (config & 0x80) return false; // RDY=1 → not ready

    value = ((uint16_t)msb << 8) | lsb;
    return true;
}

bool MCP3426_Adc::read(float &v) {
    int16_t raw;
    if (!read_raw(raw)) return false;

    v = float(raw) * m_scale;
    return true;
}

// Read voltage in range -1 to 1 (non-blocking), returns true if a new value is available
bool MCP3426_Adc::read_voltage(float& voltage) {
    int16_t raw;
    if (!read_raw(raw)) return false;

    voltage = float(raw) * m_scale_voltage;
    return true;
}

bool MCP3426_Adc::write_config() {
    m_i2c.beginTransmission(m_address);
    m_i2c.write(m_config);
    return m_i2c.endTransmission() == 0;
}