//
// Created by leo.
//

#ifndef JSON_PARSER_H
#define JSON_PARSER_H
#include <optional>
#include <string_view>
#include <regex>

#include "json_type.h"

template<typename ...Ts>
struct overload : Ts... {
  using Ts::operator()...;
};

// explicit deduction guide
template<typename ...Ts>
overload(Ts...) -> overload<Ts...>;

class JsonParser {
private:
  static std::string_view skipChars;
  static std::regex numRegex;

  inline static bool isNumPrefix(char ch);
  template <typename T>
  static std::optional<T> tryParseNum(std::string_view str);
  static std::pair<std::variant<int,double>,size_t> parseNum(std::string_view str);
  static std::pair<std::string,size_t> parseStr(std::string_view str);
  static std::pair<std::vector<JsonObj>,size_t> parseList(std::string_view str);

  static std::pair<std::map<std::string, JsonObj>, size_t> parseDict(std::string_view str);

public:
  static std::pair<JsonObj,size_t> parse(std::string_view jsonStr);
  static std::string stringify(const JsonDict& dict);
  static std::string stringify(const JsonObj& obj);
};

inline bool JsonParser::isNumPrefix(char ch) {
  return ch=='-' || ch=='+' || (ch>='0' && ch<='9');
}

#endif //JSON_PARSER_H
