#include "format.h"

#include <cstring>

Format::Format(Color col, bool bold, bool italic, bool underline, bool strike, bool encol) :
    _col(col), _encol(encol)
{
    _attribs[0] = bold;
    _attribs[1] = italic;
    _attribs[2] = underline;
    _attribs[3] = strike;
    memset(_enables, 0, sizeof _enables);
}

QChar Format::getCode(int index) {
    return foptCodes[index];
}
