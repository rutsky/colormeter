#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>

class QAction;
class QLabel;
class QMenu;
class QToolBar;
class QScrollArea;
class QScrollBar;
class QWidget;
class QHBoxLayout;

class RenderArea;
 
class ImageViewer : public QMainWindow
{
  Q_OBJECT

public:
  ImageViewer();

private slots:
  void open();
  void zoomIn();
  void zoomOut();
  void normalSize();
  void fitToWindow();
  void about();

private:
  void createActions();
  void createMenus();
  void createToolBars();
  void updateActions();
  void scaleImage(double factor);
  void adjustScrollBar(QScrollBar *scrollBar, double factor);

  QScrollArea *scrollArea;
  RenderArea  *renderArea;
  double scaleFactor;
  
  QHBoxLayout *centralWidgetLayout;
  QWidget     *centralWidget;

  QAction *openAct;
  QAction *exitAct;
  QAction *zoomInAct;
  QAction *zoomOutAct;
  QAction *normalSizeAct;
  QAction *fitToWindowAct;
  QAction *aboutAct;
  QAction *aboutQtAct;

  QMenu *fileMenu;
  QMenu *viewMenu;
  QMenu *helpMenu;
  
  QToolBar *viewToolBar;
};

#endif 
