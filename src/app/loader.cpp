#include "loader.h"
#include "CoreAudio.h"
#include "coretypes.h"
#include <QAudioDecoder>
#include <qaudiobuffer.h>
#include <qaudiodecoder.h>
#include <qaudioformat.h>
#include <qdebug.h>
#include <qurl.h>

Loader::Loader() {
  decoder = new QAudioDecoder(this);
  connect(decoder, &QAudioDecoder::bufferReady, this, &Loader::readBuffer);
  connect(decoder, &QAudioDecoder::finished, this, &Loader::finishReading );
}

Loader::~Loader() { decoder->deleteLater(); }

void Loader::readBuffer() {
//   qDebug() << "buffer is ready";
//   if(decoder->error()){
//     qDebug() << "error" << decoder->errorString();

//   }
  auto buf = decoder->read();
  auto sz = buf.byteCount() / sizeof(Sample);

  auto ptr = buf.data<Sample>();
//   if(tmpBuffer.capacity() < tmpBuffer.size() + sz){
//     tmpBuffer.reserve( tmpBuffer.size() + sz );
//   }


  for (int i = 0; i < sz; ++i) {
    tmpBuffer.push_back(ptr[i]);
  }
  //qDebug() << tmpBuffer.size();
}

void Loader::finishReading() {
  auto size = tmpBuffer.size();
  resulting_buffer.left = new Sample[size / 2 + 1];
  resulting_buffer.right = new Sample[size / 2 + 1];
  size_t core_i = 0;
  for (size_t i = 0; i < size; ) {

    resulting_buffer.left[core_i] = tmpBuffer[i];
    ++i;
    resulting_buffer.right[core_i] = tmpBuffer[i];
    ++i;

    ++core_i;
  }
  resulting_buffer.current = 0;
  resulting_buffer.isStereo = true;
  resulting_buffer.size = core_i;

  tmpBuffer.clear();
  emit onFinish();
}

void Loader::startDecoding(const QString &fname) {

  format.setSampleFormat(QAudioFormat::SampleFormat::Float);
  format.setSampleRate(ae::CoreAudio::SamplingFrequency);
  format.setChannelConfig(QAudioFormat::ChannelConfigStereo);
  decoder->setAudioFormat(format);
  decoder->setSource(fname);
  decoder->start();

}

StereoAudioBuffer Loader::getResultingBuffer() { return resulting_buffer; }
