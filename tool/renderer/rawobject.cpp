#include "rawobject.h"
#include "renderer/program.h"
#include "debug.h"

using namespace monad;

RawObject::RawObject(GLuint buffer, GLuint vao, GLenum usageMode, GLenum renderMode)
    : buffer(buffer), vao(vao), usageMode(usageMode), renderMode(renderMode) {
    //glBindVertexArray(vao);
    //glBindBuffer(GL_ARRAY_BUFFER, buffer);
}

void RawObject::setData(int vertexCount, int dataSize, const float *data) {
    _vertexCount = vertexCount;
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, usageMode);
    //glBindVertexArray(vao);
    //glBufferData(GL_ARRAY_BUFFER, dataSize, data, usageMode);
    //glBindVertexArray(0);
}

float* RawObject::mapData(int vertexCount, int dataSize) {
    _vertexCount = vertexCount;
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, nullptr, usageMode);
    return (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void RawObject::unmapData() {
    glUnmapBuffer(GL_ARRAY_BUFFER);
    //glBindVertexArray(0);
}

void RawObject::attrib(GLint size, GLsizei stride, GLint pointer) {
    startAttrib();
    doAttrib(size, stride, pointer);
    endAttrib();
}

void RawObject::startAttrib() {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
}

void RawObject::doAttrib(GLint size, GLsizei stride, GLint pointer) {
    auto pointerId = _attribCount++;
    glVertexAttribPointer(pointerId, size, GL_FLOAT, FALSE, stride * sizeof(float), (void*)(pointer * sizeof(float)));
    glEnableVertexAttribArray(pointerId);
}

void RawObject::endAttrib() {
    glBindVertexArray(0);
}

void RawObject::render(const SHARED_PTR(Program) program, int instanceCount) {
    startRender(program);
    doRender(instanceCount);
    endRender();
}

void RawObject::startRender(const SHARED_PTR(Program) program) {
    dbgEnter("Render object");
    glBindVertexArray(vao);
    glUseProgram(program->resource);
}

void RawObject::doRender(int instanceCount) {
    glDrawArraysInstanced(renderMode, 0, _vertexCount, instanceCount);
}

void RawObject::endRender() {
    glBindVertexArray(0);
    dbgExit();
}
