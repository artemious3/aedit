#ifndef AEDIT_TIMELINESCENE_H
#define AEDIT_TIMELINESCENE_H

#include "coretypes.h"
#include <QGraphicsScene>
#include <qcontainerfwd.h>
#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
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

  QGraphicsRectItem *selectionRect = nullptr;
  int selectionStart = 0, selectionEnd = 0;

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  //void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

public:
  TimelineScene(QWidget *widget = nullptr);
  ~TimelineScene();

  void setBuffer(StereoAudioBuffer buffer);
  void drawWaveform();
  void drawSelection();
};

#endif //AEDIT_TIMELINESCENE_H