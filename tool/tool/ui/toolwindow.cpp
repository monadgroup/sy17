#include <iostream>
#include <cmath>
#include "toolwindow.h"
#include "dependency/shadermanager.h"
#include "renderer/camera.h"
#include "debug.h"
#include "imgui/imgui_gl.h"

using namespace monad;
using namespace demo;

std::vector<Timer*> monad::timers{};
std::vector<std::weak_ptr<Camera>> monad::cameras{};
std::vector<std::pair<std::string, Texture*>> monad::textures{};
Camera *monad::lastCamera = nullptr;
vec3 monad::lockCameraPos = {0, 0, 0};
Direction monad::lockCameraDir = {0, 0, 0};
float monad::lockCameraFov = 0;

ToolWindow::ToolWindow()
        : DemoWindow({
            WndProc,
            /*960, 540,*/1707, 720,
            false,
            true,
            true,
            true
        }) {
    if (error) ExitProcess(1);
}

LRESULT CALLBACK ToolWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_LBUTTONDOWN:
            ((ToolWindow *) GetWindowLongPtr(hWnd, GWLP_USERDATA))->_buttonDown();
            break;
        case WM_MBUTTONDOWN:
            if (ImGui::GetIO().WantCaptureMouse) ImGuiGl::MouseDownCallback(21);
            break;
        case WM_RBUTTONDOWN:
            if (ImGui::GetIO().WantCaptureMouse) ImGuiGl::MouseDownCallback(1);
            break;
        case WM_LBUTTONUP:
            ((ToolWindow *) GetWindowLongPtr(hWnd, GWLP_USERDATA))->_buttonUp();
            break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if (ImGui::GetIO().WantCaptureKeyboard) ImGuiGl::KeyCallback(wParam, true);
            else ((ToolWindow *) GetWindowLongPtr(hWnd, GWLP_USERDATA))->_keyDown(wParam);
            break;
        case WM_KEYUP:
            if (ImGui::GetIO().WantCaptureKeyboard) ImGuiGl::KeyCallback(wParam, false);
            else ((ToolWindow *) GetWindowLongPtr(hWnd, GWLP_USERDATA))->_keyUp(wParam);
            break;
        case WM_CHAR:
        case WM_SYSCHAR:
            if (ImGui::GetIO().WantCaptureKeyboard) ImGuiGl::CharCallback(wParam);
            break;
        case WM_MOUSEWHEEL:
            if (ImGui::GetIO().WantCaptureMouse) ImGuiGl::ScrollCallback(0, (SHORT) HIWORD(wParam) / (double) WHEEL_DELTA);
            else ((ToolWindow *) GetWindowLongPtr(hWnd, GWLP_USERDATA))->_mouseWheel((short)(wParam >> 16));
            break;
        case 0x020E: // MOUSEHWHEEL
            if (ImGui::GetIO().WantCaptureMouse) ImGuiGl::ScrollCallback(-(SHORT) HIWORD(wParam) / (double) WHEEL_DELTA, 0);
            break;
        default: return DemoWindow::WndProc(hWnd, uMsg, wParam, lParam);
    }

    return TRUE;
}

UINT ToolWindow::show(DEVMODE *mode) {
    auto err = DemoWindow::show(mode);
    if (err) return err;

    ImGuiGl::Init(data.hWnd);

    glEnable(GL_DEBUG_OUTPUT);

    SetWindowLongPtr(data.hWnd, GWLP_USERDATA, (LONG_PTR)this);

    char folderBuff[MAX_PATH];
    GetFullPathName("../shaders", MAX_PATH, folderBuff, nullptr);
    globalManager.startWatching(std::string(folderBuff));

    return 0;
}

void ToolWindow::beforeRender() {
    updateCameras();
}

void ToolWindow::_buttonDown() {
    if (!_isControlling && ImGui::GetIO().WantCaptureMouse) ImGuiGl::MouseDownCallback(0);
    else _isDown = true;
}

void ToolWindow::_buttonUp() {
    if (!_isDown) return;
    _isDown = false;

    _walkDirection = 0;
    _strafeDirection = 0;
    _flyDirection = 0;
    _rollDirection = 0;

    _isControlling = !_isControlling;

    if (_isControlling) {
        RECT windowRect{};
        if (!GetWindowRect(data.hWnd, &windowRect)) return;
        _centerPos = {(windowRect.left + windowRect.right) / 2, (windowRect.top + windowRect.bottom) / 2};
        SetCursorPos(_centerPos.x, _centerPos.y);
        ShowCursor(false);
    } else ShowCursor(true);

    if (_isLocked) return;
    _isLocked = true;
    log("Cameras locked");

    if (monad::lastCamera) {
        lockCameraPos = monad::lastCamera->getPosition();
        lockCameraDir = monad::lastCamera->getDirection();
        lockCameraFov = monad::lastCamera->getFov();
    } else {
        lockCameraPos = {0, 0, 0};
        lockCameraDir = {0, 0, 0};
        lockCameraFov = 60;
    }
}

