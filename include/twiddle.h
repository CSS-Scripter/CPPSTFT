#pragma once

#include <vector>
#include <cmath>
#include <numbers>

struct Twiddle
{
    std::vector<double> cos;
    std::vector<double> sin;

    Twiddle(size_t size)
    : cos (size, 0.0)
    , sin (size, 0.0)
    {
        const double twoPi = 2.0 * std::numbers::pi;
        for (size_t i = 0; i < size; ++i)
        {
            cos[i] = std::cos(twoPi * i / size);
            sin[i] = std::sin(twoPi * i / size);
        }
    }
};
