#include "json_type.h"

#include <cstdint>
#include <stdexcept>

#include "json_err.h"
#include "json_parser.h"

template<typename T>
bool JsonObj::is() const {
  return std::holds_alternative<T>(inner);
}

template <typename T>
T& JsonObj::get() const {
  return std::get<T>(inner);
}

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

JsonDict::JsonDict(JsonDictInner&& dict) : dict(std::forward<std::map<std::string, JsonObj>>(dict)) {}

void JsonDict::set(const std::string& key, const JsonObj& val) {
  dict.insert_or_assign(key, val);
}
void JsonDict::set(const std::string& key, JsonObj&& val) {
  dict.insert_or_assign(key,std::forward<JsonObj>(val));
}

const std::map<std::string, JsonObj>& JsonDict::get()const {
    return dict;
}

std::optional<bool> JsonDict::getBool(const std::string& key) const {
  auto it = dict.find(key);
  if(it==dict.end()) return std::nullopt;
  if(std::holds_alternative<bool>(it->second.inner)) {
    return std::get<bool>(it->second.inner);
  }
  const auto index = it->second.inner.index();
  throw std::invalid_argument(
    JsonError::detail(
        JsonErrorCode::ValueTypeMismatch,
      "type index:" + std::to_string(index) + " " + std::string(JsonObj::type(index)) + " is not bool."
      )
    );
}

std::optional<int> JsonDict::getInt(const std::string& key) const {
    auto it = dict.find(key);
    if (it == dict.end()) return std::nullopt;
    if (std::holds_alternative<int>(it->second.inner)) {
        return std::get<int>(it->second.inner);
    }
    const auto index = it->second.inner.index();
    throw std::invalid_argument(
        JsonError::detail(
            JsonErrorCode::ValueTypeMismatch,
            "type index:" + std::to_string(index) + " " + std::string(JsonObj::type(index)) + " is not int."
        )
    );
}

std::optional<double> JsonDict::getDouble(const std::string& key) const {
    auto it = dict.find(key);
    if (it == dict.end()) return std::nullopt;
    if (std::holds_alternative<double>(it->second.inner)) {
        return std::get<double>(it->second.inner);
    }
    const auto index = it->second.inner.index();
    throw std::invalid_argument(
        JsonError::detail(
            JsonErrorCode::ValueTypeMismatch,
            "type index:" + std::to_string(index) + " " + std::string(JsonObj::type(index)) + " is not double."
        )
    );
}

std::optional<std::string> JsonDict::getString(const std::string& key) const {
    auto it = dict.find(key);
    if (it == dict.end()) return std::nullopt;
    if (std::holds_alternative<std::string>(it->second.inner)) {
        return std::get<std::string>(it->second.inner);
    }
    const auto index = it->second.inner.index();
    throw std::invalid_argument(
        JsonError::detail(
            JsonErrorCode::ValueTypeMismatch,
            "type index:" + std::to_string(index) + " " + std::string(JsonObj::type(index)) + " is not string."
        )
    );
}

std::optional<JsonList> JsonDict::getList(const std::string& key) const {
    auto it = dict.find(key);
    if (it == dict.end()) return std::nullopt;
    if (std::holds_alternative<JsonList>(it->second.inner)) {
        return std::get<JsonList>(it->second.inner);
    }
    const auto index = it->second.inner.index();
    throw std::invalid_argument(
        JsonError::detail(
            JsonErrorCode::ValueTypeMismatch,
            "type index:" + std::to_string(index) + " " + std::string(JsonObj::type(index)) + " is not list."
        )
    );
}

std::optional<JsonDict> JsonDict::getDict(const std::string& key) const {
    auto it = dict.find(key);
    if (it == dict.end()) return std::nullopt;
    if (std::holds_alternative<JsonDict>(it->second.inner)) {
        return std::get<JsonDict>(it->second.inner);
    }
    const auto index = it->second.inner.index();
    throw std::invalid_argument(
        JsonError::detail(
            JsonErrorCode::ValueTypeMismatch,
            "type index:" + std::to_string(index) + " " + std::string(JsonObj::type(index)) + " is not dict."
        )
    );
}

std::string JsonDict::dump() const {
    return JsonParser::stringify(JsonDict(dict));
}