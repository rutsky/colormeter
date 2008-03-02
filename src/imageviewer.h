#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>

class QWidget;
class QAction;
class QMenu;
class QToolBar;
class QHBoxLayout;

class RenderArea;
 
class ImageViewer : public QMainWindow
{
  Q_OBJECT

public:
  ImageViewer();

private slots:
  bool open();
  void zoomIn();
  void zoomOut();
  void normalSize();
  void about();

private:
  void scaleImage( double factor, bool absolute = false );
  void createActions();
  void createMenus();
  void createToolBars();
  void updateActions();

private:
  double scaleFactor_;
  
  RenderArea *renderArea_;
  
  QHBoxLayout *centralWidgetLayout_;
  QWidget     *centralWidget_;

  QAction *openAct_;
  QAction *exitAct_;
  QAction *zoomInAct_;
  QAction *zoomOutAct_;
  QAction *normalSizeAct_;
  QAction *aboutAct_;
  QAction *aboutQtAct_;

  QMenu *fileMenu_;
  QMenu *viewMenu_;
  QMenu *helpMenu_;
  
  QToolBar *viewToolBar_;
};

#endif 
