#include "qimageview.h"

#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>

QImageView::QImageView(QWidget *parent) :
	QWidget(parent), image(0)
{
}

QImageView::~QImageView()
{
	QImage* p = image;
	image = 0;
	delete p;
}

void QImageView::setData(const unsigned char *p, int w, int h)
{
	QImage* old = image;
	if (p)
	{
		image = new QImage(p, w, h, QImage::Format_RGB32);
		setMinimumSize(w, h);
		resize(w, h);
	}
	else
	{
		image = 0;
	}
	delete old;
}

void QImageView::paintEvent(QPaintEvent *)
{
	if (image)
	{
		QPainter painter(this);

		painter.drawImage(0, 0, *image);
	}
}

void QImageView::mouseDoubleClickEvent(QMouseEvent * e)
{
	emit doubleClicked(e->x(), e->y());
}

void QImageView::save()
{
	if (image)
	{
		QString filename = QFileDialog::getSaveFileName(this, "Choose a file name");
		if (!filename.isEmpty())
			image->save(filename, "PNG");
	}
}

