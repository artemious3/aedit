#include "TimelineScene.h"
#include <QPainterPath>
#include <QGraphicsItem>
#include <algorithm>
#include <QGraphicsView>
#include <QPainter>
#include <cstddef>
#include <qcolor.h>
#include <qdebug.h>
#include <qgraphicsitem.h>
#include <QGraphicsSceneMouseEvent>
#include <qnamespace.h>
#include <qtimer.h>
#include "AudioPixmap.h"
#include "CoreAudio.h"


TimelineScene::TimelineScene(QWidget *widget) : QGraphicsScene(widget) {
  buffer = {nullptr, nullptr, 0, 0, false};
  updater = new QTimer(this);

  connect(updater, &QTimer::timeout, this, &TimelineScene::updatePosPointer);
  updater->start(64);
}

TimelineScene::~TimelineScene() {
  delete posPointer;
  delete updater;
}

void TimelineScene::setBuffer(StereoAudioBuffer buffer_) {
  buffer = buffer_;
}

void TimelineScene::drawWaveform() {
  clear();
  viewWidth = views().first()->width();
  viewHeight = views().first()->height();
  posPointer = nullptr;
  if (buffer.size == 0 || views().size() == 0) {
    qDebug() << "No buffer or no views";
    return;
  }

  auto height = views().first()->height();
  auto width = views().first()->width();

  AudioPixmap leftWaveform(buffer.left, buffer.size, width, height / 2,
                           LEFT_WAVEFORM_COLOR);
  AudioPixmap rightWaveform(buffer.right, buffer.size, width, height / 2,
                            RIGHT_WAVEFORM_COLOR);

  QGraphicsPixmapItem *leftItem = new QGraphicsPixmapItem(leftWaveform);
  QGraphicsPixmapItem *rightItem = new QGraphicsPixmapItem(rightWaveform);

  leftItem->setPos(0, 0);
  rightItem->setPos(0, height / 2);

  addItem(leftItem);
  addItem(rightItem);
}

void TimelineScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    selectionStart = event->scenePos().x();
    selectionEnd = selectionStart;
  }
  
  if(selectionRect){
    removeItem(selectionRect);
    delete selectionRect;
    selectionRect = nullptr;
  }
}

void TimelineScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  selectionEnd = event->scenePos().x();
  drawSelection();
}

void TimelineScene::drawSelection() {
  auto left = selectionStart;
  auto right = selectionEnd;
  if(right < left){
    std::swap(left, right);
  }

  if (!selectionRect) {
    selectionRect = addRect(left, 0, right-left,
                            sceneRect().height(), QPen(), SELECTION_COLOR);
  } else {
    selectionRect->setRect(left, 0, right-left, views().first()->height());
  }
}


// DANGEROUS TEST
void TimelineScene::updatePosPointer() {
  using namespace ae;
  auto buf = CoreAudio::getBuffer();
  auto cur = buf.current;
  auto size = buf.size;

  if(!posPointer){
    posPointer = new QGraphicsLineItem(0,0,0, height());
    posPointer->setPen(posPtrPen);
    addItem(posPointer);
  }
  
  posPointer->setPos( (float)cur/size * viewWidth, 0 );
}
