#include "uniform.h"
#include "gl.h"
#include "list.h"

using namespace monad;

template<> void Uniform<int>::updateVal() { glProgramUniform1iv(program, resource, 1, &val); }
template<> void Uniform<List<int>>::updateVal() { glProgramUniform1iv(program, resource, val.size(), val.begin()); }
template<> void Uniform<float>::updateVal() { glProgramUniform1fv(program, resource, 1, &val); }
template<> void Uniform<List<float>>::updateVal() { glProgramUniform1fv(program, resource, val.size(), val.begin()); }
template<> void Uniform<double>::updateVal() { glProgramUniform1dv(program, resource, 1, &val); }
template<> void Uniform<List<double>>::updateVal() { glProgramUniform1dv(program, resource, val.size(), val.begin()); }
template<> void Uniform<ivec2>::updateVal() { glProgramUniform2iv(program, resource, 1, (int*)&val); }
template<> void Uniform<List<ivec2>>::updateVal() { glProgramUniform2iv(program, resource, val.size(), (int*)val.begin()); }
template<> void Uniform<vec2>::updateVal() { glProgramUniform2fv(program, resource, 1, (float*)&val); }
template<> void Uniform<List<vec2>>::updateVal() { glProgramUniform2fv(program, resource, val.size(), (float*)val.begin()); }
template<> void Uniform<dvec2>::updateVal() { glProgramUniform2dv(program, resource, 1, (double*)&val); }
template<> void Uniform<List<dvec2>>::updateVal() { glProgramUniform2dv(program, resource, val.size(), (double*)val.begin()); }
template<> void Uniform<ivec3>::updateVal() { glProgramUniform3iv(program, resource, 1, (int*)&val); }
template<> void Uniform<List<ivec3>>::updateVal() { glProgramUniform3iv(program, resource, val.size(), (int*)val.begin()); }
template<> void Uniform<vec3>::updateVal() { glProgramUniform3fv(program, resource, 1, (float*)&val); }
template<> void Uniform<List<vec3>>::updateVal() { glProgramUniform3fv(program, resource, val.size(), (float*)val.begin()); }
template<> void Uniform<dvec3>::updateVal() { glProgramUniform3dv(program, resource, 1, (double*)&val); }
template<> void Uniform<List<dvec3>>::updateVal() { glProgramUniform3dv(program, resource, val.size(), (double*)val.begin()); }
template<> void Uniform<ivec4>::updateVal() { glProgramUniform4iv(program, resource, 1, (int*)&val); }
template<> void Uniform<List<ivec4>>::updateVal() { glProgramUniform4iv(program, resource, val.size(), (int*)val.begin()); }
template<> void Uniform<vec4>::updateVal() { glProgramUniform4fv(program, resource, 1, (float*)&val); }
template<> void Uniform<List<vec4>>::updateVal() { glProgramUniform4fv(program, resource, val.size(), (float*)val.begin()); }
template<> void Uniform<dvec4>::updateVal() { glProgramUniform4dv(program, resource, 1, (double*)&val); }
template<> void Uniform<List<dvec4>>::updateVal() { glProgramUniform4dv(program, resource, val.size(), (double*)val.begin()); }
template<> void Uniform<Matrix4>::updateVal() { glProgramUniformMatrix4fv(program, resource, 1, GL_TRUE, val.m); }
