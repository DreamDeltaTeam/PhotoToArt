#include <iostream>
#include <vector>
#include <string>
#include <random>
#include "lodepng.h"
#include <time.h>
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

class Polygon{
public:
    std::vector<int> data;
    Polygon(vector<int> data){
        this->data = data;
    }
};

//Константы цветов
//Color constants
Color WHITE = Color(255,255,255);
Color BLACK = Color(0,0,0);
Color RED = Color(255,0,0);
Color GREEN = Color(0,255,0);
Color BLUE = Color(0,0,255);
Color Yellow = Color(255,255,0);
Color SKYBLUE = Color(0,255,255);
Color PURPLE = Color(255,0,255);



class Image{
public:

//Ширина, высота, число каналов цвета
//Width, height, number of channels in color

    int w;
    int h;
    int rgb = 3;

//Данные изображения
//Image data

    vector<byte> source;
    Image(int w,int h,int rgb){
        this->w=w;
        this->h=h;
        this->rgb = rgb;
        this->source.resize(w*h*rgb);
    }

//Установка цвета конкретного пикселя
//Setting pixel color

    void setPixel(int x, int y, Color &c) {
        int tmp = y*rgb*w+x*rgb;
        source[tmp]=c.r;
        source[tmp+1]=c.g;
        source[tmp+2]=c.b;
    }

//Взятие цвета конкретного пикселя
//Getting pixel color

    Color getColor(int x, int y){
        int tmp = y*rgb*w+x*rgb;
        int r = source[tmp];
        int g = source[tmp]+1;
        int b = source[tmp]+2;
        return Color(r,g,b);
    }
};

//Рисование квадрата в координатах x,y; размерами w,h и цветом с
//Drawing quard in coords x,y; width and height w, h and color c

Image genQuard(Image &image,int x,int y, int w,int h,Color &c){
    for (int i=0;i<h;i++){
        for (int j=0;j<w;j++){
           int x1 =j+x;
           int y1 = i+y;
           image.setPixel(x1,y1,c);
        }
    }
    return image;
}

//Рисование окружности с центром в координатах x,y , радиуса r и цвета c
//Drawing circle with center in coords x,y ; radius r and color c

Image genCircle(Image &image,int x,int y, int r,Color &c){
    if (x>0 && y>0 && x < image.w && y< image.h){
        for (int i=-r;i<r;i++){
            for (int j=-r;j<r;j++){
                int x1 =j;
                int y1 = i;
                if (x1*x1 + y1*y1 <= r*r){
                    x1+=x;
                    y1+=y;
                    image.setPixel(x1,y1,c);
                }
          }
        }
    }
    return image;
}



//Заполнение изображения цветом c
//Filling image with color с

Image fillImage(Image &image, Color &c){
    int rgb=3;
    for (int i=0;i<image.h;i++){
        for (int j=0;j<image.w;j++){
           int x1 =j;
           int y1 = i;
           image.setPixel(x1,y1,c);
        }
    }
    return image;
}

Image fillPolygon(Image &image, Color &c, Polygon &p){
    int minX = INT_MAX;
    for (int i=0;i<p.data.size(); i+=2){
        if (p.data[i]<minX) minX = p.data[i];
    }
    int minY = INT_MAX;
    for (int i=0;i<p.data.size(); i+=2){
        if (p.data[i]<minY) minY = p.data[i];
    }
    int maxX = INT_MIN;
    for (int i=0;i<p.data.size(); i+=2){
        if (p.data[i]>maxX) maxX = p.data[i];
    }
    int maxY = INT_MIN;
    for (int i=0;i<p.data.size(); i+=2){
        if (p.data[i]>maxY) maxY = p.data[i];
    }
    bool inside = false;
    for (int i=minY; i<=maxY; i++){
        for (int j=minX; j<maxX;j++){
            for (int l=0;l<p.data.size()-4;l++){
                int x1=p.data[l];
                int y1=p.data[l+1];
                int x2=p.data[l+2];
                int y2=p.data[l+3];


                int a=(y1-y2) / (x1-x2);
                int b=y2-x2*(y1-y2)/(x1-x2);

                if (i==j*a+b){
                    inside=!inside;
                    break;
                }
            }

            if (inside){
                image.setPixel(i,j,c);
            }
        }
    }

    return image;

}

//Функция генерации случайного числа
//Function of generation random int

int randInt(int start, int end){
    std::mt19937 gen;
    gen.seed(time(0));
    std::uniform_int_distribution<int> distrib(start, end);
    return distrib(gen);
}


//Тестирование графических примитивов
//Testing graphics

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





    img = genQuard(img,randInt(0,w),randInt(0,h),100,100,RED);
    vector<int> z = {1,1,100,1,200,200};
    Polygon p1 = Polygon(z);
    img = fillPolygon(img,RED,p1);

//  img = genCircle(img, distrib(gen),10,100,green);

    std::string filename = "image.png";
    lodepng::encode(filename, img.source, w, h, LCT_RGB, 8);

#ifdef __WIN32__
    std::system("image.png");
#else
    std::system("xviewer /home/timofey/git/build-hello-Desktop_Qt_5_11_0_GCC_64bit-Debug/image.png");
#endif
    return 0;
}
