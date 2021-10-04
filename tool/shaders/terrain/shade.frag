#include "../common/rasterFrag.glsl"
#include "../common/materials.glsl"
#include "../common/math.glsl"
#include "../common/noise.glsl"
#include "../common/mapNormal.glsl"
#include "../marsHeightmap.glsl"

in vec3 vPosition;
in float vDepth;

void main() {
    vec3 normal = marsHeightmapNormal(vPosition.xz);
    rasterFrag(vPosition, vec3(0), normal, vPosition, MATERIAL_TERRAIN, vDepth);
}
