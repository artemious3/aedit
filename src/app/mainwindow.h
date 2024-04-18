//
// Created by artemious on 21.02.24.
//

#ifndef AEDIT_MAINWINDOW_H
#define AEDIT_MAINWINDOW_H

#include <QWidget>
#include <QAudio>
#include <QAudioFormat>
#include <QAudioSink>
#include <QAudioDecoder>
#include <qaudiodecoder.h>
#include "TimelineScene.h"
#include "loader.h"



namespace ae {

    QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
    QT_END_NAMESPACE


    class MainWindow : public QWidget {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() override;

      private:
        TimelineScene *tlScene;
        Loader* loader;


        bool isPressed = false;
        // void keyPressEvent(QKeyEvent* keyEvent);
        // void keyReleaseEvent(QKeyEvent* keyEvent);
        Ui::MainWindow *ui;

      public slots:
        void openFile();
        void onBufReady();

        void on_playBtn_clicked();
        void on_pauseBtn_clicked();
        void on_stopBtn_clicked();
        void on_gain_clicked();

        void onError(QAudioDecoder::Error err );

        void onBufferChanged();

      public:
        const TimelineScene* getTimeline();

    };
} // ae

#endif //AEDIT_MAINWINDOW_H
