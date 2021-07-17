#ifndef CHECK_RESULTS_H
#define CHECK_RESULTS_H

#include <cmath>
#include <limits>
#include <vector>

namespace helpers {

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    almost_equal(T x, T y, int ulp)
{
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::fabs(x-y) <= std::numeric_limits<T>::epsilon() * std::fabs(x+y) * ulp
        // unless the result is subnormal
        || std::fabs(x-y) < std::numeric_limits<T>::min();
}


template<class T>
typename std::enable_if<std::numeric_limits<T>::is_integer, bool>::type
    check_result(const T test, const T truth)
{
    if (test != truth) {
        std::cout << "Wrong result: Got " << test
            << " but expected " << truth << '.'
            << std::endl;
        return false;
    }
    return true;
}

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    check_result(const T test, const T truth)
{
    if (!almost_equal(test, truth, 2)) {
        std::cout << "Wrong result: Got " << test
            << " but expected " << truth << '.'
            << std::endl;
        return false;
    }
    return true;
}

template<class T>
typename std::enable_if<std::numeric_limits<T>::is_integer, bool>::type
    check_results(const T* test, const T* truth, const size_t size)
{
    for(size_t i = 0; i < size; ++i) {
        if (test[i] != truth[i]) {
            std::cout << "Wrong result at position " << i
                << ": Got " << test[i]
                << " but expected " << truth[i] << std::endl;
            return false;
        }
    }
    return true;
}

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    check_results(const T* test, const T* truth, const size_t size)
{
    for(size_t i = 0; i < size; ++i) {
        if (!almost_equal(test[i], truth[i], 2)) {
            std::cout << "Wrong result at position " << i
                << ": Got " << test[i]
                << " but expected " << truth[i] << std::endl;
            return false;
        }
    }
    return true;
}

template<class T>
bool check_results(const std::vector<T>& test, const std::vector<T>& truth) {
    if(test.size() != truth.size()) {
        std::cout << "Wrong result: Vectors differ in size!" << std::endl;
        return false;
    }

    return check_results(test.data(), truth.data(), truth.size());
}


} //namespace helpers

#endif
