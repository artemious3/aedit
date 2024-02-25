//
// Created by artemious on 25.02.24.
//
#include "portaudio.h"
#include "CoreInfo.h"

int paCycleCallback( const void *inputBuffer, void *outputBuffer,
                     unsigned long framesPerBuffer,
                     const PaStreamCallbackTimeInfo* timeInfo,
                     PaStreamCallbackFlags statusFlags,
                     void *userData)
{
    (void) inputBuffer;

    monoBuffer * data = (monoBuffer*)userData;
    Sample * output = (Sample*) outputBuffer;

    unsigned long remainingInBuffer = data->bufferSize - data->current;
    unsigned long remainingToRead = framesPerBuffer;
    unsigned long toReadNow = (remainingInBuffer > remainingToRead) ? remainingToRead : remainingInBuffer;
    unsigned long totalRead = 0;

    unsigned long i = 0;

    while(totalRead < framesPerBuffer){
        unsigned long lastRead = data->current + toReadNow;
        for(; data->current < lastRead; ++data->current, ++i){
            output[i] = data->buffer[ data->current ];
        }
        remainingInBuffer -= toReadNow;
        remainingToRead -= toReadNow;
        totalRead += toReadNow;

        if(remainingInBuffer == 0){
            data->current = 0;
            remainingInBuffer = data->bufferSize;
        }

        toReadNow = (remainingInBuffer > remainingToRead) ? remainingToRead : remainingInBuffer;
    }

    return 0;
}