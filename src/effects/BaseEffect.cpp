#include "BaseEffect.h"
#include <QFormLayout>
#include <qalgorithms.h>
#include <qapplication.h>
#include <qgridlayout.h>
#include <qmessagebox.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <QMessageBox>
#include "CoreAudio.h"
#include <QApplication>
#include "mainwindow.h"
#include <QtConcurrent>
#include <qtconcurrentrun.h>
#include <qthreadpool.h>
#include <qtmetamacros.h>

using namespace ae;

void BaseEffect::setUpUi(QWidget* widget ) {
    gui = widget;
    layout = new QFormLayout(widget);

    applyBtn = new QPushButton("Apply",     widget);
    layout->insertRow(0, applyBtn);
    applyBtn->setFocusPolicy(Qt::NoFocus);
    widget->setFocusPolicy(Qt::NoFocus);
    connect(applyBtn, &QPushButton::clicked, this, &BaseEffect::apply);
}

void BaseEffect::apply(){
    if(!gui){
        qDebug() << "BaseEffect: No gui installed!";
        return;
    }

    auto window = MainWindow::getInstance();

    updateProperties();

    auto tl =  window->getTimeline();
    auto sel = tl->getSelection();
    auto buf = CoreAudio::getBuffer();

    auto beg = sel.first;
    auto size = sel.second - sel.first;


    if( size <= 1 ){
        QMessageBox::information(window, "Info", "Select an audio area.");
        return;
    }


    auto max = buf.size - sel.first;

    gui->setEnabled(false);
    window->blockAudio(true);


QThreadPool::globalInstance()->start([=]{

     _process(&buf.left[ beg ], size, max, 0);
    reset();    
    _process(&buf.right[ beg ], size, max, 1);

    emit modifiedBuffer(beg, beg + size, objectName());
    gui->setEnabled(true);
    window->blockAudio(false);
    
});





}

BaseEffect::~BaseEffect() {
    qDeleteAll(gui->children());
}    


void BaseEffect::reset() {}
