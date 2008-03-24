#include <QtGui>

#include <cmath>

#include "colorstat.h"

namespace
{
  template< class T > T sqr( T x ) { return x * x; }
  
  template< typename T, size_t N > size_t array_size( T (&)[N] ) { return N; }
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
  upperText_    = new QLabel;
  diagramLabel_ = new QLabel;
  
  editor_ = new QTextBrowser;
 
  QVBoxLayout *colorsStatisticsLayout = new QVBoxLayout;
  colorsStatisticsLayout->addWidget(upperText_);
  colorsStatisticsLayout->addWidget(editor_, 1000);
  colorsStatisticsLayout->addWidget(diagramLabel_);
  colorsStatisticsLayout->addStretch(1);
  
  setLayout(colorsStatisticsLayout);
  
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
  {
    upperText_->setText(tr("No image opened."));
    editor_->hide();
    diagramLabel_->hide();
  }
  else
  {
    upperText_->setText(tr("Processing..."));
    editor_->hide();
    diagramLabel_->hide();
    
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
        {
          QRgb color = image.pixel(column, row) & 0x00FFFFFFUL;
          ++layer.colorToCount[color];
        }
      }
    }
    
    if (layer.colorToCount.size() > 0)
    {
      // Removing absolutely white pixels
      {
        ColorsInfo::color_to_count_map_type::iterator it = layer.colorToCount.find(qRgba(255, 255, 255, 0));
        if (it != layer.colorToCount.end())
          layer.colorToCount.erase(it);
      }
      
      // Removing absolutely black pixels
      {
        ColorsInfo::color_to_count_map_type::iterator it = layer.colorToCount.find(qRgba(0, 0, 0, 0));
        if (it != layer.colorToCount.end())
          layer.colorToCount.erase(it);
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
        
        if (layer.colorToCount.empty())
        {
          layer.colorToCount.insert(oldLayer.colorToCount.begin().key(), oldLayer.colorToCount.begin().value());
          layer.countToColor.insert(oldLayer.colorToCount.begin().value(), oldLayer.colorToCount.begin().key());
        }
        
        Q_ASSERT(layer.colorToCount.size() < oldLayer.colorToCount.size());
        layer.calcColorStats();
      }
    }
    
    // Updating report text
    
    upperText_->setText(tr("<p>%1 pixels, %2 colors.</p>").arg(pixmap_.width() * pixmap_.height()).arg(layers_.front().colorToCount.size()));
    editor_->show();
    insertTable();
    
    double const pixdx = 35, pixdy = 20, dx = 1, dy = 20, xmax = layers_.size() - 1, ymax = 260, riskLength = 10,
        labelWidth = 30, labelHeight = 20, pixymax = ymax / dy * pixdy, pixxmax = xmax / dx * pixdx; 
    
    QRect rect = QRect(
       -(labelWidth + riskLength), -pixymax,
       labelWidth + riskLength + pixxmax, labelHeight + riskLength + pixymax);

    QPixmap pixmap(rect.width() + 20, rect.height() + 20);
    pixmap.fill();
    QPainter painter;
        
    painter.begin(&pixmap);
    
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(Qt::black), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.translate(-rect.topLeft().x(), -rect.topLeft().y() + 20);
        
    // Rendering axes
    painter.drawLine(0, 0, 0, -ymax / dy * pixdy);
    for (int y = 0; y <= ymax; y += dy)
    {
      painter.drawLine(-riskLength / 2.0, y / dy * pixdy - pixymax, riskLength / 2.0, y / dy * pixdy - pixymax);
      painter.drawText(QRect(-(riskLength + labelWidth), y / dy * pixdy - pixymax - dy / 2.0, labelWidth, labelHeight),
        Qt::AlignRight,QString("%1").arg(ymax - y));
    }
      
    painter.drawLine(0, 0, pixdx * xmax, 0);
    for (int x = 0; x <= xmax; x += dx)
    {
      painter.drawLine(x / dx * pixdx, -riskLength / 2.0, x / dx * pixdx, riskLength / 2.0);
      painter.drawText(QRect(x / dx * pixdx - dx / 4.0, riskLength, labelWidth, labelHeight), QString("%1").arg(x));
    }
    
    // Calculating diagram points
    typedef QRgb ColorsInfo::*ColorPtrType;
    ColorPtrType members[] = { &ColorsInfo::minColor, &ColorsInfo::maxColor, &ColorsInfo::avgColor }; // OYAEBU
    QVector<QVector<QPoint> > pointsVec;
    for (int memberInd = 0; memberInd < (int)array_size(members); ++memberInd)
    {
      pointsVec.push_back(QVector<QPoint>());
      pointsVec.push_back(QVector<QPoint>());
      pointsVec.push_back(QVector<QPoint>());
      QVector<QPoint> &redPoints   = *(pointsVec.end() - 3);
      QVector<QPoint> &greenPoints = *(pointsVec.end() - 2);
      QVector<QPoint> &bluePoints  = *(pointsVec.end() - 1);
      
      for (int layerInd = 0; layerInd < layers_.size(); ++layerInd)
      {
        QRgb color = layers_[layerInd].*members[memberInd];
        
        redPoints.push_back  (QPoint(double(layerInd) / dx * pixdx, -double(qRed  (color)) / dy * pixdy));
        greenPoints.push_back(QPoint(double(layerInd) / dx * pixdx, -double(qGreen(color)) / dy * pixdy));
        bluePoints.push_back (QPoint(double(layerInd) / dx * pixdx, -double(qBlue (color)) / dy * pixdy));
      }
    }
    
    // Rendering diagram
    QVector<Qt::PenStyle> styles;
    styles << Qt::DashDotLine << Qt::DashLine << Qt::SolidLine;
    QVector<QColor> colors;
    colors << Qt::red << Qt::green << Qt::blue;
      
    for (int i = 0; i < pointsVec.size(); ++i)
    {
      painter.setPen(QPen(colors[i % 3], 2, styles[i / 3], Qt::RoundCap, Qt::RoundJoin));
      
      QVector<QPoint> const &points = pointsVec[i];
      
      if (points.size() <= 2)
        painter.drawPolyline(&(*points.begin()), points.size());
      else
      {
        QPainterPath path;
        path.moveTo(points[0]);
        double const dist = 1.0 / 3.0;
        double const dx   = dist * (points[1].x() - points[0].x());
        
        for (int j = 0; j < points.size() - 1; ++j)
        {
          QPoint 
            curr(points[j]),
            next(points[j + 1]),
            prev(j >= 1 ? points[j - 1] : QPoint(0, curr.y() - next.y() + curr.y())),
            nextNext(j < points.size() - 2 ? points[j + 2] : QPoint(0, next.y() + next.y() - curr.y()));
          
          path.cubicTo(QPoint(curr.x() + dx, curr.y() + (next.y()     - prev.y()) / 2.0 * dist),
                       QPoint(next.x() - dx, next.y() - (nextNext.y() - curr.y()) / 2.0 * dist),
                       points[j + 1]);
        }
        painter.drawPath(path);
      }
    }
    
    painter.end();
    
    diagramLabel_->setPixmap(pixmap);
    diagramLabel_->show();
  }
}

