#ifndef DB7C6F59_C2CB_4E81_AFBC_DCC2C5F8D54A
#define DB7C6F59_C2CB_4E81_AFBC_DCC2C5F8D54A

#include "FFTProcessor.h"
#include "Utils.h"
#include <qwidget.h>
#include <QCheckBox>
#include <QSpinBox>

class Pitch : public FFTProcessor{

    double koef = 1.4;
    std::vector<float> lastPhases;
    std::vector<float> lastSynthPhases;

    

    void processFftChunk(Utils::Frequencies& freqs) override;
    void updateProperties() override;
    //void _process(Sample*, int, int) override;

public:
    void setUpUi(QWidget*) override;
    Pitch();

};


#endif /* DB7C6F59_C2CB_4E81_AFBC_DCC2C5F8D54A */
