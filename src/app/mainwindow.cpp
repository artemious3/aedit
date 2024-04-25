//
// Created by artemious on 21.02.24.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "BaseEffect.h"
#include "TimelineScene.h"
#include "loader.h"
#include "ui_MainWindow.h"
#include <QAudioDecoder>
#include <QFileDialog>
#include <QKeyEvent>
#include <qaudiodecoder.h>
#include <qaudioformat.h>
#include <qfiledialog.h>
#include <qlogging.h>
#include <qmainwindow.h>
#include <qnamespace.h>
#include <qpushbutton.h>
#include "CoreAudio.h"
#include "portaudio.h"
#include "Constructor.h"

namespace ae {
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
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


  on_effectsBox_textActivated("Gain");

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

// void MainWindow::on_gain_clicked(){
//   auto effect = Constructor::getEffect("EQ");
//   effect->setUpUi(ui->effectWidget);

//   connect(effect, &BaseEffect::modifiedBuffer, this, &MainWindow::onBufferChanged);
// }


} // namespace ae

void ae::MainWindow::on_actionOpen_triggered() {
  QString fname =
      QFileDialog::getOpenFileName(this, "Open file", "", "Music (*.mp3)");
  if (fname.isEmpty()) {
    return;
  }

  loader->startDecoding(fname);
  ui->fnameLabel->setText(fname);
}

const TimelineScene* ae::MainWindow::getTimeline() {   
  return tlScene; 
}

void ae::MainWindow::onBufferChanged() {
  tlScene->drawWaveform();
}

void ae::MainWindow::on_effectsBox_textActivated(const QString& text) {
  if(currentEffect){
    delete currentEffect;
  }
  currentEffect = Constructor::getEffect(text.toStdString());
  if(!currentEffect){
    qDebug() << "Constructor: no such effect";
    return;
  }
  currentEffect->setUpUi(ui->effectWidget);
  connect(currentEffect, &BaseEffect::modifiedBuffer, this, &MainWindow::onBufferChanged);
}

void ae::MainWindow::on_navButton_toggled(bool b) {
  if(b){
    tlScene->setMouseBehaviour(MouseBehaviour::Navigation);
  }
}



void ae::MainWindow::on_selButton_toggled(bool b) {
    if(b){
      tlScene->setMouseBehaviour(MouseBehaviour::Selection);
    }
}

void ae::MainWindow::keyPressEvent(QKeyEvent* keyEvent) {    
  if(keyEvent->key() == Qt::Key_Space){
    if(CoreAudio::isPlaying()){
      qDebug() << "pausing";
      CoreAudio::pause();
    } else {
      qDebug() << "playing";
      CoreAudio::play();
    }
  }
}
