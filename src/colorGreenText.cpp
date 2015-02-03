#include "../include/colorGreenText.h"

colorGreenText::colorGreenText()
{
    //ctor
}

void colorGreenText::update(char letter, unsigned int current, unsigned int length)
{
    m_RGB.r = 16.0d/255;
    m_RGB.g = 176.0d/255;
    m_RGB.b = 0.0d/255;
}
