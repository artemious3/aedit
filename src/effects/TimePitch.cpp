

#include "TimePitch.h"
#include "CoreAudio.h"
#include "Utils.h"
#include <qspinbox.h>

void TimePitch::_process(Sample *buf, int size, int max_in) {
  // auto new_size = (int)std::ceil(size * koef);
  // Sample* newBuf = new Sample[size];
  // for(int i = 0; i < size; ++i){
  //     double oldInd = (double)i / koef;
  //     auto fl = std::floor(oldInd);
  //     auto cl = std::ceil(oldInd);
  //     newBuf[i] = (cl - oldInd) * buf[(int)cl] + (oldInd - fl) *
  //     buf[(int)fl];
  // }

  // for(int i = 0; i < size; ++i){
  //     buf[i] = newBuf[i];
  // }
  // for(int i = new_size; i < size; ++i){
  //     buf[i] = 0;
  // }
  
  // delete[] newBuf;

  auto new_size = (int)std::ceil(size * koef);
  Sample* newBuf = new Sample[size];
  for(int i = 0; i < size; ++i){
      double oldInd = (double)i / koef;
      int integ = std::floor(oldInd);
      int beg = std::max(integ - 3, 0);
      int end = std::min(integ + 3, size);
      double val = 0.0;
      for(int j = beg; j <= end; ++j){
        double l = 1.0;
        for(int k = beg; k <= end; ++k){
          if(j == k){
            continue;
          }
          l *= (oldInd - k) / ((double)j - k);
        }
        val += buf[j] * l;
      }
      newBuf[i] = val;
  }

  for(int i = 0; i < std::min(new_size, size); ++i){
      buf[i] = newBuf[i];
  }
  for(int i = new_size; i < size; ++i){
      buf[i] = 0;
  }
  
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
