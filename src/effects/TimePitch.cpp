

#include "TimePitch.h"
#include "CoreAudio.h"
#include "Utils.h"
#include <qspinbox.h>

void TimePitch::_process(Sample *buf, int size, int max_in) {
  qDebug() << "time pitch!";

  auto new_size = (int)std::ceil(size * koef);
  Sample* newBuf = new Sample[size * koef];
  for(int i = 0; i < size*koef; ++i){
      double oldInd = (double)i / koef;
      auto fl = std::floor(oldInd);
      auto cl = std::ceil(oldInd);
      newBuf[i] = (cl - oldInd) * buf[(int)cl] + (oldInd - fl) *
      buf[(int)fl];
  }

  for(int i = 0; i < std::min(new_size, max_in); ++i){
      buf[i] = newBuf[i];
  }
  for(int i = new_size; i < std::min(max_in, size); ++i){
      buf[i] = 0;
  }
  
  delete[] newBuf;

//   qDebug() << "new process!";

//   auto new_size = (int)std::ceil(size * koef);
//   Sample *newBuf = new Sample[new_size];
//   const auto chunkSize = 128;
//   const auto hop = chunkSize / 2;
//   const auto stretchChunk = chunkSize * koef;

//   const auto win = Utils::cosSumWindow(chunkSize, chunkSize, 0.5);

//   for (int k = 0; k < new_size; k += hop) {
//     if( k + chunkSize > size){
//         break;
//     }

//     for (int i = k; i < k + chunkSize; ++i) {

//       float ind = (float)i / koef;
//       double interp = 0.0;
//       for (int j = i; j < i + stretchChunk; ++j) {
//         interp += buf[j] * Utils::sinc(ind, ae::CoreAudio::SamplingFrequency);
//       }

//       newBuf[i] += interp * win[i];
//     }

//   }

//   for (int i = 0; i < std::min(new_size, max_in); ++i) {
//     buf[i] = newBuf[i];
//   }
//   for (int i = new_size; i < size; ++i) {
//     buf[i] = 0;
//   }
}

void TimePitch::updateProperties() { koef = koefBox->value(); }

void TimePitch::setUpUi(QWidget *w) {
  BaseEffect::setUpUi(w);

  koefBox = new QDoubleSpinBox(w);
  koefBox->setValue(1.0);
  koefBox->setMinimum(0.2);
  koefBox->setMaximum(4.0);
  layout->addRow("Ratio", koefBox);

  w->show();
}

TimePitch::TimePitch() {}
