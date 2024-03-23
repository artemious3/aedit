//
// Created by artemious on 27.02.24.
//

#include "CoreInfo.h"

namespace ae {
/*Unsafe function, does not check ranges due to fast response necessary
* Octave is indexed from 1
*/
    double CoreInfo::getFreq(Note note, short octave) {
        short ind = 2 + (octave-1)*12 + static_cast<short>(note);
        return freqTable[ind];
    }
    
} 