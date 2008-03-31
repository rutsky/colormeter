#ifndef COLORSTAT_H
#define COLORSTAT_H

#include <QWidget>
#include <QColor>
#include <QMap>
#include <QMultiMap>
#include <QVector>

class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QTextBrowser;
class QPushButton;

struct ColorsInfo
{
  // TODO: Incapsulation
  
  // TODO: integer overflow?
  typedef QMap<QRgb, quint64> color_to_count_map_type;
  
  color_to_count_map_type colorToCount;
  
  // Calculates nColors, minColor, avgColor, maxColor, colorVariance
  void calcColorStats();
    
  quint64 nPixels;
  QRgb minColor, avgColor, maxColor, colorVariance;
};

class ColorStatistics : public QWidget
{
  Q_OBJECT

public:
  ColorStatistics( QWidget *parent = 0 );

public slots:
  void setPixmap( QPixmap const &pixmap );
  
private slots:
  void updateReport();
  void insertTable();
  void saveTableToFile();
  void saveDiagramToFile();
  
private:
  QPixmap pixmap_; // TODO: store only one copy of pixmap
  
  QPixmap diagram_;
  
  QVector<ColorsInfo> layers_;
  
  QHBoxLayout        *mainLayout_;
  QLabel             *upperText_;
  QPushButton        *saveTable_, *saveDiagram_;
  QLabel             *diagramLabel_;
  QTextBrowser       *editor_;
  QString            tableInHTML_;
};

#endif // COLORSTAT_H
