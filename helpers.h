#ifndef HELPERS_H
#define HELPERS_H

#include <vector>
#include "image.h"

typedef unsigned char byte;
using namespace std;

// Эта структура задает ограничивающий прямоугольник примитива.
struct Hitbox
{
    int x1, x2;
    int y1, y2;
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
