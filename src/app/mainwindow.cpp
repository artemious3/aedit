//
// Created by artemious on 21.02.24.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "BaseEffect.h"
#include "loader.h"
#include "ui_MainWindow.h"
#include <QAudioDecoder>
#include <QFileDialog>
#include <QKeyEvent>
#include <qaudiodecoder.h>
#include <qaudioformat.h>
#include <qfiledialog.h>
#include <qlogging.h>
#include <qpushbutton.h>
#include "CoreAudio.h"
#include "portaudio.h"
#include "Constructor.h"

namespace ae {
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  PaError err = CoreAudio::init();
  CoreAudio::initializeTestStream();
  if (err != paNoError) {
    qDebug() << Pa_GetErrorText(err);
  }

  loader = new Loader;

  tlScene = new TimelineScene(ui->timeline);
  ui->timeline->setSceneRect(0, 0, ui->timeline->frameSize().width(),
                             ui->timeline->frameSize().height());
  ui->timeline->setScene(tlScene);
  tlScene->drawWaveform();

  on_gain_clicked();

  connect(ui->openFileBtn, &QPushButton::clicked, this, &MainWindow::openFile);

  connect(loader, &Loader::onFinish, this, &MainWindow::onBufReady);
}

MainWindow::~MainWindow() {
  CoreAudio::terminate();
  delete loader;
  delete ui;
}

void MainWindow::onError(QAudioDecoder::Error err) { qDebug() << "error!"; }

void MainWindow::onBufReady() {
    CoreAudio::setBuffer(loader->getResultingBuffer());
    tlScene->drawWaveform();
}

// void MainWindow::keyPressEvent(QKeyEvent *keyEvent) {
//   if (!keyEvent->isAutoRepeat()) {
//     QWidget::keyPressEvent(keyEvent);
//     CoreAudio::play();
//   }
// }

// void MainWindow::keyReleaseEvent(QKeyEvent *keyEvent) {
//   if (!keyEvent->isAutoRepeat()) {
//     CoreAudio::stop();
//   }
// }


void MainWindow::on_playBtn_clicked(){
  CoreAudio::play();
}

void MainWindow::on_pauseBtn_clicked(){
  CoreAudio::pause();
}

void MainWindow::on_stopBtn_clicked(){
  CoreAudio::stop();
}

void MainWindow::on_gain_clicked(){
  auto effect = Constructor::getEffect("EQ");
  effect->setUpUi(ui->effectWidget);

  connect(effect, &BaseEffect::modifiedBuffer, this, &MainWindow::onBufferChanged);
}


} // namespace ae

void ae::MainWindow::openFile() {
  QString fname =
      QFileDialog::getOpenFileName(this, "Open file", "", "Music (*.mp3)");
  if (fname.isEmpty()) {
    return;
  }

  loader->startDecoding(fname);
}

const TimelineScene* ae::MainWindow::getTimeline() {   
  return tlScene; 
}

void ae::MainWindow::onBufferChanged() {
  tlScene->drawWaveform();
}
