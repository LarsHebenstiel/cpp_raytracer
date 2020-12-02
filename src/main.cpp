#include<iostream>
#include "vec.hpp"

using std::cout;
using std::cin;
using std::endl;

int main() {
    vec3d v(1,2,3);
    vec3d u(3,4,1);

    cout << v << endl;
    cout << u << endl;

    cout << v + u << endl;
    cout << v - u << endl;

    cout << v + v + u << endl;
    cout << v * 2 + u << endl;

    return 0;
}