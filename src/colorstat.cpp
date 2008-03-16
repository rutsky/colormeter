#include <QtGui>

#include "colorstat.h"

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
  
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  
  updatePixmapInfo();
  
}

void ColorStatistics::setPixmap( QPixmap const &pixmap )
{
  pixmap_ = pixmap;
  updatePixmapInfo();
}

void ColorStatistics::updatePixmapInfo()
{
  if (pixmap_.isNull())
    upperText_->setText(tr("No image opened."));
  else
  {
    QImage image = pixmap_.toImage();

    {
      QProgressDialog progress(tr("Couting colors..."), 0, 0, pixmap_.height(), this);
      progress.setModal(true);
      // TODO: Progress cancellation handling
   
      for (int row = 0; row < pixmap_.height(); ++row)
      {
        progress.setValue(row);
        qApp->processEvents();
        
        for (int column = 0; column < pixmap_.width(); ++column)
          ++colorToCount_[image.pixel(column, row)];
      }
    }
    
    {
      QProgressDialog progress(tr("Sorting colors..."), 0, 0, colorToCount_.size(), this);
      progress.setModal(true);
      // TODO: Progress cancellation handling
      
      long progressValue(0);
      for (color_to_count_map_type::const_iterator it = colorToCount_.begin(); it != colorToCount_.end(); ++it, ++progressValue)
      {
        progress.setValue(progressValue);
        qApp->processEvents();

        countToColor_.insert(it.value(), it.key());
      }
    }
    
    upperText_->setText(tr("<p>%1 pixels, %2 colors.</p>").arg(pixmap_.width() * pixmap_.height()).arg(colorToCount_.size()));
  }
}
