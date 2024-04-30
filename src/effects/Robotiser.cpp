#include "Robotiser.h"
#include "BaseEffect.h"
#include <cmath>


Robotiser::Robotiser(){
    CHUNK_SIZE = 512;
    HOP_SIZE = 256;
}

void Robotiser::processFftChunk(Utils::Frequencies& freqs) {
    
    for(int i = 0; i < freqs.size(); ++i){
        auto ampl = std::abs(freqs[i]);
        freqs[i] = std::polar(ampl, 0.f);
    }

}
void Robotiser::updateProperties(){
}

void Robotiser::setUpUi(QWidget* w) {
    BaseEffect::setUpUi(w);    
}
