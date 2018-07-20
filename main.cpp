#include <iostream>
#include <vector>
#include <string>
#include <random>
#include "lodepng.h"
#include <time.h>
#include "helpers.h"
typedef unsigned char byte;
using namespace std;

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

int randInt(float mu, float sigma2){
    std::mt19937 gen;
    gen.seed(time(0));
    std::normal_distribution<float> distrib(mu, sigma2);
    return int(distrib(gen));
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
    Mask quard = genQuad(100,100,10,w,h);
    img = put(img,quard,RED,1);


    std::string filename = "image.png";
    lodepng::encode(filename, img.source, w, h, LCT_RGB, 8);

#ifdef __WIN32__
    std::system("image.png");
#else
    std::system("xviewer /home/timofey/git/build-hello-Desktop_Qt_5_11_0_GCC_64bit-Debug/image.png");
#endif
    return 0;
}
