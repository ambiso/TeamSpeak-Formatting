#ifndef COLOREMERGENCY_H
#define COLOREMERGENCY_H

#include "modeFactory.h"

class colorEmergency: public modeFactory
{
    public:
        colorEmergency();
        void update(char letter, unsigned int current, unsigned int length);
    protected:
    private:
};

#endif // COLOREMERGENCY_H
