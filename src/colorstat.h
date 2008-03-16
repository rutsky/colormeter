#ifndef COLORSTAT_H
#define COLORSTAT_H

#include <QWidget>

class ColorStatistics : public QWidget
{
  Q_OBJECT

public:
  ColorStatistics( QWidget *parent = 0 );

public slots:
  void setPixmap( QPixmap const &pixmap );
  
private:
  QPixmap pixmap_; // TODO: store only one copy of pixmap
};

#endif // COLORSTAT_H
