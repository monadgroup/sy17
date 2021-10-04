#include "renderer/context.h"
#include "shader.h"
#include "dependency/shadermanager.h"
#include "export/journals.h"
#include "renderer/common.h"
#include "renderer/camera.h"
#include "renderer/timer.h"
#include <shlwapi.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include "debug.h"
#include <GL/glu.h>
#include "ui/toolwindow.h"
#include "imgui/imgui.h"
#include "imgui/imgui_gl.h"

using namespace monad;

static int maxMsgLen;
static char* msgBuffer;
auto lastUpdateTime = timeGetTime();
static Timer *frameTimer;
static Timer *uiTimer;

static int selectedTexture = -1;
static bool showTexPreview = false;

#define COLOR(r, g, b, a) ImVec4((r)/255.0f, (g)/255.0f, (b)/255.0f, (a))

static int colorCount = 16;
static ImU32 colors[] = {
    ImGui::GetColorU32(COLOR(186, 104, 200, 1)),
    ImGui::GetColorU32(COLOR(220, 231, 117, 1)),
    ImGui::GetColorU32(COLOR(79, 195, 247, 1)),
    ImGui::GetColorU32(COLOR(255, 213, 79, 1)),
    ImGui::GetColorU32(COLOR(77, 182, 172, 1)),
    ImGui::GetColorU32(COLOR(174, 213, 129, 1)),
    ImGui::GetColorU32(COLOR(100, 181, 246, 1)),
    ImGui::GetColorU32(COLOR(255, 183, 77, 1)),
    ImGui::GetColorU32(COLOR(240, 98, 146, 1)),
    ImGui::GetColorU32(COLOR(255, 138, 101, 1)),
    ImGui::GetColorU32(COLOR(229, 115, 155, 1)),
    ImGui::GetColorU32(COLOR(255, 241, 118, 1)),
    ImGui::GetColorU32(COLOR(149, 117, 205, 1)),
    ImGui::GetColorU32(COLOR(121, 134, 203, 1)),
    ImGui::GetColorU32(COLOR(77, 208, 225, 1)),
    ImGui::GetColorU32(COLOR(129, 199, 132, 1))
};

//#define PROFILER_ENABLED

static void processDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, const std::string &msg) {
    switch (type) {
        case GL_DEBUG_TYPE_PUSH_GROUP:
        case GL_DEBUG_TYPE_POP_GROUP:
            return;
    }
    switch (id) {
        case 131185: // buffer location info
        case 1282:   // invalid program error
            return;
    }

    auto logStream = std::stringstream();
    switch (source) {
        case GL_DEBUG_SOURCE_API:             logStream << "    [GL] "; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   logStream << "    [WM] "; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: logStream << "[SHADER] "; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     logStream << "   [LIB] "; break;
        case GL_DEBUG_SOURCE_APPLICATION:     logStream << "   [LOG] "; break;
        default:                              logStream << " [OTHER] "; break;
    }
    logStream << std::setw(6) << std::to_string(id);
    logStream << " " << msg;
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:         logStream << "HIGH "; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       logStream << "MED "; break;
        case GL_DEBUG_SEVERITY_LOW:          logStream << "LOW "; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: logStream << "NOTIFY "; break;
    }
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:               logStream << "ERROR: "; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: logStream << "DEPRECATED: "; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  logStream << "UNDEFINED: "; break;
        case GL_DEBUG_TYPE_PORTABILITY:         logStream << "PORTABILITY: "; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         logStream << "PERFORMANCE: "; break;
        case GL_DEBUG_TYPE_MARKER:              logStream << "MARKER: "; break;
    }
    auto str = logStream.str();
    log(str.c_str(), false);
}

bool monad::operator<(const ShaderKey &a, const ShaderKey &b) {
    return a.name < b.name || a.type < b.type;
}

void monad::init(demo::DemoWindow *cb) {
    glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &maxMsgLen);
    msgBuffer = new char[maxMsgLen];
    frameTimer = new Timer("Frame");
    uiTimer = new Timer("UI");

    //monad::timers.push_back(frameTimer);
    monad::timers.push_back(uiTimer);
}

void monad::frameStart() {
    ImGuiGl::NewFrame();

    frameTimer->start();
}

static std::string getTexString(Texture *texture, std::string name) {
    name.append(": ");
    switch (texture->internalFormat) {
        case GL_DEPTH_COMPONENT: name.append("Depth"); break;
        case GL_DEPTH_STENCIL: name.append("Depth/stencil"); break;
        case GL_RED: name.append("R"); break;
        case GL_RG: name.append("RG"); break;
        case GL_RGB: name.append("RGB"); break;
        case GL_RGBA: name.append("RGBA"); break;
        case GL_R16F: name.append("R16F"); break;
        case GL_RG16F: name.append("RG16F"); break;
        case GL_RGB16F: name.append("RGB16F"); break;
        case GL_R32F: name.append("R32F"); break;
        case GL_RG32F: name.append("RG32F"); break;
        case GL_RGB32F: name.append("RGB32F"); break;
        case GL_RGBA32F: name.append("RGBA32F"); break;
    }

    name.append(" (");
    name.append(std::to_string(texture->size.x));
    name.append("x");
    name.append(std::to_string(texture->size.y));
    name.append(")");

    return name;
}

