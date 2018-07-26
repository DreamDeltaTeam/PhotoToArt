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
std::mt19937 gen2;
std::random_device rd;
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
    std::normal_distribution<double> distrib(mu,sigma2);
    double rez = distrib(gen);
    return d_clamp(rez, start, end);
}

int uid_randDouble(double start, double end)
{
    std::uniform_int_distribution<int> dis(start,end);
    double rez = dis(gen2);
    return d_clamp(rez, start, end);
}

Point mutate_dot(Point p, double side,float mu,float sigma2,int w,int h)
{
    double x = uid_randDouble(p.x-side,p.x+side);
    double y = uid_randDouble(p.y-side, p.y+side);

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

    for (int i=0;i<data.size();i++){
        double x = (data[i].x - p.x)*cos(alpha)-(data[i].y - p.y)*sin(alpha);
        double y = (data[i].x - p.x)*sin(alpha)+(data[i].y - p.y)*cos(alpha);
        x += p.x;
        y += p.y;
        if (x<0 || x>w) return tmp;
        if (y<0 || y>h) return tmp;
    }

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
    for (int i=0; i<tmp.size();i++){
        if (tmp[i].x+dx<0 || tmp[i].x+dx>w) return tmp;
        if (tmp[i].y+dy<0 || tmp[i].y+dy>h) return tmp;
    }
    for(int i=0; i < tmp.size();i++){
        tmp[i].x += dx;
        tmp[i].y += dy;
        tmp[i].x = d_clamp(tmp[i].x,0,w-1);
        tmp[i].y = d_clamp(tmp[i].y,0,h-1);
    }

    return tmp;
}

vector<Point>   mutate_polygon(const Polygon &data,int w,int h){
    Polygon tmp = data;
    double side = 10;
    double mu = w/2;
    double sigma2=w/10;
    Point mutated_dot = mutate_dot(Point(data[0].x,data[0].y),side,mu,sigma2, w,h);
    Point p = data[0];
    tmp = move_polygon(mutated_dot, p, data,w,h);
    tmp = rotate_polygon(mass_center(data),mu,sigma2,data,w,h);
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


vector<Image> mutateImage(Image source, int randDots, int generations, Polygon shape){
    vector <Image> images;
    Image current = Image(source.getWidth(),source.getHeight(),source.channels);
    Image tmp = Image(source.getWidth(),source.getHeight(),source.channels);
    current = fillImage(current,BLACK);
    tmp = current;
    int w = current.getWidth();
    int h = current.getHeight();
    long cou =0;

    for (int i=0; i < randDots; i++) {
        Point  p = Point(uid_randDouble(0, w -1), uid_randDouble(0, h - 1));

        Polygon primitive =  move_polygon(shape[0],p,shape,w,h);
        cout << to_string(p.x)+" "+to_string(p.y) << endl;
        Polygon poly = primitive;
        Polygon poly2;
        unsigned min_error = getError(current,source);

        bool improved = false;
        for (int j = 0; j < generations; j++) {
            cout << to_string(cou)+"/"+to_string(randDots*generations) <<endl;
            poly2 = mutate_polygon(poly,w,h);
            tmp = put(current,makePolygon(poly2,w,h),source.getPixel(mass_center(poly2)), 0.3);
            unsigned cur_error = getError(tmp,source);
            if (min_error > cur_error){
                poly = poly2;
                min_error = cur_error;
                improved = true;
            }

            cou++;
        }

        if(improved) {
            current = put(current,makePolygon(poly,w,h),source.getPixel(mass_center(poly)), 0.3);
        }

        images.push_back(current);

    }
    return images;
}


int main()
{
    gen.seed(time(0));
    gen2 = mt19937(rd());
    gen2.seed(time(0));
    int rgb=3;
    int delay = 25;
    Image load = Image("kekkk.png");

    int w = load.width;
    int h = load.height;
    vector<Image> images;
    string linux1 = "convert -delay "+ to_string(delay) +" -loop 0 ";


    Polygon polygon = {
        {randInt(w/2,w/10,0,w),randInt(h/2,h/10,0,h)},
        {randInt(w/2,w/10,0,w),randInt(h/2,h/10,0,h)},
        {randInt(w/2,w/10,0,w),randInt(h/2,h/10,0,h)},
        {randInt(w/2,w/10,0,w),randInt(h/2,h/10,0,h)},
        {randInt(w/2,w/10,0,w),randInt(h/2,h/10,0,h)},
        {randInt(w/2,w/10,0,w),randInt(h/2,h/10,0,h)}
    };

    int scale = 5;
    int dx = 20;
    int dy = 20;

    Polygon polygon2 = {
        {(2+dx)*scale,(6+dy)*scale},
        {(5+dx)*scale,(4+dy)*scale},
        {(5+dx)*scale,(1+dy)*scale},
        {(2+dx)*scale,(0+dy)*scale},
        {(0+dx)*scale,(4+dy)*scale}
    };
    string fnames = " ";
    int g=0;
    images = mutateImage(load,5000,10,polygon2);

    for (int i=0;i<images.size();i++){
        std::stringstream ss;
        ss << "image";
        ss << std::setfill('0') << std::setw(3) << i;
        ss << ".png";
        std::string filename = ss.str();
        images[i].Save(filename);
        linux1 = linux1 + filename+ " ";
        cout << filename << endl;
        fnames+=filename+" ";
    }

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
