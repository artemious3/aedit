#include "EffectSelection.h"
#include <QWidget>
#include <qcolor.h>
#include <qfont.h>
#include <qgraphicsitem.h>
#include <qnamespace.h>
#include <qpainter.h>
#include "CoreInfo.h"

int EffectSelection::cur_hue = 0;

using namespace ae;

void EffectSelection::setSelected(bool sel) {


  if (!begText) {
    begText = new QGraphicsTextItem(this);
    begText->setFont(QFont("Monospace", FONT_SIZE));
    begText->setPlainText(CoreInfo::getTimeString(beg));
    begText->setPos(0, 0);
    
  }
  if (!endText) {
    endText = new QGraphicsTextItem(this);
    endText->setFont(QFont("Monospace", FONT_SIZE));
    endText->setPlainText(CoreInfo::getTimeString(end));

    auto brect = endText->boundingRect();
    auto end_y = (rect().width() <= 2 * brect.width()) ? brect.height() : 0;
    endText->setPos(rect().width() - endText->boundingRect().width(), end_y);
  }

  if (sel) {
    begText->show();
    endText->show();
    color.setAlpha(150);
  
  } else {
    begText->hide();
    endText->hide();
    color.setAlpha(100);
  }

  setBrush(color);
  update();
}



EffectSelection::~EffectSelection() {
  begText->deleteLater();
  endText->deleteLater();
}

EffectSelection::EffectSelection(qreal x, qreal y, qreal w, qreal h, int b, int e) : beg(b), end(e),
     color(QColor::fromHsv(cur_hue, DEF_SATURATION, DEF_VALUE, 100)),
      QGraphicsRectItem(x, y, w, h) {
  cur_hue = (cur_hue + HUE_STEP) % 256;
}

