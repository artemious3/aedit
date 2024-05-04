#ifndef DB7C6F59_C2CB_4E81_AFBC_DCC2C5F8D54A
#define DB7C6F59_C2CB_4E81_AFBC_DCC2C5F8D54A

#include "FFTProcessor.h"
#include "Utils.h"
#include <qspinbox.h>
#include <qwidget.h>
#include <QCheckBox>
#include <QSpinBox>

struct ae_signal {
  float magn;
  float bin_freq;
};

class Pitch : public FFTProcessor{

    double koef = 1.4;
    std::vector<float> lastPhases;
    std::vector<float> lastSynthPhases;

    std::vector<ae_signal> analysis;
    std::vector<ae_signal> synthesis;

    QDoubleSpinBox* pitchShiftBox;
    void processFftChunk(Utils::Frequencies& freqs) override;
    void updateProperties() override;
    //void _process(Sample*, int, int) override;

public:
    void setUpUi(QWidget*) override;
    Pitch();
    virtual void reset() override;

};


#endif /* DB7C6F59_C2CB_4E81_AFBC_DCC2C5F8D54A */
