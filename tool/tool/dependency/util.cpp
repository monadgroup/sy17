#include "util.h"
#include <shlwapi.h>
#include <stdio.h>


std::string dirName(std::string path) {
    PathRemoveFileSpec(&path.front());
    path.resize(strlen(path.data()));
    path.shrink_to_fit();
    return path;
}

std::string concatPaths(const std::string &first, const std::string &second) {
    char finalBuff[MAX_PATH];
    first.copy(finalBuff, first.length(), 0);
    finalBuff[first.length()] = 0;
    PathAppend(finalBuff, second.c_str());
    return std::string(finalBuff);
}

