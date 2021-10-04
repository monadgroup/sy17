#include <cmath>

float __cdecl sinf(float x) {
    float r;
    __asm("fsin" : "=t" (r) : "0" (x));
    return r;
}

float __cdecl cosf(float x) {
    float r;
    __asm("fcos" : "=t" (r) : "0" (x));
    return r;
}

float __cdecl tanf(float x) {
    float r;
    __asm("fptan; fstp %0" : "=t" (r) : "0" (x));
    return r;
}

float __cdecl sqrtf(float x) {
    float r;
    __asm("fsqrt" : "=t" (r) : "0" (x));
    return r;
}

double __cdecl log2(double x) {
    double r;
    __asm("fld1; fxch; fyl2x" : "=t" (r) : "0" (x));
    return r;
}

float __cdecl log10f(float x) {
    float r;
    __asm("fldlg2; fxch; fyl2x" : "=t" (r) : "0" (x));
    return r;
}

float __cdecl logf(float x) {
    float r;
    __asm("fldln2; fxch; fyl2x" : "=t" (r) : "0" (x));
    return r;
}

float __cdecl powf(float x, float y) {
    float r;
    __asm("fyl2x; fld1; fld %%st(1); fprem; f2xm1; faddp; fscale; fxch %%st(1); fstp %%st" : "=t" (r) : "0" (x), "u" (y) : "st(1)");
    return r;
}

float __cdecl expf(float x) {
    float r;
    __asm("fldl2e; fmulp; fld %%st; frndint; fsubr %%st, %%st(1); fxch; f2xm1; fld1; faddp; fscale; fstp %%st(1)" : "=t" (r) : "0" (x));
    return r;
}

float __cdecl fmodf(float x, float y) {
    float r;
    __asm("fprem" : "=t" (r) : "0" (x), "u" (y));
    return r;
}
