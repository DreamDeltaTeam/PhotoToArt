#include "helpers.h"

int getError(Image &img1, Image &img2)
{
    int result = 0;

    for(int i = 0; i < img1.h; i++){
        for(int j = 0; j < img1.w; j++){
            Color c1 = img1.getColor(i, j);
            Color c2 = img2.getColor(i, j);

            result += sqr(c1.r - c2.r) + sqr(c1.g - c2.g) + sqr(c1.b - c2.b);
        }
    }

    return result;
}



Image put(Image &prev, Mask &mask, Color c, float alpha)
{
    Image new_image = prev;
    Hitbox &h = mask.getHitbox();
    for(int i = h.x1; i < h.x2; i++) {
        for(int j = h.y1; j < h.y2; j++) {
            if(mask.getMask(x, y)) {
                Color prev_color = prev.getColor(i,j);
                Color new_color = {(prev_color.r * alpha + c.r * (1-alpha)),
                                   (prev_color.g * alpha + c.g * (1-alpha)),
                                   (prev_color.b * alpha + c.b * (1-alpha))};

                new_image.setPixel(i, j, new_color);

            }
        }
    }
    return new_image;
}

Mask genQuad(int x, int y, int side, int w, int h)
{
    Mask m(w, h);
    for(int i = x; i < x + side; i++) {
        for(int j = y; j < y + side; j++) {
            m.setMask(i,j);
        }
    }
    return m;
}
