#ifndef C162DA96_4665_4786_B790_40F219E6E46A
#define C162DA96_4665_4786_B790_40F219E6E46A

#include <cstdint>
#include <QFile>
#include <QDataStream>
#include <limits>
#include <memory>
#include <stdexcept>
#include "CoreAudio.h"
#include "coretypes.h"

struct WAV_HEADER {
  /* RIFF Chunk Descriptor */
  uint8_t RIFF[4] = {'R', 'I', 'F', 'F'}; // RIFF Header Magic header
  uint32_t ChunkSize;                     // RIFF Chunk Size
  uint8_t WAVE[4] = {'W', 'A', 'V', 'E'}; // WAVE Header
  /* "fmt" sub-chunk */
  uint8_t fmt[4] = {'f', 'm', 't', ' '}; // FMT header
  uint32_t Subchunk1Size = 16;           // Size of the fmt chunk
  uint16_t AudioFormat = 1; // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM
                            // Mu-Law, 258=IBM A-Law, 259=ADPCM
  uint16_t NumOfChan = 2;   // Number of channels 1=Mono 2=Sterio
  uint32_t SamplesPerSec = 16000;   // Sampling Frequency in Hz
  uint32_t bytesPerSec = 16000 * 2; // bytes per second
  uint16_t blockAlign = 4;          // 2=16-bit mono, 4=16-bit stereo
  uint16_t bitsPerSample = 16;      // Number of bits per sample
  /* "data" sub-chunk */
  uint8_t Subchunk2ID[4] = {'d', 'a', 't', 'a'}; // "data"  string
  uint32_t Subchunk2Size;                        // Sampled data length
};

using namespace ae;

class Exporter{

    static const int16_t KOEF = std::numeric_limits<int16_t>::max();


    public:
    static void exportCoreBuffer(const QString& fname){


        QFile f(fname);
        if(!f.open(QFile::WriteOnly)){
            throw std::runtime_error("Can not open file");
        }
        QDataStream stream (&f);

        auto buf = CoreAudio::getBuffer();

        WAV_HEADER header;

        int num_of_chan = buf.isStereo ? 2 : 1;
        header.Subchunk2Size = num_of_chan * buf.size * 16;
        header.SamplesPerSec = CoreAudio::SamplingFrequency;
        header.ChunkSize = 4 + (8 + header.Subchunk1Size) + (8 + header.Subchunk2Size);

        stream.writeRawData(reinterpret_cast<const char*>(&header), sizeof(WAV_HEADER));
        for(int i = 0; i < buf.size; ++i){
            int16_t l = buf.left[i]*KOEF;
            int16_t r = buf.right[i] * KOEF;
            stream.writeRawData( reinterpret_cast<const char*>(&l), 2);
            stream.writeRawData( reinterpret_cast<const char*>(&r), 2);
        }

        f.close();
    }



};

#endif /* C162DA96_4665_4786_B790_40F219E6E46A */
