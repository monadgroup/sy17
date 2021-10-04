#include "common/rasterFrag.glsl"

in vec3 VertPos;
in vec3 VertNormal;
in vec3 VertLocalPos;
in float VertMaterial;
in float VertDepth;

void main() {
    rasterFrag(VertPos, vec3(0), VertNormal, VertLocalPos, VertMaterial, VertDepth);
}
