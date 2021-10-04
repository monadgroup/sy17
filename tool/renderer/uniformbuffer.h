#pragma once

#include "locatable.h"

namespace monad {

    class UniformBuffer : public LocatableUniform<GLuint, GLuint> {
    protected:
        void updateVal() override;

    public:
        UniformBuffer(const GLuint &def, GLuint program, const char *name);

        void updateLocation(const char *name) override;
    };

}
