#pragma once
#include "renderer/scene.h"
#include "declarations.h"

namespace monad {

    class RoverScene : public Scene {
        SHARED_PTR(Program) _program = nullptr;
        SHARED_PTR(Program) _tessProgram = nullptr;
        SHARED_PTR(Program) _postProgram = nullptr;

        Uniform<float> *_tessLevelInner;
        Uniform<float> *_tessLevelOuter;

        SHARED_PTR(Camera) _camera{};

        Object *_boxObject{};
        Object *_quadObject{};

        int counter = 0;

    public:
        RoverScene() = default;

        void init(demo::DemoWindow *window) override;
        void enter() override;
        void frame() override;
        void leave() override;
    };

}
