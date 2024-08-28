#include "json_parser.h"
#include <iostream>
#include <chrono>

int main() {
  auto begin = std::chrono::high_resolution_clock::now();
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
  auto [obj, eaten] = JsonParser::parse(str);
  auto end = std::chrono::high_resolution_clock::now();
  std::cout<<"Time: "<<std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count()<<std::endl;
  // JsonDict dict = std::get<JsonDict>(obj.inner);
  // auto book=dict["book"];
  // if(std::holds_alternative<std::nullptr_t>(book.inner)) {
  //   std::cout<<"book is null"<<std::endl;
  // }
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
            [&] (std::string val) {
                std::cout<<val<<std::endl;
            },
          [&] (auto val) {
              std::cout<<JsonParser::stringify(val)<<std::endl;
          }
      },obj.inner);
  auto encoded = JsonParser::stringify(obj);
  return 0;
}