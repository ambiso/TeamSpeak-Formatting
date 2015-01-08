#ifndef COLORFADEOUT_H
#define COLORFADEOUT_H

#include "modeFactory.h"

class colorFadeout: public modeFactory
{
    public:
        colorFadeout();
        void update(char letter, unsigned int current, unsigned int length);
    protected:
    private:
};

#endif // COLORFADEOUT_H
