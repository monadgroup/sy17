#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace monad {

    class Runtime {
    public:
        HINSTANCE hInstance;

        explicit Runtime(HINSTANCE hInstance);

        int run();
    };

}

