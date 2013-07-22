/** @todo Bookmark
  * @todo Pan
  * @todo Palette swap
  * @todo Enable/disable cycle detection, simply connected-optimization
  * @todo Deepzoom using polynomial evaluation (for multiple precision)
  z^2 + c
  z^4 + 2z^2*c +c^2 + c => x^2 + 2xc + c^2 + c
  */

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include "qfractalview.h"

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	QMainWindow* mw = new QMainWindow;
	QAction* actdraw = new QAction("Draw Set", mw);
	QAction* actzoomout = new QAction("Zoom Out", mw);
	QAction* actresize = new QAction("Resize Window", mw);
	QMenuBar* menu = mw->menuBar();
	QMenu* toolsmenu = menu->addMenu("&Tools");
	actzoomout->setShortcut(Qt::CTRL + Qt::Key_Minus);
	toolsmenu->addAction(actdraw);
	toolsmenu->addAction(actzoomout);
	toolsmenu->addAction(actresize);

	QFractalView* frac = new QFractalView(mw);

	mw->setCentralWidget(frac);
	mw->resize(600, 600);

	mw->show();
	frac->connect(actdraw, SIGNAL(triggered()), SLOT(redraw()));
	frac->connect(actzoomout, SIGNAL(triggered()), SLOT(zoomout()));
	frac->connect(actresize, SIGNAL(triggered()), SLOT(resizeFractal()));

	int res = app.exec();
	return res;
}

