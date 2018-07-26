#include "image.h"
float getError(const Image &img1, const Image &img2){
    Color col1;
    Color col2;
    float sum = 0;

    int width = img1.getWidth();
    int height = img2.getHeight();

    for(unsigned int i = 0; i < img1.getHeight(); i++){
        for(unsigned int u = 0; u < img1.getWidth(); u++){
            col1 = img1.getPixel(u, i);
            col2 = img2.getPixel(u, i);

            float diff =(col1.r-col2.r)*(col1.r-col2.r)+
                  (col1.g-col2.g)*(col1.g-col2.g)+
                  (col1.b-col2.b)*(col1.b-col2.b);

            diff /= float(width * height);
            sum += diff;
        }
    }
    return sum;
}

double maxX(Polygon p){
    double max = std::numeric_limits<double>::min();

    for (Point point : p){
        if (point.x>max) max = point.x;
    }
    return max;
}

double maxY(Polygon p){
    double max = std::numeric_limits<double>::min();

    for (Point point : p){
        if (point.y>max) max = point.y;
    }
    return max;
}

double minX(Polygon p){
    double max = std::numeric_limits<double>::max();

    for (Point point : p){
        if (point.x<max) max = point.x;
    }
    return max;
}

double minY(Polygon p){
    double max = std::numeric_limits<double>::max();

    for (Point point : p){
        if (point.y<max) max = point.y;
    }
    return max;
}

unsigned int Image::getHeight() const
{
    return height;
}

unsigned int Image::getWidth() const
{
    return width;
}
