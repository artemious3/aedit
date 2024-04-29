#include "Gain.h"
#include "BaseEffect.h"
#include <qnamespace.h>
#include <qslider.h>
#include <qgridlayout.h>
#include <qwidget.h>
#include <qlabel.h>
#include <QGridLayout>


void Gain::_process(Sample* buf, int size, int max_out, short){
    for(int i = 0; i < size; ++i){
        buf[i] *= _koef;
    }
}

void Gain::setUpUi(QWidget* widget){
    BaseEffect::setUpUi(widget);
    _koefSlider = new QSlider(Qt::Horizontal);
    layout->insertRow(0, "Gain", _koefSlider);
    widget->show();
}

void Gain::updateProperties(){
    _koef = ((double)_koefSlider->value() / _koefSlider->maximum()) * 2;
}