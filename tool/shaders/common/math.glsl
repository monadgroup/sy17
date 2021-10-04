#define M_PI 3.1415926535897932384626433832795
#define M_PI2 2*M_PI
#define M_PI4 4*M_PI
#define TAU (2*PI)
#define PHI (sqrt(5)*0.5 + 0.5)
#define INFINITY pow(2., 8.)
#define EPSILON 1e-10

#define MIN_RADIUS 0.00005

#define saturate(x) clamp((x), 0, 1)

float sgn(float x) {
    return x < 0 ? -1 : 1;
}
vec2 sgn(vec2 v) {
	return vec2((v.x<0)?-1:1, (v.y<0)?-1:1);
}

vec2 doR(vec2 p, float a) {
    return cos(a)*p + sin(a)*vec2(p.y, -p.x);
}

// Rotate around a coordinate axis (i.e. in a plane perpendicular to that axis) by angle <a>.
// Read like this: R(p.xz, a) rotates "x towards z".
// This is fast if <a> is a compile-time constant and slower (but still practical) if not.
void pR(inout vec2 p, float a) {
	p.xy = doR(p, a);
}
