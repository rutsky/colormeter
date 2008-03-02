#include <QtGui>

#include "imageviewer.h"
#include "renderarea.h"

ImageViewer::ImageViewer()
{
  renderArea_ = new RenderArea;
    
  centralWidgetLayout_ = new QHBoxLayout;
  centralWidgetLayout_->addWidget(renderArea_);
  centralWidgetLayout_->addWidget(new QPushButton("One"));
  centralWidgetLayout_->addWidget(new QPushButton("Two"));

  centralWidget_ = new QWidget;
  centralWidget_->setLayout(centralWidgetLayout_);
 
  setCentralWidget(centralWidget_);

  createActions();
  updateActions();
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
      scaleFactor_ = 1.0;
      updateActions();
      
      return true;
    }
  }
  
  return false;
}

void ImageViewer::zoomIn()
{
  scaleImage(2.0);
}

void ImageViewer::zoomOut()
{
  scaleImage(0.5);
}

void ImageViewer::normalSize()
{
  scaleImage(1.0, true);
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

  zoomInAct_ = new QAction(tr("Zoom &In"), this);
  zoomInAct_->setShortcut(tr("Ctrl++"));
  connect(zoomInAct_, SIGNAL(triggered()), this, SLOT(zoomIn()));

  zoomOutAct_ = new QAction(tr("Zoom &Out"), this);
  zoomOutAct_->setShortcut(tr("Ctrl+-"));
  connect(zoomOutAct_, SIGNAL(triggered()), this, SLOT(zoomOut()));

  normalSizeAct_ = new QAction(tr("&Normal Size"), this);
  normalSizeAct_->setShortcut(tr("Ctrl+S"));
  connect(normalSizeAct_, SIGNAL(triggered()), this, SLOT(normalSize()));

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
  viewMenu_->addAction(zoomInAct_);
  viewMenu_->addAction(zoomOutAct_);
  viewMenu_->addAction(normalSizeAct_);

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
}

void ImageViewer::updateActions()
{
  if (renderArea_->pixmap().isNull())
  {
    normalSizeAct_->setEnabled(false);
    zoomInAct_->setEnabled(false);
    zoomOutAct_->setEnabled(false);
  }
  else
  {
    normalSizeAct_->setEnabled(true);
    zoomInAct_->setEnabled(scaleFactor_ <= 16.0);
    zoomOutAct_->setEnabled(scaleFactor_ >= 0.125);
  }
}

void ImageViewer::scaleImage( double factor, bool absolute )
{
  if (absolute)
  {
    scaleFactor_ = factor;
    renderArea_->setScale(factor, absolute);
  }
  else
  {
    scaleFactor_ *= factor;
    renderArea_->setScale(factor, absolute);
  }
  
  updateActions();
}
