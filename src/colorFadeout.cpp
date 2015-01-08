#include "../include/colorFadeout.h"

colorFadeout::colorFadeout()
{
    //ctor
}

void colorFadeout::update(char letter, unsigned int current, unsigned int length)
{
    m_RGB.r = (double)current/length * 0.9;
    m_RGB.g = m_RGB.r;
    m_RGB.b = m_RGB.r;
}
