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
#include <QMainWindow>
#include <qlistwidget.h>
#include "BaseEffect.h"
#include "TimelineScene.h"
#include "loader.h"

namespace ae {

    QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
    QT_END_NAMESPACE


    class MainWindow : public QMainWindow {
    Q_OBJECT

    private:
        static ae::MainWindow* _instance;
        explicit MainWindow(QWidget *parent = nullptr);
    public:
        ~MainWindow() override;



      private:
        TimelineScene *tlScene;
        Loader* loader;
        BaseEffect* currentEffect = nullptr;

        bool isPressed = false;
        void keyPressEvent(QKeyEvent* keyEvent) override;
        // void keyReleaseEvent(QKeyEvent* keyEvent);
        Ui::MainWindow *ui;


      public slots:
        void on_actionOpen_triggered();
        void onBufReady();

        void on_playBtn_clicked();
        void on_pauseBtn_clicked();
        void on_stopBtn_clicked();
        // void on_gain_clicked();

        void on_effectsBox_textActivated(const QString& text);

        void on_navButton_toggled(bool b);
        void on_selButton_toggled(bool b);
        void on_historyList_currentRowChanged(int row);
        

        void onError(QAudioDecoder::Error err );

        void onBufferChanged(int, int, QString);


      public:
        const TimelineScene* getTimeline();

      public:
        static ae::MainWindow* getInstance();
        static void releaseInstance();

    };
} // ae

#endif //AEDIT_MAINWINDOW_H
