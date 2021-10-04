#pragma once
#include <GL/gl.h>

namespace monad {

    class Locatable {
    public:
        virtual void setProgram(GLuint val) = 0;
        virtual GLuint getProgram() = 0;

        virtual void updateLocation(const char *name) = 0;
    };

    template<class T, class TR>
    class LocatableUniform : public Locatable {
    protected:
        GLuint program;
        TR resource;
        T val;

        virtual void updateVal() = 0;

    public:
        LocatableUniform(const T &def, GLuint program) : val(def), program(program) { }

        void setProgram(GLuint val) override { program = val; updateVal(); }
        GLuint getProgram() override { return program; }

        TR getLocation() { return resource; }

        void setVal(const T &val) { this->val = val; updateVal(); }
        T getVal() { return val; }
    };

}
