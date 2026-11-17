#include <iostream>
#include <vector>
#include <printf.h>

#include "generator.h"
#include "stft.h"

static const double samplerate = 16000.0;

int main()
{

    const auto wave = generateWave(
        samplerate,
        2 * samplerate,
        {{
            .hz = 200,
            .phase = 0.0,
            .amplitude = 1.2
        }, {
            .hz = 1200,
            .phase = std::numbers::pi,
            .amplitude = 3.1
        }, {
            .hz = 3400,
            .phase = 1.5,
            .amplitude = 1.95,
        }, {
            .hz = 2300,
            .phase = 0.1,
            .amplitude = 1.0,
        }}
    );

    double max = 0.0;
    double min = 0.0;
    for (const auto& sample : wave)
    {
        if (sample > max) max = sample;
        if (sample < min) min = sample;
    }

    printf("Wave created with largest sample %f and lowest sample %f\n", max, min);

    // Overlap frames by 75%
    const size_t frameSize = 400;
    const size_t hop       = frameSize * 0.25;
    STFT stft { samplerate, frameSize, hop };
    stft.analyze(wave);

    return 0;
}
