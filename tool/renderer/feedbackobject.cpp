#include "feedbackobject.h"

using namespace monad;

FeedbackObject::FeedbackObject(GLuint buffer, GLuint vao, GLuint feedback, int bufferSize, RawObject *object)
    : RawObject(buffer, vao, GL_STATIC_DRAW, GL_TRIANGLES), feedback(feedback), object(object) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(float), nullptr, GL_STATIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FeedbackObject::renderFeedback(const SHARED_PTR(Program) program, int instanceCount) {
    startRenderFeedback(program);
    doRenderFeedback(instanceCount);
    endRenderFeedback();
}

void FeedbackObject::startRenderFeedback(const SHARED_PTR(Program) program) {
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, buffer);
    glEnable(GL_RASTERIZER_DISCARD);
    object->startRender(program);
    glBeginTransformFeedback(GL_TRIANGLES); // todo: dont hardcode this
}

void FeedbackObject::doRenderFeedback(int instanceCount) {
    object->doRender(instanceCount);
}

void FeedbackObject::endRenderFeedback() {
    glEndTransformFeedback();
    object->endRender();
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
    glDisable(GL_RASTERIZER_DISCARD);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

void FeedbackObject::render(const SHARED_PTR(Program) program, int instanceCount) {
    startRender(program);
    doRender(instanceCount);
    endRender();
}

void FeedbackObject::startRender(const SHARED_PTR(Program) program) {
    RawObject::startRender(program);
}

void FeedbackObject::doRender(int instanceCount) {
    glDrawTransformFeedbackInstanced(renderMode, feedback, instanceCount);
}

void FeedbackObject::endRender() {
    RawObject::endRender();
}
