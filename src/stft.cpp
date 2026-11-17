#include "stft.h"

#include <printf.h>
#include <iostream>
#include <span>
#include <cmath>
#include <numbers>


STFT::STFT(
    double samplerate,
    size_t frameSize,
    size_t hop
)
: _samplerate { samplerate    }
, _frameSize  { frameSize     }
, _hop        { hop           }
, _bins       ( _createBins() )
, _twiddle    ( frameSize     )
, _hann       ( _createHann() )
{
}


std::vector<Bin> STFT::_createBins()
{
    const size_t nBins = static_cast<size_t>(_frameSize / 2.0) + 1;
    std::vector<Bin> bins(nBins);

    const double hzStep = _samplerate / _frameSize;
    for (size_t k = 0; k < nBins; ++k)
    {
        bins[k] = {
            .hz = k * hzStep,
            .real = 0.0,
            .imag = 0.0
        };
    }

    return bins;
}

std::vector<double> STFT::_createHann()
{
    constexpr double twoPi = 2.0 * std::numbers::pi;

    std::vector<double> window (_frameSize);
    for (size_t i = 0; i < _frameSize; ++i)
    {
        window[i] = 0.5 - 0.5 * std::cos(twoPi * i / _frameSize);
    }

    return window;
}



void STFT::analyze(
    const std::vector<double>& wave
)
{
    if (wave.size() < _frameSize)
    {
        printf("Cannot analyze wave shorter than 1 frame");
        return;
    }

    const auto frameIndices = _frameStartIndices(wave.size());
    printf("Analyzing %ld frames\n", frameIndices.size());

    for (const auto& frameStart : frameIndices)
    {
        std::span<const double> frame {
            wave.begin() + frameStart,
            _frameSize
        };

        printf("Analyzing frame\n");
        for (size_t k = 0; k < _bins.size(); ++k)
        {
            auto& bin = _bins[k];
            bin.real = 0.0;
            bin.imag = 0.0;
            for (size_t i = 0; i < frame.size(); ++i)
            {
                const size_t t = (k * i) % _frameSize;
                bin.real += frame[i] * _twiddle.cos[t] * _hann[i];
                bin.imag -= frame[i] * _twiddle.sin[t] * _hann[i];
            }
            const double power = bin.power();
            if (power > 100)
                printf("\t%fhz = %f\n", bin.hz, power);
        }
        printf("\n");
    }
}


std::vector<size_t> STFT::_frameStartIndices(
    const size_t windowSize
)
{
    const size_t nFrames = static_cast<size_t>(1 + (windowSize - _frameSize) / _hop);
    std::vector<size_t> indices (nFrames);

    for (size_t i = 0; i < nFrames; ++i)
    {
        indices[i] = i * _hop;
    }

    return indices;
}
