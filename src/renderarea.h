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

signals:
  void zoomIn();
  void zoomOut();
  
public slots:
  void setPixmap( QPixmap const &pixmap );
  void setScale( double scaleFactor, bool absolute = false );
  
protected:
  void keyPressEvent  ( QKeyEvent *event );
  void keyReleaseEvent( QKeyEvent *event );
  void mouseMoveEvent( QMouseEvent *event );
  void wheelEvent( QWheelEvent *event );
  
private:
  void setDragging( bool drag );
  void replaceRenderingPixmap( QPixmap const &pixmap );

private:
  bool drag_;
  
  QPixmap    pixmap_;    // Original pixmap
  
  QGraphicsScene      *scene_;
  QGraphicsPixmapItem *pixmapItem_;
};

#endif // RENDERAREA_H
