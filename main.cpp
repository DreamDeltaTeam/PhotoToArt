#include <iostream>
#include <vector>
#include <string>
#include <random>
#include "lodepng.h"
#include <time.h>
#include "helpers.h"
#include "image.h"
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





//Заполнение изображения цветом c
//Filling image with color с

Image fillImage(Image &image, Color &c){
    int rgb=3;
    for (int i=0;i<image.getHeight();i++){
        for (int j=0;j<image.getWidth();j++){
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
    img = fillImage(img,BLACK);
    bool newImage = true;
    int images = 100;
    int changex = 1;
    int changey = 1;
    int delay = 10;
    string linux1 = "convert -delay "+ to_string(delay) +" -loop 0 ";



    for(int g = 0;g<images;g){
        img = fillImage(img,BLACK);
    vector<Mask> masks;

    for (int i=0;i<50;i++){
        Mask cur_mask = genQuad(randInt(w/2+changex,w/10+changey,0,w),randInt(h/2,h/10,0,h),10,w,h);
        masks.push_back(cur_mask);
        Color c = Color(randInt(w/2,w/9,0,255),randInt(w/2,w/9,0,255),randInt(w/2,w/9,0,255));
        img = put(img,cur_mask,c,0);
    }



    std::string filename = "image" + to_string(g) + ".png";
    img.Save(filename);

linux1 = linux1 + filename+ " ";
    cout <<"image"+ to_string(g)+".png" << endl;
 g++;
    if(newImage == true){
    img = fillImage(img,white);
    }


}

linux1 = linux1 + " gif5.gif";
std::string windows = "cmd /c convert -delay "+to_string(delay)+" -loop 0 *.png anim.gif";
#ifdef __WIN32__
std::system(windows.data());
std::system("convert -quality 100 *.png outputfile.mpeg");
std::system("anim.gif");
#else
    cout << linux1.data() << endl;
    std::system("cd /home/timofey/git/build-hello-Desktop_Qt_5_11_0_GCC_64bit-Debug/");
    std::system(linux1.data());
    std::system("xviewer gif5.gif");

#endif


    return 0;
}
