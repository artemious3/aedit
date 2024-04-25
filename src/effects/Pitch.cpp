#include "Pitch.h"
#include "BaseEffect.h"
#include "Utils.h"
#include <complex>

struct ae_signal{
    float magn;
    float bin_freq;
};



void Pitch::processFftChunk(Utils::Frequencies &freqs) {
  auto n = freqs.size();
  auto n_h = freqs.size() / 2;
  auto hop = n_h;

  Utils::Frequencies new_freqs( freqs.size());

  std::vector<ae_signal> analysis (freqs.size(), {0,0});
  std::vector<ae_signal> synthesis (freqs.size(), {0,0});

//1. ANALYSIS
  for (int i = 0; i <= n_h; ++i){
    auto cur = freqs[i];
    float magn =  std::abs(cur);
    float phase = std::atan2(cur.imag(), cur.real());
    float bin_freq = 2 * M_PI * (double)i / CHUNK_SIZE;

    //actual change in phase since last chunk
    auto act_dphase = phase - lastPhases[i];
    //expected change, as if it was a constant sinusoid
    auto exp_dphase = bin_freq * hop;

    auto freq_diverg = Utils::normalise( act_dphase - exp_dphase );
    //smth from calculus
    double bin_diverg = freq_diverg * CHUNK_SIZE / (2 * M_PI * hop);

    analysis[i].bin_freq = (float)bin_freq + bin_diverg;
    analysis[i].magn = magn;
    
    lastPhases[i] = phase;
  }

//2. MODIFYING

for(int i = 0; i <= n_h; ++i){
    int new_i = std::round((float)i * koef);

    if(new_i >= n_h){
        break; // we can break, further values only get greater
    }

    synthesis[new_i].magn += analysis[i].magn;
    //synthesis[n-new_i].magn += analysis[i].magn;
    synthesis[new_i].bin_freq = analysis[i].bin_freq * koef;
    //synthesis[n - new_i].bin_freq = analysis[i].bin_freq * koef;
}

// 3. SYNTHESYS

for(int i = 0; i <= n_h; ++i){
    auto bin_freq = 2 * M_PI * i / CHUNK_SIZE;

    auto bin_diverg = synthesis[i].bin_freq - (float)i;
    float dphase = bin_diverg * 2.0 * M_PI * (float)hop / (float)CHUNK_SIZE;
    dphase += bin_freq * hop;

    float act_phase = Utils::normalise( lastSynthPhases[i] + dphase );

    new_freqs[i] = std::polar(synthesis[i].magn, act_phase);
    new_freqs[n-i] = std::conj(new_freqs[i]);

    lastSynthPhases[i] = act_phase;
    //lastSynthPhases[n-i] = act_phase;
}

  freqs = std::move(new_freqs);
}


void Pitch::updateProperties() {}

void Pitch::setUpUi(QWidget *widget) { BaseEffect::setUpUi(widget); }

Pitch::Pitch() : lastPhases(CHUNK_SIZE, 0), lastSynthPhases(CHUNK_SIZE, 0) {}


