#include "Reverb.h"
#include "BaseEffect.h"
#include <algorithm>
#include <climits>

void Reverb::_process(Sample* buf, int size, int max_in) {
    //ASSUMPTION: combDelays is sorted

    for(int d = 0; d < COMB_AMOUNT-1; ++d){
        for(int i = combDelays[d]; i <  combDelays[d+1]; ++i){
            for(int j = 0; j < d; ++j){
                buf[i] += buf[ i - combDelays[j] ] * combGains[j] / d;
            }
        }
    }

    for(int i = combDelays[COMB_AMOUNT-1]; i < size; ++i){
        for(int j = 0; j < COMB_AMOUNT; ++j){
            buf[i] += buf[ i - combDelays[j] ] * combGains[j] / COMB_AMOUNT;
        }
    }

    Sample* sourceBuf  = new Sample[size];
    for(int i = 0; i < size; ++i){
        sourceBuf[i] = buf[i];
    }

    for(int i = allPassDelay[0]; i < size; ++i){
        int M = allPassDelay[0];
        buf[i] = (- allPassGain[0]* sourceBuf[i]) + sourceBuf[ i -  M ] + (allPassGain[0] * buf[i - M]);
    }

    delete[] sourceBuf;
}

Reverb::Reverb() {
    std::sort(&combDelays[0], &combDelays[4]);    
}

void Reverb::updateProperties(){
    
}

void Reverb::setUpUi(QWidget* w) {
    BaseEffect::setUpUi(w);
    
}
