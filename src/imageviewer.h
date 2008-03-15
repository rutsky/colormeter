#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>

class QWidget;
class QAction;
class QMenu;
class QToolBar;
class QHBoxLayout;
class QComboBox;
class QLabel;

class RenderArea;
 
// TODO: Normal signals and slots!
class ImageViewer : public QMainWindow
{
  Q_OBJECT

public:
  ImageViewer();

public slots:
  void normalSize();
  void zoomIn();
  void zoomOut();
  
private slots:
  void open();
  void about();
  void sceneScaleChangeTo( int index );
  void sceneScaleChanged( int index );
  void updateStatusBar();

private:
  void scaleImage( double factor, bool absolute = false );
  void createActions();
  void createMenus();
  void createToolBars();
  void createStatusBar();

private:
  int const positiveScales_, negativeScales_;
  QVector<double> scaleFactors_;
  int const normalScaleIndex_;
  
private:
  RenderArea *renderArea_;
  
  QHBoxLayout *centralWidgetLayout_;
  QWidget     *centralWidget_;
  QComboBox   *sceneScaleCombo_;
  QLabel      *statusBarText_;
  QString     fileName_;

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
