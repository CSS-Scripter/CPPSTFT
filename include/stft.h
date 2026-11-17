#pragma once

#include <cstddef>
#include <vector>
#include <cmath>

#include "twiddle.h"

struct Bin
{
    double hz;
    double real;
    double imag;

    double power() const noexcept
    {
        return real * real + imag * imag;
    }
};


class STFT
{
    double _samplerate;

    size_t _frameSize;
    size_t _hop;

    std::vector<Bin> _bins;
    Twiddle _twiddle;

    const std::vector<double> _hann;


public:
    STFT(double samplerate, size_t frameSize, size_t hop);

    void analyze(const std::vector<double>& wave);

private:
    std::vector<Bin> _createBins();
    std::vector<double> _createHann();

    std::vector<size_t> _frameStartIndices(const size_t windowSize);
};
