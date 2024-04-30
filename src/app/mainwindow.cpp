//
// Created by artemious on 21.02.24.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "BaseEffect.h"
#include "CoreInfo.h"
#include "TimelineScene.h"
#include "exporter.h"
#include "loader.h"
#include "ui_MainWindow.h"
#include <QAudioDecoder>
#include <QFileDialog>
#include <QKeyEvent>
#include <qaudiodecoder.h>
#include <qaudioformat.h>
#include <qcoreevent.h>
#include <qfiledialog.h>
#include <qlogging.h>
#include <qmainwindow.h>
#include <qmessagebox.h>
#include <qnamespace.h>
#include <qpushbutton.h>
#include <stdexcept>
#include "CoreAudio.h"
#include "portaudio.h"
#include "Constructor.h"
#include <QTimer>

namespace ae {

  ae::MainWindow* MainWindow::_instance = nullptr;

  
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  PaError err = CoreAudio::init();
  CoreAudio::initializeTestStream();
  if (err != paNoError) {
    qDebug() << Pa_GetErrorText(err);
  }

  loader = new Loader;

  ui->timeline->setMouseTracking(false);
  tlScene = new TimelineScene(ui->timeline);
  ui->timeline->setSceneRect(0, 0, ui->timeline->frameSize().width(),
                             ui->timeline->frameSize().height());
  ui->timeline->setScene(tlScene);
  tlScene->drawWaveform();

  on_effectsBox_textActivated("Gain");

  connect(loader, &Loader::onFinish, this, &MainWindow::onBufReady);
  foreach( auto w, findChildren<QWidget*>() ){
    w->setFocusPolicy(Qt::NoFocus);
  }

  }

MainWindow::~MainWindow() {
  CoreAudio::terminate();
  delete loader;
  delete ui;
}

void MainWindow::onError(QAudioDecoder::Error err) { qDebug() << "error!"; }

void MainWindow::onBufReady() {
    CoreAudio::setBuffer(loader->getResultingBuffer());
    clearHistory();
}


void MainWindow::on_playBtn_clicked(){
  CoreAudio::play();
}

void MainWindow::on_pauseBtn_clicked(){
  CoreAudio::pause();
}

void MainWindow::on_stopBtn_clicked(){
  CoreAudio::stop();
}


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


 void ae::MainWindow::on_actionExport_triggered(){
  auto fname = QFileDialog::getSaveFileName(this, "Export file", {}, "PCM WAV (*.wav)");
  if(fname.isEmpty()){
    return;
  }

  try{
    Exporter::exportCoreBuffer(fname);
  } catch (const std::runtime_error&){
    QMessageBox::warning(this, "Warning", "Could not save the file");
  }
  
 }

const TimelineScene* ae::MainWindow::getTimeline() {   
  return tlScene; 
}

void ae::MainWindow::onBufferChanged(int beg, int end, QString changeInfo) {
  tlScene->drawWaveform();
  tlScene->pushEffect(beg, end);
  ui->historyList->addItem(changeInfo);
  ui->historyList->setCurrentRow(ui->historyList->count() - 1);
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
  ui->effectWidget->setFocusPolicy(Qt::NoFocus);
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
  QMainWindow::keyPressEvent(keyEvent);
  if(isBlocked){
    return;
  }
  if(keyEvent->key() == Qt::Key_Space){
    if(CoreAudio::isPlaying()){
      qDebug() << "pausing";
      CoreAudio::pause();
    } else {
      CoreAudio::play();
    }
  }
}

ae::MainWindow* ae::MainWindow::MainWindow::getInstance() {
  if(!_instance){
    _instance = new MainWindow();
  }
  return _instance;
}

void ae::MainWindow::MainWindow::releaseInstance() {
  if(_instance){
    delete _instance;
  }    
}

void ae::MainWindow::on_historyList_currentRowChanged(int row) {
  tlScene->selectEffect( row );
}



void ae::MainWindow::MainWindow::setTimeLabel(QString s) {
  ui->timeLbl->setText(s);    
}

void ae::MainWindow::MainWindow::clearHistory() {
      tlScene->resetEffects();
    ui->historyList->clear();
    tlScene->drawWaveform();
}

void ae::MainWindow::MainWindow::blockAudio(bool b) {    
  isBlocked = b;
  ui->playBtn->setEnabled(!b);
  ui->pauseBtn->setEnabled(!b);
  ui->stopBtn->setEnabled(!b);
  if(b){
    CoreAudio::pause();
  }
}
