#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>

class QWidget;
class QAction;
class QMenu;
class QToolBar;
class QHBoxLayout;
class QComboBox;

class RenderArea;
 
class ImageViewer : public QMainWindow
{
  Q_OBJECT

public:
  ImageViewer();

private slots:
  bool open();
  void about();
  void sceneScaleChanged( int index );

private:
  void scaleImage( double factor, bool absolute = false );
  void createActions();
  void createMenus();
  void createToolBars();

private:
  int const positiveScales_, negativeScales_;
  QVector<double> scaleFactors_;
  int const normalScaleIndex_;
  
  RenderArea *renderArea_;
  
  QHBoxLayout *centralWidgetLayout_;
  QWidget     *centralWidget_;
  QComboBox   *sceneScaleCombo_;

  QAction *openAct_;
  QAction *exitAct_;
  QAction *aboutAct_;
  QAction *aboutQtAct_;

  QMenu *fileMenu_;
  QMenu *helpMenu_;
  
  QToolBar *viewToolBar_;
};

#endif 
