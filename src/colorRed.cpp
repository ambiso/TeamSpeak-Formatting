#include "../include/colorRed.h"

colorRed::colorRed()
{
    //ctor
}

void colorRed::update(char letter, unsigned int current, unsigned int length)
{
    m_RGB.r = 1.0d;
    m_RGB.g = 0.0d;
    m_RGB.b = 0.0d;
}
