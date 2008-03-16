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
  
  mainLayout_ = new QVBoxLayout;
  mainLayout_->addWidget(tabWidget);
  setLayout(mainLayout_);
  
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
    upperText_->setText(tr("<p>%1 pixels</p>").arg(pixmap_.width() * pixmap_.height())); // TODO: integer overflow?
    
    QImage image = pixmap_.toImage();

    QProgressDialog progress(tr("Couting colors..."), 0, 0, pixmap_.height(), this);
    progress.setModal(true);
   
    for (int row = 0; row < pixmap_.height(); ++row)
    {
      progress.setValue(row);
      qApp->processEvents();
      if (progress.wasCanceled())
      {
        // TODO
      }
      
      for (int column = 0; column < pixmap_.width(); ++column)
        ++colorsCounts_[image.pixel(column, row)];
    }
  }
}
