#pragma once
#include <string>
#include "renderer/gl.h"

namespace monad {

    class Timer {
    public:
        std::string name = "";
        GLuint startResource = 0;
        GLuint stopResource = 0;

        GLuint64 startTime;
        GLuint64 stopTime;
        GLuint64 duration;

        explicit Timer(const char *name);

        void start();
        void stop();
        void poll();
    };

}
