#include <vector>
#include <iostream>

using namespace std;

int main() {
    vector<int> vec {1, 2, 3, 4, 5, 6};
    int& ref_to_elem = vec[0];
    for (int i = 0; i < 10; i++) {
        vec.push_back(0);
    }
    assert(&ref_to_elem == &vec[0]);
}
