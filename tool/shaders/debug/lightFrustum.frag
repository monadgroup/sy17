#include "../common/rasterFrag.glsl"

in vec3 VertPos;

void main() {
	rasterFrag(VertPos, vec3(0), vec3(0), VertPos, -1.5, 0);
}
