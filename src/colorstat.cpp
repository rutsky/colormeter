#include <QtGui>

#include <cmath>

#include "colorstat.h"

namespace
{
  // TODO
  int value( QRgb const &x )
  {
    return qRed(x) + qGreen(x) + qBlue(x);
  }
  
  template< class T > T sqr( T x ) { return x * x; }
} // End of anonymous namespace

// Calculates minColor, avgColor, maxColor
void ColorsInfo::calcColorStats()
{
  qDebug() << colorToCount.empty() << " " << colorToCount.size() << " " << countToColor.size(); // debug
  Q_ASSERT(!colorToCount.empty() && colorToCount.size() == countToColor.size());
    
  minColor = qRgb(255, 255, 255);
  maxColor = qRgb(0, 0, 0);
    
  // Seriously, is there overflow or not?
  quint64 rTotal, gTotal, bTotal; // TODO: more overflow stable counting
  
  // Calculating minimum, maximum and average colors
  rTotal = 0, gTotal = 0, bTotal = 0, nColors = 0;
  typedef color_to_count_map_type::const_iterator CI;
  for (CI it = colorToCount.begin(); it != colorToCount.end(); ++it)
  {
    // TODO: be smart
    if (qRed(minColor)   > qRed(it.key()))
      minColor = qRgb(qRed(it.key()), qGreen(minColor), qBlue(minColor));
    if (qGreen(minColor) > qGreen(it.key()))
      minColor = qRgb(qRed(minColor), qGreen(it.key()), qBlue(minColor));
    if (qBlue(minColor)  > qBlue(it.key()))
      minColor = qRgb(qRed(minColor), qGreen(minColor), qBlue(it.key()));
    
    if (qRed(maxColor)   < qRed(it.key()))
      maxColor = qRgb(qRed(it.key()), qGreen(maxColor), qBlue(maxColor));
    if (qGreen(maxColor) < qGreen(it.key()))
      maxColor = qRgb(qRed(maxColor), qGreen(it.key()), qBlue(maxColor));
    if (qBlue(maxColor)  < qBlue(it.key()))
      maxColor = qRgb(qRed(maxColor), qGreen(maxColor), qBlue(it.key()));
    
    rTotal += it.value() * qRed  (it.key());
    gTotal += it.value() * qGreen(it.key());
    bTotal += it.value() * qBlue (it.key());
    
    nColors += it.value();
  }
  
  // TODO: Correct?
  avgColor = qRgb(rTotal / nColors, gTotal / nColors, bTotal / nColors);
  
  // Calculating color dispersion
  rTotal = 0, gTotal = 0, bTotal = 0;
  for (CI it = colorToCount.begin(); it != colorToCount.end(); ++it)
  {
    rTotal += it.value() * sqr(qRed  (it.key()) - qRed  (avgColor));
    gTotal += it.value() * sqr(qGreen(it.key()) - qGreen(avgColor));
    bTotal += it.value() * sqr(qBlue (it.key()) - qBlue (avgColor));
  }
  
  // TODO: Correct?
  colorVariance = qRgb(::sqrt(rTotal / nColors), ::sqrt(gTotal / nColors), ::sqrt(bTotal / nColors));
}

ColorStatistics::ColorStatistics( QWidget *parent )
  : QWidget(parent)
{
  upperText_ = new QLabel;
  
  QVBoxLayout *colorsStatisticsLayout = new QVBoxLayout;
  colorsStatisticsLayout->addWidget(upperText_);
  colorsStatisticsLayout->addStretch(1);
  
  QWidget *colorsStatisticsTabWidget = new QWidget;
  colorsStatisticsTabWidget->setLayout(colorsStatisticsLayout);
  
  QTabWidget *tabWidget = new QTabWidget;
  tabWidget->addTab(colorsStatisticsTabWidget, tr("Colors statistics"));
  
  mainLayout_ = new QHBoxLayout;
  mainLayout_->addWidget(tabWidget);
  setLayout(mainLayout_);
  
  //setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  
  updateReport();
}

