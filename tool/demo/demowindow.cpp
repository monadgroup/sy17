#include <cstdio>
#include "demowindow.h"
#include "metadata.h"
#include "renderer/gl.h"
#include "renderer/scene.h"
#include "time/timemanager.h"
#include "renderer/context.h"
#include "renderer/common.h"
#include "player/resource.h"
#include "debug.h"

#include "scenes/rover.h"
#include "scenes/marsTestScene.h"

#ifndef DEBUG
#include "player/resource.h"
#endif

using namespace demo;

static const PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    32,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    32,
    0,
    0,
    PFD_MAIN_PLANE,
    0, 0, 0, 0
};

DemoWindow::DemoWindow(const DemoConfig &config) : config(config) {
    // create window class
    data.hInstance = GetModuleHandle(nullptr);

    wc.style = CS_OWNDC;
    wc.lpfnWndProc = config.messageHandler;
    wc.hInstance = data.hInstance;
    wc.lpszClassName = meta::name;
    wc.hIcon = LoadIcon(data.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    if (!RegisterClass(&wc)) {
        error = true;
        return;
    }
}

void DemoWindow::beforeRender() { }

LRESULT CALLBACK DemoWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_CLOSE || uMsg == WM_KEYDOWN && wParam == VK_ESCAPE) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

