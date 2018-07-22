#ifndef IMAGE_H
#define IMAGE_H

#include "lodepng.h"
#include <vector>

typedef unsigned char uchar;


template<typename T>
T clamp(T x, T min_val, T max_val)
{
    if(x < min_val) return min_val;
    if(x > max_val) return max_val;
    return x;
}

typedef unsigned char uchar;

struct Color
{
    uchar r,g,b;
    Color(uchar r,uchar g,uchar b){
        this->r=r;
        this->g=g;
        this->b=b;
    }
};

struct Point
{
    int x, y;
    void clamp(int w, int h)
    {
        if (x < 0) x = 0;
        if (x >= w) x = w-1;
        if (y < 0) y = 0;
        if (y >= h) y = h-1;
    }
    Point(int x,int y){
        this->x=x;
        this->y=y;
    }
};

template<typename T>
class Array3D
{
protected:
    int width, height;
    int num_channels;
    std::vector<T> values;
public:
    Array3D();
    Array3D(int width, int height, int num_channels);
    Array3D(const Array3D<T> &other);

    int getWidth() { return width; }
    int getHeight() { return height; }
    int getNumChannels() { return num_channels; }

    T& operator() (int y, int x, int ch = 0);
    T& at(int y, int x, int ch);
    T value_at(int y, int x, int ch) const;
};

class Image : public Array3D<uchar>
{
public:
    Image();
    Image(int width, int height, int num_channels);
    Image(const std::string &filename);
    Image(const Image &other) : Array3D<uchar>(other) {}

    void Resize(int new_width, int new_height);
    void Save(const std::string &save_path);
    void setPixel(int x, int y, Color c);
    Color getPixel(int x, int y) const;
};

class Mask : public Array3D<int>
{
public:
    Mask();
    Mask(int width, int height);
    Mask(const Mask &other) : Array3D<int>(other) {}

    int getPixel(int x, int y) {
        return at(y, x, 0);
    }
};

class IntegralImage : public Array3D<double>
{
public:
    IntegralImage(Image &other);
    Color avgColor(Point p1, Point p2);
};

Image rgb2gray(Image &rgb);

#endif // IMAGE_H
