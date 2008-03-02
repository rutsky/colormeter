#include <QtGui>

#include "renderarea.h"

RenderArea::RenderArea( QWidget *parent )
  : QGraphicsView(parent)
{
  this->setBackgroundRole(QPalette::Base);
  this->setAutoFillBackground(true);
  
  scene_ = new QGraphicsScene;
  pixmapItem_ = scene_->addPixmap(pixmap_);
  
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
  this->setTransform(QTransform());
  
  pixmap_ = pixmap;
  
  scene_->removeItem(pixmapItem_);
  delete pixmapItem_;
  this->setSceneRect(pixmap_.rect());
  
  pixmapItem_ = scene_->addPixmap(pixmap_);
}

void RenderArea::setScale( double factor, bool absolute )
{
  if (absolute)
    this->setTransform(QTransform().scale(factor, factor));
  else
    this->scale(factor, factor);
}
