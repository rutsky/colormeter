#include <QtGui>

#include "colorstat.h"

ColorStatistics::ColorStatistics( QWidget *parent )
  : QWidget(parent)
{
  upperText_ = new QLabel;
  
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(upperText_);
  mainLayout->addStretch(1);
  setLayout(mainLayout);
  
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
  }
}
