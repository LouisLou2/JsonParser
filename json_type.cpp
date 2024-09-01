#include "json_type.h"

#include <cstdint>
#include <stdexcept>

#include "json_err.h"
#include "json_parser.h"

std::array<std::string_view,7> JsonObj::typeNames = {"null","bool","int","double","string","JsonList(std::vector<JsonObj>)","JsonDict(std::map<std::string,JsonObj>)"};

JsonObj::JsonObj(const JsonList& lis): inner(lis) {}

JsonObj::JsonObj(const JsonDict& dict): inner(dict) {}

JsonObj::JsonObj(const JsonDictInner& dict):JsonObj(JsonDict(dict)){}

JsonObj::JsonObj(std::nullptr_t) : inner(nullptr) {}

JsonObj::JsonObj(bool b): inner(b) {}

JsonObj::JsonObj(int i): inner(i) {}

JsonObj::JsonObj(double d): inner(d) {}

JsonObj::JsonObj(const std::string& str): inner(str) {}

JsonObj::JsonObj(std::string&& str): inner(std::move(str)) {}

JsonObj::JsonObj(JsonList&& lis) : inner(std::move(lis)) {}

JsonObj::JsonObj(JsonDictInner&& dict):JsonObj(JsonDict(std::move(dict))){}
JsonObj::JsonObj(JsonDict&& dict) : inner(std::move(dict)) {}

bool JsonObj::support(const uint8_t index) {
  return index>=0 && index<7;
}

std::string_view JsonObj::type(const uint8_t index){
  if (index < 0 || index>=7) {
    return "Unsupported Type";
  }
  return JsonObj::typeNames[index];
}

// JsonDict
JsonDict::JsonDict(const JsonDictInner& dict) : dict(dict) {}

JsonDict::JsonDict(JsonDictInner&& dict) : dict(std::forward<JsonDictInner>(dict)) {}

void JsonDict::set(const std::string& key, const JsonObj& val) {
  dict.insert_or_assign(key, val);
}
void JsonDict::set(const std::string& key, JsonObj&& val) {
  dict.insert_or_assign(key,std::forward<JsonObj>(val));
}

const std::map<std::string, JsonObj>& JsonDict::data()const {
    return dict;
}

std::string JsonDict::dump() const {
    return JsonParser::stringify(JsonDict(dict));
}