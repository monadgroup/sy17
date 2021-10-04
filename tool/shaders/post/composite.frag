#include "../common/scene_data.glsl"

in vec2 texCoords;
out vec4 outColor;

uniform sampler2D sColor;
uniform sampler2D sBloom;
uniform sampler2D sFlare;

uniform float exposure;
uniform float vignetteStrength;
uniform float vignetteSize;
uniform float vignettePower;
uniform float grainAmount;

float A = 0.15;
float B = 0.50;
float C = 0.10;
float D = 0.20;
float E = 0.02;
float F = 0.30;
float W = 11.2;

vec3 uc2Tonemap(vec3 x) {
    return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

void main() {
    vec3 color = texture(sColor, texCoords).rgb;
    vec3 bloom = texture(sBloom, texCoords).rgb;
    vec3 flare = texture(sFlare, texCoords).rgb;

    vec3 inColor = max(vec3(0), color + bloom + flare);

    // vignette
    float d = clamp(length(texCoords * 2.0 - 1.0) - vignetteSize, 0.0, 1.0);
    inColor *= 1.0 - clamp(pow(d, vignettePower) * vignetteStrength, 0, 1);

    // grain
    float grainStrength = 50 * grainAmount;
    float x = (texCoords.x + 4) * (texCoords.y + 4) * (time + 10) * 10;
    float grain = clamp(mod((mod(x, 13.0) + 1.0) * (mod(x, 123.0) + 1.0), 0.01) - 0.005, 0.0, 1.0) * grainStrength;
    inColor *= 1.0 - grain;

    // color grading
    inColor = pow(inColor, vec3(1, 1.05, 1.175));

    // tonemapping
    inColor = uc2Tonemap(inColor * exposure);

    vec3 whiteScale = 1 / uc2Tonemap(vec3(W));
    inColor *= whiteScale;


    // gamma correct
    inColor = pow(inColor, vec3(1 / 2.2));

    // output luma for FXAA
    float luma = sqrt(dot(inColor, vec3(0.299, 0.587, 0.114)));
    outColor = vec4(inColor, luma);
}
