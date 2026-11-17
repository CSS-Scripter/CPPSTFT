#pragma once

#include <vector>
#include <cstddef>
#include <numbers>
#include <cmath>
#include <span>
#include <numeric>
#include <printf.h>

#include "frequency.h"


inline void normalizeWave(
    std::span<double> wave
)
{
    double peak = 0.0;
    for (double sample : wave)
    {
        peak = std::max(peak, std::abs(sample));
    }

    if (peak > 1.0) {
        for (double& sample : wave)
        {
            sample /= peak;
        }
    }
}


inline std::vector<double> generateWave(
    double samplerate,
    size_t windowSize,
    const std::vector<Frequency>& frequencies
)
{
    constexpr double twoPi = 2.0 * std::numbers::pi;

    std::vector<double> window(windowSize, 0.0);
    for (size_t t = 0; t < windowSize; ++t)
    {
        const double time = static_cast<double>(t) / samplerate;

        auto& sample = window[t];
        for (const auto& f : frequencies)
        {
            const double omega = twoPi * f.hz;
            sample += f.amplitude * std::sin(omega * time + f.phase);
        }
    }

    normalizeWave(window);

    return window;
}
