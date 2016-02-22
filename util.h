#ifndef UTIL_H
#define UTIL_H

#include <QChar>
#include <QString>

#define NUM_PROPERTIES (4)
static constexpr QChar foptCodes[4] = {'B', 'I', 'U', 'S'};
enum Fopt {
    flagStart = 256*256*256,
    bold = flagStart,
    italic,
    underline,
    strike,
    flagEnd = NUM_PROPERTIES,
};
#define COLOR_OPT (static_cast<Fopt>(0))

QString tohex(int x, int width);

#endif // UTIL_H

