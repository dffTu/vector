#include <iostream>
#include "vector.h"

int main() {
    Vector<Vector<int>> v(5, Vector<int>(5, 1));
    for (int i = 0; i < 5; ++i) {
        std::cout << v[i][0] << "\n";
    }
    return 0;
}