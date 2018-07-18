#include <iostream>
#include <vector>
#include <string>
#include "lodepng.h"

typedef unsigned char byte;
using namespace std;
class Color{
public:
    int r;
    int g;
    int b;
    Color(int r,int g,int b){
        this->r=r;
        this->g=g;
        this->b=b;
    }
};

class Image{
public:
    int w;
    int h;
    int rgb = 3;
    vector<byte> source;
    Image(int w,int h,int rgb){
        this->w=w;
        this->h=h;
        this->rgb = rgb;
        this->source.resize(w*h*rgb);
    }

    void setPixel(int x, int y, Color &c) {
        int tmp = y*rgb*w+x*rgb;
        source[tmp]=c.r;
        source[tmp+1]=c.g;
        source[tmp+2]=c.b;
    }

    Color getColor(int x, int y){
        int tmp = y*rgb*w+x*rgb;
        int r = source[tmp];
        int g = source[tmp]+1;
        int b = source[tmp]+2;
        return Color(r,g,b);
    }
};

Image genQuard(Image image,int x,int y, int w,int h,Color c){
    for (int i=0;i<image.h;i++){
        for (int j=0;j<image.w;j++){
           int x1 =j;
           int y1 = i;
           if (x1 >=x && y1>=y && x1<=x+w && y1<=y+h){
               int tmp = y1*image.rgb*image.w+x1*image.rgb;
               image.source[tmp]=c.r;
               image.source[tmp+1]=c.g;
               image.source[tmp+2]=c.b;
           }
        }
    }
    return image;
}

Image genCircle(Image image,int x,int y, int r,Color c){
    if (x>0 && y>0 && x < image.w && y< image.h){
        for (int i=-r;i<r;i++){
            for (int j=-r;j<r;j++){
                int x1 =j;
                int y1 = i;
                if (x1*x1 + y1*y1 <= r*r){
                    x1+=x;
                    y1+=y;
                    int tmp = y1*image.rgb*image.w+x1*image.rgb;
                    image.source[tmp]=c.r;
                    image.source[tmp+1]=c.g;
                    image.source[tmp+2]=c.b;
                }
          }
        }
    }
    return image;
}

Image fillImage(Image image, Color c){
    int rgb=3;
    for (int i=0;i<image.h;i++){
        for (int j=0;j<image.w;j++){
           int x1 =j;
           int y1 = i;
           int tmp = y1*rgb*image.w+x1*image.rgb;
           image.source[tmp]=c.r;
           image.source[tmp+1]=c.g;
           image.source[tmp+2]=c.b;
        }
    }
    return image;
}

int main()
{
    int w = 640;
    int h = 360;
    int rgb=3;
    Image img = Image(w,h,rgb);
    Color red = Color(255,0,0);
    Color white = Color(255,255,255);
    Color green = Color(0,255,0);
    img = fillImage(img,white);
    img = genQuard(img,100,100,100,100,red);
    img = genCircle(img,200,200,20,green);

    std::string filename = "image.png";
    lodepng::encode(filename, img.source, w, h, LCT_RGB, 8);
    return 0;
}
