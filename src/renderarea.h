#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QGraphicsView>

class QPixmap;
class QGraphicsView;

class RenderArea : public QGraphicsView
{
  Q_OBJECT

public:
  RenderArea( QWidget *parent = 0 );
  QPixmap const & pixmap() const;

public slots:
  void setPixmap( QPixmap const &pixmap );
  void setScale( double scaleFactor, bool absolute = false );

private:
  QPixmap pixmap_;
  
  QGraphicsScene      *scene_;
  QGraphicsPixmapItem *pixmapItem_;
};

#endif 
