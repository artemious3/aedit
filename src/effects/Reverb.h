#ifndef D827DEA6_68A6_4BD2_AAA5_1A8C9E362EE8
#define D827DEA6_68A6_4BD2_AAA5_1A8C9E362EE8

#include "BaseEffect.h"
class Reverb : public BaseEffect{

    static const int COMB_AMOUNT = 6;

    float combGains[COMB_AMOUNT] = {0.742f, 0.733f, 0.342f, 0.456f, 0.202, 0.306};
    int combDelays[COMB_AMOUNT] = {4799, 4677, 3654, 5801, 2300, 3278 };

    float allPassGain[2] = {0.634, 0.767};
    float allPassDelay [2]= {1357, 2057};

protected:
    virtual void _process(Sample* buf, int size, int max_in, short) override;
    virtual void updateProperties() override;
 
public:
    void setUpUi(QWidget*) override;
    Reverb();
};

#endif /* D827DEA6_68A6_4BD2_AAA5_1A8C9E362EE8 */
