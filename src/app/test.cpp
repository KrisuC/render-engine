#include <vector>
#include <iostream>
#include <variant>
#include <type_traits>

using namespace std;

int main() {
    string path = "abcde/fuck.png";
    std::string suffix = path.substr(path.find_last_of('.'), path.size());
    cout << suffix << endl;
}
