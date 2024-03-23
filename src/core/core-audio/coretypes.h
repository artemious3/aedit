//
// Created by artemious on 21.02.24.
//

#ifndef AEDIT_CORETYPES_H
#define AEDIT_CORETYPES_H

#include "stdint.h"

typedef float Sample;

typedef struct {
    Sample *buffer;
    long long current;
    long long bufferSize;
} MonoAudioBuffer;

typedef struct {
    Sample * leftBuffer;
    Sample * rightBuffer;
    long long current;
    long long bufferSize;
} StereoAudioBuffer;



#endif //AEDIT_CORETYPES_H
