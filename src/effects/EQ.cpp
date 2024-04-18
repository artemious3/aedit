#include "EQ.h"
#include "BaseEffect.h"
#include "Utils.h"
#include "coretypes.h"
#include <algorithm>
#include <cmath>

void EQ::cosSumWindow(Sample *buf, int size, double a0) {
  auto a1 = 1 - a0;
  for (int i = 0; i < size; ++i) {
    buf[i] *= (a0 - a1 * std::cos(2 * M_PI * i / size));
  }
}

void EQ::processFftChunk(Utils::Frequencies& freqs) {
    for(int i = 128; i < freqs.size() - 128; ++i){
        freqs[i] *= 0.1;
    }
}

void EQ::setUpUi(QWidget *widget) {
    BaseEffect::setUpUi(widget);
}

void EQ::_process(Sample *buf, int size, int max_range) {

  Sample *inBuf = new Sample[CHUNK_SIZE];
  Sample *outBuf = new Sample[size + CHUNK_SIZE];
  for (int i = 0; i < size; ++i) {
    outBuf[i] = 0;
  }

  Utils::Frequencies freqs;

  for (int i = 0; i < size; i += CHUNK_SIZE/2) {
    auto curChunkSize = std::min(CHUNK_SIZE, max_range - i);

    for (int inb_i = 0, b_i = i; inb_i < curChunkSize; inb_i++, b_i++) {
      inBuf[inb_i] = buf[b_i];
    }
    cosSumWindow(inBuf, curChunkSize);

    freqs = Utils::fft(inBuf, curChunkSize);

    processFftChunk(freqs);

    Utils::ifft_additive(freqs, &outBuf[i], curChunkSize);

  }


  for(int i = 0; i < size; ++i){
    buf[i] = outBuf[i];
  }

  delete[] outBuf;
  delete[] inBuf;
}


void EQ::updateProperties() {
    ;
}
