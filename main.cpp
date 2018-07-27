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

#include "cxxopts.hpp"

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

double normal_double(float mu, float sigma2,double start, double end)
{
    std::normal_distribution<double> distrib(mu,sigma2);
    double rez = distrib(gen);
    return d_clamp(rez, start, end);
}

double uniform_double(double start, double end)
{
    std::uniform_real_distribution<double> dis(start,end);
    double rez = dis(gen);
    return d_clamp(rez, start, end);
}

class PolyParams
{
public:
    Polygon poly;
    int x, y;
    int w,h;

    float angle;
    float scaleX;
    float scaleY;

    float mu;
    float sigma2;

    Point massCenter(const Polygon &p){
        int x=0;
        int y=0;
        for (const Point &i : p){
            x+=i.x;
            y+=i.y;
        }
        return {x/p.size(), y/p.size()};
    }

    Point mutateDot(Point p,int w,int h,int side)
    {
        double x = uniform_double(p.x-side,p.x+side);
        double y = uniform_double(p.y-side, p.y+side);

        x = d_clamp(x,0,w-1);
        y = d_clamp(y,0,h-1);

        return {x,y};
    }

    Polygon computePoints()
    {
        Polygon new_poly = poly;
        new_poly = rotatePolygon(massCenter(new_poly),angle,new_poly);
        new_poly = scalePolygon(new_poly,scaleX,scaleY);
        new_poly = movePolygon(massCenter(new_poly),Point(x,y),new_poly);
        return new_poly;

    }

