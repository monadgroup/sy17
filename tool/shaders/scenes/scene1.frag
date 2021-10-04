#include "../common/sceneBefore.glsl"

// individual materials
#include "../materials/solidRough.glsl"

EvaluatedMaterial evaluateMaterial(float material, MaterialInput val) {
    return solidRoughMaterial(val, vec3(1, 1, 1));
}

vec3 getSky(vec3 d) {
    return vec3(0);
}

vec3 lightScene(EvaluatedMaterial material, vec3 p, vec3 d, vec3 n) {
    return material.albedo + material.emissive;
}

#include "../common/sceneAfter.glsl"
