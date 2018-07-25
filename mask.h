#ifndef MASKA_H
#define MASKA_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "image.h"

class Mask
{
public:
    int width;
    int height;
    vector <int> data;
    Mask (int width, int height)
    {
        this ->width = width;
        this -> height = height;
        data.resize(width * height);
    }
    void putOne(int x, int y)
    {
        data [width*y + x]=1;
    }
    void putZero(int x, int y)
    {
        data [width*y + x]=0;
    }

    int getPixel(int x, int y) const
    {
        return data[width*y + x];
    }
};


inline int intersection(Point p1, Point p2,int y)
{
   return p1.x + (p2.x - p1.x) * (y - p1.y) / (p2.y-p1.y);
}


Mask makePolygon(vector <Point> points,int w,int h);

Mask genQuad(int x,int y,int side,int w,int h);

#endif
