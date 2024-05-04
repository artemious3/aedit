#include "exporter.h"
#include <stdexcept>
#include "CoreAudio.h"
#include "coretypes.h"

using namespace ae;


const int16_t Exporter::KOEF =  std::numeric_limits<int16_t>::max();

void Exporter::exportCoreBuffer(const QString& fname) {
    
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
        header.bytesPerSec =  num_of_chan * header.SamplesPerSec * 2;
        header.ChunkSize = 4 + (8 + header.Subchunk1Size) + (8 + header.Subchunk2Size);

        stream.writeRawData(reinterpret_cast<const char*>(&header), sizeof(WAV_HEADER));
        for(int i = 0; i < buf.size; ++i){
            int16_t l = std::min((int16_t)(buf.left[i] * (Sample)KOEF), KOEF);
            int16_t r =  std::min((int16_t)(buf.right[i] * (Sample)KOEF), KOEF);
            stream.writeRawData( reinterpret_cast<const char*>(&l), 2);
            stream.writeRawData( reinterpret_cast<const char*>(&r), 2);
        }

        f.close();

}
