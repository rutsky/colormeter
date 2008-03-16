#include <QtGui>

#include "imageviewer.h"
#include "renderarea.h"
#include "colorstat.h"

ImageViewer::ImageViewer()
  : positiveScales_  (4)
  , negativeScales_  (4)
  , normalScaleIndex_(negativeScales_)
{
  // Initialzing scales
  for (int i = negativeScales_ + 1; i >= 2; --i)
    scaleFactors_ << 1.0 / i;
  scaleFactors_ << 1.0;
  for (int i = 2; i <= positiveScales_ + 1; ++i)
    scaleFactors_ << 1.0 * i;
  
  this->installEventFilter(this);
  
  // Initializing widgets
  renderArea_ = new RenderArea;
  colorStatistics_ = new ColorStatistics;
    
  tabWidget_ = new QTabWidget;
  tabWidget_->addTab(colorStatistics_, tr("Colors statistics"));
  
  centralWidgetLayout_ = new QHBoxLayout;
  centralWidgetLayout_->addWidget(renderArea_);
  centralWidgetLayout_->addWidget(tabWidget_);

  centralWidget_ = new QWidget;
  centralWidget_->setLayout(centralWidgetLayout_);
 
  setCentralWidget(centralWidget_);

  createActions();
  createMenus();
  createToolBars();
  createStatusBar();
  
  connect(renderArea_, SIGNAL(zoomIn()),  this, SLOT(zoomIn ( void )));
  connect(renderArea_, SIGNAL(zoomOut()), this, SLOT(zoomOut( void )));
  connect(this, SIGNAL(pixmapChanged( QPixmap const &, QString const & )), renderArea_,      SLOT(setPixmap( QPixmap const & )));
  connect(this, SIGNAL(pixmapChanged( QPixmap const &, QString const & )), colorStatistics_, SLOT(setPixmap( QPixmap const & )));

  setWindowTitle(tr("ColorMeter"));
  resize(500, 400);
}

void ImageViewer::open()
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
      fileName_ = fileName;
      emit pixmapChanged(pixmap, fileName);
      
      return; // Success
    }
  }
  
  return; // Failure
}

void ImageViewer::updateStatusBar()
{
  ImageViewer::updatePixmapInfo(renderArea_->pixmap(), QString());
}

void ImageViewer::updatePixmapInfo( QPixmap const &pixmap, QString const &fileName )
{
  if (pixmap.isNull())
    statusBarText_->setText(tr("Ready"));
  else
    statusBarText_->setText(tr("<b>%2x%3x%4</b> %1").arg(fileName).arg(pixmap.width()).arg(pixmap.height()).arg(pixmap.depth()));
}

void ImageViewer::sceneScaleChangeTo( int index )
{
  if (index >= 0 && index < scaleFactors_.size())
    sceneScaleCombo_->setCurrentIndex(index);
}

void ImageViewer::sceneScaleChanged( int index )
{
  Q_ASSERT(index >= 0 && index < scaleFactors_.size());
  renderArea_->setScale(scaleFactors_[index], true);
}

void ImageViewer::normalSize()
{
  sceneScaleChangeTo(normalScaleIndex_);
}

void ImageViewer::zoomIn()
{
  sceneScaleChangeTo(sceneScaleCombo_->currentIndex() + 1);
}

void ImageViewer::zoomOut()
{
  sceneScaleChangeTo(sceneScaleCombo_->currentIndex() - 1);
}

void ImageViewer::about()
{
  // TODO
  QMessageBox::about(this, tr("ColorMeter"),
      tr("<p>The <b>ColorMeter</b> or something...</p>"));
}

void ImageViewer::createActions()
{
  openAct_ = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
  openAct_->setShortcut(QKeySequence::Open);
  openAct_->setStatusTip(tr("Open an existing file"));
  connect(openAct_, SIGNAL(triggered()), this, SLOT(open()));

  exitAct_ = new QAction(tr("E&xit"), this);
  exitAct_->setShortcut(tr("Ctrl+Q"));
  connect(exitAct_, SIGNAL(triggered()), this, SLOT(close()));
  
  normalSizeAct_ = new QAction(tr("&Normal size"), this);
  normalSizeAct_->setShortcut(tr("Ctrl+S"));
  connect(normalSizeAct_, SIGNAL(triggered()), this, SLOT(normalSize()));
  
  zoomInAct_ = new QAction(tr("Zoom &in"), this);
  zoomInAct_->setShortcut(QKeySequence::ZoomIn);
  connect(zoomInAct_, SIGNAL(triggered()), this, SLOT(zoomIn()));
  
  zoomOutAct_ = new QAction(tr("Zoom &out"), this);
  zoomOutAct_->setShortcut(QKeySequence::ZoomOut);
  connect(zoomOutAct_, SIGNAL(triggered()), this, SLOT(zoomOut()));

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

  viewMenu_ = new QMenu(tr("&View"), this);
  viewMenu_->addAction(normalSizeAct_); 
  viewMenu_->addAction(zoomInAct_);
  viewMenu_->addAction(zoomOutAct_);
  
  helpMenu_ = new QMenu(tr("&Help"), this);
  helpMenu_->addAction(aboutAct_);
  helpMenu_->addAction(aboutQtAct_);

  menuBar()->addMenu(fileMenu_);
  menuBar()->addMenu(viewMenu_);
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
  connect(sceneScaleCombo_, SIGNAL(currentIndexChanged( int )),
          this, SLOT(sceneScaleChanged( int )));
  normalSize();
  
  viewToolBar_->addWidget(sceneScaleCombo_);
}

void ImageViewer::createStatusBar()
{
  statusBarText_ = new QLabel();
  statusBarText_->setFrameStyle(QFrame::NoFrame);
  statusBar()->addWidget(statusBarText_, 1);
  
  connect(this, SIGNAL(pixmapChanged( QPixmap const &, QString const & )),
          this, SLOT(updatePixmapInfo( QPixmap const &, QString const & )));
  updateStatusBar();
}
