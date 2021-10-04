#pragma once
#include "../../demo/demowindow.h"
#include <vector>
#include <map>
#include <memory>
#include "vector.h"

namespace monad {

    class Camera;
    class Timer;
    class Texture;

    extern std::vector<Timer*> timers;

    extern std::vector<std::weak_ptr<Camera>> cameras;
    extern Camera *lastCamera;

    extern std::vector<std::pair<std::string, Texture*>> textures;

    extern vec3 lockCameraPos;
    extern Direction lockCameraDir;
    extern float lockCameraFov;

    class ToolWindow : public demo::DemoWindow {
        bool _wasLocked = false;
        bool _isLocked = false;
        bool _isDown = false;
        bool _isControlling = false;

        const float _rotateSensitivity = 0.3;
        const float _moveSensitivity = 0.01;
        const float _fovSensitivity = 0.01;

        ivec2 _centerPos{};
        float _walkDirection = 0;
        float _strafeDirection = 0;
        float _flyDirection = 0;
        float _rollDirection = 0;
        float _speed = _moveSensitivity;

        void _buttonDown();
        void _buttonUp();
        void _keyDown(WPARAM keyCode);
        void _keyUp(WPARAM keyCode);
        void _mouseWheel(int delta);

        static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    protected:
        void beforeRender() override;

    public:
        explicit ToolWindow();

        UINT show(DEVMODE *mode) override;
        void startLoop() override;

        void updateCameras();
    };

}
