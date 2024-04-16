#ifndef AEDIT_TIMELINESCENE_H
#define AEDIT_TIMELINESCENE_H

#include "coretypes.h"
#include <QGraphicsScene>
#include <qcontainerfwd.h>
#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
#include <qnamespace.h>
#include <qpixmap.h>
#include <qscopedpointer.h>
#include <qvariant.h>
#include <qwidget.h>

class TimelineScene : public QGraphicsScene {
  Q_OBJECT

public:
  static constexpr QColor SELECTION_COLOR = QColor(100, 100, 100, 100);
  static constexpr QColor LEFT_WAVEFORM_COLOR = QColor(200, 200, 240, 255);
  static constexpr QColor RIGHT_WAVEFORM_COLOR = QColor(240, 200, 200, 255);

private:
  StereoAudioBuffer buffer;
  QTimer* updater;

  int viewWidth = 0;
  int viewHeight = 0; 

  QGraphicsRectItem *selectionRect = nullptr;
  QGraphicsLineItem *posPointer = nullptr;
  int selectionStart = 0, selectionEnd = 0;

  const QPen posPtrPen = {Qt::white, 4, Qt::SolidLine, Qt::RoundCap};

private slots:
  void updatePosPointer();

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

public:
  TimelineScene(QWidget *widget = nullptr);
  ~TimelineScene();

  void setBuffer(StereoAudioBuffer buffer);
  void drawWaveform();
  void drawSelection();
};

#endif // AEDIT_TIMELINESCENE_H