static void drawTimer(int i,Timer *timer, ImDrawList *drawList, double totalWidth, float timerRowHeight, float timerBarHeight, ImVec2 maxSize, ImVec2 windowPos) {
    auto startOffset = timer->startTime - frameTimer->startTime;
    auto width = timer->stopTime - timer->startTime;

    auto pos = ImVec2(startOffset / totalWidth * maxSize.x, timerRowHeight * i);
    auto barSize = ImVec2(width / totalWidth * maxSize.x, timerBarHeight);

    auto topLeft = ImVec2(windowPos.x + pos.x, windowPos.y + pos.y);
    drawList->AddRectFilled(
            topLeft,
            ImVec2(topLeft.x + barSize.x, topLeft.y + barSize.y),
            colors[i % colorCount]
    );

    auto oldPos = ImGui::GetCursorScreenPos();
    ImGui::SetCursorScreenPos(ImVec2(topLeft.x + barSize.x + 5, topLeft.y));
    std::string nameStr;
    nameStr.append(timer->name);
    nameStr.append(" ");

    std::stringstream durationStrm;
    durationStrm << std::fixed << std::setprecision(2) << timer->duration / 1000000.0;
    nameStr.append(durationStrm.str());
    nameStr.append("ms (");

    std::stringstream percentStrm;
    percentStrm << std::fixed << std::setprecision(2) << 100.0 * timer->duration / frameTimer->duration;
    nameStr.append(percentStrm.str());
    nameStr.append("%)");
    ImGui::Text(nameStr.c_str());

    ImGui::SetCursorScreenPos(oldPos);
}

