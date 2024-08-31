#ifndef JSON_TYPE_H
#define JSON_TYPE_H
#include <map>
#include <optional>
#include <string>
#include <variant>
#include <array>
#include <cstdint>
#include <vector>

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
  [[nodiscard]] const std::map<std::string, JsonObj>& get() const;
  // get value
  [[nodiscard]] std::optional<bool> getBool(const std::string& key) const;
  [[nodiscard]] std::optional<int> getInt(const std::string& key) const;
  [[nodiscard]] std::optional<double> getDouble(const std::string& key) const;
  [[nodiscard]] std::optional<std::string> getString(const std::string& key) const;
  [[nodiscard]] std::optional<JsonList> getList(const std::string& key) const;
  [[nodiscard]] std::optional<JsonDict> getDict(const std::string& key) const;
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
  T& get() const;

  static std::string_view type(uint8_t index);
  static bool support(uint8_t index);
};

enum class JsonType {
  Null=0,
  Bool=1,
  Int=2,
  Double=3,
  String=4,
  List=5,
  Dict=6
};

#endif //JSON_TYPE_H
