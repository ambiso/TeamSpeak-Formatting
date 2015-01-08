#ifndef COLORFADEINANDOUT_H
#define COLORFADEINANDOUT_H


#include "modeFactory.h"
#include <math.h>

class colorFadeInAndOut: public modeFactory
{
    public:
        colorFadeInAndOut();
        void update(char letter, unsigned int current, unsigned int length);
    protected:
    private:
};

#endif // COLORFADEINANDOUT_H
