#ifndef COLORRED_H
#define COLORRED_H

#include "modeFactory.h"

class colorRed: public modeFactory
{
    public:
        colorRed();
        void update(char letter, unsigned int current, unsigned int length);
    protected:
    private:
};

#endif // COLORRED_H
