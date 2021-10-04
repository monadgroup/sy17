#include "../foilBumpmap.glsl"

EvaluatedMaterial foilMaterial(MaterialInput val) {
    val.p *= 40;

    float height = foilBumpmap(val.p.yz) * abs(val.n.x) + foilBumpmap(val.p.xz) * abs(val.n.y) + foilBumpmap(val.p.xy) * abs(val.n.z);

    vec3 bump = BUMP_3D(foilBumpmapNormal, val.p, val.n);

    return EvaluatedMaterial(
        normalize(val.n + bump * 0.5),
        mix(vec3(1, 0.1, 0.01), vec3(1, 0.4, 0.05), height),
        vec3(0),
        0,
        0.7,
        1,
        1
    );
}
