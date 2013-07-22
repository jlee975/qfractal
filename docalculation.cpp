#include "docalculation.h"

#include <iostream>

#include "mandelbrot.h"

namespace
{

int msb(int x)
{
	if (x == 0) return 0;

	int m = 1;
	while (x / m > 1)
		m += m;
	return m;
}


color_t color_index(int i)
{
#if 0
	color_t c = { (i % 512 >= 256 ? 255 - i % 256 : i % 256), 0, 0 };
	return c;
#else
	std::size_t ncolors = sizeof(chromamap) / sizeof(chromamap[0]);

	return chromamap[static_cast<unsigned>(i) % ncolors];
#endif
}

}

// im must be a pointer to an array of size >= 4wh bytes
DoCalculation::DoCalculation(QObject *parent)
	: QThread(parent), image(0), w(0), h(0), f(0), centerx(0), centery(0),
	  scale(), status(0), black(), red()
{
	color_t b = { 0, 0, 0 };
	black = b;
	color_t r = { 255, 0, 0 };
	red = r;
}

void DoCalculation::setRange(
	unsigned char* im,
	int w_,
	int h_,
	Mandelbrot* f_,
	double centerx_,
	double centery_,
	double scale_
)
{
	image = im;
	w = w_;
	h = h_;
	f = f_;
	centerx = centerx_;
	centery = centery_;
	scale = scale_;

	status_t* t = status;
	status = new status_t[w_ * h_];
	delete[] t;

	for (std::size_t i = 0; i < static_cast<unsigned>(w * h); ++i)
		status[i] = Untested;
}

DoCalculation::~DoCalculation() {
	delete[] status;
}

DoCalculation::status_t DoCalculation::updatePoint(int x, int y)
{
	status_t s = status[y * w + x];
	if (s == Untested)
	{
		// initial point
		double cx = ((x + x - w) * scale) / w + centerx;
		double cy = ((y + y - h) * scale) / h + centery;

		int e = (*f)(cx, cy);
		s = (e < 0 ? InSet : NotInSet);
		status[y * w + x] = s;
		color_t c = (e != -1 ? color_index(e) : black);
		unsigned char* p = image + (4 * (y * w + x));

		p[0] = c.blue;
		p[1] = c.green;
		p[2] = c.red;
		p[3] = 0;
	}
	return s;
}

// Check if the boundary of a square is in set => interior in set (for
// simple connected sets like Mandelbrot)
void DoCalculation::fillSquare(int x0, int y0, int x1, int y1)
{
	// do the square
	for (int i = x0 + 1; i < x1; ++i)
	{
		if (updatePoint(i, y0) != InSet) return;
	}

	for (int i = x0 + 1; i < x1; ++i)
	{
		if (updatePoint(i, y1) != InSet) return;
	}

	for (int i = y0 + 1; i < y1; ++i)
	{
		if (updatePoint(x0, i) != InSet) return;
	}

	for (int i = y0 + 1; i < y1; ++i)
	{
		if (updatePoint(x1, i) != InSet) return;
	}

	for (int i = y0 + 1; i < y1; ++i)
	{
		for (int j = x0 + 1; j < x1; ++j)
		{
			/** @todo Wrong once
			if (updatePoint(j,i) != InSet)
				std::cout << "ZOMG WRONG" << std::endl;
			*/
			status[i * w + j] = InSet;
			unsigned char* p = image + (4 * (i * w + j));
			p[0] = black.blue;
			p[1] = black.green;
			p[2] = black.red;
			p[3] = 0;
		}
	}
}

void DoCalculation::run()
{
	if (!f || !image) return;

	/// @todo Refactor into "progressive"

	for (int step = std::max(msb(w), msb(h)); step > 0; step /= 2)
	{
		for (int y = 0; y < h; y += step)
		{
			for (int x = 0; x < w; x += step)
			{
				if (updatePoint(x, y) == InSet)
				{
					// only take this step if f->simplyConnected
					if (step > 2 && x > 0 && y > 0 && status[(y-1) * w + (x - 1)] == Untested && status[(y-step)*w+x] == InSet && status[(y-step)*w+(x-step)] == InSet && status[y * w + (x - step)] == InSet)
					{
						fillSquare(x - step, y - step, x, y);
					}
				}
			}
		}
		emit ready();
	}
}

#if 0
int lsb(int x)
{
	if (x == 0) return 0;

	int m = 1;

	while ((x & m) == 0)
		m += m;
	return m;
}
#endif
