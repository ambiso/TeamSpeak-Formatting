#ifndef FCHAR_H
#define FCHAR_H

#include "format.h"

class FChar {
    QChar _c;
    Format _f;
public:
    FChar(QChar c = '\0', Format f = Format()) :
       _c(c),
       _f(f) {}
    QChar getChar() const {
        return _c;
    }
    Format& format() {
        return _f;
    }
    Format const &getFormat() const {
        return _f;
    }
};

#endif // FCHAR_H
