#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdlib>
#include "ui/runtime.h"

#ifdef DEBUG
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    srand(0xdeadbeef);
    auto runtime = monad::Runtime(hInstance);
    return runtime.run();
}
#else
typedef void (*func_ptr) ();

extern func_ptr __CTOR_LIST__[];
extern func_ptr __DTOR_LIST__[];

static void
__do_global_dtors()
{
    static func_ptr *p = __DTOR_LIST__ + 1;

    while (*p)
    {
        (*(p)) ();
        p++;
    }
}

static void
__do_global_ctors()
{
    auto nptrs = (unsigned long) (ptrdiff_t) __CTOR_LIST__[0];
    unsigned long i;

    if (nptrs == (unsigned long) -1)
    {
        for (nptrs = 0; __CTOR_LIST__[nptrs + 1] != 0; nptrs++);
    }

    for (i = nptrs; i >= 1; i--)
    {
        __CTOR_LIST__[i] ();
    }
}

extern "C" int WINAPI MonadMain() {
    __do_global_ctors();
    srand(0xdeadbeef);
    auto runtime = monad::Runtime(GetModuleHandle(nullptr));
    auto ret = runtime.run();
    __do_global_dtors();
    ExitProcess(ret);
}
#endif