    Polygon movePolygon(Point point_from,Point point_to,Polygon data){
        Polygon tmp = data;
        double dx = -point_from.x + point_to.x;
        double dy = -point_from.y + point_to.y;
        for (int i=0; i < tmp.size();i++){
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


    Polygon rotatePolygon(Point p,float angle,Polygon data){
        float alpha = angle;
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

    Polygon scalePolygon(Polygon data,float scaleX,float scaleY){
        Polygon tmp = data;

        for (int i=0; i < tmp.size();i++){
            if (tmp[i].x*scaleX<0 || tmp[i].x*scaleX>w) return tmp;
            if (tmp[i].y+scaleY<0 || tmp[i].y*scaleY>h) return tmp;
        }
        for(int i=0; i < tmp.size();i++){
            tmp[i].x *= scaleX;
            tmp[i].y *= scaleY;
            tmp[i].x = d_clamp(tmp[i].x,0,w-1);
            tmp[i].y = d_clamp(tmp[i].y,0,h-1);
        }
        return tmp;
    }

    PolyParams mutatePolygon(){
        PolyParams tmp = *this;
        double coord_sigma = w/10;
        tmp.mu = w/2;
        tmp.sigma2=w/10;
        double mu_angle = 0;
        double sigma_angle = 30;

        tmp.angle = normal_double(mu_angle,sigma_angle, -180, 180);

        tmp.x += normal_double(0, coord_sigma, -w, w);
        tmp.y += normal_double(0, coord_sigma, -h, h);

        tmp.x = clamp(tmp.x, 0, w-1);
        tmp.y = clamp(tmp.y, 0, h-1);

        tmp.scaleX += normal_double(0, 0.05, -0.5, 0.5);
        tmp.scaleX = clamp((double)tmp.scaleX, 0.5, 2.0);

        tmp.scaleY += normal_double(0, 0.05, -0.5, 0.5);
        tmp.scaleY = clamp((double)tmp.scaleY, 0.5, 2.0);
        return tmp;

    }



    PolyParams(Polygon poly,int x,int y, int w,int h, float angle,float scaleX,float scaleY){
        this->poly=poly;
        this->x=x;
        this->y=y;
        this->w=w;
        this->h=h;
        this->angle=angle;
        this->scaleX=scaleX;
        this->scaleY = scaleY;
    }

    PolyParams(Polygon poly, int w, int h) :
        poly(poly), w(w), h(h)
    {
        x = w / 2;
        y = h / 2;

        angle = 0;
        scaleX = 1.0f;
        scaleY = 1.0f;
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


vector<Image> mutateImage(Image source, int randDots, int generations, int retries, PolyParams shape){
    vector <Image> images;
    ofstream file;
    file.open("err_fun.data");
    if (!file.is_open()){
        file.open("err_fun.data");
    }
    Image current = Image(source.getWidth(),source.getHeight(),source.channels);
    Image tmp = Image(source.getWidth(),source.getHeight(),source.channels);
    current = fillImage(current,BLACK);
    tmp = current;
    int w = current.getWidth();
    int h = current.getHeight();
    long cou = 0;


    for (int i=0; i < randDots; i++) {
        bool improved = false;
        bool changed = false;

        Point  p = Point(uniform_double(0, w - 1), uniform_double(0, h - 1));
        PolyParams primitive = PolyParams(shape.poly,w,h);
        primitive.x = p.x;
        primitive.y = p.y;
        primitive.computePoints();

        //cout << to_string(p.x)+" "+to_string(p.y) << endl;
        PolyParams poly = PolyParams(primitive.poly,w,h);
        PolyParams poly2 = PolyParams({},w,h);
        float min_error = getError(current,source);
        cout << to_string(i)+"/"+to_string(randDots) <<endl;

        for(int k = 0; k < retries; k++) {
            improved = false;
            for (int j = 0; j < generations; j++) {
                poly2 = poly.mutatePolygon();
                poly2 = PolyParams(poly2.computePoints(),poly2.x,poly2.y,w,h,poly2.angle,poly2.scaleX,poly2.scaleY);
                tmp = put(current,makePolygon(poly2.poly,w,h),source.getPixel(poly2.massCenter(poly2.poly)), 0.3);
                float cur_error = getError(tmp,source);
                if (min_error > cur_error){
                    poly = poly2;
                    min_error = cur_error;
                    improved = true;
                }
            }

            if(improved) {
                primitive = poly;
                changed = true;
            }
        }

        cout << "Error: " << min_error << endl;
        file << to_string(min_error) <<endl;
        if(changed) {
            cout << "Improved!" << endl;
            current = put(current,makePolygon(primitive.poly,w,h),source.getPixel(poly.massCenter(poly.poly)), 0.3);
        } else {
            cout << "No improvement" << endl;
        }

        if (changed){
            std::stringstream ss;
            ss << "image";
            ss << std::setfill('0') << std::setw(3) << cou;
            ss << ".png";
            std::string filename = ss.str();
            current.Save(filename);
            cou++;
        }
    }

    images.push_back(current);
    file.flush();
    file.close();
    return images;
}


int main(int argc, char **argv)
{
    cxxopts::Options options("PhotoToArt", "Use genetic algorithm to generate a painting!");

    int rand_dots = 2000;
    int generations = 5;
    int retries = 5;
    string input_image;
    float alpha = 0.3;
    int scale = 5;

    options.allow_unrecognised_options()
          .add_options()
          ("d,dots", "Number of primitives", cxxopts::value<int>(rand_dots))
          ("g,gen", "Number of mutations per point", cxxopts::value<int>(generations))
          ("r,retries", "Number of genetic algorithm steps", cxxopts::value<int>(retries))
          ("a,alpha", "Alpha", cxxopts::value<float>(alpha))
          ("i, input", "Input image", cxxopts::value<std::string>(input_image))
          ("s, scale", "Scale", cxxopts::value<int>(scale));


    options.parse(argc, argv);
    gen.seed(time(0));
    gen2 = mt19937(rd());
    gen2.seed(time(0));
    int rgb=3;
    int delay = 25;
    string fname ="athens.png";

    Image load = Image(input_image);

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

    PolyParams tmp = PolyParams(polygon2,w,h);

    images = mutateImage(load,rand_dots, generations, retries, tmp);

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
    std::system("ffmpeg -r 25 -i image%03d.png -c:v libx264 -vf fps=25 -pix_fmt yuv420p out.mp4");
#else
    cout << linux1.data() << endl;
    std::system("cd /home/timofey/git/build-hello-Desktop_Qt_5_11_0_GCC_64bit-Debug/");
    std::system(linux1.data());
    std::system("xviewer gif5.gif");
    std::system("ffmpeg -r 25 -i image%03d.png -c:v libx264 -vf fps=25 -pix_fmt yuv420p out.mp4");
#endif



    return 0;
}
