//
// Created by artemious on 21.02.24.
//

#ifndef AEDIT_MAINWINDOW_H
#define AEDIT_MAINWINDOW_H

#include <QWidget>
#include <QAudio>
#include <QAudioFormat>
#include <QAudioSink>



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

        bool isPressed = false;
        void keyPressEvent(QKeyEvent* keyEvent);
        void keyReleaseEvent(QKeyEvent* keyEvent);
        Ui::MainWindow *ui;
        void playStart();
        void playStop();
    };
} // ae

#endif //AEDIT_MAINWINDOW_H
