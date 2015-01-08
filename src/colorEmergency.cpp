#include "../include/colorEmergency.h"

colorEmergency::colorEmergency()
{
    //ctor
}


void colorEmergency::update(char letter, unsigned int current, unsigned int length)
{
    if((current % 4) <= 1)
    { //D41508
        m_RGB.r = (double)0xD4/255;
        m_RGB.g = (double)0x15/255;
        m_RGB.b = (double)0x08/255;
    }
    else
    {
        m_RGB.r = 0x10/255;
        m_RGB.g = 0x10/255;
        m_RGB.b = 0x10/255;
    }
}
