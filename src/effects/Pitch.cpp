#include "Pitch.h"
#include "BaseEffect.h"
#include "FFTProcessor.h"
#include "Utils.h"
#include <complex>
#include <qspinbox.h>

void Pitch::processFftChunk(Utils::Frequencies &freqs) {
  auto n = CHUNK_SIZE;
  auto n_h = CHUNK_SIZE / 2;
  auto hop = HOP_SIZE;

  Utils::Frequencies new_freqs(freqs.size());


  // for(int i= 0; i < n_h; ++i){
  //   analysis[i] = {0.0, 0.0};
  // }

  float bin_freq_step = 2 * M_PI / (float)CHUNK_SIZE;
  float bin_freq = 0;

  // 1. ANALYSIS
  for (int i = 0; i <= n_h; ++i) {
    auto cur = freqs[i];
    float magn = std::abs(cur);
    float phase = std::atan2(cur.imag(), cur.real());

    // actual change in phase since last chunk
    auto act_dphase = phase - lastPhases[i];
    // expected change, as if it was a constant sinusoid
    auto exp_dphase = bin_freq * hop;

    auto freq_diverg = Utils::normalise(act_dphase - exp_dphase);
    // smth from calculus`
    double bin_diverg =
        freq_diverg * (float)CHUNK_SIZE / (2.0 * M_PI) / (float)hop;

    analysis[i].bin_freq = (float)i + bin_diverg;
    analysis[i].magn = magn;
    lastPhases[i] = phase;
    bin_freq += bin_freq_step;
  }

  CHECK_STOP

  // 2. MODIFYING

  for(int i = 0; i <= n_h; ++i){
    synthesis[i] = {0.0f,0.0f};
  }

  float frac_bin = 0;
  for (int i = 0; i <= n_h; ++i) {
    int new_i = std::floor(frac_bin + 0.5f);

    if (new_i > n_h) {
      break; // we can break, further values only get greater
    }

    synthesis[new_i].magn += analysis[i].magn;
    synthesis[new_i].bin_freq = analysis[i].bin_freq * koef;
    frac_bin += koef;
  }

  CHECK_STOP

  // 3. SYNTHESYS`

  bin_freq = 0;
  for (int i = 0; i <= n_h; ++i) {
  
    auto bin_diverg = synthesis[i].bin_freq - (float)i;
    float dphase = bin_diverg * 2.0 * M_PI * (float)hop / (float)CHUNK_SIZE;
    dphase += bin_freq * hop;

    float act_phase = Utils::normalise(lastSynthPhases[i] + dphase);

    new_freqs[i] = std::polar(synthesis[i].magn, act_phase);
    new_freqs[n - i] = std::conj(new_freqs[i]);

    lastSynthPhases[i] = act_phase;
    // lastSynthPhases[n-i] = act_phase;
    bin_freq += bin_freq_step;
  }

  freqs = std::move(new_freqs);
}

void Pitch::updateProperties() { koef = std::pow(2.0, (double)pitchShiftBox->value() / 1200); }

void Pitch::setUpUi(QWidget *widget) {
  BaseEffect::setUpUi(widget);
  pitchShiftBox = new QSpinBox();
  pitchShiftBox->setMinimum(-2400);
  pitchShiftBox->setMaximum(2400);
  pitchShiftBox->setValue(400);
  layout->addRow("Pitch shift (cents)", pitchShiftBox);
}

Pitch::Pitch()
: FFTProcessor(2048, 512), lastSynthPhases(CHUNK_SIZE, 0.0), lastPhases(CHUNK_SIZE, 0.0),
     synthesis(CHUNK_SIZE/2 + 1, {0, 0}), analysis(CHUNK_SIZE/2 + 1, {0, 0}) {


      }

void Pitch::reset() {
  for (int i = 0; i < CHUNK_SIZE; ++i) {
    lastPhases[i] = 0.0;
    lastSynthPhases[i] = 0.0;
  }
}
