//
// Created by artemious on 25.02.24.
//

#include "CoreAudio.h"
#include <cmath>
#include <iostream>



namespace ae {

    int32_t CoreAudio::SamplingFrequency = 48000;
    PaStream * CoreAudio::stream = nullptr;
    cycledBuffer CoreAudio::testData = {nullptr, 0, 0};

    PaError CoreAudio::init() {
        srand(time(nullptr));

        auto err = Pa_Initialize();
        if(err != paNoError){
            return err;
        }

        CoreAudio::testData.buffer = new Sample [128000];
        double arg = 0.0;
        double step = 1.0 / SamplingFrequency;
        double angFreq = 2 * M_PI * 140;
        int32_t ampl = 1 << 30;

        for(int i = 0; i < 128000; ++i){
            testData.buffer[i] = sin(angFreq * arg);
            //std::cerr << testData.buffer[i] << '\n';
            arg += step;
        }
        CoreAudio::testData.bufferSize = 128000;
        CoreAudio::testData.current = 0;


        err = Pa_OpenDefaultStream(&stream,
                             0,          /* no input channels */
                             2,          /* stereo output */
                             paFloat32,  /* 32 bit floating point output */
                             SamplingFrequency,
                             128,        /* frames per buffer, i.e. the number
                                                   of sample frames that PortAudio will
                                                   request from the callback. Many apps
                                                   may want to use
                                                   paFramesPerBufferUnspecified, which
                                                   tells PortAudio to pick the best,
                                                   possibly changing, buffer size.*/
                             &paCycleCallback , /* this is your callback function */
                             &testData );
        if(err != paNoError){
            std::cerr << Pa_GetErrorText(err);
            return err;
        }
    }

    PaError CoreAudio::terminate() {
        if(stream != nullptr){
            Pa_StopStream(stream);
            Pa_CloseStream(stream);
        }


        auto err = Pa_Terminate();
        delete[] testData.buffer;

        return err;
    }

    PaError CoreAudio::startPlayback() {
        return Pa_StartStream(stream);
    }

    PaError CoreAudio::stopPlayback() {
        return Pa_StopStream(stream);
    }
} // ae