UINT DemoWindow::show(DEVMODE *mode) {
    DWORD dwStyle;

    DWORD dwExStyle = WS_EX_APPWINDOW;

    if (config.fullscreen) {
        if (ChangeDisplaySettings(mode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) return 1;
        dwStyle = WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        ShowCursor(false);
    } else {
#ifdef DEBUG
        dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
        dwExStyle = WS_EX_OVERLAPPEDWINDOW;
#else
        dwStyle = WS_VISIBLE | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU;
#endif
    }

    RECT rec{};
    rec.left = 0;
    rec.top = 0;
    rec.right = config.width;
    rec.bottom = config.height;
    AdjustWindowRect(&rec, dwStyle, 0);
    data.hWnd = CreateWindowEx(
        dwExStyle,
        wc.lpszClassName, wc.lpszClassName,
        dwStyle,
        GetSystemMetrics(SM_CXSCREEN) - rec.right + rec.left >> 1,
        GetSystemMetrics(SM_CYSCREEN) - rec.bottom + rec.top >> 1,
        rec.right - rec.left, rec.bottom - rec.top,
        nullptr, nullptr, data.hInstance, nullptr
    );

    int pixelFormat;

    if (!data.hWnd) return 2;
    if (!(data.hDC = GetDC(data.hWnd))) return 3;
    if (!(pixelFormat = ChoosePixelFormat(data.hDC, &pfd))) return 4;
    if (!SetPixelFormat(data.hDC, pixelFormat, &pfd)) return 5;
    if (!(data.hRC = wglCreateContext(data.hDC))) return 6;
    if (!wglMakeCurrent(data.hDC, data.hRC)) return 7;
    if (glextInit() != nullptr) return 8;

    SetForegroundWindow(data.hWnd);
    SetFocus(data.hWnd);

    return 0;
}

void DemoWindow::hide() {
    if (data.hRC) {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(data.hRC);
    }

    if (data.hDC) ReleaseDC(data.hWnd, data.hDC);
    if (data.hWnd) DestroyWindow(data.hWnd);

    UnregisterClass(wc.lpszClassName, data.hInstance);

    if (config.fullscreen) {
        ChangeDisplaySettings(nullptr, 0);
        ShowCursor(true);
    }
}

void DemoWindow::displayProgress(float progress) {
    auto logoWidth = config.width / 3;
    auto sizeMultiplier = logoWidth / 381.0f;

    auto logoHeight = (int)(118 * sizeMultiplier);

    // logo rectangle coordinates
    auto logoBottomY = config.height >> 1;
    auto logoTopY = logoBottomY - logoHeight;
    auto logoLeftX = config.width / 2 - logoWidth / 2;

    SelectObject(hProgressDc, GetStockObject(NULL_PEN));
    SelectObject(hProgressDc, GetStockObject(BLACK_BRUSH));

    // black out window
    Rectangle(hProgressDc, 0, 0, logoWidth, logoHeight);

    auto textX = logoLeftX + (int)(41 * sizeMultiplier);
    auto textY = logoTopY - (int)(7 * sizeMultiplier);
    auto textHeight = (int)(145 * sizeMultiplier);
    auto textWidth = logoWidth - (int)(41 * sizeMultiplier);

    auto barWidth = (int)((logoWidth - (int)(14 * sizeMultiplier)) * progress);
    auto barRight = logoLeftX + (int)(7 * sizeMultiplier) + barWidth;

    // draw text
    SetBkMode(hProgressDc, TRANSPARENT);

    SetTextCharacterExtra(hProgressDc, (int)(-17 * sizeMultiplier));
    SelectObject(hProgressDc, titleFont);

    // dark background text
    SetTextColor(hProgressDc, 0x00333333);
    TextOut(hProgressDc, textX, textY, "monad", 5);

    // cutout background text to reduce aliasing
    RECT progressRect = {
        textX, textY,
        barRight, textY + textHeight
    };
    Rectangle(hProgressDc, progressRect.left, progressRect.top, progressRect.right, progressRect.bottom);

    // light progress text
    SetTextColor(hProgressDc, 0x00ffffff);
    DrawText(hProgressDc, "monad", -1, &progressRect, DT_BOTTOM | DT_LEFT | DT_SINGLELINE);

    // cutout from m
    POINT polyPoints[4];

    polyPoints[0] = { logoLeftX + (int)(32 * sizeMultiplier), logoTopY + (int)(111 * sizeMultiplier) };
    polyPoints[1] = { logoLeftX + (int)(84 * sizeMultiplier), logoTopY + (int)(111 * sizeMultiplier) };
    polyPoints[2] = { logoLeftX + (int)(32 * sizeMultiplier), logoTopY + (int)(7 * sizeMultiplier) };
    Polygon(hProgressDc, polyPoints, 3);

    // draw lambda
    SelectObject(hProgressDc, lambdaBrush);

    polyPoints[0] = { logoLeftX + (int)(28 * sizeMultiplier), logoTopY + (int)(7 * sizeMultiplier) };
    polyPoints[1] = { logoLeftX + (int)(7 * sizeMultiplier), logoTopY + (int)(7 * sizeMultiplier) };
    polyPoints[2] = { logoLeftX + (int)(59 * sizeMultiplier), logoTopY + (int)(111 * sizeMultiplier) };
    polyPoints[3] = { logoLeftX + (int)(80 * sizeMultiplier), logoTopY + (int)(111 * sizeMultiplier) };
    Polygon(hProgressDc, polyPoints, 4);

    polyPoints[0] = { logoLeftX + (int)(43 * sizeMultiplier), logoTopY + (int)(39 * sizeMultiplier) };
    polyPoints[1] = { logoLeftX + (int)(53 * sizeMultiplier), logoTopY + (int)(61 * sizeMultiplier) };
    polyPoints[2] = { logoLeftX + (int)(28 * sizeMultiplier), logoTopY + (int)(111 * sizeMultiplier) };
    polyPoints[3] = { logoLeftX + (int)(7 * sizeMultiplier), logoTopY + (int)(111 * sizeMultiplier) };
    Polygon(hProgressDc, polyPoints, 4);

    // draw progress bar
    SelectObject(hProgressDc, GetStockObject(WHITE_BRUSH));
    Rectangle(hProgressDc, logoLeftX + (int)(7 * sizeMultiplier), logoBottomY + (int)(20 * sizeMultiplier), barRight, logoBottomY + (int)(40 * sizeMultiplier));

    BitBlt(data.hDC, 0, 0, config.width, config.height, hProgressDc, 0, 0, SRCCOPY);
}

void DemoWindow::initialize() {
    // setup OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    // enable/disable vsync
    auto wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    if (wglSwapIntervalEXT) wglSwapIntervalEXT(config.vsync);

    // create double-buffered DC for progress bar
    hProgressDc = CreateCompatibleDC(data.hDC);
    titleFont = CreateFont((int)(145 * config.width / (3.f * 381.0f)), 0, 0, 800, false, false, false, ANSI_CHARSET, 0, 0, 0, ANTIALIASED_QUALITY, 0, "Arial Bold");
    lambdaBrush = CreateSolidBrush(RGB(0x34, 0x49, 0x5e));
    auto hBitmap = CreateCompatibleBitmap(data.hDC, config.width, config.height);
    SelectObject(hProgressDc, hBitmap);

    // load resources
    monad::init(this);

    auto sceneList = List<monad::Scene*>(1);
    sceneList.push_back(new monad::MarsTestScene());

    // set initial screen size
    RECT windowRect{};
    GetWindowRect(data.hWnd, &windowRect);

    manager = new monad::TimeManager(meta::bpm / 60 * meta::rpb, sceneList);
    context = new monad::Context(manager, nullptr, { windowRect.right - windowRect.left, windowRect.bottom - windowRect.top });

    // create and initialize scenes - todo: move init() to constructor
    for (auto &i : sceneList) {
        i->context = context;
        i->init(this);
    }
}

void DemoWindow::startLoop() {
    monad::Scene *lastScene = nullptr;

    MSG msg{};
    auto done = false;
    while (!done) {
#ifndef DEBUG
        if (manager->getRow() > 1856) {
            done = true;
            break;
        }
#endif

        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                done = true;
                break;
            }
            DispatchMessage(&msg);
        }

        beforeRender();
        manager->update();
        auto scene = manager->getScene();

        // keep track of scenes for enter/leave calls
        if (scene != lastScene) {
            if (lastScene != nullptr) lastScene->leave();
            scene->enter();
        }
        lastScene = scene;

        monad::frameStart();
        scene->frame();
        monad::frameEnd();

        SwapBuffers(data.hDC);
    }
}
