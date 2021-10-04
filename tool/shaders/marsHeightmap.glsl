#include "common/terrain_data.glsl"

uniform sampler2D inHeightmap;

float marsHeightmap(vec2 p) {
    float bigHills = fbm(p/5);

    float heightmap = texture(inHeightmap, p/5).r*0.7+0.3;
    float bigHeightmap = pow(texture(inHeightmap, p/30).r*1.5, 2)*10;

    float heightSign = sign(heightmap);

    // crater handling
    vec2 craterVec = p - craterPos;
    float craterDist = length(craterVec) / craterSize;
    float craterAngle = atan(craterVec.y, craterVec.x)/M_PI*4 - 0.15;
    float rotSample = texture(inHeightmap, vec2(craterAngle, craterDist)).r;

    craterDist += rotSample * saturate(pow(craterDist, 2));

    //craterDist += pow(rotSample, 5) * craterDist / 5;
    //craterDist += rotSample * max(0, craterDist - 2);

    float insideHeight = pow(craterDist, craterFalloff);
    float outsideHeight =  min(0, pow(craterDist/2-2, 3))/5;
    //outsideHeight *= 1 - saturate(pow(normalize(craterVec).y, 20));
    outsideHeight = 1 - outsideHeight*0.5;

    float terrainHeight = min(insideHeight, outsideHeight)-1;
    //terrainHeight += pow(rotSample*max(0, craterDist-0.5)*2, 2);

    float smoothness = min(1, craterDist/5);

    float baseMap = (bigHeightmap + bigHills) * smoothness;
    float finalHeight = terrainHeight*5 + baseMap;

    // pull towards height=1 around x
    float xDist = saturate(abs((p.y - 40.61)/2));
    float heightPull = cos(M_PI * xDist)/2 + 0.5;
    finalHeight = heightSign*pow(heightmap, 3)*5 + mix(finalHeight, 0.5, heightPull*step(1, craterDist));

    return finalHeight;
}

vec3 marsHeightmapNormal(vec2 p) {
    MAP_NORMAL(marsHeightmap, 0.01);
}

vec3 largeMarsHeightmapNormal(vec2 p) {
    MAP_NORMAL(marsHeightmap, 0.5);
}
