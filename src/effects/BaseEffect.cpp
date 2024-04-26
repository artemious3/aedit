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

using namespace ae;

void BaseEffect::setUpUi(QWidget* widget ) {
    gui = widget;
    layout = new QFormLayout(widget);

    applyBtn = new QPushButton("Apply",     widget);
    layout->insertRow(0, applyBtn);
    applyBtn->setFocusPolicy(Qt::FocusPolicy::TabFocus);

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

    qDebug() << max << size;
    _process(&buf.left[ beg ], size, max);
    _process(&buf.right[ beg ], size, max);

    emit modifiedBuffer();    
}

BaseEffect::~BaseEffect() {
    qDeleteAll(gui->children());
}
