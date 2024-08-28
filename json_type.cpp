#include "json_type.h"

template<typename T>
bool JsonObj::is() const {
  return std::holds_alternative<T>(inner);
}

template <typename T>
T& JsonObj::get() const {
  return std::get<T>(inner);
}

JsonObj::JsonObj(const JsonList& lis): inner(lis) {}

JsonObj::JsonObj(const JsonDict& dict): inner(dict) {}

JsonObj::JsonObj(std::nullptr_t) : inner(nullptr) {}

JsonObj::JsonObj(bool b): inner(b) {}

JsonObj::JsonObj(int i): inner(i) {}

JsonObj::JsonObj(double d): inner(d) {}

JsonObj::JsonObj(const std::string& str): inner(str) {}

JsonObj::JsonObj(std::string&& str): inner(std::move(str)) {}
