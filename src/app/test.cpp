#include <vector>
#include <iostream>
#include <variant>

using namespace std;

template <typename T>
class MyClass {
public:
    T value;
    MyClass *ptr;
};

using FC = MyClass<float>;
using IC = MyClass<int>;

int main() {
    variant<FC, IC> v;
    std::get<IC>(v).value;
    v.index();
}
