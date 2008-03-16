#ifndef COLORSTAT_H
#define COLORSTAT_H

#include <QWidget>
#include <QColor>
#include <QMap>

class QLabel;
class QVBoxLayout;
class QHBoxLayout;

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
  
  typedef QMap<QRgb, long> color_to_count_map_type;
  typedef QMap<long, QRgb> count_to_color_map_type;
  
  // TODO: integer overflow
  color_to_count_map_type colorToCount_;
  count_to_color_map_type countToColor_;
  
  QHBoxLayout *mainLayout_;
  QLabel      *upperText_;
};

#endif // COLORSTAT_H
