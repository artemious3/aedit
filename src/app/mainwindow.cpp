//
// Created by artemious on 21.02.24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"
#include "coreInfo.h"
#include <QKeyEvent>

namespace ae {
    MainWindow::MainWindow(QWidget *parent) :
            QWidget(parent), ui(new Ui::MainWindow) {
      ui->setupUi(this);
      synthBuffer = new ae::AudioSynthBuffer(this);

      aFormat.setSampleRate(SAMPLING_FREQUENCY);
      aFormat.setSampleFormat(QAudioFormat::Int16);
      aFormat.setChannelCount(1);

      playback = new QAudioSink(aFormat, this);
      synthBuffer->open(QIODevice::ReadOnly);
    }

    MainWindow::~MainWindow() {
      delete ui;
    }

    void MainWindow::playStart() {


    }

    void MainWindow::keyReleaseEvent(QKeyEvent *keyEvent) {
      isPressed = false;
      QWidget::keyReleaseEvent(keyEvent);
      playback->suspend();
      //playback->reset();
      //qDebug() << playback->state();
    }

    void MainWindow::keyPressEvent(QKeyEvent *keyEvent) {
      if (isPressed) {
        return;
      }
      QWidget::keyPressEvent(keyEvent);

      if(notes.find(keyEvent->key()) == notes.end()){
        return;
      } else{
        isPressed = true;
        synthBuffer->setFrequency(notes[keyEvent->key()]);
        playback->start(synthBuffer);
      }

      //qDebug() << playback->state();

    }
} // ae