void ColorStatistics::insertTable()
{
  // TODO: divide, clean up
  
  editor_->clear();
  QTextCursor cursor = editor_->textCursor();
  
  QTextTableFormat tableFormat;
  tableFormat.setAlignment(Qt::AlignHCenter);
  tableFormat.setBackground(QColor("white"));
  tableFormat.setCellPadding(2);
  tableFormat.setCellSpacing(0);
  
  QVector<QString> headersLine1, headersLine2;
  headersLine1 << 
      tr("#") << tr("Pixel % ?") << tr("Colors in pallete") << 
      tr("Minimum") << QString() << QString() << QString() <<
      tr("Maximum") << QString() << QString() << QString() << 
      tr("Average") << QString() << QString() << QString() << 
      tr("Variance") << tr("Average color");
  headersLine2 << 
      QString() << QString() << QString() << 
      tr("Value") << tr("R") << tr("G") << tr("B") <<
      tr("Value") << tr("R") << tr("G") << tr("B") <<
      tr("Value") << tr("R") << tr("G") << tr("B") <<
      QString() << QString();
  Q_ASSERT(headersLine1.size() == headersLine2.size());
  
  QTextTable *table = cursor.insertTable(2 + layers_.size(), headersLine1.size(), tableFormat);
  
  QTextFrame *frame = cursor.currentFrame();
  QTextFrameFormat frameFormat = frame->frameFormat();
  frameFormat.setBorder(1);
  frame->setFrameFormat(frameFormat);

  QTextCharFormat format       = cursor.charFormat();
  
  QTextCharFormat headerFormat = cursor.charFormat();
  headerFormat.setFontWeight(QFont::Bold);

  // Filling table headers
  for (int i = 0; i < headersLine1.size(); ++i)
  {
    QString headerString;
    
    headerString = headersLine1[i];
    if (!headerString.isNull())
    {
      QTextTableCell cell = table->cellAt(0, i);
      QTextCursor cellCursor = cell.firstCursorPosition();
      cellCursor.insertText(QString("%1").arg(headerString), headerFormat);
      
      QTextCharFormat format = cell.format();
      format.setBackground(QColor("#e0e0e0"));
      format.setVerticalAlignment(QTextCharFormat::AlignMiddle);
      cell.setFormat(format);
    }
    
    headerString = headersLine2[i];
    if (!headerString.isNull())
    {
      QTextTableCell cell = table->cellAt(1, i);
      QTextCursor cellCursor = cell.firstCursorPosition();
      cellCursor.insertText(QString("%1").arg(headerString), headerFormat);
      
      QTextCharFormat format = cell.format();
      format.setBackground(QColor("#e0e0e0"));
      format.setVerticalAlignment(QTextCharFormat::AlignMiddle);
      cell.setFormat(format);
    }
  }
  
  // Merging cells
  for (int i = 0; i < headersLine2.size(); ++i)
    if (headersLine2[i].isNull())
      table->mergeCells(0, i, 2, 1);
  table->mergeCells(0,  3, 1, 4);
  table->mergeCells(0,  7, 1, 4);
  table->mergeCells(0, 11, 1, 4);
  
  // Filling table
  for (int layerInd = 0; layerInd < layers_.size(); ++ layerInd)
  {
    ColorsInfo const &layer = layers_[layerInd];
    
    QVector<QString> values;
    values << tr("%1").arg(layerInd) << tr("?") << tr("%1").arg(layer.colorToCount.size()) <<
        tr("%1").arg(layer.minColor & 0x00FFFFFFUL) << 
        tr("%1").arg(qRed(layer.minColor)) << tr("%1").arg(qGreen(layer.minColor)) << tr("%1").arg(qBlue(layer.minColor)) <<
        tr("%1").arg(layer.maxColor & 0x00FFFFFFUL) << 
        tr("%1").arg(qRed(layer.maxColor)) << tr("%1").arg(qGreen(layer.maxColor)) << tr("%1").arg(qBlue(layer.maxColor)) <<
        tr("%1").arg(layer.avgColor & 0x00FFFFFFUL) << 
        tr("%1").arg(qRed(layer.avgColor)) << tr("%1").arg(qGreen(layer.avgColor)) << tr("%1").arg(qBlue(layer.avgColor)) <<
        tr("%1").arg(qRed(layer.colorVariance) + qBlue(layer.colorVariance) + qGreen(layer.colorVariance)) << tr("     ");
    Q_ASSERT(values.size() == table->columns());
    
    for (int i = 0; i < values.size(); ++i)
    {
      QTextTableCell cell = table->cellAt(2 + layerInd, i);
      QTextCursor cellCursor = cell.firstCursorPosition();
      cellCursor.insertText(QString("%1").arg(values[i]), format);
    }
    
    // Color cell
    {
      QTextTableCell cell = table->cellAt(2 + layerInd, values.size() - 1);
      QTextCharFormat format = cell.format();
      format.setBackground(QColor(layer.avgColor));
      cell.setFormat(format);
    }
  }
}
