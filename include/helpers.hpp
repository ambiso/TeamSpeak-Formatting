#ifndef HELPERS_HPP_INCLUDED
#define HELPERS_HPP_INCLUDED

#include <string>
#include <sstream>
#include <iomanip>
#include "modeFactory.h"

std::string toLower(const std::string &in);
std::string compress(const std::string &in);
std::string reFormat(const std::string &in, modeFactory* mode);

#endif // HELPERS_HPP_INCLUDED
