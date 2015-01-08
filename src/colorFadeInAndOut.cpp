#include "../include/colorFadeInAndOut.h"

colorFadeInAndOut::colorFadeInAndOut()
{
    //ctor
}

void colorFadeInAndOut::update(char letter, unsigned int current, unsigned int length)
{
    m_RGB.r = 1-sin((double)current/length*3.14159265359);
    m_RGB.g = m_RGB.r;
    m_RGB.b = m_RGB.r;
}
