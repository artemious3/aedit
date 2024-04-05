//
// Created by artemious on 21.02.24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "coretypes.h"
#include "ui_MainWindow.h"
#include <QKeyEvent>
#include <QFileDialog>
#include <QAudioDecoder>
#include <qaudiodecoder.h>
#include <qaudioformat.h>
#include <qfiledialog.h>
#include <qlogging.h>
#include <qpushbutton.h>


#include "portaudio.h"
#include "CoreAudio.h"

namespace ae {
    MainWindow::MainWindow(QWidget *parent) :
            QWidget(parent), ui(new Ui::MainWindow) {
      ui->setupUi(this);


      decoder = new QAudioDecoder(this);
      QAudioFormat format;
      format.setSampleFormat(QAudioFormat::Float);
      format.setSampleRate(48'000);
      format.setChannelConfig(QAudioFormat::ChannelConfigStereo);
      decoder->setAudioFormat(format);

      PaError err = CoreAudio::init();
      CoreAudio::initializeTestStream();
      if(err != paNoError){
          qDebug() << Pa_GetErrorText(err);
      }

      tlScene = new TimelineScene(ui->timeline);
      tlScene->setBuffer(CoreAudio::stereoStream);
      ui->timeline->setSceneRect(0, 0, ui->timeline->frameSize().width(), ui->timeline->frameSize().height());
      ui->timeline->setScene(tlScene);
      tlScene->drawWaveform();

      connect(ui->playBtn, &QPushButton::clicked, this, &MainWindow::openFile);
      connect(decoder, &QAudioDecoder::bufferReady, this, &MainWindow::onBufReady);
      connect(decoder, &QAudioDecoder::finished, this, &MainWindow::onBufFinished);
            }
    MainWindow::~MainWindow() {
        CoreAudio::terminate();
        delete ui;
    }

    void MainWindow::onError(QAudioDecoder::Error err) { qDebug() << "error!"; }

    void MainWindow::onBufFinished() {
        qDebug() << "finishing";

        StereoAudioBuffer core_buf;
        auto sample_sz = samples.size();
        core_buf.left = new Sample[sample_sz];
        core_buf.right = new Sample[sample_sz];
        size_t core_i = 0;
        for (size_t i = 0; i < sample_sz; ++i) {
            core_buf.left[core_i] = samples[i];
            ++i;
            core_buf.right[core_i] = samples[i];
            ++core_i;
        }
        core_buf.current = 0;
        core_buf.isStereo = true;
        core_buf.size = core_i;

        CoreAudio::setBuffer(core_buf);
        tlScene->setBuffer(core_buf);
        tlScene->drawWaveform();
    }

    void MainWindow::keyPressEvent(QKeyEvent *keyEvent) {
      if(!keyEvent->isAutoRepeat()){
          QWidget::keyPressEvent(keyEvent);
          CoreAudio::startPlayback();
      }

    }

    void MainWindow::keyReleaseEvent(QKeyEvent *keyEvent) {
        if(!keyEvent->isAutoRepeat()){
            CoreAudio::stopPlayback();
        }

    }
} // ae


void ae::MainWindow::onBufReady() {
        qDebug() << "buffer is ready";
        auto buf = decoder->read();
        auto sz = buf.byteCount() / sizeof(Sample);
        
        auto ptr = buf.data<Sample>();
        for (int i = 0; i < sz; ++i) {
            samples.push_back(ptr[i]);
        }

        qDebug() << samples.size();

        // CoreAudio::setBuffer(core_buf);
        // tlScene->setBuffer(CoreAudio::stereoStream);
        // tlScene->drawWaveform();
        
}
void ae::MainWindow::MainWindow::openFile() {
        QString fname = QFileDialog::getOpenFileName(this, "Open file", "",
                                                     "Music (*.mp3)");
        if (fname.isEmpty()) {  
            return;
        }

        decoder->setSource(fname);
        decoder->start();
}
