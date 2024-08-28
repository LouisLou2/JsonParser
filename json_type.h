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
    std::nullptr_t,//0
    bool,//1
    int,//2
    double,//3
    std::string,//4
    JsonList,//5
    JsonDict//6
  >inner; //注意类型参数填写过程和enum class JsonType一致
  // constructor
  JsonObj(const JsonList& lis);
  JsonObj(const JsonDict& dict);
  JsonObj(std::nullptr_t);
  JsonObj(bool b);
  JsonObj(int i);
  JsonObj(double d);
  JsonObj(const std::string& str);
  // move
  JsonObj(std::string&& str);


  template <typename T>
  bool is() const;

  template<typename T>
  T& get() const;
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
