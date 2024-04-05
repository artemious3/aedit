//
// Created by artemious on 25.02.24.
//

#include "CoreAudio.h"
#include "coretypes.h"
#include <cmath>
#include <cstdint>
#include <iostream>



namespace ae {

int32_t CoreAudio::SamplingFrequency = 48000;
int32_t CoreAudio::BytesPerCallback = 64;

    PaStream * CoreAudio::stream = nullptr;
    StereoAudioBuffer CoreAudio::stereoStream = {nullptr, nullptr, 0, 0};

    PaError CoreAudio::init() {
        auto err = Pa_Initialize();
        if(err != paNoError){
            return err;
        }

        err = Pa_OpenDefaultStream(&stream,
                             0,
                            2,
                            paFloat32,
                            SamplingFrequency,
                             BytesPerCallback,
                             &paUsualCallback,
                             &stereoStream);


        if(err != paNoError){
            std::cerr << Pa_GetErrorText(err);
        }
        return err;
    }

    PaError CoreAudio::terminate() {
        if(!stream){
            Pa_CloseStream(stream);
        }

        auto err = Pa_Terminate();
        delete[] stereoStream.left;
        delete[] stereoStream.right;
        return err;
    }

    PaError CoreAudio::startPlayback() {
        return Pa_StartStream(stream);
    }

    PaError CoreAudio::stopPlayback() {
        stereoStream.current = 0;
        return Pa_StopStream(stream);
    }

    void CoreAudio::initializeTestStream() {
        CoreAudio::stereoStream.left = new Sample [TEST_BUF_SZ];
        CoreAudio::stereoStream.right = new Sample [TEST_BUF_SZ];
        double arg = 0.0;
        double step = 1.0 / SamplingFrequency;
        double angFreq = 2 * M_PI * 700;
        //int32_t ampl = 1 << 30;

        for(int i = 0; i < TEST_BUF_SZ; ++i){
            stereoStream.left[i] = sin(angFreq * arg);
            stereoStream.right[i] = sin(angFreq * arg);
            //std::cerr << stereoStream.buffer[i] << '\n';
            arg += step;
        }
        CoreAudio::stereoStream.size = TEST_BUF_SZ;
        CoreAudio::stereoStream.current = 0;
    }
    } // namespace ae
    void ae::CoreAudio::setBuffer(StereoAudioBuffer buffer) {
        delete[] stereoStream.left;
        delete[] stereoStream.right;
        stereoStream = buffer;
    
}
