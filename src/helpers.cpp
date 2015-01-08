#include "../include/helpers.hpp"

std::string toLower(const std::string& in)
{
    std::string out;
    for(unsigned int i = 0; i < in.length(); i++)
    {
        if(in.at(i) >= 'A' && in.at(i) <= 'Z')
            out += in.at(i) - 'A' + 'a';
        else
            out += in.at(i);
    }
    return out;
}

std::string compress(const std::string& in)
{
    std::string out;
    const std::string startstr = "[COLOR=#", endstr = "[/COLOR]";
    const int COLOR_LEN = 6, LETTER_OFFSET = startstr.length() + COLOR_LEN + 1;

    std::string lastcolor, color, letters;
    lastcolor = in.substr(0 + startstr.length(), COLOR_LEN);
    for(unsigned int pos = 0; pos < in.length(); pos += startstr.length() + COLOR_LEN + 2 + endstr.length())
    {
        pos = in.find(startstr, pos);
        color = in.substr(pos + startstr.length(), COLOR_LEN);
        if(color == lastcolor)
        {
            letters += in.at(pos + LETTER_OFFSET);
        }
        else
        {
            out += startstr + lastcolor + "]" + letters + endstr;
            letters = in.at(pos + LETTER_OFFSET);
        }
        lastcolor = color;
    }
    out += startstr + lastcolor + "]" + letters + endstr;
    return out;
}

std::string reFormat(const std::string& in, modeFactory* color)
{
    if(color == NULL)
        exit(2);
    std::stringstream res;
    for(unsigned int i = 0; i < in.length(); i++)
    {
        color->update(in.at(i), i, in.length());
        res << "[COLOR=#" << std::hex
                          << std::setw(2) << std::setfill('0') << color->getR()
                          << std::setw(2) << std::setfill('0') << color->getG()
                          << std::setw(2) << std::setfill('0') << color->getB()
            << "]" << in.at(i) << "[/COLOR]";
    }
    return res.str();
}
