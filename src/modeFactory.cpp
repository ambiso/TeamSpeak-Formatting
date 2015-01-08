#include "../include/modeFactory.h"

modeFactory::modeFactory()
{
    //ctor
}

modeFactory::~modeFactory()
{
    //dtor
}

unsigned int modeFactory::getR()
{
    return m_RGB.r * 255;
}
unsigned int modeFactory::getG()
{
    return m_RGB.g * 255;
}
unsigned int modeFactory::getB()
{
    return m_RGB.b * 255;
}
