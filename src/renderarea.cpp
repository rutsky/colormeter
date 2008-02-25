#include <QtGui>

#include "renderarea.h"

RenderArea::RenderArea( QWidget *parent )
    : QWidget(parent)
{
  scaleFactor = 1.0;
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);
}

QSize RenderArea::minimumSizeHint() const
{
  if (image.isNull())
    return QSize(0, 0);
  else
    return image.size();
}

QSize RenderArea::sizeHint() const
{
  if (image.isNull())
    return QSize(0, 0);
  else
    return image.size();
}

void RenderArea::setImage( QImage const &image )
{
  this->image = image;
  scaleFactor = 1.0;
  setScale(scaleFactor);
  update();
}

void RenderArea::paintEvent( QPaintEvent * /* event */ )
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, false);
  
  painter.scale(scaleFactor, scaleFactor);
  painter.drawImage(0, 0, image);

  //painter.setPen(palette().dark().color());
  //painter.setBrush(Qt::NoBrush);
  //painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
} 

void RenderArea::setScale( double scaleFactor )
{
  this->scaleFactor = scaleFactor;
  resize(image.size() * scaleFactor);
}
