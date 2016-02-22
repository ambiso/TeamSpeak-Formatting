#include "formatting.h"

#include "format.h"

static QString opening(Fopt fopt, int compression)
{
    if(fopt >= flagStart) {
        return QString("[") + foptCodes[fopt-flagStart] + "]";
    }
    return Color(fopt).formatted(compression);
}

static QString closing(Fopt fopt)
{
    if(fopt >= flagStart) {
        return QString("[/") + foptCodes[fopt-flagStart] + "]";
    }
    return "[/COLOR]";
}

static bool match(Fopt a, Fopt b) {
    if(a >= flagStart && b >= flagStart) {
        return a == b;
    }
    //if they're not both boolean formatting options
    //they only match if they are both colors
    return (a < flagStart) && (b < flagStart);
}

#define OPENING (true)
#define CLOSING (false)

static Fopt idxToFopt(int idx) {
    return static_cast<Fopt>(idx + flagStart);
}

static int foptToIdx(Fopt fopt) {
    return fopt-flagStart;
}

static QString produceString(FChar const input[], int const length, std::pair<Fopt, bool> fstr[][NUM_PROPERTIES+1], short options[], int const compression)
{
    QString s;
    for(int i = 0; i < length; ++i) {
        for(int j = 0; j < options[i]; ++j) {
            if(fstr[i][j].second == OPENING) {
                s += opening(fstr[i][j].first, compression);
            } else {
                s += closing(fstr[i][j].first);
            }
        }
        s += input[i].getChar();
    }
    return s;
}

/*
 * Input attributes are enabled wherever they change
 */
QString formatToString(FChar const input[], int const length, int const stacksize, int const compression)
{
    std::pair<Fopt, bool> fstr[length][NUM_PROPERTIES+1]; //<format option, opening : closing>
    short options[length]; //number of total options to process for each character
    memset(options, 0, sizeof options);
    memset(fstr, 0, sizeof fstr);
    Fopt fstck[stacksize];
    int stackpos = 0;

    //primitive synthesis
    for(int i = 0; i < length; ++i) {
        FChar const &cur = input[i];
        Format const f = cur.getFormat();

        //figure out closing option that's lowest on stack
        int lowest = stackpos;
        Fopt lowest_fopt = flagStart;
        for(int j = 0; j < NUM_PROPERTIES; ++j) {
            if(f.getAttribEn(j) && !f.getAttrib(j)) {
                for(int k = lowest-1; k >= 0; --k) {
                    if(match(fstck[k], idxToFopt(j))) {
                        lowest = k;
                        lowest_fopt = idxToFopt(j);
                    }
                }
            }
        }
        if(f.getColorEn()) {
            for(int k = lowest-1; k >= 0; --k) {
                if(match(fstck[k], COLOR_OPT)) {
                    lowest = k;
                    lowest_fopt = COLOR_OPT;
                }
            }
        }
        if(lowest < stackpos) {
            //there's an option that needs closing
            fstr[i][options[i]++] = std::make_pair<Fopt, bool>(static_cast<Fopt>(lowest_fopt), CLOSING);
            int old_stackpos = stackpos;
            stackpos = lowest;
            for(int j = lowest+1; j < old_stackpos; ++j) {
                //reopen everything that's been closed but shouldn't be
                if((fstck[j] >= flagStart && f.getAttrib(foptToIdx(fstck[j]))) || (fstck[j] < flagStart && f.getColor().toFopt() == fstck[j])) {
                    fstr[i][options[i]++] = std::make_pair<Fopt, bool>(static_cast<Fopt>(fstck[j]), OPENING);
                    fstck[stackpos++] = fstck[j];
                }
            }
        }

        for(int j = 0; j < NUM_PROPERTIES; ++j) {
            if(f.getAttribEn(j) && f.getAttrib(j)) {
                fstr[i][options[i]++] = std::make_pair<Fopt,bool>(idxToFopt(j), OPENING);
                fstck[stackpos++] = idxToFopt(j); //push opening onto stack
            }
        }
        if(f.getColorEn() && f.getColor().diff(Color(), 0)) {
            fstr[i][options[i]++] = std::make_pair<Fopt,bool>(f.getColor().toFopt(), OPENING);
            fstck[stackpos++] = f.getColor().toFopt(); //push opening onto stack
        }
    }
    //TODO: filters for optimization
    return produceString(input, length, fstr, options, compression);
}
