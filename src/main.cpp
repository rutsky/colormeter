/* ColorMeter
 * Counting some statistics about colors.
 * Based on imageviewer and basicdrawing examples from Qt tutorial.
 * Bob Rutsky <rutsky_v@rambler.ru>
 * 25.02.2008
 */

#include <QApplication>

#include "imageviewer.h"

int main( int argc, char *argv[] )
{
  QApplication app(argc, argv);

  ImageViewer imageViewer;
  imageViewer.show();

  return app.exec();
} 
