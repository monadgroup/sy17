#include "shader.h"
#include "program.h"
#include "debug.h"
#include "dependency/shadermanager.h"
#include <regex>

using namespace std;
using namespace monad;

// todo: move this somewhere else?
static const TBuiltInResource defaultTBuiltInResource = {
    /* .MaxLights = */ 32,
    /* .MaxClipPlanes = */ 6,
    /* .MaxTextureUnits = */ 32,
    /* .MaxTextureCoords = */ 32,
    /* .MaxVertexAttribs = */ 64,
    /* .MaxVertexUniformComponents = */ 4096,
    /* .MaxVaryingFloats = */ 64,
    /* .MaxVertexTextureImageUnits = */ 32,
    /* .MaxCombinedTextureImageUnits = */ 80,
    /* .MaxTextureImageUnits = */ 32,
    /* .MaxFragmentUniformComponents = */ 4096,
    /* .MaxDrawBuffers = */ 32,
    /* .MaxVertexUniformVectors = */ 128,
    /* .MaxVaryingVectors = */ 8,
    /* .MaxFragmentUniformVectors = */ 16,
    /* .MaxVertexOutputVectors = */ 16,
    /* .MaxFragmentInputVectors = */ 15,
    /* .MinProgramTexelOffset = */ -8,
    /* .MaxProgramTexelOffset = */ 7,
    /* .MaxClipDistances = */ 8,
    /* .MaxComputeWorkGroupCountX = */ 65535,
    /* .MaxComputeWorkGroupCountY = */ 65535,
    /* .MaxComputeWorkGroupCountZ = */ 65535,
    /* .MaxComputeWorkGroupSizeX = */ 1024,
    /* .MaxComputeWorkGroupSizeY = */ 1024,
    /* .MaxComputeWorkGroupSizeZ = */ 64,
    /* .MaxComputeUniformComponents = */ 1024,
    /* .MaxComputeTextureImageUnits = */ 16,
    /* .MaxComputeImageUniforms = */ 8,
    /* .MaxComputeAtomicCounters = */ 8,
    /* .MaxComputeAtomicCounterBuffers = */ 1,
    /* .MaxVaryingComponents = */ 60,
    /* .MaxVertexOutputComponents = */ 64,
    /* .MaxGeometryInputComponents = */ 64,
    /* .MaxGeometryOutputComponents = */ 128,
    /* .MaxFragmentInputComponents = */ 128,
    /* .MaxImageUnits = */ 8,
    /* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
    /* .MaxCombinedShaderOutputResources = */ 8,
    /* .MaxImageSamples = */ 0,
    /* .MaxVertexImageUniforms = */ 0,
    /* .MaxTessControlImageUniforms = */ 0,
    /* .MaxTessEvaluationImageUniforms = */ 0,
    /* .MaxGeometryImageUniforms = */ 0,
    /* .MaxFragmentImageUniforms = */ 8,
    /* .MaxCombinedImageUniforms = */ 8,
    /* .MaxGeometryTextureImageUnits = */ 16,
    /* .MaxGeometryOutputVertices = */ 256,
    /* .MaxGeometryTotalOutputComponents = */ 1024,
    /* .MaxGeometryUniformComponents = */ 1024,
    /* .MaxGeometryVaryingComponents = */ 64,
    /* .MaxTessControlInputComponents = */ 128,
    /* .MaxTessControlOutputComponents = */ 128,
    /* .MaxTessControlTextureImageUnits = */ 16,
    /* .MaxTessControlUniformComponents = */ 1024,
    /* .MaxTessControlTotalOutputComponents = */ 4096,
    /* .MaxTessEvaluationInputComponents = */ 128,
    /* .MaxTessEvaluationOutputComponents = */ 128,
    /* .MaxTessEvaluationTextureImageUnits = */ 16,
    /* .MaxTessEvaluationUniformComponents = */ 1024,
    /* .MaxTessPatchComponents = */ 120,
    /* .MaxPatchVertices = */ 32,
    /* .MaxTessGenLevel = */ 64,
    /* .MaxViewports = */ 16,
    /* .MaxVertexAtomicCounters = */ 0,
    /* .MaxTessControlAtomicCounters = */ 0,
    /* .MaxTessEvaluationAtomicCounters = */ 0,
    /* .MaxGeometryAtomicCounters = */ 0,
    /* .MaxFragmentAtomicCounters = */ 8,
    /* .MaxCombinedAtomicCounters = */ 8,
    /* .MaxAtomicCounterBindings = */ 1,
    /* .MaxVertexAtomicCounterBuffers = */ 0,
    /* .MaxTessControlAtomicCounterBuffers = */ 0,
    /* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
    /* .MaxGeometryAtomicCounterBuffers = */ 0,
    /* .MaxFragmentAtomicCounterBuffers = */ 1,
    /* .MaxCombinedAtomicCounterBuffers = */ 1,
    /* .MaxAtomicCounterBufferSize = */ 16384,
    /* .MaxTransformFeedbackBuffers = */ 4,
    /* .MaxTransformFeedbackInterleavedComponents = */ 64,
    /* .MaxCullDistances = */ 8,
    /* .MaxCombinedClipAndCullDistances = */ 8,
    /* .MaxSamples = */ 4,
    /* .limits = */ {
       /* .nonInductiveForLoops = */ true,
       /* .whileLoops = */ true,
       /* .doWhileLoops = */ true,
       /* .generalUniformIndexing = */ true,
       /* .generalAttributeMatrixVectorIndexing = */ true,
       /* .generalVaryingIndexing = */ true,
       /* .generalSamplerIndexing = */ true,
       /* .generalVariableIndexing = */ true,
       /* .generalConstantMatrixVectorIndexing = */ true,
}};


