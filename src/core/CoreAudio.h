//
// Created by artemious on 25.02.24.
//

#ifndef AEDIT_COREAUDIO_H
#define AEDIT_COREAUDIO_H

#include <cstdlib>
#include "CoreInfo.h"
#include <portaudio.h>

extern "C" int paCycleCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData);

namespace ae {

    class CoreAudio {
        static int32_t SamplingFrequency;

    public:
        static cycledBuffer testData;
        static PaStream * stream;

        static PaError init();
        static PaError terminate();
        static PaError startPlayback();
        static PaError stopPlayback();
    };

} // ae

#endif //AEDIT_COREAUDIO_H
