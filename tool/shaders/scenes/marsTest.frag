#include "../common/sceneBefore.glsl"
#include "../common/materials.glsl"
#include "../common/materialMix.glsl"
#include "../common/bump3d.glsl"
#include "../marsHeightmap.glsl"
#include "../common/getCliffAmount.glsl"
#include "../materials/terrain.glsl"
#include "../materials/metal.glsl"
#include "../materials/dirtyMetal.glsl"
#include "../materials/foil.glsl"
#include "../materials/solar.glsl"
#include "../materials/rock.glsl"
#include "../materials/solidRough.glsl"
#include "../materials/obelisk.glsl"
#include "../common/obeliskGlow.glsl"
#include "../materials/laser.glsl"
#include "../materials/rocket.glsl"

uniform float obeliskAmount;
uniform float obeliskMode;
uniform float citadelAmount;
uniform float citadelMode;
uniform float guardAmount;
uniform float guardMode;
uniform float nightAmount;

EvaluatedMaterial evaluateMaterial(float material, MaterialInput val) {
    if (material <= -1) return EvaluatedMaterial(vec3(0), vec3(0), vec3(1, 1, 0), 0, 0, 0, 0);
    if (material <= MATERIAL_TERRAIN) return terrainMaterial(val);
    if (material <= MATERIAL_METAL) return metalMaterial(val);
    if (material <= MATERIAL_DIRTY_METAL) return dirtyMetalMaterial(val);
    if (material <= MATERIAL_FOIL) return foilMaterial(val);
    if (material <= MATERIAL_SOLAR) {
        return materialMix(foilMaterial(val), solarMaterial(val), saturate(val.n.y));
    }
    if (material <= MATERIAL_ROCK) return rockMaterial(val);
    if (material <= MATERIAL_OBELISK) return obeliskMaterial(val);
    if (material <= MATERIAL_OBELISK_PILLARS) return obeliskGlow(obeliskMode, vec3(0.2, 0.2, 1), 1, obeliskAmount, val);
    if (material <= MATERIAL_OBELISK_SUPPORTS) return obeliskGlow(citadelMode, vec3(0.1, 0.1, 1), 0.2, citadelAmount, val);
    if (material <= MATERIAL_OBELISK_GUARDS) return obeliskGlow(guardMode, vec3(1, 0.2, 0.2), 1, guardAmount, val);
    if (material <= MATERIAL_LASER) return laserMaterial(val);
    if (material <= MATERIAL_ROCKET) return rocketMaterial(val);

    return solidRoughMaterial(val, vec3(1, 1, 1));
}

vec3 getDayFog(vec3 d) {
    vec3 sky1 = vec3(254, 168/2, 133/2)/255;
    vec3 sky2 = vec3(255, 244/2, 204/2)/255;

    vec3 sunburst = vec3(1, 0.9, 0.8);
    float sunburstAmount = pow(saturate(dot(d, lightDirection.xyz)), 2);

    return mix(mix(sky1, sky2, saturate(d.y))*0.5, sunburst, sunburstAmount);
}

vec3 getSkyPattern(vec3 d) {
    return vec3(
       fbm(d.xyz),
       fbm(d.yzx),
       fbm(d.zxy)
   );
}

vec3 getNightFog(vec3 d, vec3 p) {
    vec3 skyPattern = getSkyPattern(d) * pow(saturate(fbm(d.xzy)), 3);
    vec3 baseColor = pow(saturate(skyPattern), vec3(5, 4, 1)) / 10;

    return baseColor;
}

vec3 getFog(vec3 d, vec3 p) {
    return mix(getDayFog(d), getNightFog(d, p), nightAmount);
}

vec3 getSky(vec3 d, vec3 p) {
    float sunAmount = pow(dot(d, lightDirection.xyz), 2000);
    vec3 skyColor = getFog(d, p);

    return mix(skyColor, vec3(50), saturate(sunAmount));
}

float testHills(vec3 p, vec3 lightDir) {
    float offsetHeight = marsHeightmap(p.xz + lightDir.xz);
    return saturate((p.y + lightDir.y - offsetHeight)*5);
}

vec3 lightScene(EvaluatedMaterial material, vec3 p, vec3 d, vec3 n, float depth) {
    float aoAmount = texture(sAo, texCoords).r;

    vec3 ssPos = (viewMatrix * vec4(p, 1)).xyz;
    vec3 ssNormal = inverse(transpose(mat3(viewMatrix))) * n;

    float lightAmount = getLightIntensity(sLightDepth, p);
    vec3 sunAlbedo = getLightRadiance(material, p, -d, lightDirection.xyz) * lightAmount;
    vec3 ambientAlbedo = vec3(0.05) * material.albedo * material.roughness;

    for (float i = 0.3; i <= 1; i += 0.2) {
        sunAlbedo *= testHills(p, lightDirection.xyz * i);
    }

    return (sunAlbedo + ambientAlbedo) * aoAmount;
}

#include "../common/sceneAfter.glsl"

