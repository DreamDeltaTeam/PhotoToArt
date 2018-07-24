#include "helpers.h"


int getError(Image &img1, Image &img2)
{
    int result = 0;

    for(int i = 0; i < img1.getHeight(); i++){
        for(int j = 0; j < img1.getWidth(); j++){
            Color c1 = img1.getPixel(i, j);
            Color c2 = img2.getPixel(i, j);

            result += sqr(c1.r - c2.r) + sqr(c1.g - c2.g) + sqr(c1.b - c2.b);
        }
    }

    return result;
}



Image put(Image &prev, Mask &mask, Color c, float alpha)
{
    Image new_image = prev;
    for(int i = 0; i < prev.getHeight(); i++) {
        for(int j = 0; j < prev.getWidth(); j++) {
            if(mask.getPixel(j, i)) {
                Color prev_color = prev.getPixel(j,i);
                Color new_color = Color((prev_color.r * alpha + c.r * (1-alpha)),
                                   (prev_color.g * alpha + c.g * (1-alpha)),
                                   (prev_color.b * alpha + c.b * (1-alpha)));

                new_image.setPixel(j, i, new_color);
            }
        }
    }
    return new_image;
}




