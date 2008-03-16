#ifndef COLORSTAT_H
#define COLORSTAT_H

#include <QWidget>
#include <QColor>
#include <QMap>

class QLabel;
class QVBoxLayout;

class ColorStatistics : public QWidget
{
  Q_OBJECT

public:
  ColorStatistics( QWidget *parent = 0 );

public slots:
  void setPixmap( QPixmap const &pixmap );
  
private slots:
  void updatePixmapInfo();
  
private:
  QPixmap pixmap_; // TODO: store only one copy of pixmap
  
  // TODO: integer overflow
  QMap<QRgb, long> colorsCounts_;
  
  QVBoxLayout *mainLayout_;
  QLabel      *upperText_;
};

#endif // COLORSTAT_H
