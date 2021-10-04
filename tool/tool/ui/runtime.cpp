#include "ui/runtime.h"
#include "toolwindow.h"
#include "export/journals.h"
#include "export/exporter.h"
#include <fstream>
#include <iostream>
#include "../../../glslang/glslang/Public/ShaderLang.h"

using namespace monad;

Runtime::Runtime(HINSTANCE hInstance) : hInstance(hInstance) {

}

int Runtime::run() {
    ShInitialize();

    // allow virtual terminal escape sequences
    auto hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        std::cout << "Oh no 1!" << std::endl;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        std::cout << "Oh no 2!" << std::endl;
    }

    dwMode |= 0x0004;
    if (!SetConsoleMode(hOut, dwMode)) {
        std::cout << "Oh no 3!" << std::endl;
    }

    auto window = ToolWindow();

    auto err = window.show(nullptr);
    if (err) {
        window.hide();
        return err + 1;
    }

    srand(0xdeadbeef);
    //printf("show=%i\n", rand());

    window.initialize();
    window.startLoop();
    window.hide();

    // export all that data
    auto stream = exporter::exportData(shaderJournal, trackJournal, window.manager);
    std::ofstream outFile;
    outFile.open("../player/export.cpp");
    outFile << stream.rdbuf();
    outFile.close();

    return 0;
}
