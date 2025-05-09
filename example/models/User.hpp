#include <sstream>
#include <string>

class User {
public:
  std::string name;
  int age;
  std::string email;
  
  User(std::string n, int a, std::string e) : name(n), age(a), email(e) {}

  std::string to_json() const {
    std::ostringstream oss;
    
    oss << "{";
    oss << "\"name\": \"" << name << "\",";
    oss << "\"age\": " << age << ",";
    oss << "\"email\": \"" << email << "\"";
    oss << "}";

    return oss.str();
  }
};
