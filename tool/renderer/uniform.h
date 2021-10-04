#pragma once

#include "vector.h"
#include "locatable.h"
#include "gl.h"

namespace monad {

    template<class T>
    class Uniform : public LocatableUniform<T, GLint> {
    protected:
        void updateVal() override;

    public:
        Uniform(const T &def, GLuint program, const char *name)
                : LocatableUniform<T, GLint>(def, program) {
            updateLocation(name);
        }

        void updateLocation(const char *name) override {
            LocatableUniform<T, GLint>::resource = glGetUniformLocation(LocatableUniform<T, GLint>::program, name);
            updateVal();
        }
    };

}
