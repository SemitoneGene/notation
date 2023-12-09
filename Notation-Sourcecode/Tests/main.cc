#include <iostream>
#include "Geometry.h"

int main(int argc, char *argv[]) {
    const auto p = komp::Point{ 1.0, 2.0 };
    std::cout << "Hello Komp { " << p.x << ", " << p.y << " }" << std::endl;
}
