#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <iomanip>
#include <cmath>
#include <time.h>
#include <sstream>

#include "lodepng.h"

#include "helpers.h"
#include "image.h"
#include "mask.h"
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
Color YELLOW = Color(255,255,0);
Color SKYBLUE = Color(0,255,255);
Color PURPLE = Color(255,0,255);


//Функция генерации случайного числа
//Function of generation random int

template <typename T>
T clamp(T x, T left, T right)
{
    if(x < left) return left;
    if(x > right) return right;

    return x;
}

double d_clamp(double x, double left, double right)
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

int randDouble(float mu, float sigma2,double start, double end)
{
    std::normal_distribution<double> distrib(mu, sigma2);
    double rez = distrib(gen);
    return d_clamp(rez, start, end);
}


Point mutate_dot(Point p, int side,float mu,float sigma2,int w,int h)
{
    double x = randDouble(mu,sigma2,p.x-side,p.x+side);
    double y = randDouble(mu,sigma2,p.y-side, p.y+side);

    x = d_clamp(x,0,w-1);
    y = d_clamp(y,0,h-1);

    return {x,y};
}

Point mass_center(const Polygon &p){
    int x=0;
    int y=0;
    for (const Point &i : p){
        x+=i.x;
        y+=i.y;
    }
    return {x/p.size(), y/p.size()};
}

vector<Point> rotate_polygon(Point p, float mu, float sigma2, const Polygon &data,int w,int h){
    int alpha = randInt(mu,sigma2,0,360);
    Polygon tmp = data;
    for(int i = 0; i < data.size();i++){
        double x = (data[i].x - p.x)*cos(alpha)-(data[i].y - p.y)*sin(alpha);
        double y = (data[i].x - p.x)*sin(alpha)+(data[i].y - p.y)*cos(alpha);

        x += p.x;
        y += p.y;

        x = d_clamp(x,0,w-1);
        y = d_clamp(y,0,h-1);

        tmp[i].x = x;
        tmp[i].y = y;
    }
    return tmp;
}

Polygon move_polygon (Point point_from, Point point_to, const Polygon &data,int w,int h){
    Polygon tmp = data;
    double dx = point_from.x-point_to.x;
    double dy = point_from.y-point_to.y;
    for(int i=0; i < tmp.size();i++){
        tmp[i].x += dx;
        tmp[i].y += dy;
        tmp[i].x = d_clamp(data[i].x,0,w-1);
        tmp[i].y = d_clamp(data[i].y,0,h-1);
    }

    return tmp;
}

vector<Point> mutate_polygon(const Polygon &data,int w,int h){
    Polygon tmp = data;
    std::cout << tmp.size() << std::endl;
    int side = 200;
    int mu = w/10;
    int sigma2=w/4;
    Point mutated_dot = mutate_dot(Point(data[0].x,data[0].y),side,mu,sigma2, w,h);
    Point p = data[0];
    tmp = move_polygon(mutated_dot, p, tmp,w,h);
    tmp = rotate_polygon(mass_center(data),mu,sigma2,data,w,h);
    std::cout << tmp.size() << std::endl;
    return tmp;
}

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
    for (int i=0;i<image.getHeight();i++){
        for (int j=0;j<image.getWidth();j++){
           image.setPixel(j,i,c);
        }
    }
    return image;
}


//Тестирование графических примитивов
//Testing graphics


Image mutateImage(Image source, int randDots, int generations,Polygon primetive, float mu, float sigma2){
    Image img = Image(source.getWidth(),source.getHeight(),source.channels);
    Image img2 = Image(source.getWidth(),source.getHeight(),source.channels);
    img = fillImage(img,WHITE);
    img2 = fillImage(img2,WHITE);
    int w = img.getWidth();
    int h = img.getHeight();
    for (int i=0 ; i<randDots; i++){
        Polygon poly = primetive;
        poly =  move_polygon(Point(poly[0].x,poly[0].y),Point(randDouble(mu,sigma2,0,w),randDouble(mu,sigma2,0,h)),poly,w,h);
        Polygon poly2 = {};
        for (int j = 0; j<generations; j++){
            poly2 = mutate_polygon(poly,w,h);
            img = put(img,makePolygon(poly,w,h),source.getPixel(mass_center(poly)),0.0);
            img2 = put( img2,makePolygon(poly2,w,h),source.getPixel(mass_center(poly2)),0.0);
            if (getError(img,source)>getError(img2,source)){
                poly2 = poly;
                img2=img;
            } else {
                poly = poly2;
                img=img2;
            }
        }
    }
    img= img2;

    return img;
}


int main()
{
    gen.seed(time(0));
    int rgb=3;
    int images = 100;
    int delay = 25;
    Image load = Image("primitive.png");

    int w = load.width;
    int h = load.height;
    Image img = Image(w,h,rgb);
    img = fillImage(img,BLACK);
    string linux1 = "convert -delay "+ to_string(delay) +" -loop 0 ";


    Polygon polygon = {
        {randInt(w/2,w/10,0,w),randInt(h/2,h/10,0,h)},
        {randInt(w/2,w/10,0,w),randInt(h/2,h/10,0,h)},
        {randInt(w/2,w/10,0,w),randInt(h/2,h/10,0,h)},
        {randInt(w/2,w/10,0,w),randInt(h/2,h/10,0,h)},
        {randInt(w/2,w/10,0,w),randInt(h/2,h/10,0,h)},
        {randInt(w/2,w/10,0,w),randInt(h/2,h/10,0,h)}
    };

    int scale = 10;
    int dx = 30;
    int dy = 30;

    Polygon polygon2 = {
        {(2+dx)*scale,(6+dy)*scale},
        {(5+dx)*scale,(4+dy)*scale},
        {(5+dx)*scale,(1+dy)*scale},
        {(2+dx)*scale,(0+dy)*scale},
        {(0+dx)*scale,(4+dy)*scale}
    };



string fnames = " ";
    //for(int g = 0;g<images;g++){
        //img = fillImage(img,BLACK);
        int g=0;
        Mask cur_mask = makePolygon(polygon2,w,h);
        Color c = Color(randInt(w/2,w/10,0,255),randInt(w/2,w/10,0,255),randInt(w/2,w/10,0,255));
        //img = put(img,cur_mask,c,0);

        img = mutateImage(load,10,20,polygon2,w/2,w/10);
        //polygon2 = mutate_polygon(polygon2,w,h);



        std::stringstream ss;

        ss << "image";
        ss << std::setfill('0') << std::setw(3) << g;
        ss << ".png";
        std::string filename = ss.str();//"image" + to_string(g) + ".png";
        img.Save(filename);

        linux1 = linux1 + filename+ " ";
        cout << filename << endl;

        fnames+=filename+" ";


    //}

linux1 = linux1 + " gif5.gif";


std::string windows = "cmd /c convert -delay "+to_string(delay)+" -loop 0"+fnames+"anim.gif";
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
