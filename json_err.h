//
// Created by leo.
//

#ifndef JSON_ERR_H
#define JSON_ERR_H

#include <string_view>
#include <string>

enum class JsonErrorCode {
  BrokenStructure=0,
  InvalidNumberFormat=1,
  InvalidStringFormat=2,
  InvalidBoolFormat=3,
  InvalidNullFormat=4,

  // for getValue
  ValueTypeMismatch=5,
  KeyNotFound=6
};

struct JsonError {
  JsonErrorCode code;
  std::string message;

  JsonError(JsonErrorCode code, const std::string& message);
  // move constructor
  JsonError(JsonErrorCode code, std::string&& message);

  static std::string_view errorName(JsonErrorCode ec);
  void report() const;
  [[nodiscard]] std::string what() const;

  static std::string detail(JsonErrorCode code, std::string_view msg);
};

#endif //JSON_ERR_H
