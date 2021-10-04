#include <cstdlib>
#include "ui/runtime.h"
#include "resource.h"
#include "metadata.h"
#include "demo/demowindow.h"
#include "time/timemanager.h"

using namespace monad;

static void checkbox(HWND parent, int id) {
    PostMessage(GetDlgItem(parent, id), BM_SETCHECK, BST_CHECKED, 0);
}

static DWORD selectedDisplaySetting;
static bool isFullscreen;
static bool isVsync;

static INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_INITDIALOG: {
            // set icon
            auto hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON1));
            SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

            checkbox(hDlg, ID_VSYNC);
            checkbox(hDlg, ID_FS);

            // populate combobox
            DEVMODE dm = { 0 };
            dm.dmSize = sizeof(dm);
            DWORD iModeNum = 0;
            for (iModeNum = 0; EnumDisplaySettings(nullptr, iModeNum, &dm) != 0; iModeNum++) {
                char buff[50];
                itoa((int)dm.dmPelsWidth, buff, 10);
                strcat(buff, "x");
                itoa((int)dm.dmPelsHeight, buff + strlen(buff), 10);
                strcat(buff, " ");
                itoa((int)dm.dmDisplayFrequency, buff + strlen(buff), 10);
                strcat(buff, "Hz");

                SendDlgItemMessage(hDlg, ID_RESOLUTION, CB_ADDSTRING, 0, (LPARAM)buff);
            }
            SendDlgItemMessage(hDlg, ID_RESOLUTION, CB_SETCURSEL, iModeNum - 1, 0);

            // set title and subtitle
            SetWindowText(hDlg, meta::name);
            SetDlgItemText(hDlg, ID_TITLE, meta::name);
            SetDlgItemText(hDlg, ID_SUBTITLE, meta::subtitle);

            break;
        }
        case WM_CLOSE:
            EndDialog(hDlg, IDB_EXIT);
            break;
        case WM_COMMAND: {
            if (wParam == IDB_START) {
                selectedDisplaySetting = (DWORD)SendDlgItemMessage(hDlg, ID_RESOLUTION, CB_GETCURSEL, 0, 0);
                isFullscreen = (bool)IsDlgButtonChecked(hDlg, ID_FS);
                isVsync = (bool)IsDlgButtonChecked(hDlg, ID_VSYNC);

                EndDialog(hDlg, 1);
            } else if (wParam == IDB_EXIT) {
                EndDialog(hDlg, 0);
            }
            break;
        }
        case WM_LBUTTONDOWN:
            return SendMessage(hDlg, WM_NCLBUTTONDOWN, HTCAPTION, 0);
        default:
             return false;
    }
    return true;
}

Runtime::Runtime(HINSTANCE hInstance) : hInstance(hInstance) {

}

int Runtime::run() {
    auto exitButton = DialogBox(hInstance, MAKEINTRESOURCE(IDD_INIT), nullptr, DialogProc);
    if (exitButton == 0) return 0;

    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

    DEVMODE dm{};
    EnumDisplaySettings(nullptr, selectedDisplaySetting, &dm);

    // create the demo window
    demo::DemoConfig conf = {
        demo::DemoWindow::WndProc,
        (int)dm.dmPelsWidth,
        (int)dm.dmPelsHeight,
        isFullscreen,
        isVsync,
        true,
        false
    };

    auto window = demo::DemoWindow(conf);
    auto err = window.show(&dm);
    if (err) {
        window.hide();
        return err + 1;
    }

    srand(0xdeadbeef);

    // initialize graphics things
    window.initialize();

    window.manager->play();

    // start graphics loop
    window.startLoop();
    window.hide();
    return 0;
}
