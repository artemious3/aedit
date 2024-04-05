#include "coretypes.h"
#include <qbrush.h>
#include <qgraphicsitem.h>
#include <qpixmap.h>

class AudioPixmap : public QPixmap{
private:
  const Sample* sample;
  int size;
  void drawWaveform(int beg, int end);
  QBrush brush;

public:
  AudioPixmap(const Sample *sample, int sz, int width, int height, QBrush brush);
  void scale(int width, int height, int start, int end);

};