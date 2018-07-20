#ifndef HELPERS_H
#define HELPERS_H

#include <vector>

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

    void setPixel(int x, int y, Color &c);

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



// Эта структура задает ограничивающий прямоугольник примитива.
struct Hitbox
{
    int x1, x2;
    int y1, y2;
};

// Все просто, добавляется только хитбокс, и теперь только один канал
class Mask : public Image
{
    Hitbox hitbox;
public:
    Mask(int w, int h) : Image(w, h, 1)
    {
        hitbox.x1 = 0;
        hitbox.y1 = 0;

        hitbox.x2 = w - 1;
        hitbox.y2 = h - 1;
    }

    bool getMask(int x, int y)
    {
        return source[y * w + x];
    }

    void setMask(int x, int y)
    {
        source[y * w + x] = 1;
    }

    Hitbox& getHitbox() { return hitbox; }
};

/* Функция ошибок для алгоритма */
int getError(Image &img1, Image &img2);

/* Функция, закрашивающая по битовой маске область на картинке */
Image put(Image &prev, Mask &mask, Color c, float alpha);

/* Функция, которая генерирует битовую маску для квадрата */
Mask genQuad(int x, int y, int side, int w, int h);

/*======== Внутренниие вспомогательные функции =========*/
/* Это использовать не нужно! */
static int sqr(int a)
{
    return a * a;
}

#endif // HELPERS_H
