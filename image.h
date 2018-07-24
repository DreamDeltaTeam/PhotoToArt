#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include "lodepng.h"
#include <string>
#include <vector>
#include <fstream>

typedef unsigned char byte;
using namespace std;

class Color
{
public:
    byte r;
    byte g;
    byte b;
    Color(){
    }
    Color(byte r, byte g, byte b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
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

    Point(){
        x=0;
        y=0;
    }
    Point (int x,int y){
        this->x=x;
        this->y=y;
    }
};

typedef vector<Point> Polygon;

class Image
{
public:
    unsigned int width;
    unsigned int height;
    unsigned int channels;
    std::vector<byte> data;
	
    Image(int w, int h, int chnl)
    {
        width = w;
        height = h;
        channels = chnl;

        data.resize(width * height * channels);
    }
	
    Image (const Image &img){
        width=img.width;
        height=img.height;
        channels=img.channels;
        data=img.data;
    }

    Image(const string &name)
    {
        lodepng::decode(data,width,height,name,LCT_RGB,8);
        channels=3;
    }

    Color getPixel(int x, int y) const
    {
        Color c;
        c.r = data[y * width * channels + x * channels];
        c.g = data[y * width * channels + x * channels + 1];
        c.b = data[y * width * channels + x * channels + 2];


        return c;
    }

    void setPixel(int x, int y, Color c)
    {
        data[y * width * channels + x * channels]=c.r;
        data[y * width * channels + x * channels + 1]=c.g;
        data[y * width * channels + x * channels + 2]=c.b;
    }

    void Save(const string &name){
        lodepng::encode(name,data,width,height,LCT_RGB,8);
    }
    unsigned int getWidth() const;
    unsigned int getHeight() const;
};


unsigned int getError(const Image &img1, const Image &img2);


#endif // IMAGE_H
