//
// Created by artemious on 21.02.24.
//

#ifndef AEDIT_COREINFO_H
#define AEDIT_COREINFO_H

#include "stdint.h"

enum Note {
    C = 0,
    Cs,
    D,
    Ds,
    E,
    F,
    Fs,
    G,
    Gs,
    A,
    As,
    H
};

typedef float Sample;

typedef struct {
    Sample *buffer;
    long long current;
    long long bufferSize;
} monoBuffer;

double getNoteFrequency(unsigned short octave, Note note){

}





#endif //AEDIT_COREINFO_H
