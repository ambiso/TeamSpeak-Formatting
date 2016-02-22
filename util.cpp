#include "util.h"

#include <vector>

QString tohex(int x, int width)
{
    QString s = QString::number(x, 16);
    return QString("0").repeated(width-s.size()) + s;
}
