#ifndef MODEFACTORY_H
#define MODEFACTORY_H

#include "hsv.cpp"

class modeFactory
{
    public:
        modeFactory();
        virtual ~modeFactory();
        unsigned int getR();
        unsigned int getG();
        unsigned int getB();
        virtual void update(char letter, unsigned int current, unsigned int length) = 0;
    protected:
        rgb m_RGB;
    private:
};

#include "colorRainbow.h"
#include "colorFadeout.h"
#include "colorFadeInAndOut.h"
#include "colorEmergency.h"
#include "colorRed.h"

#endif // MODEFACTORY_H
