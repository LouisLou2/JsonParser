//
// Created by leo.
//
#include <charconv>
#include <unordered_map>
#include "json_parser.h"

#include "json_err.h"

std::string_view JsonParser::skipChars = " \t\n\r\v\f\0";
std::regex JsonParser::numRegex{"[+-]?[0-9]+(\\.[0-9]*)?([eE][+-]?[0-9]+)?"};

template <typename T>
std::optional<T> JsonParser::tryParseNum(std::string_view str) {
  T value;
  auto res = std::from_chars(str.data(),str.end(),value);
  // auto it1=str.end();
  // auto it2=str.data()+str.size();
  // auto it3=res.ptr;
  if (res.ec==std::errc() && res.ptr==str.end()) {
    return value;
  }
  return std::nullopt;
}


std::pair<std::variant<int,double>,size_t> JsonParser::parseNum(std::string_view str) {
  std::cmatch match;
  if(!std::regex_search(str.data(),str.end(),match,JsonParser::numRegex) ){
    // 在此函数调用jsonParser::isNumPrefix，但仍可能regex搜索不到，例如-x这种仅有负号的情况
    throw std::invalid_argument(JsonError::detail(JsonErrorCode::InvalidNumberFormat,str));
  }
  std::string numStr = match.str();
  if (auto num = JsonParser::tryParseNum<int>(numStr)) {
    return std::make_pair(*num,numStr.size());
  }
  if (auto num = JsonParser::tryParseNum<double>(numStr)) {
    return std::make_pair(*num,numStr.size());
  }
  throw std::invalid_argument(JsonError::detail(JsonErrorCode::InvalidNumberFormat,numStr));
}

std::pair<std::string, size_t> JsonParser::parseStr(std::string_view str) {
  // example: "hello world"(with ")
  std::string res;
  size_t i = 1; // 0是引号
  while( i<str.size() && str[i]!='"' ) {
    res+=str[i];
    ++i;
  }
  if(str[i]!='"') {
    throw std::invalid_argument(JsonError::detail(JsonErrorCode::InvalidStringFormat,str));
  }
  ++i;
  return  {res,i};
}

std::pair<std::vector<JsonObj>, size_t> JsonParser::parseList(std::string_view str) {
  // example: [1,2,3.3,"d",{"a":1},[1,2]]
  std::vector<JsonObj> lis;
  size_t i = 1;// 0是[
  while(i<str.size()) {
    if(str[i]==']') {
      ++i;
      break;
    }
    if(str[i]==' '|| str[i]=='\n'|| str[i]==',') {
      ++i;
      continue;
    }
    auto [obj,eaten] = parse(str.substr(i));
    if(!eaten) {
      throw std::invalid_argument(JsonError::detail(JsonErrorCode::BrokenStructure,str));
    }
    i+=eaten;
    lis.push_back(std::move(obj));
  }
  if (str[i-1]!=']') {
    throw std::invalid_argument(JsonError::detail(JsonErrorCode::BrokenStructure,str));
  }
  return {lis,i};
}

std::pair<JsonDictInner, size_t> JsonParser::parseDict(std::string_view str) {
  JsonDictInner dict;
  size_t i = 1; // 0是{
  bool nowKey = true;
  std::string keyStr;
  while(i<str.size()) {
    if (str[i]=='}') {
      ++i;
      break;
    }
    if (str[i]==' '|| str[i]=='\n' || str[i]==',') {
      ++i;
      continue;
    }
    if (nowKey) {
      auto [key,eaten] = parseStr(str.substr(i));
      // consume blank chars
      i+=eaten;
      while(str[i]==' ') {
        ++i;
      }
      if (str[i]!=':') {
        throw std::invalid_argument(JsonError::detail(JsonErrorCode::BrokenStructure,str));
      }
      keyStr=key;
      i+=1;
      nowKey = false;
    }else {
      // consume blank chars
      while(str[i]==' ') ++i;
      auto [val,eaten] = parse(str.substr(i));
      // consume blank chars and ','
      i+=eaten;
      dict.insert_or_assign(keyStr,std::move(val));
      while(str[i]==' ' || str[i]==',') ++i;
      nowKey = true;
    }
  }
  if (str[i-1]!='}') {
    throw std::invalid_argument(JsonError::detail(JsonErrorCode::BrokenStructure,str));
  }
  return {dict,i};
}

