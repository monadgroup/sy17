#pragma once
#include "renderer/context.h"
#include "demo/demowindow.h"

namespace monad {

    void init(demo::DemoWindow *window);
    void frameStart();
    void frameEnd();

}