void monad::frameEnd() {
    // draw texture debuggers
    uiTimer->start();
    if (ImGui::Begin("Textures")) {
        ImGui::PushItemWidth(-1);
        if (ImGui::ListBoxHeader("##dummy", ImVec2(0, ImGui::GetWindowContentRegionMax().y - 30))) {
            for (int i = 0; i < monad::textures.size(); i++) {
                auto texEntry = monad::textures[i];
                auto itemSel = i == selectedTexture;

                auto texSize = texEntry.second->size;

                // fit size
                auto maxSize = vec2(100, 100);
                auto fitSize = texSize.x > texSize.y
                               ? vec2(maxSize.x, (float) texSize.y / texSize.x * maxSize.x)
                               : vec2((float) texSize.x / texSize.y * maxSize.y, maxSize.y);

                ImGui::PushID(i);
                auto backupPos = ImGui::GetCursorScreenPos();
                if (ImGui::Selectable("##dummy", itemSel, 0, ImVec2(0, fitSize.y))) {
                    selectedTexture = i;
                    showTexPreview = true;
                }
                ImGui::SetCursorScreenPos(backupPos);

                ImGui::Image((ImTextureID) texEntry.second->resource, ImVec2(fitSize.x, fitSize.y), ImVec2(0, 1),
                             ImVec2(1, 0));
                ImGui::SameLine();

                ImGui::Text(getTexString(texEntry.second, texEntry.first).c_str());
                ImGui::PopID();
            }

        }
        ImGui::ListBoxFooter();
        ImGui::PopItemWidth();
    }
    ImGui::End();

    if (showTexPreview && selectedTexture >= 0 && selectedTexture < monad::textures.size()) {
        if (ImGui::Begin("Texture Preview", &showTexPreview)) {

            auto texEntry = monad::textures[selectedTexture];

            auto maxSize = ImGui::GetWindowContentRegionMax();
            maxSize.y -= 50;

            auto texSize = texEntry.second->size;
            auto fitSize = vec2(1, (float)texSize.y / texSize.x) * maxSize.x;
            if (fitSize.y > maxSize.y) {
                fitSize = vec2((float)texSize.x / texSize.y, 1) * maxSize.y;
            }

            ImGui::Image((ImTextureID) texEntry.second->resource, ImVec2(fitSize.x, fitSize.y), ImVec2(0, 1), ImVec2(1, 0));
            ImGui::Text(getTexString(texEntry.second, texEntry.first).c_str());
        }
        ImGui::End();
    }

    uiTimer->stop();
    frameTimer->stop();

    // draw performance window
    if (ImGui::Begin("Performance")) {
        frameTimer->poll();

        auto drawList = ImGui::GetWindowDrawList();
        auto windowPos = ImGui::GetWindowPos();
        auto maxSize = ImGui::GetWindowContentRegionMax();

        windowPos.x += 5;
        windowPos.y += 25;
        maxSize.x -= 10;
        maxSize.y -= 30;

        auto timerRowHeight = maxSize.y / (monad::timers.size() + 1);
        auto timerBarHeight = timerRowHeight * 9 / 10;

        auto totalWidth = 50 * 1000000.0;

        /*auto isSorted
        for (auto &timer : monad::timers) {
            timer->poll();
        }*/

        auto isSorted = true;
        for (auto i = 0; i < monad::timers.size(); i++) {
            auto timer = monad::timers[i];
            timer->poll();

            if (i > 0 && isSorted) {
                auto lastTimer = monad::timers[i - 1];
                if (lastTimer->startTime > timer->startTime) isSorted = false;
            }
        }

        if (!isSorted) {
            std::cout << "Resorting!" << std::endl;
            std::sort(monad::timers.begin(), monad::timers.end(), [](Timer *a, Timer *b) {
                return a->startTime < b->startTime;
            });
        }

        drawTimer(0, frameTimer, drawList, totalWidth, timerRowHeight, timerBarHeight, maxSize, windowPos);
        for (auto i = 0; i < monad::timers.size(); i++) {
            auto timer = monad::timers[i];
            drawTimer(i + 1, timer, drawList, totalWidth, timerRowHeight, timerBarHeight, maxSize, windowPos);
        }

        /*for (const auto &timer: monad::timers) {
            drawList->AddRectFilled
        }*/
    }
    ImGui::End();

    ImGui::Render();

    auto currentTime = timeGetTime();
    if(currentTime - lastUpdateTime > 100) {
        // recompile shaders
        globalManager.update();

        // check+render timers
        // todo: need a better way to do this
#ifdef PROFILER_ENABLED
        auto frameDuration = frameTimer->getDuration();

        std::string str;
        auto accumulateTime = frameDuration;
        for (const auto &timer : monad::timers) {
            auto duration = timer->getDuration();
            accumulateTime -= duration;
            str.append(timer->name);
            str.append(": ");
            str.append(std::to_string(duration / 1000000.0));
            str.append("ms (");
            str.append(std::to_string(100.0 * duration / frameDuration));
            str.append("%), ");
        }

        str.append("??: ");
        str.append(std::to_string(accumulateTime / 1000000.0));
        str.append("ms (");
        str.append(std::to_string(100.0 * accumulateTime / frameDuration));
        str.append("%), ");

        str.append("FRAME: ");
        str.append(std::to_string(frameDuration / 1000000.0));
        str.append("ms");
        log(str.c_str());
#endif

        lastUpdateTime = currentTime;
    }

    // process OpenGL debug messages
    GLenum source;
    GLenum type;
    GLuint id;
    GLenum severity;
    GLsizei msgLen;

    //dbgEnter("Message log");
    while (glGetDebugMessageLog(1, maxMsgLen, &source, &type, &id, &severity, &msgLen, msgBuffer)) {
        if (!msgLen) continue;
        processDebugMessage(source, type, id, severity, std::string(msgBuffer, msgLen));
    }
    //dbgExit();
}

static std::map<const ShaderKey, std::weak_ptr<Shader>> shaderCache;

std::vector<ShaderKey> monad::shaderJournal;

SHARED_PTR(Shader) Context::getShader(const char *path, GLenum type) const {
    // look in the "shaders" directory
    char combinedStr[MAX_PATH];
    PathCombine(combinedStr, "../shaders", path);

    // resolve path, relative to shader folder
    char outStr[MAX_PATH];
    GetFullPathName(combinedStr, MAX_PATH, outStr, nullptr);

    ShaderKey key = { std::string(outStr), type };
    shaderJournal.push_back(key);

    auto entry = shaderCache.find(key);

    auto exists = entry != shaderCache.end();
    if (exists && entry->second.expired()) {
        shaderCache.erase(entry);
        entry = shaderCache.end();
        exists = false;
    }

    if (!exists) {
        auto res = glCreateShader(type);
        auto shader = MAKE_SHARED(Shader, res, type, key.name);
        shader->build();
        monad::globalManager.add(shader);
        shaderCache.insert(std::pair<const ShaderKey, std::weak_ptr<Shader>>(key, shader));
        return shader;
    }

    return entry->second.lock();
}

SHARED_PTR(Camera) Context::createCamera(Track<vec3> *tCameraPos, Track<ForwardDirection> *tGymbalDir,
                                         Track<float> *tArmLength, Track<Direction> *tCameraDir, Track<float> *tFov,
                                         Track<vec2> *tClipRange) const {
    auto camera = std::make_shared<Camera>(tCameraPos, tGymbalDir, tArmLength, tCameraDir, tFov, tClipRange);
    monad::cameras.push_back(camera);
    return camera;
}

Timer* Context::createTimer(const char *name) {
    auto timer = new Timer(name);
    monad::timers.push_back(timer);
    return timer;
}
