#include "Robotiser.h"
#include "BaseEffect.h"
#include <cmath>


Robotiser::Robotiser(){
    CHUNK_SIZE = 256;
}

void Robotiser::processFftChunk(Utils::Frequencies& freqs) {
    
    for(int i = 0; i < freqs.size(); ++i){
        auto ampl = std::abs(freqs[i]);
        freqs[i] = {ampl, 0};
    }

}
void Robotiser::updateProperties(){
}

void Robotiser::setUpUi(QWidget* w) {
    BaseEffect::setUpUi(w);    
}
