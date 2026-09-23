// --------------------------------------------------------------------------------------
// Project: MicroManipulatorStepper
// License: MIT (see LICENSE file for full description)
//          All text in here must be included in any redistribution.
// Author:  M. S. (diffraction limited)
// --------------------------------------------------------------------------------------

#pragma once

#include <stdarg.h>

//*** MACRO *****************************************************************************

#define LOG_DEBUG(...) Logger::instance().debug(__VA_ARGS__)
#define LOG_INFO(...) Logger::instance().info(__VA_ARGS__)
#define LOG_WARNING(...) Logger::instance().warn(__VA_ARGS__)
#define LOG_ERROR(...) Logger::instance().error(__VA_ARGS__)
#define LOG_RAW(...) Logger::instance().raw(__VA_ARGS__)

//*** ENUM ******************************************************************************

enum class ELogLevel {
  DEBUG,
  INFO,
  WARN,
  ERROR,
  NONE
};

//*** CLASS *****************************************************************************

class Logger {
  public:
    static Logger& instance();

    void begin(unsigned long baudrate = 115200, bool wait_for_connection=false);
    void set_level(ELogLevel level);

    void debug(const char* fmt, ...);
    void info(const char* fmt, ...);
    void warn(const char* fmt, ...);
    void error(const char* fmt, ...);
    void raw(const char* fmt, ...);

  private:
    Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void log(ELogLevel level, const char* fmt, va_list args);
    const char* log_prefix(ELogLevel level);

    ELogLevel current_level = ELogLevel::DEBUG;
};

//*** FUNCTION **************************************************************************

void error_trap(const char* message="");

