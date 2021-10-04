#pragma once
#include "vector.h"

namespace demo {
    class DemoWindow;
}

namespace monad {

    class Context;

    class Scene {
    public:
        Context *context;

        virtual void init(demo::DemoWindow *window) = 0;
        virtual void enter() = 0;
        virtual void frame() = 0;
        virtual void leave() = 0;
    };

}
