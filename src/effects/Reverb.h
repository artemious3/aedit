#ifndef D827DEA6_68A6_4BD2_AAA5_1A8C9E362EE8
#define D827DEA6_68A6_4BD2_AAA5_1A8C9E362EE8

#include "BaseEffect.h"
class Reverb : public BaseEffect{

    static const int COMB_AMOUNT = 4;

    float combGains[COMB_AMOUNT] = {0.742f, 0.733f, 0.715f, 0.697f};
    int combDelays[COMB_AMOUNT] = {4799, 4677, 3654, 5801};

    float allPassGain[2] = {0.634, 0.767};
    float allPassDelay [2]= {457, 1057};

protected:
    virtual void _process(Sample* buf, int size, int max_in) override;
    virtual void updateProperties() override;
 
public:
    void setUpUi(QWidget*) override;
    Reverb();
};

#endif /* D827DEA6_68A6_4BD2_AAA5_1A8C9E362EE8 */
