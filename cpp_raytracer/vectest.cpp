#include <iostream>
#include "vec.h"

using std::cout;
using std::cin;
using std::endl;

int main() {

    vec3 v(1,1,1);

    cout << v.norm() << endl;

    return 0;
}