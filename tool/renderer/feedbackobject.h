#pragma once
#include "rawobject.h"

namespace monad {

    class FeedbackObject : public RawObject {
    public:
        GLuint feedback;
        RawObject *object;

        FeedbackObject(GLuint buffer, GLuint vao, GLuint feedback, int bufferSize, RawObject *object);

        void renderFeedback(const SHARED_PTR(Program) program, int instanceCount = 1);
        void startRenderFeedback(const SHARED_PTR(Program) program);
        void doRenderFeedback(int instanceCount = 1);
        void endRenderFeedback();

        void render(const SHARED_PTR(Program) program, int instanceCount = 1) override;
        void startRender(const SHARED_PTR(Program) program) override;
        void doRender(int instanceCount = 1) override;
        void endRender() override;

    };

}
