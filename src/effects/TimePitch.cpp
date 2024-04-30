

#include "TimePitch.h"
#include "CoreAudio.h"
#include "TimelineScene.h"
#include "Utils.h"
#include <qspinbox.h>
#include <QMessageBox>
#include "CoreAudio.h"
#include "mainwindow.h"

void TimePitch::_process(Sample *buf, int size, int max_in, short channel) {
  auto new_size = (int)std::ceil(size * koef);
  Sample* newBuf = new Sample[new_size];
  for(int i = 0; i < new_size; ++i){
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


  auto after_shift = new_size - size;

  if(after_shift > 0){
    Utils::shiftBuf(buf, size, max_in, after_shift);
  }

  for(int i = 0; i < new_size; ++i){
    buf[i] = newBuf[i];
  }

  if(after_shift < 0){
    Utils::shiftBuf(buf, size, max_in, after_shift);
  }

  delete[] newBuf;
}


void TimePitch::apply(){
  using namespace ae;
    if(!gui){
        qDebug() << "BaseEffect: No gui installed!";
        return;
    }

    auto window = MainWindow::getInstance();

    updateProperties();

    auto tl =  window->getTimeline();
    auto sel = tl->getSelection();
    auto buf = CoreAudio::getBuffer();

    auto beg = sel.first;
    auto size = sel.second - sel.first;

    if( size <= 1 ){
        QMessageBox::information(window, "Info", "Select an audio area.");
        return;
    }

    auto max = buf.size - sel.first;

    auto new_size = buf.size - size + koef * size;
    if(new_size > buf.size ){
      CoreAudio::resizeStream(new_size);
      buf = CoreAudio::getBuffer();
    }


    gui->setEnabled(false);
    _process(buf.left + beg, size, max, 0);
    _process(buf.right + beg, size, max, 1);
    gui->setEnabled(true);


    if(new_size < buf.size){
       CoreAudio::resizeStream(new_size);
       buf = CoreAudio::getBuffer();
    }

    emit modifiedBuffer(beg, beg + size*koef, "TimePitch");


    window->clearHistory();

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