static EShLanguage toLanguage(GLenum type) {
    switch (type) {
        case GL_VERTEX_SHADER: return EShLangVertex;
        case GL_TESS_CONTROL_SHADER: return EShLangTessControl;
        case GL_TESS_EVALUATION_SHADER: return EShLangTessEvaluation;
        case GL_GEOMETRY_SHADER: return EShLangGeometry;
        case GL_FRAGMENT_SHADER: return EShLangFragment;
        case GL_COMPUTE_SHADER: return EShLangCompute;
    }
}

static regex errorRegex(R"(^(\w+): (\d+):(\d+): (.*)$)", regex::ECMAScript);
static regex whitespaceRegex(R"(\s+)", regex::ECMAScript);

static void writeRemappedLine(const std::string &line, const vector<ShaderFragment> &fragments) {
    if (regex_match(line, whitespaceRegex)) return;

    smatch m;
    if (!regex_match(line, m, errorRegex)) {
        log(line.c_str());
        return;
    }

    auto errorType = m[1].str();
    auto fragmentNumber = std::stoi(m[2].str());
    auto lineNumber = std::stoi(m[3].str());
    auto description = m[4].str();

    if (fragmentNumber < 0 || fragmentNumber >= fragments.size()) {
        errorType.append(": ");
        errorType.append(std::to_string(fragmentNumber));
        errorType.append("?:");
        errorType.append(std::to_string(lineNumber));
        errorType.append("?: ");
        errorType.append(description);
        log(errorType.c_str());
        return;
    }

    auto fragment = fragments[fragmentNumber];
    auto realLine = fragment.line + lineNumber;

    errorType.append(": ");
    errorType.append(ShaderManager::nicePath(fragment.source));
    errorType.append(":");
    errorType.append(std::to_string(realLine));
    errorType.append(": ");
    errorType.append(description);
    log(errorType.c_str());
}

static void writeRemappedError(std::string error, const vector<ShaderFragment> &fragments) {
    size_t pos = 0;
    while ((pos = error.find('\n')) != std::string::npos) {
        auto line = error.substr(0, pos);
        writeRemappedLine(line, fragments);
        error.erase(0, pos + 1);
    }
    writeRemappedLine(error, fragments);
}

Shader::Shader(GLuint resource, GLenum type, const std::string &path)
        : ShaderDependency(path),
          resource(resource),
          handle(ShConstructCompiler(toLanguage(type), 0)) {
    isDependency = false;
}

Shader::~Shader() {
    ShDestruct(handle);
}

void Shader::build() {
    monad::ShaderDependency::build();

    prependInfo();

    // copy C++ string vector to C string array
    auto strArray = (const char **)(new LPSTR[fragments.size()]);
    for (auto i = 0; i < fragments.size(); i++) {
        strArray[i] = fragments[i].content.c_str();
    }

    // verify shader
    auto result = ShCompile(
        handle,
        strArray,
        fragments.size(),
        nullptr,
        EShOptNoGeneration,
        &defaultTBuiltInResource,
        0
    );
    auto infoLog = std::string(ShGetInfoLog(handle));
    if (infoLog.rfind("Warning,", 0) == 0) {
        infoLog.erase(0, infoLog.find("\n") + 1);
        infoLog.erase(0, infoLog.find_first_not_of(' '));
        infoLog.erase(infoLog.find_last_not_of(' ') + 1);
    }

    if (result == 0) {
        log("Shader compilation error:");
        writeRemappedError(infoLog, fragments);
        return;
    }

    if (infoLog.length() > 0 && infoLog.rfind("No errors.", 0) != 0) {
        writeRemappedError(infoLog, fragments);
    }

    glShaderSource(resource, fragments.size(), strArray, nullptr);
    glCompileShader(resource);
    delete[] strArray;

    auto compileOk = GL_FALSE;
    glGetShaderiv(resource, GL_COMPILE_STATUS, &compileOk);
    if (!compileOk) {
        auto maxLength = 0;
        glGetShaderiv(resource, GL_INFO_LOG_LENGTH, &maxLength);
        auto logval = new char[maxLength];
        glGetShaderInfoLog(resource, maxLength, &maxLength, logval);
        log(logval);
        delete[] logval;
    }

    // update programs
    for (auto program = programs.begin(); program != programs.end(); ) {
        if (program->expired()) programs.erase(program);
        else {
            program->lock()->link();
            program++;
        }
    }
}
