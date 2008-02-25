#include <QtGui>

#include "imageviewer.h"
#include "renderarea.h"

ImageViewer::ImageViewer()
{
  renderArea = new RenderArea;

  scrollArea = new QScrollArea;
  scrollArea->setBackgroundRole(QPalette::Dark);
  //scrollArea->setWidget(imageLabel);
  scrollArea->setWidget(renderArea);

  centralWidgetLayout = new QHBoxLayout;
  centralWidgetLayout->addWidget(scrollArea);
  centralWidgetLayout->addWidget(new QPushButton("One"));
  centralWidgetLayout->addWidget(new QPushButton("Two"));

  centralWidget = new QWidget;
  centralWidget->setLayout(centralWidgetLayout);
 
  setCentralWidget(centralWidget);

  createActions();
  createMenus();
  createToolBars();

  setWindowTitle(tr("Image Viewer"));
  resize(500, 400);
}

void ImageViewer::open()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
  if (!fileName.isEmpty())
  {
    QImage image(fileName);
    if (image.isNull())
    {
      QMessageBox::information(this, tr("Image Viewer"), tr("Cannot load %1.").arg(fileName));
      return;
    }
    renderArea->setImage(image);
    //imageLabel->setPixmap(QPixmap::fromImage(image));
    scaleFactor = 1.0;

    fitToWindowAct->setEnabled(true);
    updateActions();

//     if (!fitToWindowAct->isChecked())
//       imageLabel->adjustSize();
  }
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
  //imageLabel->adjustSize();
  scaleFactor = 1.0;
}

void ImageViewer::fitToWindow()
{
  bool fitToWindow = fitToWindowAct->isChecked();
  scrollArea->setWidgetResizable(fitToWindow);
  if (!fitToWindow)
  {
    normalSize();
  }
  updateActions();
}

void ImageViewer::about()
{
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

  zoomInAct = new QAction(tr("Zoom &In (50%)"), this);
  zoomInAct->setShortcut(tr("Ctrl++"));
  zoomInAct->setEnabled(false);
  connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

  zoomOutAct = new QAction(tr("Zoom &Out (50%)"), this);
  zoomOutAct->setShortcut(tr("Ctrl+-"));
  zoomOutAct->setEnabled(false);
  connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

  normalSizeAct = new QAction(tr("&Normal Size"), this);
  normalSizeAct->setShortcut(tr("Ctrl+S"));
  normalSizeAct->setEnabled(false);
  connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

  fitToWindowAct = new QAction(tr("&Fit to Window"), this);
  fitToWindowAct->setEnabled(false);
  fitToWindowAct->setCheckable(true);
  fitToWindowAct->setShortcut(tr("Ctrl+F"));
  connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

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
  viewMenu->addSeparator();
  viewMenu->addAction(fitToWindowAct);

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
  zoomInAct->setEnabled(!fitToWindowAct->isChecked());
  zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
  normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void ImageViewer::scaleImage( double factor )
{
  scaleFactor *= factor;
  //imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());
  renderArea->setScale(scaleFactor);

  adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
  adjustScrollBar(scrollArea->verticalScrollBar(), factor);

  zoomInAct->setEnabled(scaleFactor <= 8.0);
  zoomOutAct->setEnabled(scaleFactor > 0.125);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
  scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));
} 
