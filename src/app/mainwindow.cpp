//
// Created by artemious on 21.02.24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"
#include <QKeyEvent>
#include "portaudio.h"


#include "CoreAudio.h"

namespace ae {
    MainWindow::MainWindow(QWidget *parent) :
            QWidget(parent), ui(new Ui::MainWindow) {
      ui->setupUi(this);
      PaError err = CoreAudio::init();
      CoreAudio::initializeTestStream();
      if(err != paNoError){
          qDebug() << Pa_GetErrorText(err);
      }
    }

    MainWindow::~MainWindow() {
        CoreAudio::terminate();
        delete ui;
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
