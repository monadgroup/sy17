#include "debug.h"
#include "renderer/gl.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <stack>
#include <sstream>

static std::stack<std::string> groupNames = std::stack<std::string>();
static std::string lastLogStr;
static int lastLength;
static std::time_t lastTime;
static int logCount = 0;

static void startLog(const std::time_t &time, int &length) {
    auto tm = std::localtime(&time);
    auto indent = std::string(groupNames.size() * 2, ' ');
    auto stream = std::stringstream();

    stream << "[" << std::put_time(tm, "%I:%M:%S %p") << "] " << indent;
    auto str = stream.str();
    length = str.size();
    std::cout << str;
}

static void writeLog(const std::string &str) {
    auto currentTime = std::time(nullptr);

    int logStartLength = 0;
    /*if (lastLogStr == str && std::difftime(currentTime, lastTime) <= 1) {
        logCount++;
        lastTime = currentTime;

        std::cout << "\033[1A";
        startLog(currentTime, logStartLength);

        if (logCount > 500) {
            std::cout << "\033[1B\033[0G";
            return;
        }

        std::cout << "\033[" << (lastLength + 1) << "G" << " [";
        if (logCount == 500) {
            std::cout << ">500";
        } else {
            std::cout << "x" << logCount;
        }
        std::cout << "]" << std::endl;

        return;
    }*/

    startLog(currentTime, logStartLength);
    std::cout << str << std::endl;

    /*lastLogStr = str;
    lastTime = currentTime;
    logCount = 1;
    lastLength = logStartLength + str.size();*/
}

void log(const char *str, bool addGlEntry) {
    /*if (addGlEntry) {
        glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_OTHER, 0, GL_DEBUG_SEVERITY_NOTIFICATION, -1, str);
    } else {
        writeLog(std::string(str));
    }*/
    writeLog(std::string(str));
}

void dbgEnter(const char *name) {
    auto strName = std::string(name);
#ifdef LOG_OUT_GROUPS
    writeLog(">>> " + strName);
#endif
    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, name);
    groupNames.push(strName);
}

void dbgExit() {
    if (groupNames.empty()) return;
    auto name = std::move(groupNames.top());
    groupNames.pop();
#ifdef LOG_OUT_GROUPS
    writeLog("<<< " + std::string(name));
#endif
    glPopDebugGroup();
}
