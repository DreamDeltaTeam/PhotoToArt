#include <iostream>
#include <vector>
#include <string>
#include <random>
#include "lodepng.h"
#include <time.h>
#include "helpers.h"
typedef unsigned char byte;
using namespace std;

std::mt19937 gen;

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

//Функция генерации случайного числа
//Function of generation random int

int clamp(int x, int left, int right)
{
    if(x < left) return left;
    if(x > right) return right;

    return x;
}

int randInt(float mu, float sigma2,int start, int end)
{
    std::normal_distribution<float> distrib(mu, sigma2);
    int rez = int(distrib(gen));
    return clamp(rez, start, end);
}


//Тестирование графических примитивов
//Testing graphics

int main()
{
    gen.seed(time(0));
    int w = 640;
    int h = 360;
    int rgb=3;
    Image img = Image(w,h,rgb);
    Color red = Color(255,0,0);
    Color white = Color(255,255,255);
    Color green = Color(0,255,0);
    img = fillImage(img,white);

    vector<Mask> masks;

    for (int i=0;i<500;i++){
        Mask cur_mask = genQuad(randInt(w/2,w/10,0,w),randInt(h/2,h/10,0,h),10,w,h);
        masks.push_back(cur_mask);
        Color c(randInt(w/2,w/10,0,255),randInt(w/2,w/10,0,255),randInt(w/2,w/10,0,255));
        img = put(img,masks[i],c,1);
    }



    std::string filename = "image.png";
    lodepng::encode(filename, img.source, w, h, LCT_RGB, 8);

#ifdef __WIN32__
    std::system("image.png");
#else
    std::system("xviewer /home/timofey/git/build-hello-Desktop_Qt_5_11_0_GCC_64bit-Debug/image.png");
#endif
    return 0;
}
