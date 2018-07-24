#include "image.h"
unsigned int getError(const Image &img1, const Image &img2){
    Color col1;
    Color col2;
    unsigned int help=0;
    for(unsigned int i = 0; i<img1.height; i++){
        for(unsigned int u = 0;u<img1.width; u++){
            col1 = img1.getPixel(u, i);
            col2 = img2.getPixel(u, i);
            help+=(col1.r-col2.r)*(col1.r-col2.r)+
                  (col1.g-col2.g)*(col1.g-col2.g)+
                  (col1.b-col2.b)*(col1.b-col2.b);
        }
    }
    return help;
}
unsigned int Image::getHeight() const
{
    return height;
}

unsigned int Image::getWidth() const
{
    return width;
}
