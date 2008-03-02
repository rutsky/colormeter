#include <QtGui>

#include "imageviewer.h"
#include "renderarea.h"

ImageViewer::ImageViewer()
  : positiveScales_  ( 4 )
  , negativeScales_  ( 4 )
  , normalScaleIndex_( negativeScales_ )
{
  // Initialzing scales
  for (int i = negativeScales_ + 1; i >= 2; --i)
    scaleFactors_ << 1.0 / i;
  scaleFactors_ << 1.0;
  for (int i = 2; i <= positiveScales_ + 1; ++i)
    scaleFactors_ << 1.0 * i;
  
  // Initializing widgets
  renderArea_ = new RenderArea;
    
  centralWidgetLayout_ = new QHBoxLayout;
  centralWidgetLayout_->addWidget(renderArea_);
  centralWidgetLayout_->addWidget(new QPushButton("One"));
  centralWidgetLayout_->addWidget(new QPushButton("Two"));

  centralWidget_ = new QWidget;
  centralWidget_->setLayout(centralWidgetLayout_);
 
  setCentralWidget(centralWidget_);

  createActions();
  createMenus();
  createToolBars();

  setWindowTitle(tr("ColorMeter"));
  resize(500, 400);
}

bool ImageViewer::open()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath(),
      tr("Images (*.png *.xpm *.jpg *.jpeg *.bmp *.gif *.pbm *.pgm *.ppm *.tiff *.tif *.xbm *.xpm);;Any files (*)"));
  if (!fileName.isEmpty())
  {
    QPixmap pixmap(fileName);
    if (pixmap.isNull())
      QMessageBox::information(this, tr("Image Viewer"), tr("Cannot load %1.").arg(fileName));
    else
    {
      renderArea_->setPixmap(pixmap);
      sceneScaleChanged(normalScaleIndex_);
      return true;
    }
  }
  
  return false;
}

void ImageViewer::sceneScaleChanged( int index )
{
  Q_ASSERT(index >= 0 && index < scaleFactors_.size());
  renderArea_->setScale(scaleFactors_[index], true);
}

void ImageViewer::about()
{
  // TODO
  QMessageBox::about(this, tr("About Image Viewer"),
      tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
         "and QScrollArea to display an image. QLabel is typically used "
         "for displaying a text, but it can also display an image. "
         "QScrollArea provides a scrolling view around another widget. "
         "If the child widget exceeds the size of the frame, QScrollArea "
         "automatically provides scroll bars. </p><p>The example "
         "demonstrates how QLabel's ability to scale its contents "
         "(QLabel::scaledContents), and QScrollArea's ability to "
         "automatically resize its contents "
         "(QScrollArea::widgetResizable), can be used to implement "
         "zooming and scaling features. </p><p>In addition the example "
         "shows how to use QPainter to print an image.</p>"));
}

void ImageViewer::createActions()
{
  openAct_ = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
  openAct_->setShortcut(tr("Ctrl+O"));
  openAct_->setStatusTip(tr("Open an existing file"));
  connect(openAct_, SIGNAL(triggered()), this, SLOT(open()));

  exitAct_ = new QAction(tr("E&xit"), this);
  exitAct_->setShortcut(tr("Ctrl+Q"));
  connect(exitAct_, SIGNAL(triggered()), this, SLOT(close()));

  aboutAct_ = new QAction(tr("&About"), this);
  connect(aboutAct_, SIGNAL(triggered()), this, SLOT(about()));

  aboutQtAct_ = new QAction(tr("About &Qt"), this);
  connect(aboutQtAct_, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void ImageViewer::createMenus()
{
  fileMenu_ = new QMenu(tr("&File"), this);
  fileMenu_->addAction(openAct_);
  fileMenu_->addSeparator();
  fileMenu_->addAction(exitAct_);

  helpMenu_ = new QMenu(tr("&Help"), this);
  helpMenu_->addAction(aboutAct_);
  helpMenu_->addAction(aboutQtAct_);

  menuBar()->addMenu(fileMenu_);
  menuBar()->addMenu(helpMenu_);
}

void ImageViewer::createToolBars()
{
  viewToolBar_ = addToolBar(tr("File"));
  viewToolBar_->addAction(openAct_);
  
  sceneScaleCombo_ = new QComboBox;
  QStringList scales;
  for (int i = 0; i < scaleFactors_.size(); ++i)
    scales << tr("%1%").arg(100 * scaleFactors_[i], 0, 'f', 1);
  sceneScaleCombo_->addItems(scales);
  sceneScaleCombo_->setCurrentIndex(normalScaleIndex_);
  connect(sceneScaleCombo_, SIGNAL(currentIndexChanged( int )),
          this, SLOT(sceneScaleChanged( int )));
  
  viewToolBar_->addWidget(sceneScaleCombo_);
}
