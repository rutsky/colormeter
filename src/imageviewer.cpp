#include <QtGui>

#include "imageviewer.h"

ImageViewer::ImageViewer()
{
  pixmapItem = new QGraphicsPixmapItem;
  scene = new QGraphicsScene;
  scene->addItem(pixmapItem);
  view  = new QGraphicsView(scene);
    
  centralWidgetLayout = new QHBoxLayout;
  centralWidgetLayout->addWidget(view);
  centralWidgetLayout->addWidget(new QPushButton("One"));
  centralWidgetLayout->addWidget(new QPushButton("Two"));

  centralWidget = new QWidget;
  centralWidget->setLayout(centralWidgetLayout);
 
  setCentralWidget(centralWidget);

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
    QPixmap newPixmap(fileName);
    if (newPixmap.isNull())
      QMessageBox::information(this, tr("Image Viewer"), tr("Cannot load %1.").arg(fileName));
    else
    {
      scaleFactor = 1.0;
      
      pixmap = newPixmap;
      pixmapItem->setPixmap(pixmap);
      
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
  openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
  openAct->setShortcut(tr("Ctrl+O"));
  openAct->setStatusTip(tr("Open an existing file"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

  exitAct = new QAction(tr("E&xit"), this);
  exitAct->setShortcut(tr("Ctrl+Q"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  zoomInAct = new QAction(tr("Zoom &In"), this);
  zoomInAct->setShortcut(tr("Ctrl++"));
  connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

  zoomOutAct = new QAction(tr("Zoom &Out"), this);
  zoomOutAct->setShortcut(tr("Ctrl+-"));
  connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

  normalSizeAct = new QAction(tr("&Normal Size"), this);
  normalSizeAct->setShortcut(tr("Ctrl+S"));
  connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

  aboutAct = new QAction(tr("&About"), this);
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  aboutQtAct = new QAction(tr("About &Qt"), this);
  connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void ImageViewer::createMenus()
{
  fileMenu = new QMenu(tr("&File"), this);
  fileMenu->addAction(openAct);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  viewMenu = new QMenu(tr("&View"), this);
  viewMenu->addAction(zoomInAct);
  viewMenu->addAction(zoomOutAct);
  viewMenu->addAction(normalSizeAct);

  helpMenu = new QMenu(tr("&Help"), this);
  helpMenu->addAction(aboutAct);
  helpMenu->addAction(aboutQtAct);

  menuBar()->addMenu(fileMenu);
  menuBar()->addMenu(viewMenu);
  menuBar()->addMenu(helpMenu);
}

void ImageViewer::createToolBars()
{
  viewToolBar = addToolBar(tr("File"));
  viewToolBar->addAction(openAct);
}

void ImageViewer::updateActions()
{
  if (pixmapItem->pixmap().isNull())
  {
    normalSizeAct->setEnabled(false);
    zoomInAct->setEnabled(false);
    zoomOutAct->setEnabled(false);
  }
  else
  {
    normalSizeAct->setEnabled(true);
    zoomInAct->setEnabled(scaleFactor <= 16.0);
    zoomOutAct->setEnabled(scaleFactor >= 0.125);
  }
}

void ImageViewer::scaleImage( double factor, bool absolute )
{
  if (absolute)
  {
    scaleFactor = factor;
    view->setTransform(QTransform().scale(scaleFactor, scaleFactor));
  }
  else
  {
    scaleFactor *= factor;
    view->scale(factor, factor);
  }
  
  updateActions();
}
