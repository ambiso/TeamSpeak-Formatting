#include "../include/colorRainbow.h"

colorRainbow::colorRainbow()
{
    //ctor
}

void colorRainbow::update(char letter, unsigned int current, unsigned int length)
{
    hsv edit;
    double progress = (double)current/length;
    edit.h = progress * 360;
    edit.s = 1.0;
    edit.v = 1.0;
    m_RGB = hsv2rgb(edit);
}
