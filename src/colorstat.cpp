#include <QtGui>

#include "colorstat.h"

ColorStatistics::ColorStatistics( QWidget *parent )
  : QWidget(parent)
{
}

void ColorStatistics::setPixmap( QPixmap const &pixmap )
{
  pixmap_ = pixmap;
}
