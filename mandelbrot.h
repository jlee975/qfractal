#ifndef MANDELBROT_H
#define MANDELBROT_H

class Mandelbrot
{
public:
    Mandelbrot();
    int operator()(double, double) const;
private:
    const double eps;
};

#endif // MANDELBROT_H
