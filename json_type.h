#ifndef JSON_TYPE_H
#define JSON_TYPE_H
#include <map>
#include <string>
#include <variant>
#include <vector>

struct JsonObj;

using JsonList = std::vector<JsonObj>;
using JsonDict = std::map<std::string, JsonObj>;

struct JsonObj {
  std::variant<
    std::nullptr_t,
    bool,
    int,
    double,
    std::string,
    JsonList,
    JsonDict
  >inner{};

  template <typename T>
  bool is() const;

  template<typename T>
  T& get() const;
};


#endif //JSON_TYPE_H
