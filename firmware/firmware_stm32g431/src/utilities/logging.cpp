// --------------------------------------------------------------------------------------
// Project: MicroManipulatorStepper
// License: MIT (see LICENSE file for full description)
//          All text in here must be included in any redistribution.
// Author:  M. S. (diffraction limited)
// --------------------------------------------------------------------------------------

#include "logging.h"
#include "Arduino.h"
#include <stdarg.h>

Logger& Logger::instance() {
  static Logger logger;
  return logger;
}

void Logger::begin(unsigned long baudrate, bool wait_for_connection) {
  Serial.begin(baudrate);
  if(wait_for_connection)
    while(!Serial);
}

void Logger::set_level(ELogLevel level) {
  current_level = level;
}

void Logger::debug(const char* fmt, ...) {
  if (current_level <= ELogLevel::DEBUG) {
      va_list args;
      va_start(args, fmt);
      log(ELogLevel::DEBUG, fmt, args);
      va_end(args);
  }
}

void Logger::info(const char* fmt, ...) {
  if (current_level <= ELogLevel::INFO) {
      va_list args;
      va_start(args, fmt);
      log(ELogLevel::INFO, fmt, args);
      va_end(args);
  }
}

void Logger::warn(const char* fmt, ...) {
  if (current_level <= ELogLevel::WARN) {
      va_list args;
      va_start(args, fmt);
      log(ELogLevel::WARN, fmt, args);
      va_end(args);
  }
}

void Logger::error(const char* fmt, ...) {
  if (current_level <= ELogLevel::ERROR) {
      va_list args;
      va_start(args, fmt);
      log(ELogLevel::ERROR, fmt, args);
      va_end(args);
  }
}

void Logger::raw(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    va_list args_copy;
    va_copy(args_copy, args);
    log(ELogLevel::NONE, fmt, args_copy);
    va_end(args_copy);

    va_end(args);
}
/*
void Logger::raw(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  log(ELogLevel::NONE, fmt, args);
  va_end(args);
}*/

void Logger::log(ELogLevel level, const char* fmt, va_list args) {
  char buf[128];  // Adjust size as needed
  vsnprintf(buf, sizeof(buf), fmt, args);

  Serial.print(log_prefix(level));
  Serial.println(buf);
}

const char* Logger::log_prefix(ELogLevel level) {
  switch (level) {
    case ELogLevel::DEBUG: return "D)";
    case ELogLevel::INFO:  return "I)";
    case ELogLevel::WARN:  return "W)";
    case ELogLevel::ERROR: return "E)";
    default:               return "";
  }
}

void error_trap(const char* message) {
  while(true) {
    HAL_Delay(1000);
    if(message != nullptr)
      LOG_ERROR(message);
  }
}