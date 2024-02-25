//
// Created by artemious on 21.02.24.
//

#ifndef AEDIT_MAINWINDOW_H
#define AEDIT_MAINWINDOW_H

#include <QWidget>
#include <QAudio>
#include <QAudioFormat>
#include <QAudioSink>

#include "AudioSynthBuffer.h"


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
        QAudioFormat aFormat;
        QAudioSink* playback;
        ae::AudioSynthBuffer* synthBuffer;
      std::map<int, int> notes = { {Qt::Key_A, 261.6},
                                   {Qt::Key_W, 77.2},
                                   {Qt::Key_S, 293.7},
                                   {Qt::Key_E, 311.1},
                                   {Qt::Key_D, 329.6},
                                   {Qt::Key_F, 349.2},
                                   {Qt::Key_T, 370.0},
                                   {Qt::Key_G, 392.0},
                                   {Qt::Key_Y, 415.3},
                                   {Qt::Key_H, 440.0},
                                   {Qt::Key_U, 466.2},
                                   {Qt::Key_J, 493.9} };

        bool isPressed = false;
        void keyPressEvent(QKeyEvent* keyEvent);
        void keyReleaseEvent(QKeyEvent* keyEvent);
        Ui::MainWindow *ui;
        void playStart();
        void playStop();
    };
} // ae

#endif //AEDIT_MAINWINDOW_H
