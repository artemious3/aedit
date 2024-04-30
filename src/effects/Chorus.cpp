#include "Chorus.h"
#include "BaseEffect.h"
#include "Utils.h"
#include <cmath>
#include <complex>
#include <random>


Chorus::Chorus(){
    //CHUNK_SIZE = 256;    
}

void Chorus::processFftChunk(Utils::Frequencies& freqs) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_real_distribution<> rnd(0.0,  M_PI);

    for(int i = 0; i < freqs.size(); ++i){
        auto ph =  std::atan2(freqs[i].imag(), freqs[i].real()) + rnd(gen);
        auto ampl =  (double)std::abs(freqs[i]);
        auto c = std::polar(ampl, ph);
        freqs[i] = c;
    }

    // for(int i = 0; i < freqs.size(); ++i){
    //     auto phi = std::atan2(freqs[i].imag(), freqs[i].real());
    //     auto ampl = std::abs(freqs[i]);
    //     freqs[i] = std::polar(ampl, phi * 1.2f);
    // }

}
void Chorus::updateProperties(){
}

void Chorus::setUpUi(QWidget* w) {
    BaseEffect::setUpUi(w);    
}