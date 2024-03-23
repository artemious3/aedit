//
// Created by artemious on 25.02.24.
//

#include "CoreAudio.h"
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
        delete[] stereoStream.leftBuffer;
        delete[] stereoStream.rightBuffer;

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
        CoreAudio::stereoStream.leftBuffer = new Sample [32000];
        CoreAudio::stereoStream.rightBuffer = new Sample [32000];
        double arg = 0.0;
        double step = 1.0 / SamplingFrequency;
        double angFreq = 2 * M_PI * 700;
        //int32_t ampl = 1 << 30;

        for(int i = 0; i < 32000; ++i){
            stereoStream.leftBuffer[i] = sin(angFreq * arg);
            stereoStream.rightBuffer[i] = sin(angFreq * arg);
            //std::cerr << stereoStream.buffer[i] << '\n';
            arg += step;
        }
        CoreAudio::stereoStream.bufferSize = 32000;
        CoreAudio::stereoStream.current = 0;
    }
} // ae