#include "RandomNumber.h"

#include <random>

size_t lsmf::random::GetRandomNumber(size_t min, size_t max)

{
    if (min == max)
    {
        return min;
    }
    thread_local std::random_device rd;
    thread_local std::default_random_engine eng(rd());
    thread_local std::uniform_int_distribution<size_t> distr(min, max);

    return distr(eng);

}
