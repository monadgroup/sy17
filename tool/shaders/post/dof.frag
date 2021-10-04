#include "../common/camera_data.glsl"
#include "../common/math.glsl"
#include "../common/linearizeDepth.glsl"

in vec2 texCoords;
out vec3 outTex;

uniform sampler2D sColor;
uniform sampler2D sDepth;
uniform vec2 viewportSize;

uniform vec2 focusRange;
uniform float focusGradient;
uniform float focusSize;

vec2 texel = 1 / viewportSize;
int samples = 10;
int rings = 5;

float CoC = 0.03;
float threshold = 0.5;
float gain = 1;
float bias = 0.5;
float fringe = 0.7;
float ditherAmount = 0.0001;
float blurSize = 1.25;

vec3 color(vec2 coords, float blur) {
    /*vec3 col = vec3(0.0);

    col.r = texture(sColor, coords + vec2(0.0, 1.0) * texel * fringe * blur).r;
    col.g = texture(sColor, coords + vec2(-0.866, -0.5) * texel * fringe * blur).g;
    col.b = texture(sColor, coords + vec2(0.866, -0.5) * texel * fringe * blur).b;

    float lum = dot(col, vec3(0.299, 0.587, 0.114));
    float thresh = max((lum - threshold) * gain, 0.0);
    return col + mix(vec3(0), col, thresh * blur);*/
    return texture(sColor, coords).rgb;
}

vec2 rand(vec2 coord) {
    float noiseX = clamp(fract(sin(dot(coord ,vec2(12.9898,78.233))) * 43758.5453),0.0,1.0)*2.0-1.0;
	float noiseY = clamp(fract(sin(dot(coord ,vec2(12.9898,78.233)*2.0)) * 43758.5453),0.0,1.0)*2.0-1.0;
    return vec2(noiseX, noiseY);
}

void main() {
    vec3 col = texture(sColor, texCoords).xyz;
    float depth = linearizeDepth(texture(sDepth, texCoords).r, clipRange);

    float a = depth - mix(focusRange.x, focusRange.y, 0.5); // focal plane
    float b = (depth - focusRange.y) / focusGradient; // far DOF
    float c = (focusRange.x - depth) / focusGradient; // near DOF
    float blur = clamp((a > 0) ? b : c, 0, focusSize);

    vec2 noise = rand(texCoords) * ditherAmount * blur;
    vec2 size = texel * blur;

    float s = 1;
    for (int i = 1; i <= rings; i++) {
        int ringSamples = i * samples;

        for (int j = 0; j < ringSamples; j++) {
            float stepv = M_PI * 2 / float(ringSamples);
            float pw = cos(float(j) * stepv) * float(i);
            float ph = sin(float(j) * stepv) * float(i) * 1.5;
            float mixed = mix(1.0, (float(i)) / (float(rings)), bias);
            vec2 samplePos = texCoords + vec2(pw * size.x, ph * size.y);
            float sampleDepth = linearizeDepth(texture(sDepth, samplePos).r, clipRange);
            mixed *= step(depth, sampleDepth);
            col += color(samplePos, blur) * mixed;
            s += mixed;
        }
    }
    col /= s;
    outTex = col;

    /*vec4 acc = vec4(0);
    const int N = 12;
    for (int j = -N; j <= N; j++) {
        for (int i = -N; i <= N; i++) {
            vec2 off = vec2(float(i), float(j));
            vec2 samplePos = texCoords + off / vec2(800,450);
            vec3 color = texture(sColor, samplePos).rgb;
            float depth = linearizeDepth(texture(sDepth, samplePos).r, clipRange);

            float a = depth - mix(focusRange.x, focusRange.y, 0.5); // focal plane
            float b = (depth - focusRange.y) / focusGradient;
            float c = (focusRange.x - depth) / focusGradient;

            float t = mix(c, b, step(0, a));
            float coc = 0.05 + clamp(t, 0, focusSize);
            float w = 1 / pow(coc, 2) * step(dot(off, off), coc*coc);
            acc += vec4(color*w, w);
        }
    }
    outTex = acc.xyz / acc.w;*/


    /*vec3 pos = texture(sPos, texCoords).xyz;
    float depth = distance(cameraPos.xyz, pos);

    // dof blur factor calculation
    float a = depth - mix(focusRange.x, focusRange.y, 0.5); // focal plane
    float b = (depth - focusRange.y) / focusGradient; // far DOF
    float c = (focusRange.x - depth) / focusGradient; // near DOF
    float blur = clamp((a > 0) ? b : c, 0, focusSize);

    // get dithering pattern
    vec2 noise = rand(texCoords) * ditherAmount * blur;

    vec2 size = texel * blur;
    vec3 col = inColor.rgb;
    float s = 1;
    int ringSamples;
    for (int i = 1; i <= rings; i++) {
        ringSamples = i * samples;

        for (int j = 0; j < ringSamples; j++) {
            float step = M_PI * 2 / float(ringSamples);
            float pw = cos(float(j) * step) * float(i);
            float ph = sin(float(j) * step) * float(i) * 1.5;
            float mixed = mix(1.0, (float(i)) / (float(rings)), bias);
            vec2 samplePos = texCoords + vec2(pw * w, ph * h);
            col += color(samplePos, blur) * mixed;
            s += mixed;
        }
    }
    col /= s;*/
    //outTex = vec3(blur);
}
