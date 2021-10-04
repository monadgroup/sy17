#pragma once

namespace monad {

    enum class RenderType {
        LIGHTING,
        COLOR
    };

    class IFramebuffer;

    class IRenderable {
    public:
        virtual void render(RenderType type, const IFramebuffer *target, ivec4 viewport) = 0;
    };

}
