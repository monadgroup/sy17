#include "texture.h"
#include "debug.h"

#ifdef DEBUG
#include "ui/toolwindow.h"
#endif

using namespace monad;

TextureImage::TextureImage() : texture(nullptr), level(0), isNone(true) {}

TextureImage::TextureImage(Texture *texture, GLint level) : texture(texture), level(level), isNone(false) {
}

GLuint Texture::nextTextureUnit = 1;

Texture::Texture(const char *debugName, GLenum internalFormat, GLenum format, GLenum type,
                 GLenum wrapS, GLenum wrapT,
                 GLenum magFilter, GLenum minFilter, GLenum compareMode, GLenum compareFunc)
        : internalFormat(internalFormat), format(format), type(type) {
    glGenTextures(1, &resource);
    unit = nextTextureUnit++;

    beginSet();
    doSet(GL_TEXTURE_WRAP_S, wrapS);
    doSet(GL_TEXTURE_WRAP_T, wrapT);
    doSet(GL_TEXTURE_MAG_FILTER, magFilter);
    doSet(GL_TEXTURE_MIN_FILTER, minFilter);
    doSet(GL_TEXTURE_COMPARE_MODE, compareMode);
    doSet(GL_TEXTURE_COMPARE_FUNC, compareFunc);
    finishSet();

#ifdef DEBUG
    glObjectLabel(GL_TEXTURE, resource, -1, debugName);
    monad::textures.emplace_back(std::string(debugName), this);
#endif
}

Texture::~Texture() {
    glDeleteTextures(1, &this->resource);
}

void Texture::set(GLenum prop, GLenum val) {
    beginSet();
    doSet(prop, val);
    finishSet();
}

void Texture::beginSet() {
    dbgEnter("Updating texture");
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, resource);
}

void Texture::doSet(GLenum prop, GLenum val) {
    glTexParameteri(GL_TEXTURE_2D, prop, val);
}

void Texture::finishSet() {
    glActiveTexture(GL_TEXTURE0);
    dbgExit();
}

TextureImage Texture::addImage(ivec2 size) {
    return setImage(levelCount++, size);
}

TextureImage Texture::setImage(GLint level, ivec2 size, void *data) {
    if (level == 0) this->size = size;

    beginSet();
    glTexImage2D(GL_TEXTURE_2D, level, internalFormat, size.x, size.y, 0, format, type, data);
    finishSet();
    return { this, level };
}

TextureImage Texture::getImage(GLint level) {
    return { this, level };
}

