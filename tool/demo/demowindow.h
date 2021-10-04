#pragma once
#include "list.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace monad {
    class TimeManager;
    class Context;
}

namespace demo {

    typedef LRESULT(CALLBACK *WNDPROC)(HWND, UINT, WPARAM, LPARAM);

    struct DemoConfig {
        WNDPROC messageHandler;
        int width;
        int height;
        bool fullscreen;
        bool vsync;
        bool music;
        bool loop;
    };

    struct WindowData {
        HINSTANCE hInstance;
        HWND hWnd;
        HDC hDC;
        HGLRC hRC;
    };

    class DemoWindow {
        WNDCLASSA wc{};

        HDC hProgressDc;
        HFONT titleFont;
        HBRUSH lambdaBrush;

    protected:
        virtual void beforeRender();

    public:
        static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

        monad::TimeManager *manager;
        monad::Context *context;

        bool error = false;
        DemoConfig config;
        WindowData data;

        explicit DemoWindow(const DemoConfig &config);

        virtual UINT show(DEVMODE *mode);
        virtual void hide();

        virtual void displayProgress(float progress);
        virtual void initialize();
        virtual void startLoop();
    };

}
