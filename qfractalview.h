#ifndef QFRACTALVIEW_H
#define QFRACTALVIEW_H

#include <QWidget>

class Mandelbrot;
class DoCalculation;

namespace Ui {
    class QFractalView;
}

class QFractalView : public QWidget
{
    Q_OBJECT

public:
    explicit QFractalView(QWidget *parent = 0);
    ~QFractalView();

public slots:
	void redraw();
	void zoomout();
	void resizeFractal();
private slots:
	void calc_finished();
	void zoomin(int x0, int y0);
	void on_save_clicked();

	void on_go_clicked();

private:
	struct zoom {
		double centerx;
		double centery;
		double scale;
	};

	Ui::QFractalView *ui;
	int w;
	int h;
	double centerx;
	double centery;
	double scale;
	Mandelbrot* f;
	DoCalculation* calc_thread;
	std::vector<zoom> history;
	unsigned char* pixels;

	void serializeState();
};

#endif // QFRACTALVIEW_H
