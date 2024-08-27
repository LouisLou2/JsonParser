#include "json_type.h"

template<typename T>
bool JsonObj::is() const {
  return std::holds_alternative<T>(inner);
}

template <typename T>
T& JsonObj::get() const {
  return std::get<T>(inner);
}