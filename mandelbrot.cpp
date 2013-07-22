#include "mandelbrot.h"

#include <iostream>
#include <climits>
#include <cmath>
#include <complex>

Mandelbrot::Mandelbrot()
	:eps(::ldexp(1, -40))
{
}

/** @todo Cycle Detection - due to round off, might actually get points repeating
    @todo Custom complex class
  */

int Mandelbrot::operator()(
	double x, double y
) const
{
	const long maxiter = 1l << 16;

#if 0
	// alternative method of calculation -- possibly for deep-zooming
	// Use polynomial reduction
	double r1 = 0;
	double r2 = 0;
	const double norm = x * x + y * y;

	for (long j = 0; j < maxiter; ++j)
	{
		// 8 muls, 5 adds
		double u = r1 * x;
		double t = u + r2;
		double v = r1 * y;

		if ( t * t + v * v >= 4)
			return j;
		double q1 = r1 * t;
		r1 = q1 + q1 + 1;
		r2 = r2 * r2 - r1 * r1 * norm;
	}
	return -1;
#endif

	const long cycle_begin = maxiter / 4;

	std::complex<double> c(x,y);
	std::complex<double> z = c; // hare

	for (long j = 0; j < cycle_begin; ++j)
	{
		if (std::abs(z.real()) + std::abs(z.imag()) >= 4)
			return j;

		// 3 muls, 4 adds (ideally)
		z = z * z + c;
	}

	// Look for a cycle
	std::complex<double> t = z;

	for (long j = cycle_begin; j < maxiter; ++j)
	{
		if (std::abs(z.real()) + std::abs(z.imag()) >= 4)
			return j;
		z = z * z + c;

		if (std::abs(t.real() - z.real()) + std::abs(t.imag() - z.imag()) < eps)
			return -1;

	}
	return -1;
}
