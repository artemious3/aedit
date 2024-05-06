#include "FFTProcessor.h"
#include "BaseEffect.h"
#include "Utils.h"
#include "coretypes.h"
#include <algorithm>
#include <cassert>
#include <qstringtokenizer.h>

void FFTProcessor::setUpUi(QWidget *widget) { BaseEffect::setUpUi(widget); }

void FFTProcessor::_process(Sample *buf, int size, int max_range, short) {
  Sample *inBuf = new Sample[CHUNK_SIZE];
  Sample *outBuf = new Sample[CHUNK_SIZE];
  Sample *resBuf = new Sample[size + CHUNK_SIZE + HOP_SIZE];
  for (int i = 0; i < size + CHUNK_SIZE; ++i) {
    resBuf[i] = 0;
  }

  Utils::Frequencies freqs;

  auto hop = HOP_SIZE;
  auto win =
      Utils::cosSumWindow(HOP_SIZE * 4, CHUNK_SIZE);

  for (int i = 0; i < size; i += hop) {
    auto curChunkSize = std::min(CHUNK_SIZE, max_range - i);

    for (int inb_i = 0, b_i = i; inb_i < curChunkSize; inb_i++, b_i++) {
      inBuf[inb_i] = buf[b_i] * win[inb_i];
    }
    for (int inb_i = curChunkSize; inb_i < CHUNK_SIZE; ++inb_i) {
      inBuf[inb_i] = 0;
    }

    CHECK_STOP

    freqs = Utils::fft(inBuf, CHUNK_SIZE);
    processFftChunk(freqs);
    Utils::ifft(freqs, outBuf, CHUNK_SIZE);

    CHECK_STOP

    for (int res_i = i, out_i = 0; out_i < curChunkSize; ++res_i, ++out_i) {
      resBuf[res_i] += outBuf[out_i] * win[out_i] / CHUNK_SIZE ;
    }
  }

  for (int i = 0; i < size; ++i) {
    buf[i] = resBuf[i];
  }

  delete[] resBuf;
  delete[] outBuf;
  delete[] inBuf;
}

FFTProcessor::FFTProcessor(int chunk, int hop)
    : CHUNK_SIZE(chunk), HOP_SIZE(hop) {
      assert(4 * HOP_SIZE <= CHUNK_SIZE);
    }
