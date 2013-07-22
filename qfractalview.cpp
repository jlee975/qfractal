// not very sharp: 1666666643a779bf0100006dcfd3e9bf000000000000543e
// 1666664643a779bfcecc4c6fcfd3e9bf000000000000343e

#include "qfractalview.h"
#include "ui_qfractalview.h"

#include "docalculation.h"
#include "mandelbrot.h"
#include "resizedialog.h"

struct arr3 {
	double x;
	double y;
	double scale;
};

union Memento {
	arr3 orig;
	char bytes[sizeof(arr3)];
};

QFractalView::QFractalView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QFractalView), w(400), h(400), centerx(-.75), centery(0.),
	scale(1.25), f(new Mandelbrot), calc_thread(new DoCalculation),
	history(), pixels(new unsigned char[w * h * 4])
{
    ui->setupUi(this);
    redraw();
    connect(ui->fractalview, SIGNAL(doubleClicked(int,int)), SLOT(zoomin(int,int)));
    connect(calc_thread, SIGNAL(ready()), SLOT(update()));
    connect(calc_thread, SIGNAL(finished()), SLOT(calc_finished()));
}

QFractalView::~QFractalView()
{
	calc_thread->wait();
	delete calc_thread;
	ui->fractalview->setData(0, 0, 0);

	delete[] pixels;
	delete ui;
}

void QFractalView::redraw()
{
	serializeState();
	if (f && !calc_thread->isRunning())
	{
		memset(pixels, 255, w * h * 4);
		ui->fractalview->setData(pixels, w, h);
		calc_thread->setRange(pixels, w, h, f, centerx, centery, scale);
		calc_thread->start();
	}
}

void QFractalView::calc_finished()
{
	ui->fractalview->update();
}

void QFractalView::zoomout()
{
	if (!history.empty())
	{
		centerx = history.back().centerx;
		centery = history.back().centery;
		scale = history.back().scale;
		history.pop_back();
		redraw();
	}
}

void QFractalView::zoomin(int x, int y)
{
	if (!calc_thread->isRunning())
	{
		zoom z = { centerx, centery, scale };
		history.push_back(z);

		centerx = static_cast<double>(x) * (2 * scale) / w + (centerx - scale);
		centery = static_cast<double>(y) * (2 * scale) / h + (centery - scale);
		scale /= 4;

		redraw();
	}
}

void QFractalView::serializeState()
{
	Memento m;
	m.orig.x = centerx;
	m.orig.y = centery;
	m.orig.scale = scale;

	ui->label->setText(QByteArray(m.bytes, sizeof(m.orig)).toHex());
}

void QFractalView::resizeFractal()
{
	ResizeDialog dlg(w, h);
	if (dlg.exec() == QDialog::Accepted)
	{
		if (!calc_thread->isRunning())
		{
			QSize s = dlg.choice();
			w = s.width();
			h = s.height();
			unsigned char* p = pixels;
			pixels = new unsigned char[w * h * 4];
			delete[] p;
			redraw();
		}
	}
}

void QFractalView::on_save_clicked()
{
	ui->fractalview->save();
}

void QFractalView::on_go_clicked()
{
	Memento m;
	m.orig.x = 0;
	m.orig.y = 0;
	m.orig.scale = 0;

	QByteArray b = QByteArray::fromHex(ui->label->text().toAscii());
	if (b.length() == sizeof(m.bytes))
	{
		const char * q = b.constData();
		for (std::size_t i = 0; i < sizeof(m.bytes); ++i)
			m.bytes[i] = q[i];
		centerx = m.orig.x;
		centery = m.orig.y;
		scale = m.orig.scale;
		redraw();
	}
}
