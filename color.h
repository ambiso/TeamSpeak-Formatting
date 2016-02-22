#ifndef COLOR_H
#define COLOR_H

#include "util.h"

#include <QString>

class Color {
    unsigned char rgb[3];
public:
    Color();
    Color(unsigned char r, unsigned char g, unsigned char b);
    Color(Fopt fopt);

    bool diff(Color other, int compression) const;
    QString formatted(int compression) const;
    Fopt toFopt() const;
};



#endif // COLOR_H