std::pair<JsonObj, size_t> JsonParser::parse(std::string_view jsonStr) {
  // empty json
  if (jsonStr.empty()) {
    return {JsonObj(std::nullptr_t{}),0};
  }
  // skip blank chars
  size_t offset = jsonStr.find_first_not_of(JsonParser::skipChars);
  if (offset!=0 && offset!=std::string_view::npos) {
    auto [obj,eaten] = parse(jsonStr.substr(offset));
    return {std::move(obj),eaten+offset};
  }
  // number
  if(JsonParser::isNumPrefix(jsonStr[0])) {
    auto res = parseNum(jsonStr);
    if (std::holds_alternative<int>(res.first)) {
      return {JsonObj(std::get<int>(res.first)),res.second};
    } else {
      return {JsonObj(std::get<double>(res.first)),res.second};
    }
  }
  // bool:true
  if(jsonStr[0]=='t') {
    const auto boolStr = jsonStr.substr(0,4);
    // 后面四个字符是true
    if(boolStr=="true") {
      return {JsonObj(true),4};
    }
    throw std::invalid_argument(JsonError::detail(JsonErrorCode::InvalidBoolFormat,boolStr));
  }
  // bool:false
  if(jsonStr[0]=='f') {
    const auto boolStr = jsonStr.substr(0,5);
    // 后面五个字符是false
    if(boolStr=="false") {
      return {JsonObj(false),5};
    }
    throw std::invalid_argument(JsonError::detail(JsonErrorCode::InvalidBoolFormat,boolStr));
  }
  // null
  if(jsonStr[0]=='n') {
    const auto nullStr = jsonStr.substr(0,4);
    // 后面四个字符是null
    if(nullStr=="null") {
      return {JsonObj(std::nullptr_t{}),4};
    }
    throw std::invalid_argument(JsonError::detail(JsonErrorCode::InvalidNullFormat,nullStr));
  }
  // string
  if(jsonStr[0]=='"') {
    auto res = parseStr(jsonStr);
    return {JsonObj(std::move(res.first)),res.second};
  }
  // list
  if(jsonStr[0]=='[') {
    auto res = parseList(jsonStr);
    return {JsonObj(std::move(res.first)),res.second};
  }
  // object
  if(jsonStr[0]=='{') {
    auto res = parseDict(jsonStr);
    return {JsonObj(JsonDict(std::move(res.first))),res.second};
  }
  throw std::invalid_argument(JsonError::detail(JsonErrorCode::BrokenStructure,jsonStr));
}

std::string JsonParser::stringify(const JsonDict& jsonDict) {
  std::string dictr="{";
  const auto& dict = jsonDict.get();
  for (const auto& kv: dict) {
    dictr+="\""+kv.first+"\":";
    dictr+=stringify(kv.second);
    dictr+=",";
  }
  // drop the last ','
  dictr.pop_back();
  dictr+="}";
  return dictr;
}


std::string JsonParser::stringify(const JsonObj& obj) {
  switch (static_cast<uint8_t>(obj.inner.index())) {
  case static_cast<uint8_t>(JsonType::Null):
    return "null";
  case static_cast<uint8_t>(JsonType::Bool):
    if (std::get<bool>(obj.inner)) {
      return "true";
    } else {
      return "false";
    }
  case static_cast<uint8_t>(JsonType::Int):
    return std::to_string(std::get<int>(obj.inner));
  case static_cast<uint8_t>(JsonType::Double):
    return std::to_string(std::get<double>(obj.inner));
  case static_cast<uint8_t>(JsonType::String):
    return "\""+std::get<std::string>(obj.inner)+"\"";
  case static_cast<uint8_t>(JsonType::List): {
    std::string listr="[";
    const auto lis = std::get<JsonList>(obj.inner);
    for (const auto& e: lis) {
      listr+=stringify(e);
      listr+=",";
    }
    // drop the last ','
    listr.pop_back();
    listr+="]";
    return listr;
  }
  case static_cast<uint8_t>(JsonType::Dict): {
    const auto dict = std::get<JsonDict>(obj.inner);
    return stringify(dict);
  }
  default:
    throw std::invalid_argument(JsonError::detail(JsonErrorCode::BrokenStructure,""));
  }
}
