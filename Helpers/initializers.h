#ifndef INITIALIZERS_H
#define INITIALIZERS_H

#include <algorithm>
#include <random>
#include <vector>

namespace helpers {


void random_init(float * x, size_t length) {
    std::mt19937 engine(42);
    std::uniform_real_distribution<float> dist(-1.0, +1.0);

    std::generate(x, x+length, [&]{ return dist(engine); });
}

void random_init(int * x, size_t length, int max = 100) {
    std::mt19937 engine(42);
    std::uniform_int_distribution<int> dist(0, max);

    std::generate(x, x+length, [&]{ return dist(engine); });
}


} //namespace helpers

#endif