void ToolWindow::_keyDown(WPARAM keyCode) {
    if (!_isLocked || !_isControlling) return;

    switch (keyCode) {
        case 0x57: // W
            _walkDirection = 1;
            break;
        case 0x53: // S
            _walkDirection = -1;
            break;
        case 0x41: // A
            _strafeDirection = 1;
            break;
        case 0x44: // D
            _strafeDirection = -1;
            break;
        case VK_SPACE:
            _flyDirection = 1;
            break;
        case VK_CONTROL:
            _flyDirection = -1;
            break;
        case 0x51: // Q
            _rollDirection = 1;
            break;
        case 0x45: // E
            _rollDirection = -1;
            break;
        case 0x52: // R
            lockCameraDir.roll = 0;
            lockCameraFov = 60;
            break;
        case VK_LSHIFT:
            _speed = _moveSensitivity * 10;
            break;
    }
}

void ToolWindow::_keyUp(WPARAM keyCode) {
    if (!_isLocked) return;

    if (keyCode == VK_ESCAPE) {
        _walkDirection = 0;
        _strafeDirection = 0;
        _flyDirection = 0;
        _rollDirection = 0;
        _isLocked = false;
        _isControlling = false;
        ShowCursor(true);
        log("Cameras unlocked");
    }

    if (!_isControlling) return;

    switch (keyCode) {
        case 0x57: // W
        case 0x53: // S
            _walkDirection = 0;
            break;
        case 0x41: // A
        case 0x44: // D
            _strafeDirection = 0;
            break;
        case VK_SPACE:
        case VK_CONTROL:
            _flyDirection = 0;
            break;
        case 0x51: // W
        case 0x45: // E
            _rollDirection = 0;
            break;
        case VK_LSHIFT:
            _speed = _moveSensitivity;
            break;
        case 0x50: // P
            log(("Position: (" + std::to_string(lockCameraPos.x) + "," + std::to_string(lockCameraPos.y) + "," + std::to_string(lockCameraPos.z) + ")").c_str());
            log(("Direction: (" + std::to_string(lockCameraDir.pitch * 180 / M_PI) + "," + std::to_string(lockCameraDir.yaw * 180 / M_PI) + "," + std::to_string(lockCameraDir.roll * 180 / M_PI) + ")").c_str());
            break;
    }
}

void ToolWindow::_mouseWheel(int delta) {
    if (!_isControlling) return;

    lockCameraFov = lockCameraFov - delta * _fovSensitivity;
    if (lockCameraFov < 0) lockCameraFov = 0;
    if (lockCameraFov > 180) lockCameraFov = 180;
}

void ToolWindow::updateCameras() {
    if (_wasLocked != _isLocked) {
        for (auto i = 0; i < monad::cameras.size(); i++) {
            auto cam = monad::cameras[i];
            if (cam.expired()) {
                monad::cameras.erase(monad::cameras.begin() + i);
                i--;
            } else if (_isLocked) cam.lock()->lock();
            else cam.lock()->unlock();
        }
    }
    _wasLocked = _isLocked;

    if (!_isControlling) return;

    POINT cursorPosPoint{};
    if (!GetCursorPos(&cursorPosPoint)) return;
    SetCursorPos(_centerPos.x, _centerPos.y);

    ivec2 cursorPos = {cursorPosPoint.x, cursorPosPoint.y};
    auto cursorMove = cursorPos - _centerPos;

    lockCameraDir.yaw += cursorMove.x * _rotateSensitivity * M_PI / 180 * -1;
    lockCameraDir.pitch += cursorMove.y * _rotateSensitivity * M_PI / 180;
    lockCameraDir.roll += _rollDirection * _rotateSensitivity * 10 * M_PI / 180;

    auto dirVec = lockCameraDir.toForward();
    auto upVec = lockCameraDir.toUp();

    lockCameraPos = lockCameraPos + dirVec * _walkDirection * _speed * -1;
    lockCameraPos = lockCameraPos + upVec.cross(dirVec) * _strafeDirection * _speed;
    lockCameraPos = lockCameraPos + upVec * _flyDirection * _speed;

    //log(("Camera: (" + std::to_string(lockCameraPos.x) + "," + std::to_string(lockCameraPos.y) + "," + std::to_string(lockCameraPos.z) + ")").c_str());
}

void ToolWindow::startLoop() {
    DemoWindow::startLoop();

    ImGuiGl::Shutdown();
}
