#include <iostream>
#include <cmath>
#include "vec.h"

using std::cout;
using std::cin;
using std::endl;

int main() {

    vec3 v(3,4,3);

    cout << "Vector norm is: " << v.norm() << endl;


    cout << "Normalizing!" << endl;

    v.normalize();

    cout << "Vector norm is: " << v.norm() << endl;

    return 0;
}