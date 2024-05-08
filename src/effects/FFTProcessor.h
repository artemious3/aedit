#ifndef C9DC23A8_022F_4F2E_933B_9B0CCC66B301
#define C9DC23A8_022F_4F2E_933B_9B0CCC66B301

#include "BaseEffect.h"
#include "Utils.h"
#include <qlabel.h>
#include <vector>
#include <QTimer>

class FFTProcessor : public BaseEffect{

protected:
    
    int CHUNK_SIZE = 4096;
    int HOP_SIZE = 128;
    int HOP_DIVISOR = 2;

    QLabel* estTimeLbl;
    QElapsedTimer* chunckTimer;
    int total_chunks;

private:
    virtual void processFftChunk(Utils::Frequencies& freqs) = 0;

public:
    void setUpUi(QWidget* widget) override;

protected:
    //void updateProperties() override;
    void _process(Sample*, int, int, short) override;
    void updateEstimatedTime();

    FFTProcessor(int chunk, int hop);
    FFTProcessor() = default;
    ~FFTProcessor();
}; 

#endif /* C9DC23A8_022F_4F2E_933B_9B0CCC66B301 */
