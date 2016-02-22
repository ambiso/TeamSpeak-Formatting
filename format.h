#ifndef FORMAT
#define FORMAT

#include "color.h"
#include "util.h"

class Format {
    Color _col = Color();
    bool _encol = false;
    bool _enables[NUM_PROPERTIES];
    bool _attribs[NUM_PROPERTIES];
public:
    Format(Color col = Color(), bool bold = false, bool italic = false, bool underline = false, bool strike = false, bool encol = true);
    static QChar getCode(int index);
    Color getColor() const { return _col; }
    void setColor(Color c) { _col = c; }
    bool getColorEn() const { return _encol; }
    void setColoren(bool to) { _encol = to; }
    static int numProperties() { return NUM_PROPERTIES; }
    bool getAttrib(int index) const {
        return _attribs[index];
    }
    void setAttrib(int index, bool to) {
        _attribs[index] = to;
    }
    bool getAttribEn(int index) const {
        return _enables[index];
    }
    void setAttribEn(int index, bool to) {
        _enables[index] = to;
    }
};


#endif // FORMAT

