#include <iostream>
#include <tuple>

template <typename... Ts>
class Foo {
private:
        std::tuple<Ts...> m_args;
public:
       template<typename... Args>
       Foo(Args&&... args) : m_args(std::make_tuple(std::forward<Args>(args)...))
       {
           std::cout << "test" << std::endl;
       }
 };

 int main() {
     int x = 10;
     int y = 20;
     
     Foo<int, int> foo(x, y);
    //  Foo<int, char, std::string> func(1,'c',"ss");

 }