#include "framebuffer.h"
#include "texture.h"
#include "debug.h"

using namespace monad;

IFramebuffer::IFramebuffer(GLuint resource) :  resource(resource) {}

void IFramebuffer::bind(ivec4 viewport, bool clear) const {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, resource);
    glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
    if (clear) {
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClearDepth(clearDepth);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void IFramebuffer::bind(int viewportCount, const ivec4 *viewports, bool clear) const {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, resource);
}

ScreenFramebuffer::ScreenFramebuffer() : IFramebuffer(0) {}

Framebuffer::Framebuffer(GLuint resource, int colorCount, const TextureImage *colors, const TextureImage *depth,
                         const TextureImage *stencil) : IFramebuffer(resource) {
    dbgEnter("Build framebuffer");
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, resource);
    if (colorCount > 0) {
        auto buffers = new GLuint[colorCount];
        this->colors = new TextureImage[colorCount];

        auto colorIndex = 0;
        for (auto i = 0; i < colorCount; i++) {
            auto color = colors[i];
            if (color.isNone) buffers[i] = GL_NONE;
            else {
                auto attachment = (GLenum) (GL_COLOR_ATTACHMENT0 + colorIndex++);
                buffers[i] = attachment;
                glFramebufferTexture(GL_DRAW_FRAMEBUFFER, attachment, color.texture->resource, color.level);
                this->colors[i] = color;
            }
        }
        glDrawBuffers(colorCount, buffers);
    } else {
        glDrawBuffer(GL_NONE);
    }

    if (depth) {
        glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth->texture->resource, depth->level);
        this->depth = new TextureImage(depth->texture, depth->level);
    }
    if (stencil) {
        glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, stencil->texture->resource, stencil->level);
        this->stencil = new TextureImage(stencil->texture, stencil->level);
    }

    auto framebufferStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
    switch (framebufferStatus) {
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            log("Framebuffer error: incomplete attachment");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            log("Framebuffer error: incomplete dimensions");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            log("Framebuffer error: missing attachment");
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            log("Framebuffer error: unsupported");
            break;
    }

    dbgExit();
}
