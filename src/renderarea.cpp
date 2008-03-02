#include <QtGui>

#include "renderarea.h"

RenderArea::RenderArea( QWidget *parent )
  : QGraphicsView(parent)
{
  this->setBackgroundRole(QPalette::Base);
  this->setAutoFillBackground(true);
  
  pixmapItem_ = new QGraphicsPixmapItem;
  pixmapItem_->setPixmap(pixmap_);
  
  scene_ = new QGraphicsScene;
  scene_->addItem(pixmapItem_);
  
  this->setScene(scene_);
  this->setRenderHint(QPainter::SmoothPixmapTransform, false);
  // TODO: Some caching can improve rendering speed...
  //this->setCacheMode(QGraphicsView::CacheBackground);
  //this->setDragMode(QGraphicsView::ScrollHandDrag);
  //this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
  //this->setResizeAnchor(QGraphicsView::AnchorViewCenter);
}

QPixmap const & RenderArea::pixmap() const
{
  return pixmap_;
}

void RenderArea::setPixmap( QPixmap const &pixmap )
{
  pixmap_ = pixmap;
  this->setScale(1.0, true);
  pixmapItem_->setPixmap(pixmap_);
  this->update();
}

void RenderArea::setScale( double factor, bool absolute )
{
  if (absolute)
    this->setTransform(QTransform().scale(factor, factor));
  else
    this->scale(factor, factor);
  
  //scene_->update();
  //this->update();
}
