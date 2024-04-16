//
// Created by artemious on 25.02.24.
//

#include "CoreAudio.h"
#include "coretypes.h"
#include "portaudio.h"
#include <cmath>
#include <cstdint>
#include <iostream>
#include "Utils.h"



namespace ae {

    int32_t CoreAudio::SamplingFrequency = 48000;
    int32_t CoreAudio::BytesPerCallback  = 64;

    PaStream * CoreAudio::stream = nullptr;
    StereoAudioBuffer CoreAudio::buffer = {nullptr, nullptr, 0, 0};

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
                             &buffer);

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
        if(buffer.left)
            delete[] buffer.left;
        if(buffer.right)
            delete[] buffer.right;
        return err;
    }

    PaError CoreAudio::play() {
        return Pa_StartStream(stream);
    }

    PaError CoreAudio::stop() {
        buffer.current = 0;
        return Pa_StopStream(stream);
    }

    PaError CoreAudio::pause(){
        return Pa_StopStream(stream);
    }

    void CoreAudio::initializeTestStream() {
        CoreAudio::buffer.left = new Sample [TEST_BUF_SZ];
        CoreAudio::buffer.right = new Sample [TEST_BUF_SZ];
        double arg = 0.0;
        double step = 1.0 / SamplingFrequency;
        double angFreq = 2 * M_PI * 700;

        for(int i = 0; i < TEST_BUF_SZ; ++i){
            buffer.left[i] = sin(angFreq * arg);
            buffer.right[i] = sin(angFreq * arg);
            //std::cerr << stereoStream.buffer[i] << '\n';
            arg += step;
        }
        CoreAudio::buffer.size = TEST_BUF_SZ;
        CoreAudio::buffer.current = 0; 
    }


    } // namespace ae
    void ae::CoreAudio::setBuffer(StereoAudioBuffer buffer_) {
        if(buffer.left)
            delete[] buffer.left;
        if(buffer.right)
            delete[] buffer.right;
        buffer = buffer_;
        buffer.current = 0;
    }

    

StereoAudioBuffer ae::CoreAudio::getBuffer() {
    return buffer;
}
