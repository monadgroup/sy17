
in vec2 texCoords;
out vec3 outColor;

uniform sampler2D sBlooms[4];

uniform float bloomBias;
uniform float bloomPower;
uniform float bloomAmount;

void main() {
    vec3 bloom = vec3(0);
    for (int i = 0; i < sBlooms.length(); i++) {
        bloom += texture(sBlooms[i], texCoords).rgb;
    }
    outColor = pow(bloom + bloomBias, vec3(bloomPower)) * bloomAmount;
}
