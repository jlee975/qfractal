#ifndef QIMAGEVIEW_H
#define QIMAGEVIEW_H

#include <QWidget>
#include <QImage>

class QImageView : public QWidget
{
    Q_OBJECT
public:
    explicit QImageView(QWidget *parent = 0);
	~QImageView();

	void paintEvent(QPaintEvent *);
	void setData(const unsigned char* p, int w, int h);
	void mouseDoubleClickEvent(QMouseEvent * e);
	void save();

signals:

	void doubleClicked(int, int);
public slots:

private:
	QImage* image;
};

#endif // QIMAGEVIEW_H
