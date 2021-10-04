#include "../metalBumpmap.glsl"

EvaluatedMaterial metalMaterial(MaterialInput val) {
    vec3 bump = BUMP_3D(metalBumpmapNormal, val.p, val.n);

    return EvaluatedMaterial(
        normalize(val.n + bump * 0.1),
        vec3(1),
        vec3(0),
        0,
        0.5,
        0.8,
        1
    );
}
