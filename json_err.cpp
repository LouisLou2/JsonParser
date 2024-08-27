//
// Created by leo.
//

#include "json_err.h"

#include <iostream>
#include <iostream>

JsonError::JsonError(const JsonErrorCode code, const std::string& message): code(code), message(message) {}

JsonError::JsonError(const JsonErrorCode code, std::string&& message) : code(code), message(std::move(message)) {}

std::string_view JsonError::errorName(JsonErrorCode ec) {
  switch (ec) {
    case JsonErrorCode::BrokenStructure:
      return "BrokenStructure: ";
    case JsonErrorCode::InvalidNumberFormat:
      return "InvalidNumberFormat: ";
    case JsonErrorCode::InvalidStringFormat:
      return "InvalidStringFormat: ";
    case JsonErrorCode::InvalidBoolFormat:
      return "InvalidBoolFormat: ";
    case JsonErrorCode::InvalidNullFormat:
      return "InvalidNullFormat: ";
    default:
      return "Unknown error: ";
  }
}

void JsonError::report() const {
  std::cerr << errorName(code) << message << std::endl;
}

std::string JsonError::what() const {
  return std::string(errorName(code)) + message;
}

std::string JsonError::detail(JsonErrorCode code, const std::string_view message) {
  return std::string(errorName(code)) + std::string(message);
}
