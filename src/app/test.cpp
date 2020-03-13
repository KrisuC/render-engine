#include <vector>
#include <iostream>
#include <variant>
#include <type_traits>

using namespace std;

class Base {

};

template <int i>
using Shit = std::conditional_t<(i > 0),
        int,
        std::conditional_t<(i < 20),
                char,
                double>>;

template <int i, typename Fuck = Shit<i>>
class Derived : public Base {

};

int main() {
    static_assert(std::is_base_of_v<Base, Derived<1>>, "shit");
}
