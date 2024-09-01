#ifndef JSON_TYPE_H
#define JSON_TYPE_H

#include "json_err.h"

#include <stdexcept>
#include <map>
#include <optional>
#include <string>
#include <variant>
#include <array>
#include <cstdint>
#include <vector>


enum class JsonType {
  Null=0,
  Bool=1,
  Int=2,
  Double=3,
  String=4,
  List=5,
  Dict=6
};

struct JsonObj;
using JsonDictInner = std::map<std::string, JsonObj>;
using JsonList = std::vector<JsonObj>;

struct JsonDict {
private:
  JsonDictInner dict;
public:
  // constructor
  explicit JsonDict(const JsonDictInner& dict);
  explicit JsonDict(JsonDictInner&& dict);
  JsonDict()=default;
  // insert or assign
  void set(const std::string& key, const JsonObj& val);
  void set(const std::string& key, JsonObj&& val);
  // getter
  [[nodiscard]] const std::map<std::string, JsonObj>& data() const;
  // get value
  template<typename T>
  [[nodiscard]] std::optional<T> get(const std::string& key);
  // dump
  [[nodiscard]] std::string dump() const;
};

struct JsonObj {
  std::variant<
    std::nullptr_t,//0
    bool,//1
    int,//2
    double,//3
    std::string,//4
    JsonList,//5
    JsonDict//6
  >inner; //注意类型参数填写过程和enum class JsonType一致

  static std::array<std::string_view,7> typeNames;
  // constructor
  JsonObj() = default;  // 默认构造函数
  JsonObj(const JsonList& lis);
  JsonObj(const JsonDict& dict);
  JsonObj(const JsonDictInner& dict);
  JsonObj(std::nullptr_t);
  JsonObj(bool b);
  JsonObj(int i);
  JsonObj(double d);
  JsonObj(const std::string& str);
  // move constructor
  JsonObj(std::string&& str);
  JsonObj(JsonList&& lis);
  JsonObj(JsonDictInner&& dict);
  JsonObj(JsonDict&& dict);


  template <typename T>
  bool is() const;

  template<typename T>
  T& get();

  static std::string_view type(uint8_t index);
  static bool support(uint8_t index);
};

template<typename T>
bool JsonObj::is() const {
  return std::holds_alternative<T>(inner);
}

template <typename T>
T& JsonObj::get(){
  return std::get<T>(inner);
}

template<typename T>
std::optional<T> JsonDict::get(const std::string& key){
  auto it = dict.find(key);
  if (it == dict.end()) return std::nullopt;
  if (std::holds_alternative<T>(it->second.inner)) {
    return std::get<T>(it->second.inner);
  }
  const auto index = it->second.inner.index();
  throw std::invalid_argument(
      JsonError::detail(
          JsonErrorCode::ValueTypeMismatch,
          "type index("
          + std::to_string(index)
          + ") for "
          + std::string(JsonObj::type(index))
          + " is not "
          + typeid(T).name()
      )
  );
}

#endif //JSON_TYPE_H
