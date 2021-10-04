#include <cstdlib>

void *operator new(size_t s) {
    return malloc(s);
}

void operator delete(void *p) {
    free(p);
}

void *operator new[](size_t s) {
    return malloc(s);
}

void operator delete[](void *p) {
    free(p);
}