void ColorStatistics::setPixmap( QPixmap const &pixmap )
{
  pixmap_ = pixmap;
  updateReport();
}

void ColorStatistics::updateReport()
{
  if (pixmap_.isNull())
    upperText_->setText(tr("No image opened."));
  else
  {
    QImage image = pixmap_.toImage();

    // Generating report
    
    layers_.clear();
    
    layers_.push_back(ColorsInfo());
    ColorsInfo &layer = layers_.back();
    
    // Counting colors
    {
      QProgressDialog progress(tr("Counting colors..."), 0, 0, pixmap_.height(), this);
      progress.setModal(true);
      // TODO: Progress cancellation handling
   
      for (int row = 0; row < pixmap_.height(); ++row)
      {
        progress.setValue(row);
        qApp->processEvents();
        
        for (int column = 0; column < pixmap_.width(); ++column)
          ++layer.colorToCount[image.pixel(column, row)];
      }
    }
    
    // Sorting colors
    {
      QProgressDialog progress(tr("Sorting colors..."), 0, 0, layer.colorToCount.size(), this);
      progress.setModal(true);
      // TODO: Progress cancellation handling
      
      long progressValue(0);
      typedef ColorsInfo::color_to_count_map_type::const_iterator CI;
      for (CI it = layer.colorToCount.begin(); it != layer.colorToCount.end(); ++it, ++progressValue)
      {
        progress.setValue(progressValue);
        qApp->processEvents();

        layer.countToColor.insert(it.value(), it.key());
      }
    }
    
    layer.calcColorStats();
    
    // Filtering
    {
      QProgressDialog progress(tr("Filtering..."), 0, 0, layers_.front().colorToCount.size(), this);
      progress.setModal(true);
      // TODO: Progress cancellation handling
      
      while (layers_.back().colorToCount.size() > 1)
      {
        layers_.push_back(ColorsInfo());
        ColorsInfo &layer = layers_.back();
        
        ColorsInfo const &oldLayer = *(layers_.end() - 2);
        
        progress.setValue(layers_.front().colorToCount.size() - oldLayer.colorToCount.size());
        qApp->processEvents();
        
        qDebug() << QColor(oldLayer.minColor) << " " << QColor(oldLayer.maxColor) << " " << 
            QColor(oldLayer.avgColor) << " " << QColor(oldLayer.colorVariance); // debug
        
        typedef ColorsInfo::color_to_count_map_type::const_iterator CI;
        for (CI it = oldLayer.colorToCount.begin(); it != oldLayer.colorToCount.end(); ++it)
        {
          if (qRed  (oldLayer.avgColor) - qRed  (oldLayer.colorVariance) <= qRed  (it.key()) &&
              qRed  (oldLayer.avgColor) + qRed  (oldLayer.colorVariance) >= qRed  (it.key()) &&
              qGreen(oldLayer.avgColor) - qGreen(oldLayer.colorVariance) <= qGreen(it.key()) &&
              qGreen(oldLayer.avgColor) + qGreen(oldLayer.colorVariance) >= qGreen(it.key()) &&
              qBlue (oldLayer.avgColor) - qBlue (oldLayer.colorVariance) <= qBlue (it.key()) &&
              qBlue (oldLayer.avgColor) + qBlue (oldLayer.colorVariance) >= qBlue (it.key()))
          {
            layer.colorToCount.insert(it.key(), it.value());
            layer.countToColor.insert(it.value(), it.key());
          }
        }
        
        Q_ASSERT(layer.colorToCount.size() < oldLayer.colorToCount.size());
        layer.calcColorStats();
      }
    }
    
    // Updating report text
    
    upperText_->setText(tr("<p>%1 pixels, %2 colors.</p>").arg(pixmap_.width() * pixmap_.height()).arg(layers_.front().colorToCount.size()));
  }
}
