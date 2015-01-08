#ifndef COLORRAINBOW_H
#define COLORRAINBOW_H

#include "modeFactory.h"

class colorRainbow: public modeFactory
{
    public:
        colorRainbow();
        void update(char letter, unsigned int current, unsigned int length);
    protected:
    private:
};

#endif // COLORRAINBOW_H
