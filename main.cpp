#include "json_parser.h"
#include <iostream>

int main() {
  std::string_view str = R"JSON({ "key":192, "arr":[3.3,2,3.3], "res": false, "book": null })JSON";
  auto [obj, eaten] = JsonParser::parse(str);
  JsonDict dict = std::get<JsonDict>(obj.inner);
  auto book=dict["book"];
  if(std::holds_alternative<std::nullptr_t>(book.inner)) {
    std::cout<<"book is null"<<std::endl;
  }
  return 0;
}