#ifndef C9DC23A8_022F_4F2E_933B_9B0CCC66B301
#define C9DC23A8_022F_4F2E_933B_9B0CCC66B301


#include "BaseEffect.h"
#include "Utils.h"

class EQ : public BaseEffect{
    static constexpr double HANNING_A0 = 0.54347826087;
    static constexpr int CHUNK_SIZE = 2048;

private:
    void cosSumWindow(Sample* buf, int size, double a0 = HANNING_A0);
    void processFftChunk(Utils::Frequencies& freqs);



public:
    void setUpUi(QWidget* widget) override;

protected:
    void updateProperties() override;
    void _process(Sample*, int, int) override;
}; 

#endif /* C9DC23A8_022F_4F2E_933B_9B0CCC66B301 */
