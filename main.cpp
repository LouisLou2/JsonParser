#include "json_parser.h"
#include <iostream>
#include <chrono>

void testParseStr() {
    // test1: parse string
    std::string_view str = R"JSON({
    "orderId": "123456789",
    "customer": {
        "customerId": "987654321",
        "name": "John Doe",
        "email": "johndoe@example.com",
        "phone": "+1-800-555-1234",
        "address": {
            "street": "123 Main St",
            "city": "Springfield",
            "state": "IL",
            "postalCode": "62701",
            "country": "USA"
        }
    },
    "orderDate": "2024-08-27T14:35:00Z",
    "status": "Shipped",
    "items": [
        {
            "itemId": "A001",
            "productName": "Laptop",
            "quantity": 1,
            "price": 1299.99,
            "currency": "USD",
            "options": {
                "color": "Silver",
                "storage": "512GB SSD",
                "warranty": "2 years"
            }
        },
        {
            "itemId": "A002",
            "productName": "Wireless Mouse",
            "quantity": 2,
            "price": 29.99,
            "currency": "USD",
            "options": {
                "color": "Black"
            }
        },
        {
            "itemId": "A003",
            "productName": "USB-C Adapter",
            "quantity": 3,
            "price": 19.99,
            "currency": "USD"
        }
    ],
    "payment": {
        "method": "Credit Card",
        "cardNumber": "**** **** **** 1234",
        "billingAddress": {
            "street": "123 Main St",
            "city": "Springfield",
            "state": "IL",
            "postalCode": "62701",
            "country": "USA"
        }
    },
    "shipment": {
        "carrier": "UPS",
        "trackingNumber": "1Z9999999999999999",
        "estimatedDelivery": "2024-08-30T18:00:00Z",
        "shippingAddress": {
            "street": "123 Main St",
            "city": "Springfield",
            "state": "IL",
            "postalCode": "62701",
            "country": "USA"
        }
    },
    "totalAmount": {
        "subtotal": 1369.94,
        "tax": 102.74,
        "shipping": 15.00,
        "grandTotal": 1487.68,
        "currency": "USD"
    },
    "comments": "Please deliver between 9 AM and 5 PM."
    })JSON";
    auto begin = std::chrono::high_resolution_clock::now();
    // parsed
    auto [obj, eaten] = JsonParser::parse(str);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout<<"Time: "<<std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count()<<std::endl;
    // cout stringified obj
    std::visit(
      overload{
          [&] (int val) {
              std::cout<<val<<std::endl;
          },
          [&] (double val) {
              std::cout<<val<<std::endl;
          },
            [&] (bool val) {
                std::cout<<val<<std::endl;
            },
            [&] (std::nullptr_t val) {
                std::cout<<"null"<<std::endl;
            },
            [&] (const std::string& val) {
                std::cout<<val<<std::endl;
            },
          [&] (auto val) {
              std::cout<<JsonParser::stringify(val)<<std::endl;
          }
      },obj.inner);
    // get k-v
    auto dict = obj.get<JsonDict>();
    if (const auto orderId = dict.get<std::string>("orderId")) {
        std::cout<<"orderId: "<<*orderId<<std::endl;
    }else {
        std::cout<<"orderId is null"<<std::endl;
    }
}

void testStringify() {
    // test2: stringify for map
    JsonDict obj;
    obj.set("age",12);
    obj.set("name","leo");
    obj.set("weight",65.5);
    obj.set("days",std::vector<JsonObj>{1,2,3,4,5});
    std::map<std::string,JsonObj> scores;
    scores["math"]=100;
    scores["english"]=90.5;
    obj.set("scores",scores);
    auto it=obj.get<int>("age");
    auto encoded = JsonParser::stringify(obj);
    std::cout<<encoded<<std::endl;
}
int main() {
    testParseStr();
    testStringify();
    return 0;
}