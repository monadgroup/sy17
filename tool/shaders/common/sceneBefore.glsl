#include "math.glsl"
#include "noise.glsl"
#include "camera_data.glsl"
#include "light_data.glsl"
#include "scene_data.glsl"
#include "structs.glsl"
#include "getRay.glsl"
#include "linearizeDepth.glsl"
#include "lighting.glsl"
#include "getLightIntensity.glsl"
#include "mapNormal.glsl"

in vec2 texCoords;
out vec3 outColor;
out vec3 outBloom;

uniform vec2 viewportSize;
uniform float bloomThreshold;

uniform sampler2D sPosMaterial;
uniform sampler2D sNormal;
uniform sampler2D sLocalPos;
uniform sampler2D sDepth;
uniform sampler2D sLightDepth;
uniform sampler2D sAo;
