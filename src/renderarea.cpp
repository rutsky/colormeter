#include <QtGui>

#include "renderarea.h"

RenderArea::RenderArea( QWidget *parent )
  : QGraphicsView(parent)
  , drag_        (false)
{
  this->setBackgroundRole(QPalette::Base);
  this->setAutoFillBackground(true);
  
  scene_ = new QGraphicsScene;
  pixmapItem_ = scene_->addPixmap(pixmap_);
  
  this->setScene(scene_);
  this->setRenderHint(QPainter::SmoothPixmapTransform, false);
  // TODO: Some caching can improve rendering speed...
  this->setCacheMode(QGraphicsView::CacheBackground);
  
  this->setResizeAnchor(QGraphicsView::AnchorViewCenter);
  this->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
  
  this->setToolTip(tr("<b>Hold Control</b> and drag with left mouse button or zoom with wheel"));
}

QPixmap const & RenderArea::pixmap() const
{
  return pixmap_;
}

void RenderArea::setPixmap( QPixmap const &pixmap )
{
  pixmap_ = pixmap;
  this->setTransform(QTransform());
  replaceRenderingPixmap(pixmap_);
}

void RenderArea::setScale( double factor, bool absolute )
{
  if (absolute)
    this->setTransform(QTransform().scale(factor, factor));
  else
    this->scale(factor, factor);
}

// TODO: Incorrect keyboard modifiers

void RenderArea::keyPressEvent( QKeyEvent *event )
{
  if (event->key() == Qt::Key_Control)
    setDragging(true);
  else
    QGraphicsView::keyPressEvent(event);
}

void RenderArea::keyReleaseEvent( QKeyEvent *event )
{
  if (event->key() == Qt::Key_Control)
    setDragging(false);
  else
    QGraphicsView::keyPressEvent(event);
}

void RenderArea::mouseMoveEvent( QMouseEvent *event )
{
  setDragging(event->modifiers() & Qt::ControlModifier);
  QGraphicsView::mouseMoveEvent(event);
}

void RenderArea::wheelEvent( QWheelEvent *event )
{
  if (drag_)
  {
    for (int i = 0; i < abs(event->delta() / 120); ++i)
    {
      if (event->delta() < 0)
        emit zoomIn();
      else 
        emit zoomOut();
    }
  }
  else
    QGraphicsView::wheelEvent(event);
}

void RenderArea::setDragging( bool drag )
{
  if (drag_ == drag)
    return;
  
  drag_ = drag;
  if (drag)
  {
    this->setDragMode(QGraphicsView::ScrollHandDrag);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
  }
  else
  {
    this->setDragMode(QGraphicsView::NoDrag);
    this->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
  }
}

void RenderArea::replaceRenderingPixmap( QPixmap const &pixmap )
{
  scene_->removeItem(pixmapItem_);
  delete pixmapItem_;
  this->setSceneRect(pixmap.rect());
  pixmapItem_ = scene_->addPixmap(pixmap);
}
