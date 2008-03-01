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
class QGraphicsView;
class QGraphicsScene;
class QGraphicsPixmapItem;

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
  void createActions();
  void createMenus();
  void createToolBars();
  void updateActions();
  void scaleImage( double factor, bool absolute = false );

  QGraphicsView  *view;
  QGraphicsScene *scene;
  QGraphicsPixmapItem *pixmapItem;
  QPixmap        pixmap;
  
  double scaleFactor;
  
  QHBoxLayout *centralWidgetLayout;
  QWidget     *centralWidget;

  QAction *openAct;
  QAction *exitAct;
  QAction *zoomInAct;
  QAction *zoomOutAct;
  QAction *normalSizeAct;
  QAction *aboutAct;
  QAction *aboutQtAct;

  QMenu *fileMenu;
  QMenu *viewMenu;
  QMenu *helpMenu;
  
  QToolBar *viewToolBar;
};

#endif 
