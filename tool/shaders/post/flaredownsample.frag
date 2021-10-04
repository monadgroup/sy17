in vec2 texCoords;
out vec3 outTex;

uniform sampler2D sColor;

uniform float scale;
uniform float bias;

void main() {
    outTex = max(vec3(0), texture(sColor, texCoords).rgb + bias) * scale;
}
