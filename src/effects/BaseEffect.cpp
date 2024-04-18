#include "BaseEffect.h"
#include <QFormLayout>
#include <qgridlayout.h>
#include <qobject.h>
#include <qpushbutton.h>

#include "CoreAudio.h"
#include "mainwindow.h"

using namespace ae;

void BaseEffect::setUpUi(QWidget* widget ) {
    gui = widget;
    layout = new QFormLayout(widget);

    applyBtn = new QPushButton("Apply",     widget);

    layout->insertRow(0, applyBtn);

    connect(applyBtn, &QPushButton::clicked, this, &BaseEffect::apply);
}

void BaseEffect::apply(){
    if(!gui){
        qDebug() << "BaseEffect: No gui installed!";
        return;
    }

    auto window = qobject_cast<MainWindow*>(gui->parent()) ;
    if(!window){
        qDebug() << "BaseEffect: Parent is not a window";
        return;
    }

    updateProperties();

    auto tl =  window->getTimeline();
    auto sel = tl->getSelection();
    auto buf = CoreAudio::getBuffer();

    auto beg = sel.first;
    auto size = sel.second - sel.first;
    auto max = buf.size - sel.first;

    _process(&buf.left[ beg ], size, max);
    _process(&buf.right[ beg    ], size, max);

    emit modifiedBuffer();    
}

BaseEffect::~BaseEffect() {
    applyBtn->deleteLater();
    revertBtn->deleteLater();
    saveBtn->deleteLater();
    layout->deleteLater();    
}
