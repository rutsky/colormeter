#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QImage>

class RenderArea : public QWidget
{
  Q_OBJECT

public:
  RenderArea( QWidget *parent = 0 );

  QSize minimumSizeHint() const;
  QSize sizeHint() const;

public slots:
  void setImage( QImage const &image );
  void setScale( double scaleFactor );

protected:
  void paintEvent(QPaintEvent *event);

private:
  double scaleFactor;
  QImage image;
};

#endif 
