#ifndef COLORGREENTEXT_H
#define COLORGREENTEXT_H

#include "modeFactory.h"

class colorGreenText: public modeFactory
{
    public:
        colorGreenText();
        void update(char letter, unsigned int current, unsigned int length);
    protected:
    private:
};

#endif // COLORGREENTEXT_H
