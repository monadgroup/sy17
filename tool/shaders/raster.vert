#include "common/math.glsl"
#include "common/object_data.glsl"
#include "common/camera_data.glsl"
#include "common/rasterVert.glsl"

// vertex data
layout(location=0) in vec3 inPos;
layout(location=1) in vec3 inNormal;
layout(location=2) in float inMaterial;

void main() {
    rasterVert(inPos, inPos, inNormal, inMaterial, modelMatrix);
}
