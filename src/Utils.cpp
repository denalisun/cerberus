#include "Utils.hpp"

void CLOG(std::string toLog) {
    std::cout << "[CRBRS] " << toLog << std::endl;
}

const wchar_t* chartowchar(const char *c) {
    const size_t cSize = strlen(c)+1;
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs (wc, c, cSize);

    return wc;
}