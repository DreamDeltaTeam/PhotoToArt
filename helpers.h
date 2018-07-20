#ifndef HELPERS_H
#define HELPERS_H

class Image;
class Color;

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

    Hitbox& getHitbox() { return h; }
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
