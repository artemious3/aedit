#ifndef C9DC23A8_022F_4F2E_933B_9B0CCC66B301
#define C9DC23A8_022F_4F2E_933B_9B0CCC66B301

#include "BaseEffect.h"
#include "Utils.h"
#include <vector>

class FFTProcessor : public BaseEffect{

protected:
    
    int CHUNK_SIZE = 2048;
    int HOP_DIVISOR = 2;

private:
    virtual void processFftChunk(Utils::Frequencies& freqs) = 0;

public:
    void setUpUi(QWidget* widget) override;

protected:
    //void updateProperties() override;
    void _process(Sample*, int, int) override;
}; 

#endif /* C9DC23A8_022F_4F2E_933B_9B0CCC66B301 */
