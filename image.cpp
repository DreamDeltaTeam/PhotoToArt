#include "image.h"

Image::Image() :
    Array3D<uchar>(0,0,3)
{ }

Image::Image(int width, int height, int num_channels) :
    Array3D<uchar> (width, height, num_channels)
{}

Image::Image(const std::string &filename)
{
    std::vector<uchar> out;
    unsigned w, h;
    lodepng::decode(out, w, h, filename, LCT_RGB);
    values = out;
    width = w;
    height = h;
    num_channels = 3;
}

void Image::Resize(int new_width, int new_height)
{
    width = new_width;
    height = new_height;
    values.resize(height * width * num_channels);
}


void Image::Save(const std::string &save_path)
{
    LodePNGColorType color_type = LCT_RGB;
    if(num_channels == 1) {
        color_type = LCT_GREY;
    }

    lodepng::encode(save_path, values, width, height, color_type);
}

void Image::setPixel(int x, int y, Color c)
{
    at(y,x,0) = c.r;
    at(y,x,1) = c.g;
    at(y,x,2) = c.b;
}

Color Image::getPixel(int x, int y) const
{
    Color c = {value_at(y,x,0), value_at(y,x,1), value_at(y,x,2)};
    return c;
}


Image rgb2gray(Image &rgb)
{
    if(rgb.getNumChannels() != 3) {
        // Todo: throw exception
    }
    int width = rgb.getWidth();
    int height = rgb.getHeight();

    Image gray(width, height, 1);
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            float res = 0.299 * rgb(i,j,0) + 0.587 * rgb(i,j,1) + 0.114 * rgb(i,j,2);
            res = clamp(res, 0.0f, 1.0f);
            uchar res_byte = res * 255;
            gray(i,j) = res_byte;
        }
    }
    return gray;
}



template<typename T>
Array3D<T>::Array3D()
{ }

template<typename T>
Array3D<T>::Array3D(int width, int height, int num_channels) :
    width(width), height(height), num_channels(num_channels)
{
    values.resize(width * height * num_channels);
}


template<typename T>
Array3D<T>::Array3D(const Array3D<T> &other) :
    width(other.width), height(other.height), num_channels(other.num_channels),
    values(other.values)
{ }

template<typename T>
T& Array3D<T>::operator()(int y, int x, int ch)
{
    return values[(y * width + x) * num_channels + ch];
}

template<typename T>
T &Array3D<T>::at(int y, int x, int ch)
{
    return values[(y * width + x) * num_channels + ch];
}

template<typename T>
T Array3D<T>::value_at(int y, int x, int ch) const
{
    return values[(y * width + x) * num_channels + ch];
}


Mask::Mask() : Array3D<int>()
{

}

Mask::Mask(int width, int height) :
    Array3D<int> (width, height, 1)
{ }


IntegralImage::IntegralImage(Image &other) :
    Array3D(other.getWidth(), other.getHeight(), other.getNumChannels())
{
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            for(int k = 0; k < num_channels; k++) {
                if(i == 0 && j == 0) {
                    at(i,j,k) = float(other(i,j,k)) / 255;
                    continue;
                }
                if(i == 0) {
                    at(i,j,k) = at(i,j-1,k) + float(other(i,j,k)) / 255;
                    continue;
                }
                if(j == 0) {
                    at(i,j,k) = at(i-1,j,k) + float(other(i,j,k)) / 255;
                    continue;
                }

                at(i,j,k) = at(i-1,j,k) + at(i,j-1,k) - at(i-1,j-1,k) + float(other(i,j,k));
            }
        }
    }
}

Color IntegralImage::avgColor(Point p1, Point p2)
{
    Point top_left = {std::min(p1.x, p2.x), std::min(p1.y, p2.y)};
    Point btm_right = {std::max(p1.x, p2.x), std::max(p1.y, p2.y)};

    float r = 0, g = 0, b = 0;

    Point A = top_left;
    Point B = {btm_right.x, top_left.y};
    Point C = {top_left.x, btm_right.y};
    Point D = btm_right;

    int width = btm_right.x - top_left.x;
    int height = btm_right.y - top_left.y;

    r = at(D.y, D.x, 0) - at(B.y, B.x, 0) - at(C.y, C.x, 0) + at(A.y, A.x, 0);
    g = at(D.y, D.x, 1) - at(B.y, B.x, 1) - at(C.y, C.x, 1) + at(A.y, A.x, 1);
    b = at(D.y, D.x, 2) - at(B.y, B.x, 2) - at(C.y, C.x, 2) + at(A.y, A.x, 2);

    Color c = {255 * r / (width * height), 255 * g / (width * height), 255 * b / (width * height)};
    return c;
}


template class Array3D<uchar>;
template class Array3D<int>;
template class Array3D<double>;
template class Array3D<float>;
