#include "AudioPixmap.h"
#include <QPainter>
#include <cstdlib>
#include <qbrush.h>
#include <qimage.h>
#include <qnamespace.h>
#include <qpixmap.h>

AudioPixmap::AudioPixmap(const Sample *sample, int sz, int width, int height,
                         QBrush brush_)
    : QPixmap(width, height), sample(sample), size(sz), brush(brush_) {
  drawWaveform(0, sz);
  }

void AudioPixmap::drawWaveform(int beg, int end) {
  fill(Qt::transparent);
    QPainterPath path;
    
    auto scale = height() / 2;
    auto step = (end-beg)/width();
    for (int x = 0, i = beg; x < width(); i += step, ++x) {
        path.lineTo(x, height()/2 + sample[i] * scale);
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  
    painter.setPen(QPen(brush, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawPath(path);
}
