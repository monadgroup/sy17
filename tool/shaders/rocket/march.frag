#include "../common/structs.glsl"
#include "../common/math.glsl"
#include "../common/noise.glsl"
#include "../common/camera_data.glsl"
#include "../common/scene_data.glsl"
#include "../common/terrain_data.glsl"
#include "../common/getRay.glsl"
#include "../common/linearizeDepth.glsl"
#include "../common/unlinearizeDepth.glsl"
#include "../common/sdf.glsl"
#include "../common/materials.glsl"
#include "../common/mapNormal.glsl"
#include "../common/rasterFrag.glsl"
#include "../common/minResult.glsl"

uniform float rocketHeight;
uniform float lastRocketHeight;
uniform float rotateSpeed;

RaymarchResult fScene(vec3 p) {
    p.xz -= craterPos;
    p *= 1.5;
    p.y -= rocketHeight;
    pR(p.xz, time*rotateSpeed);

    float mainCapsule = fCone(p, 0.4, 2);

    vec3 rocketP = p;

    vec3 rp1 = rocketP;
    pR(rp1.yx, radians(-10));
    float rocket1 = fCapsule(rp1 - vec3(0.2, -0.5, 0), 0.2, 0.4);

    pR(rocketP.xz, radians(120));
    vec3 rp2 = rocketP;
    pR(rp2.yx, radians(-10));
    float rocket2 = fCapsule(rp2 - vec3(0.2, -0.5, 0), 0.2, 0.4);

    pR(rocketP.xz, radians(120));
    vec3 rp3 = rocketP;
    pR(rp3.yx, radians(-10));
    float rocket3 = fCapsule(rp3 - vec3(0.2, -0.5, 0), 0.2, 0.4);

    float rocketDist = fOpUnionRound(rocket1, fOpUnionRound(rocket2, rocket3, 0.3), 0.3);
    float dist = mix(rocketDist, mainCapsule, saturate(p.y*10+0.5));

    return RaymarchResult(p, dist*0.5, MATERIAL_ROCKET);
}

#include "../common/rasterRaymarch.glsl"

void main() {
    rasterRaymarch(1, vec3(0, rocketHeight - lastRocketHeight, 0));
}
